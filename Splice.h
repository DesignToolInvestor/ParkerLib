/*
  S p l i c e . h
*/

#pragma once
#include <windows.h>
#include "ArgPass.h"

typedef long long SizeT;

class RowRefT {
public:
  RowRefT(void* rowAddr, unsigned numCol, unsigned elemSize) :
      rowAddr((BYTE*)rowAddr), numCol(numCol), elemSize(elemSize)
    { /* Nothing */ }

  inline void* operator[] (unsigned col)
    { return rowAddr + (col * elemSize); };

protected:
  BYTE* rowAddr;
  unsigned numCol, elemSize;
}; // RowRefT

class SpliceT {
 public:
  virtual RowRefT operator[] (unsigned row) = 0;
  
  virtual SizeT CompactSize() = 0;
  virtual void CompactCopy(void* Dest) = 0;
}; // SpliceT

class SpliceRowT : SpliceT {
public:
  SpliceRowT(
    void* baseAddr, unsigned numRow, unsigned numCol, unsigned elemSize, 
    unsigned spliceToBase[], unsigned numSpliceRow, ArgPassT argPass);
  ~SpliceRowT();

  inline SizeT CompactSize()
    { return SizeT(numSpliceRow) * SizeT(numCol) * SizeT(elemSize); }

  inline RowRefT operator[] (unsigned row) { 
    SizeT rowIndex = SizeT(spliceToBase[row]) * SizeT(numCol);
    SizeT byteOffSet = rowIndex * SizeT(elemSize);
    return RowRefT(addr + byteOffSet, numCol, elemSize);
  }

  void CompactCopy(void* Dest);

 protected:
  BYTE* addr;
  unsigned numRow, numCol, elemSize;
  
  unsigned numSpliceRow;
  unsigned* spliceToBase;
  BOOL ownMem;
}; // SpliceRowT