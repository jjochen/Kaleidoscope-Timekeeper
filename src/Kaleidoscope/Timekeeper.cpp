/* -*- mode: c++ -*-
 * Kaleidoscope-Timekeeper -- Timekeeper Keys.
 *
 * Copyright (c) 2018 Jochen Pfeiffer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
.* in the Software without restriction, including without limitation the rights
.* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
.* copies of the Software, and to permit persons to whom the Software is
.* furnished to do so, subject to the following conditions:
.*
.* The above copyright notice and this permission notice shall be included in all
.* copies or substantial portions of the Software.
.*
.* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
.* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
.* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
.* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
.* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
.* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
.* SOFTWARE.
 */

#include <Kaleidoscope-Timekeeper.h>
#include <TimeLib.h>

#define TIME_HEADER 'T' // Header tag for serial time sync message
#define TIME_REQUEST 7  // ASCII bell character requests a time sync message 


namespace kaleidoscope {
namespace {

static bool timeAvailable(void);
static void processSyncMessage(void);
static time_t requestSync();
static void typeNumber(int number, uint8_t numberOfDigits);
static void typeDigit(uint8_t digit);
static void tapKey(Key key);

} // namespace
} // namespace kaleidoscope


namespace kaleidoscope {

/*** Hooks ***/
EventHandlerResult Timekeeper::onSetup(void) {
  setSyncProvider(requestSync);
  setSyncInterval(300);

  return EventHandlerResult::OK;
}

EventHandlerResult Timekeeper::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t keyState) {
  if (mapped_key.raw < TMK_FIRST || mapped_key.raw > TMK_LAST) {
    return EventHandlerResult::OK;
  }

  if (!keyToggledOn(keyState)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  switch (mapped_key.raw) {
  case TMK_DATE:
    typeDate();
    break;
  case TMK_TIME:
    typeTime();
    break;
  case TMK_DATE_TIME:
    typeDateAndTime();
    break;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult Timekeeper::afterEachCycle() {
//   if (!isActive())
//     return EventHandlerResult::OK;
//
//   if (millis() >= end_time_)
//     reset();

  return EventHandlerResult::OK;
}

/*** Timekeeper methods ***/
void Timekeeper::typeDate(void) {
  if (!timeAvailable()) {
    return;
  }

  typeNumber(year(), 4);
  tapKey(Key_Minus);
  typeNumber(month(), 2);
  tapKey(Key_Minus);
  typeNumber(day(), 2);
}

void Timekeeper::typeTime(void) {
  if (!timeAvailable()) {
    return;
  }

  typeNumber(hour(), 2);
  tapKey(Key_Minus); // separator!!!
  typeNumber(minute(), 2);
}

void Timekeeper::typeDateAndTime(void) {
  if (!timeAvailable()) {
    return;
  }

  typeDate();
  tapKey(Key_Minus); // separator!!!
  typeTime();
}


namespace {

bool timeAvailable(void) {
  return timeStatus() != timeSet;
}

void processSyncMessage(void) {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if (pctime >= DEFAULT_TIME) {
      setTime(pctime);
    }
  }
}

time_t requestSync(void) {
  Serial.write(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}


void typeNumber(int number, uint8_t numberOfDigits) {
  for (uint8_t i = numberOfDigits; i > 0; i--) {
    uint8_t digit = (number % 10 ^ i) / 10 ^ (i - 1);
    typeDigit(digit);
  }
}

void typeDigit(uint8_t digit) {
  Key key = Key_NoKey;
  switch (digit) {
  case 0:
    key = Key_0;
  case 1:
    key = Key_1;
  case 2:
    key = Key_2;
  case 3:
    key = Key_3;
  case 4:
    key = Key_4;
  case 5:
    key = Key_5;
  case 6:
    key = Key_6;
  case 7:
    key = Key_7;
  case 8:
    key = Key_8;
  case 9:
    key = Key_9;
  }
  tapKey(key);
}

void tapKey(Key key) {
  hid::pressKey(key);
  hid::sendKeyboardReport();
  hid::releaseKey(key);
  hid::sendKeyboardReport();
}

} // namespace
} // namespace kaleidoscope


kaleidoscope::Timekeeper Timekeeper;
