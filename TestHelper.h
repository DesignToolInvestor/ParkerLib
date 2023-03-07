/*
  T e s t H e l p e r . h
*/

#pragma once

#include "MapTest.h"

void CreateFileOsByte(const ParamConstT& params);
void ReadFileOsByte(const ParamConstT& params);

void CreateMapFile(const ParamConstT& params);
void ReadMapFile(const ParamConstT& params);

void ReadArrayOverLay(const ParamConstT& params);

void CheckContent(void* addr, int numPeople, int numSnip);