/*
  D a t e F o r m . h

  The DateFormT class is an stream modifier for the windows system time.

  Usage:
    cout << DataFormT(sysTime) << '\n';
*/

#pragma once

#include <Windows.h>

#include <ostream>
#include <iomanip>

typedef long long SizeT;

class DateFormT {
public:
  DateFormT(SYSTEMTIME sysTime) : sysTime(sysTime)
    { /* Nothing */ }

  friend std::ostream& operator << (std::ostream& out, const DateFormT& date);

protected:
  SYSTEMTIME sysTime;
}; // DateFormT

// ToDo:  Deprecated.  The libraries for newer version of the C++ have safe strings.
inline unsigned SafeStrLen(const char* str, unsigned maxLen) {
  unsigned pos = 0;
  while ((pos < maxLen) && (str[pos]))
    pos++;
  return pos;
} // SafeStrLen