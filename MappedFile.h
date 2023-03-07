/*
  M a p p e d F i l e . h

  The MappedFleT opens a file and maps it to virtual memory.  When the class is deleted the
  memory resident content is flushed and the file is closed.
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