/*
  M a p p e d F i l e . h

  When this class is opened it maps a file to memory.  When it is deleted it flushes any changes 
  back to the disk, unmaps the file, and closes the file.
*/

#pragma once

#include <windows.h>
#include "DateForm.h"

class MappedFileT {
public:
  // This is read-only for a file that already exists
  MappedFileT(const char* fileName);

  // This creates a new file, deleting anything that already exists.
  MappedFileT(const char* fileName, SizeT size);

  ~MappedFileT();

  inline BYTE operator[] (int index) const
    { return addr[index]; }

  inline void* operator() () const
    { return addr; }

 protected:
  HANDLE fileHand;
  HANDLE memMap;
  BYTE* addr;
}; // MappedFileT