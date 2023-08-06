/*
  T i m e r . h

  This is superseded by a newer module, that is almost the same.
*/

#pragma once

typedef long long TickCountT;

TickCountT ReadTicks();
float TicksToSecs(TickCountT ticks);

// The bodies are compiled into a library which is linked to builds in a project that depends on 
// this project.