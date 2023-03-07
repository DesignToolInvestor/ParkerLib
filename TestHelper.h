/*
  T e s t H e l p e r . h
*/

#pragma once

#include "MapTest.h"

void CreateOldSchoolFile(const ParamConstT& params);
void ReadOldSchoolFile(const ParamConstT& params);

void CreateMapFile(const ParamConstT& params);
void ReadMapFile(const ParamConstT& params);

void ReadArrayOverLay(const ParamConstT& params);

void CheckContent(void* addr, int numPeople, int numSnip);