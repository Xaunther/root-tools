#ifndef Dictreading_h
#define Dictreading_h

#include <string>
using namespace std;

string GetValueFor(string, string);
string GetErrorFor(string, string);
string GetColFor(string, string, int);
string GetTreeName(string, string = "DecayTree");
string StringSearch(string, string, int = 0, int = 1);

#endif
