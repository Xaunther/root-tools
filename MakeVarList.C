#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Functions/misc.h"
using namespace std;

void MakeVarList()
{
  int N_vars = 0;
  //Read the variable names
  string* varnames = ReadVariablesWord(N_vars, "temp.txt", 3);
  SaveVariables(varnames, N_vars, "temp2.txt");
}
