//Declaration of functions to do stuff with arrays
#ifndef ArrayTools_h
#define ArrayTools_h

#include <string>
using namespace std;

int GetMaxLength(string*, int);
int GetMaxPos(double*, int);
string MergeArray_OR(string*, int);
string MergeArray_AND(string*, int);
string MergeArray(string*, int, string);
bool IsInArray(string, string*, int);

//Template functions
template <typename T> double GetArrayMean(T*, int);
template <typename T> double GetArrayStdDev(T*, int);
template <typename T> double GetArraySum(T*, int);
template <typename T> double GetArraySumSq(T*, int);
#endif
