/*
  M a p p e d F i l e . h

  When this class is opened it maps a file to memory.  When it is deleted it flushes any changes 
  back to the disk, unmaps the file, and closes the file.

  Two types are defined that are redundant with C++ standard definitions FileSizeT and ByteT.  This
  was not needed when only compiling in visual studio for a Windows 64 machine, but was the only
  simple solution for getting portability to the Cuda compiler.  Sometimes size_t was 32 bits and
  std::byte is not defined.
*/

#pragma once

// Modern C++
#include <cstddef>
#include <cstdint>

// Doesn't work with less than the full windows.h ... should but is a windows bug
#include <windows.h>

// ************************************
typedef uint64_t FileSizeT;
typedef uint8_t ByteT;

// ****************************
// ToDo:  change to use strings instead of c-strings
class MappedFileT {
public:
  // This is read-only for a file that already exists
  MappedFileT(const char* fileName);

  // This creates a new file, deleting anything that already exists.
  MappedFileT(const char* fileName, FileSizeT size);

  ~MappedFileT();

  inline ByteT operator[] (int index) const
    { return addr[index]; }

  inline void* operator() () const
    { return addr; }

 protected:
  HANDLE fileHand;
  HANDLE memMap;
  ByteT* addr;
}; // MappedFileT

// include code body
#include "MappedFile.body.h"