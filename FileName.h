/*
  F i l e N a m e . h

  This is a collection of file name handling utilities.
*/

#pragma once

#include <queue>
#include <string>

#include "DateForm.h"

typedef struct {
  char* fileName;
  int numPeople;
  SizeT fileSize;
} FileInfoT;

/*
  PathParse returns the location of delimiters in a full path.  These delimiters can be used to copy 
  the parts of the path char-array to new strings.  This is often desired when the scope of the 
  full-path is fleeting.  The win32 package has a function, "_splitpath_s", that does this using 
  preallocated buffers of arbitrary length.  This function allow the length to be calculated before 
  the allocation happens.  This avoids the buffer overflow attack allowed by "_splitpath" and the 
  double copying that is likely implied by "_splitpath_s".
*/

// ToDo:  Make everything strings.  It's less efficient, but much safer.
void PathParse(const char* fullPath, int& pathFileSep, int& fileExtSep, int& len);

FileInfoT* DirListNew(const std::string& dirMask, int& numFile);
void DirListDelete(FileInfoT* files, int numFile);