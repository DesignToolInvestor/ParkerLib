/*
  M a p T e s t . c p p
*/

// Seems to be needed up front
#include <windows.h>

// Modern C++
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iomanip>
#include <deque>

// Local code units
#include "Timmer.h"
#include "MappedFile.h"
#include "MapTest.h"
#include "TestHelper.h"
#include "FileSys.h"
#include "DateForm.h"

using namespace std;

/*
  Little support functions
*/
// ToDo:  Check errors
ParamConstT ParseArg(int argc, char* argv[]) {
  assert(argc == 5);

  int numPeople = stoi(string(argv[1]));
  int numSnip = stoi(string(argv[2]));

  return ParamConstT(numPeople, numSnip, argv[3], argv[4]);
} // ParseArg

void Report(const char* heading, float seconds, const ParamConstT& param) {
  SizeT size = SizeT(param.NumPeople()) * SizeT(param.NumSnip());
  float rate = size / seconds;

  cout << heading << '\n';
  cout << "  Time = " << setprecision(3) << seconds << " seconds\n";
  cout << "  Rate = " << setprecision(3) << rate << " byte per second\n";
  cout << '\n';
} // Report

void AppendLog(const ParamConstT& param, float seconds, const char* logFileName) {
  ofstream logStream;
  logStream.open(logFileName, ios::out | ios::app);

  SYSTEMTIME now;
  GetSystemTime(&now);

  // This is a C-style preprocessor macros that are set in the build script, as compiler directives
  const char* buildConfig = _BUILD_CONFIG;

  logStream << param.NumPeople() << ", " << param.NumSnip() << ", " << seconds << ", "
    << DateFormT(now) << ", " << buildConfig <<", " << param.RunEnv() << '\n';
  
  logStream.close();
} // AppendLog

/*
  Top level test blocks

  ToDo:  There is too much code copping in this section.
  ToDo:  Code it with function templates without a class.
  ToDo:  Code it using inheritance (think this will lose to function templates, but should try both ways)
*/
void ReportCreateOsByte(const ParamConstT& param) {
  TickCountT start = ReadTicks();
  CreateOldSchoolFile(param);
  TickCountT ticks = ReadTicks() - start;
  float seconds = TicksToSecs(ticks);
  
  Report("Creating File, OS Medeated, One Byte at a Time", seconds, param);
  AppendLog(param, seconds, "OsWriteByte.log");
} // ReportCreateOsByte

void ReportReadOsByte(const ParamConstT& param) {
  TickCountT start = ReadTicks();
  CreateOldSchoolFile(param);
  TickCountT ticks = ReadTicks() - start;
  float seconds = TicksToSecs(ticks);

  Report("Reading File, OS Medeated, One Byte at a Time", seconds, param);
  AppendLog(param, seconds, "OsReadByte.log");
} // ReportReadOsByte

void ReportCreateMapFile(const ParamConstT& param) {
  TickCountT start = ReadTicks();
  CreateMapFile(param);
  TickCountT ticks = ReadTicks() - start;
  float seconds = TicksToSecs(ticks);

  Report("Creating File, Mapped File", seconds, param);
  AppendLog(param, seconds, "MapWriteByte.log");
} // ReportReadMemMap

void ReportReadMapFile(const ParamConstT& param) {
  TickCountT start = ReadTicks();
  ReadMapFile(param);
  TickCountT ticks = ReadTicks() - start;
  float seconds = TicksToSecs(ticks);

  Report("Reading File, Mapped File", seconds, param);
  AppendLog(param, seconds, "MapReadByte.log");
} // ReportReadMemMap

void ReportReadArrayOverLay(const ParamConstT& param) {
  TickCountT start = ReadTicks();
  ReadArrayOverLay(param);
  TickCountT ticks = ReadTicks() - start;
  float seconds = TicksToSecs(ticks);

  Report("Reading File, Mapped File", seconds, param);
  AppendLog(param, seconds, "CppReadByte.log");
} // ReportReadArrayOverLay

/*
  Groups of tests
*/
const char* DIR_NAME = "C:\\Work Space\\";
const char* FILE_NAME_MASK = "Mapped File *.data";
const char* PRE_NAME = "C:\\Work Space\\Mapped File ";

const char* RUN_ENV = "W1";    // C drive => W1, F drive => W0 

