/*
  O l d R a n d . h


  This is a unit for old fashion RNG (Random Number Generators), i.e., simple seed sequence 
  generators without shuffle.  These are no longer acquitted for most large scale simulations
  but they are useful for testing.
*/

#pragma once

unsigned RandTurboPascal(unsigned seed)
{
  const unsigned A = 134'775'813;
  const unsigned B = 1;

  return (A * seed + B);
}