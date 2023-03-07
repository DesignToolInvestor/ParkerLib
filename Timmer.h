/*
  T i m m e r . h
*/

#pragma once

typedef long long TickCountT;

TickCountT ReadTicks();
float TicksToSecs(TickCountT ticks);