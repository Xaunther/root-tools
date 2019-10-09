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

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc)
    {
    case 0:
      PlotCorrVar();
      break;
    case 1:
      PlotCorrVar(*(new string(argv[1])));
      break;
    case 2:
      PlotCorrVar(*(new string(argv[1])), stoi(*(new string(argv[2]))));
      break;
    case 3:
      PlotCorrVar(*(new string(argv[1])), stoi(*(new string(argv[2]))), *(new string(argv[3])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for PlotCorrVar" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
