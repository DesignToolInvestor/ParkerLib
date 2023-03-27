/*
  O l d R a n d . h
*/

#pragma once

unsigned RandTurboPascal(unsigned seed)
{
  const unsigned A = 134'775'813;
  const unsigned C = 1;

  return (A * seed + C);
}