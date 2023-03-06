/*
  D a t e F o r m . c p p
*/

#include "DateForm.h"

using namespace std;

ostream& operator << (ostream& out, const DateFormT& date) {
  out << setw(2) << setfill('0') << date.sysTime.wHour << ':' <<
    setw(2) << setfill('0') << date.sysTime.wMinute << ' ' <<
    date.sysTime.wMonth << '-' << date.sysTime.wDay << '-' << date.sysTime.wYear;
  return out;
} // operator << DateFormT