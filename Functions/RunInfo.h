#ifndef RunInfo_h
#define RunInfo_h

#include <string>
using namespace std;

//Enumerations
enum RunNumber {I, II, All};

string GetRunCut(RunNumber, string);
RunNumber StringToRunNumber(string);
#endif
