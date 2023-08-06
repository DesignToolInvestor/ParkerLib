/*
  T i m e r . c p p
*/

#include "Timer.h"

#include <windows.h>
#include <cassert>

TickCountT ReadTicks() {
  LARGE_INTEGER result;
  int status = QueryPerformanceCounter(&result);
  assert(status != 0);

  return (TickCountT) result.QuadPart;
} // ReadTicks

float TicksToSecs(TickCountT ticks) {
  LARGE_INTEGER freq;
  int status = QueryPerformanceFrequency(&freq);
  assert(status != 0);

  return float(ticks) / float(freq.QuadPart);
} // TicksToSec