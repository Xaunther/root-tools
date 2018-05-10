#include <string>
#include "../Functions/Filereading.h"
#include "../Functions/Filesaving.h"
using namespace std;

void MakeVarList(string infile = "temp.txt", int col = 3, string outfile = "temp2.txt")
{
  int N_vars = 0;
  //Read the variable names
  string* varnames = ReadVariablesWord(N_vars, infile, col);
  SaveVariables(varnames, N_vars, outfile);
}
