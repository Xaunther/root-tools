#include <string>
#include <fstream>
#include <stdlib.h>
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
