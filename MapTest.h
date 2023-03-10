/*
  M a p T e s t . h
*/

#pragma once

// Would have preferred a struct, more in the spirit of the usage.  But this caused problems with 
// constant strings, which seem necessary.

// ToDo:  Think about C++ 20 style initialization

class ParamConstT {
protected:
  int numPeople, numSnip;

  // Should probably switch from C-strings to C++ strings
  const char* fileName;
  const char* runEnv;

public:
  // Constructors
  ParamConstT() : numPeople(0), numSnip(0), fileName(NULL), runEnv(NULL)
    { /* Nothing */ };

  ParamConstT(int numPeople, int numSnip, const char* fileName, const char* runEnv) :
      numPeople(numPeople), numSnip(numSnip), fileName(fileName), runEnv(runEnv)
    { /* Nothing */ }

  // Accessors function
  int NumPeople() const
    { return numPeople; };
  int NumSnip() const
    { return numSnip; };
  const char* FileName() const
    { return fileName; };
  const char* RunEnv() const
    { return runEnv; };
}; // ParamConstT