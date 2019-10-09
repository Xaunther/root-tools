#include <string>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
using namespace std;

void ExpYield(string yield_file, string eff_file, string out_file, string cut = "Global")
{
  int N=0;
  double yield = stod(ReadVariables(N, yield_file)[0]);
  double eff = stod(GetValueFor(cut, eff_file));
  ofstream outf;
  outf.open(out_file.c_str());
  outf << yield*eff << endl;
  outf.close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 3:
      ExpYield(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    case 4:
      ExpYield(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for ExpYield" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
