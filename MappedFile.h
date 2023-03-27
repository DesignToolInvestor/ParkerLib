/*
  M a p p e d F i l e . h

  When this class is opened it maps a file to memory.  When it is deleted it flushes any changes 
  back to the disk, unmaps the file, and closes the file.
*/

#pragma once

// Modern C++
#include <cstddef>
#include <cstdint>

// Doesn't work with less than the full windows.h ... should but is a windows bug
#include <windows.h>

// ************************************
typedef uint64_t FileSizeT;

// ****************************
// ToDo:  change to use strings instead of c-strings
class MappedFileT {
public:
  // This is read-only for a file that already exists
  MappedFileT(const char* fileName);

  // This creates a new file, deleting anything that already exists.
  MappedFileT(const char* fileName, FileSizeT size);

  ~MappedFileT();

  inline std::byte operator[] (int index) const
    { return addr[index]; }

  inline void* operator() () const
    { return addr; }

 protected:
  HANDLE fileHand;
  HANDLE memMap;
  std::byte* addr;
}; // MappedFileT

// include code body
#include "MappedFile.body.h"