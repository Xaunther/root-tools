#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
#include "TMath.h"
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
using namespace std;
void PlotUsedVars(string varlist, string tupledir, string cutfile = "Variables/Cuts.txt", string plotopt = "", string prefix = "", string treename = "")
{
  int N_vars = 0;
  string *varnames = ReadVariables(N_vars, varlist);

  string cuts = GetCuts(cutfile);

  //Data chain
  TChain *chain = GetChain(tupledir, treename);

  TCanvas *c1 = new TCanvas();
  for (int i = 0; i < N_vars; i++)
  {
    chain->Draw(varnames[i].c_str(), cuts.c_str(), plotopt.c_str());
    c1->SaveAs((prefix + varnames[i] + ".pdf").c_str());
  }
  c1->Close();
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
  switch (argc - 1)
  {
  case 2:
    PlotUsedVars(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    PlotUsedVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    PlotUsedVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    PlotUsedVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    PlotUsedVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
