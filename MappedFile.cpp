/*
  M a p p e d F i l e . c p p
*/

#include <windows.h>
#include <cassert>
#include <errhandlingapi.h>
#include <strsafe.h>

#include "MappedFile.h"

// ToDo:  Use error checking that works in release mode.

// ************************************
// This opens an existing file as read-only
MappedFileT::MappedFileT(const char* fileName) {
  this->fileHand = CreateFileA(
    fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
  assert(this->fileHand != INVALID_HANDLE_VALUE);

  this->memMap = CreateFileMappingA(fileHand, NULL, PAGE_READONLY, 0,0, NULL);
  assert(this->memMap != NULL);

  this->addr = (BYTE*) MapViewOfFile(memMap, FILE_MAP_READ, 0,0, 0);
  assert(this->addr != NULL);
} // MappedFileT(char*)

// ************************************
// This creates a new file as read/write
MappedFileT::MappedFileT(const char* fileName, SizeT size) {
  // Fix for 32-bit machiens
  SizeT const LOW_DWORD = (1ll << 32) - 1ll;
  DWORD sizeHigh = DWORD(size >> 32);
  DWORD sizeLow = DWORD(size & LOW_DWORD);

  // TODO:  Test with write thrugh
  this->fileHand = CreateFileA(
    fileName, (GENERIC_READ | GENERIC_WRITE), 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  assert(this->fileHand != INVALID_HANDLE_VALUE);
  
  DWORD distMovedHigh = sizeHigh;
  DWORD distMovedLow = SetFilePointer(fileHand, sizeLow,(PLONG) &distMovedHigh, FILE_BEGIN);
  assert((distMovedLow == sizeLow) && (distMovedHigh == sizeHigh));
  SetEndOfFile(fileHand);

  this->memMap = CreateFileMappingA(fileHand, NULL, PAGE_READWRITE, 0,0, NULL);
  if (this->memMap == NULL) {
    DWORD errNum = GetLastError();
    abort();
  }

  this->addr = (BYTE*) MapViewOfFile(memMap, FILE_MAP_WRITE, 0,0, 0);
  if (addr == NULL) {
    DWORD errNum = GetLastError();
    abort();
  }
} // MappedFileT(char*)

// ************************************
MappedFileT::~MappedFileT() {
  BOOL success = UnmapViewOfFile(this->addr);
  assert(success);
  this->addr = NULL;

  success = CloseHandle(this->memMap);
  assert(success);
  this->memMap = NULL;

  success = CloseHandle(this->fileHand);
  assert(success);
  this->fileHand = NULL;
} // ~MappedFile