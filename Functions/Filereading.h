//Declaration of ReadVariables functions
//Functions related to reading things from files

#ifndef Filereading_h
#define Filereading_h

#include <string>
#include <vector>

using namespace std;

//Structures used. If upgrading to object, create new header file
struct vartable
{
  bool abs;
  double variations[5];
};

int GetNVariables(string);
string *ReadVariables(int &, string);
vector<string> ReadVariables(string);
double *ReadNumbers(int &, string);
string *ReadVariablesWord(int &, string, int);
vartable *ReadVarTable(int, string);
double **GetData(string, int &, int);
string GetCuts(string);
string GetCutsVar(string, string, int, int, int);
void AdvanceLine(ifstream &, string);

#endif
