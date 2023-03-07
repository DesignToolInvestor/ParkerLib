/*
  C A r r a y . h

  The CArrayRef classes are designed to reference parts of a C-style array.
*/

#pragma once

#include <windows.h>

typedef long long SizeT;

class ArrayRef1T {
protected:
  BYTE* addr;
  unsigned size, elemSize;

public:
  ArrayRef1T(void* addr, unsigned size, unsigned elemSize) :
      addr((BYTE*) addr), size(size), elemSize(elemSize)
    { /* Nothing */ }

  inline void* operator[] (unsigned index)
    { return addr + (SizeT(index) * SizeT(elemSize)); };
}; // ArrayRef1T

class ArrayRef2T {
protected:
  BYTE* addr;
  unsigned size0, size1, elemSize;

public:
  ArrayRef2T(void* addr, unsigned size0, unsigned size1, unsigned elemSize) :
      addr((BYTE*) addr), size0(size0), size1(size1), elemSize(elemSize)
    { /* Nothing */ }

  inline ArrayRef1T operator[] (unsigned index0) {
    SizeT byteOffset = SizeT(index0) * SizeT(size1) * SizeT(elemSize);
    return ArrayRef1T(addr + byteOffset, size1, elemSize); 
  };
}; // ArrayRef2T