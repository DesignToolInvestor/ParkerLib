/*
  T e s t H e l p e r . c p p
*/

// ToDo:  Play around with reducing the include surface area
#include <windows.h> // may need to be first due to a bug

#include <stdio.h>
#include <cassert>

#include "TestHelper.h"
#include "MappedFile.h"
#include "MapTest.h"
#include "CArray.h"

typedef long long QWORD;

void CreateOldSchoolFile(const ParamConstT& params) {
  FILE* file;
  errno_t errorNum = fopen_s(&file, params.FileName(), "wb");
  assert(errorNum == 0);
  //if (erroNum != NULL)
  //  abort(erroNum)

  assert(file != 0);

  for (int people = 0; people < params.NumPeople(); people++)
    for (int snip = 0; snip < params.NumSnip(); snip++) {
      BYTE value = (people + snip) % 256;
      size_t writeSize = fwrite(&value, 1,1, file);
      assert(writeSize == 1);
    }

  fclose(file);
} // CreateOldSchoolFile

void ReadOldSchoolFile(const ParamConstT& params) {
  FILE* file;
  errno_t errorNum = fopen_s(&file, params.FileName(), "r");
  assert(errorNum == 0);
  assert(file != NULL);

  for (int people = 0; people < params.NumPeople(); people++)
    for (int snip = 0; snip < params.NumSnip(); snip++) {
      BYTE value;
      size_t readSize = fread(&value, 1, 1, file);

      assert(readSize == 1);
      assert(value == (people + snip) % 256);
    }

  fclose(file);
} // ByteREadFile

void CreateMapFile(const ParamConstT& params) {
  SizeT size = SizeT(params.NumPeople()) * SizeT(params.NumSnip());
  MappedFileT mappedFile(params.FileName(), size);

  BYTE* byteArray = (BYTE*) mappedFile();

  for (int people = 0; people < params.NumPeople(); people++)
    for (int snip = 0; snip < params.NumSnip(); snip++) {
      QWORD index = (QWORD(people) * QWORD(params.NumSnip())) + QWORD(snip);
      byteArray[index] = (people + snip) % 256;
    }
} // ReadMapFile

void ReadMapFile(const ParamConstT& params) {
  MappedFileT mappedFile(params.FileName());

  BYTE* byteArray = (BYTE*)mappedFile();

  for (int people = 0; people < params.NumPeople(); people++)
    for (int snip = 0; snip < params.NumSnip(); snip++) {
      QWORD index = (QWORD(people) * QWORD(params.NumSnip())) + QWORD(snip);
      if (byteArray[index] != (people + snip) % 256)
        abort();
    }
} // ReadMapFile

void ReadArrayOverLay(const ParamConstT& params) {
  MappedFileT mappedFile(params.FileName());
  ArrayRef2T arrayRef(mappedFile(), params.NumPeople(), params.NumSnip(), 1);

  for (int people = 0; people < params.NumPeople(); people++)
    for (int snip = 0; snip < params.NumSnip(); snip++) {
      BYTE readVal = *((BYTE*) arrayRef[people][snip]);
      BYTE expectVal = (people + snip) % 256;
      if (readVal != expectVal)
        abort();
    }
} // ReadMapFile

// Vestigial.  Not used.  But might think about increasing code reuse.
void CheckContent(void* addr, int numPeople, int numSnip) {
  BYTE* byteArray = (BYTE*)addr;

  for (int people = 0; people < numPeople; people++)
    for (int snip = 0; snip < numSnip; snip++) {
      QWORD index = (QWORD(people) * QWORD(numSnip)) + QWORD(snip);
      assert(byteArray[index] == (people + snip) % 256);
    }
} // CheckContent