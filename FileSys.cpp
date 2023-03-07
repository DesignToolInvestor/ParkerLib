/*
  F i l e S y s . c p p

  This is not a general module for managing files.  In particular, it is too vertically integrated.  It
  finds the files, checks the size of each file, and parses the file name for the number of people that
  is in the filename.  It would be nice to have a version of this code that was extensible.  That is,
  code that would find all the files that match the pattern and the preform some user provided function
  on the file information record.  But file handling is only used in this project as part of the testing
  harness, so creating more reusable file handling code seems like an excessive burden for this project.

  If reused in some other project where file handling is more central, that project should assume 
  responsibility for a more reusable version of this module.  The big problem with this is dealing with
  the ownership of the fileName;
*/

#include "FileSys.h"

#include <Windows.h>
#include <cassert>
#include <string.h>

#include "CokieLib.h"

using namespace std;

/*
  There should be a library function that just does the right thing, but I can't find it.  I can find 
  functions that are unsafe (i.e., with respect to buffer overflow attack) or that require preallocation
  of large buffers and/or double copying.
*/
void PathParse(const char* fullPath, int& pathFileSep, int& fileExtSep, int& len) {
  pathFileSep = fileExtSep = len = 0;

  // ToDo:  deal with the case where there is no extension
  while (fullPath[len] != 0) {
    if (fullPath[len] == '\\')
      pathFileSep = len;
    else if (fullPath[len] == '.')
      fileExtSep = len;

    len++;
  }
}; // PathParse

FileInfoT* DirListNew(const string& dirMask, int& numFile) {
  queue<FileInfoT> fileInfoQue;
  BOOL done = false;

  WIN32_FIND_DATAA fileInfo;
  HANDLE file = FindFirstFileA((LPCSTR) &dirMask[0], &fileInfo);
  if (file == NULL)
    done = true;
  else
    assert((fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);

  while (!done) {
    FileInfoT elem;

    // parse file name
    const char* fileName = fileInfo.cFileName;
    int pathFileSep, fileExtSep, len;
    PathParse(fileName, pathFileSep, fileExtSep, len); // This is unsafe, but got string form the OS

    // copy file name
    char* elemFileName = new char[len + 1];
    memcpy(elemFileName, fileName, len);
    elemFileName[len] = 0;
    elem.fileName = elemFileName;

    // parse num of people
    int numStart = fileExtSep;
    while ((0 < numStart) && ('0' <= fileName[numStart - 1]) && (fileName[numStart - 1] <= '9'))
      numStart--;

    int numDig = fileExtSep - numStart;
    char* numStr = new char[numDig + 1];
    memcpy(numStr, fileName + numStart, numDig); // ugly
    numStr[numDig] = 0;
    elem.numPeople = atoi(numStr);
    delete [] numStr;
    
    // do file size
    elem.fileSize = (SizeT(fileInfo.nFileSizeHigh) << 32) + fileInfo.nFileSizeLow;

    fileInfoQue.push(elem);

    done = (FindNextFileA(file, &fileInfo) == 0);
  } while (!done);
  FindClose(file);

  FileInfoT* result = NULL;
  numFile = int(fileInfoQue.size());
  if (0 < numFile) {
    result = new FileInfoT[numFile];
    for (int index = 0; index < numFile; index++) {
      result[index] = fileInfoQue.front();
      fileInfoQue.pop();
    }
  }

  return result;
} // LogRand

void DirListDelete(FileInfoT* files, int numFile) {
  for (int index = 0; index < numFile; index++)
    delete[] files[index].fileName;
  delete [] files;
} // DirListDelete