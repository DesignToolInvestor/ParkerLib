/*
  T i m m e r . h

  This is superseded by a newer module, that is almost the same.
*/

#pragma once

typedef long long TickCountT;

TickCountT ReadTicks();
float TicksToSecs(TickCountT ticks);