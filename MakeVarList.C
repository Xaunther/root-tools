#include <string>
#include "Functions/Filereading.h"
#include "Functions/Filesaving.h"
using namespace std;

void MakeVarList(string infile = "temp.txt", int col = 3, string outfile = "temp2.txt")
{
  int N_vars = 0;
  //Read the variable names
  string* varnames = ReadVariablesWord(N_vars, infile, col);
  SaveVariables(varnames, N_vars, outfile);
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 0:
    MakeVarList();
    break;
  case 1:
    MakeVarList(*(new string(argv[1])));
    break;
  case 2:
    MakeVarList(*(new string(argv[1])), stoi(*(new string(argv[2]))));
    break;
  case 3:
    MakeVarList(*(new string(argv[1])), stoi(*(new string(argv[2]))), *(new string(argv[3])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
