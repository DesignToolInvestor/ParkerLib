/*
  S p l i c e . c p p
*/

#include <windows.h>
#include "Splice.h"

SpliceRowT::SpliceRowT(
  void* baseAddr, unsigned numRow, unsigned numCol, unsigned elemSize,
  unsigned* spliceToBase, unsigned numSpliceRow, ArgPassT argPass) :
    addr((BYTE*) baseAddr), numRow(numRow), numCol(numCol), elemSize(elemSize), numSpliceRow(numSpliceRow)
{
  if ((argPass == byRef) || (argPass == byTransfer))
    this->spliceToBase = spliceToBase;
  else { // byCopy
    this->spliceToBase = new unsigned[numSpliceRow];
    memcpy(this->spliceToBase, spliceToBase, numSpliceRow * sizeof(unsigned));
  }

  this->ownMem = ((argPass == byCopy) || (argPass == byTransfer));
} // SpliceRowT

SpliceRowT::~SpliceRowT() {
  if (ownMem)
    delete spliceToBase;
} // ~spliceToBase

void SpliceRowT::CompactCopy(void* dest) {
  unsigned rowSizeByte = numCol * elemSize;

  for (unsigned spliceRow = 0; spliceRow < numRow; spliceRow++) {
    SizeT destByteOffset = SizeT(spliceToBase[spliceRow]) * SizeT(numCol) * SizeT(elemSize);
    memcpy((BYTE*) dest + destByteOffset, (*this)[spliceRow][0], rowSizeByte);
  }
} // CompactCopy