const double MIN_SIZE = 10E6;
const double MAX_SIZE = 10e9;

const int SNIP_PER_PERSON = 10;

const int MIN_NUM_PEOPLE = int(round(sqrt(MIN_SIZE / SNIP_PER_PERSON)));
const int MAX_NUM_PEOPLE = int(round(sqrt(MAX_SIZE / SNIP_PER_PERSON)));
const SizeT TOTAL_SIZE_LIM = SizeT(round(70E9));

int* LogRandRuns(int minNumPeople, int maxNumPeople, SizeT totalSizeLim, int& numRuns) {
  SizeT totalSize = 0;
  numRuns = 0;
  BOOL done = false;

  deque<int> queue;

  double logMin = log(minNumPeople);
  double logMax = log(maxNumPeople);
  double logRange = logMax - logMin;
  do {
    double logNumPeople = logRange * rand()/double(RAND_MAX) + logMin;
    int numPeople = int(round(exp(logNumPeople)));
    SizeT size = SizeT(numPeople) * SizeT(numPeople) * SizeT(SNIP_PER_PERSON);

    if (totalSizeLim < totalSize + size)
      done = true;
    else {
      queue.push_back(numPeople);
      numRuns++;
      totalSize += size;
    }
  } while (!done);

  int* result = new int[numRuns];
  for (int count = 0; count < numRuns; count++) {
    result[count] = queue.front();
    queue.pop_front();
  }
  
  return result;
} // LogRand

// ToDo:  combine these two functions so that the common parts are reused.
void BulkWrite(int minNumPeople, int maxNumPeople, SizeT totalSizeLim, const char* fullPreName) {
  int numFile;
  int* numPeopleEachFile = LogRandRuns(MIN_NUM_PEOPLE, MAX_NUM_PEOPLE, TOTAL_SIZE_LIM, numFile);

  for (int fileNum = 0; fileNum < numFile; fileNum++) {
    unsigned numPeople = numPeopleEachFile[fileNum];
    unsigned numSnip = SNIP_PER_PERSON * numPeople;

    stringstream fullFileName;
    fullFileName << fullPreName << numPeople << ".data";
    string fullFileNameStr = fullFileName.str();

    ParamConstT param(numPeople, numSnip, fullFileNameStr.c_str(), RUN_ENV);
    ReportCreateMapFile(param);
  }
} // BulkWrite

void BulkRead(const char* dirName, const char* fileNameMask) {
  int numFile;
  string fullMask = string(dirName) + string(fileNameMask);
  FileInfoT* files = DirListNew(fullMask.c_str(), numFile);

  for (int fileNum = 0; fileNum < numFile; fileNum++) {
    int numPeople = files[fileNum].numPeople;
    int numSnip = SNIP_PER_PERSON * numPeople;
    const char* fileName = files[fileNum].fileName;
    assert(files[fileNum].fileSize = SizeT(numPeople) * SizeT(numSnip));

    string fullFileName = string(DIR_NAME) + string(fileName);
    ParamConstT param(numPeople, numSnip, fullFileName.c_str(), RUN_ENV);

    ReportReadMapFile(param);
  }

  DirListDelete(files, numFile);
} // BulkRead

/*
  main

  This is sort of a scratch area for experimentation.  If working in an IDE and not doing too many runs
  it is easier to modify the commands here than to do stuff on the command line.  If you start to do many
  run, then this should be cleaned up, i.e., more arguments and multiple executables for more friendly
  command line experimentation.
*/
int main(int argc, char* argv[])
{
  // Multiple tests on a single file
  //ParamConstT param;
  //ParseArg(argc,argv, param);

  //long long size = ((long long) param.NumPeople()) * ((long long) param.NumSnip());
  //  cout << "  Size = " << setprecision(3) << float(size) << " byte\n\n";

  // Various top test ... kludgey but comment out the ones you don't want, not worth fixing at this time.
  //ReportCreateOsByte(param);
  //ReportReadOsByte(param);
  //ReportCreateMapFile(param);
  //ReportReadMapFile(param);
  //ReportReadArrayOverLay(param);

  //BulkWrite(MIN_NUM_PEOPLE, MAX_NUM_PEOPLE, TOTAL_SIZE_LIM, PRE_NAME);
  BulkRead(DIR_NAME, FILE_NAME_MASK);

  return 0;
} // main