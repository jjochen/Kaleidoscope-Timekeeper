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

#pragma once

#include <Kaleidoscope.h>
#include <Kaleidoscope-Ranges.h>
#include <TimeLib.h>


#define Key_TimekeeperDate (Key){ .raw = kaleidoscope::TMK_DATE }
#define Key_TimekeeperTime (Key){ .raw = kaleidoscope::TMK_TIME }
#define Key_TimekeeperDateTime (Key){ .raw = kaleidoscope::TMK_DATE_TIME }
#define Key_Timekeeper Key_TimekeeperDate

namespace kaleidoscope {

enum {
  TMK_FIRST = kaleidoscope::ranges::TMK_FIRST,
  TMK_DATE = TMK_FIRST,
  TMK_TIME,
  TMK_DATE_TIME,
  TMK_LAST = TMK_DATE_TIME
};

class Timekeeper : public KaleidoscopePlugin {
 public:
  Timekeeper(void);

  void begin(void) final;

  static void typeDate(void);
  static void typeTime(void);
  static void typeDateAndTime(void);

 private:
  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
  static void loopHook(bool is_post_clear);
  static bool timeAvailable(void);
  static void processSyncMessage(void);
  static time_t requestSync();
  static void typeNumber(int number, uint8_t numberOfDigits);
  static void typeDigit(uint8_t digit);
  static void tapKey(Key key);
};

}

extern kaleidoscope::Timekeeper Timekeeper;
