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
using namespace std;
void PlotUsedVars(string cutfile = "Variables/Cuts.txt", string plotopt = "", string tupledir = "tuples.dir")
{
  int N_files = 0;
  int N_vars = 0;
  string* varnames = ReadVariables(N_vars, "Variables/VariablesUsed.txt");
  string* filenames = ReadVariables(N_files, tupledir);

  string treename = GetTreeName(tupledir);
  string cuts = GetCuts(cutfile);

  //Data chain
  TChain* chain = new TChain(treename.c_str());

  //Add to chain and get N of entries
  for (int i = 0; i < N_files; i++)
  {
    chain->Add(filenames[i].c_str());
  }

  TCanvas* c1 = new TCanvas();
  for (int i = 0; i < N_vars; i++)
  {
    chain->Draw(varnames[i].c_str(), cuts.c_str(), plotopt.c_str());
    c1->SaveAs((varnames[i] + ".pdf").c_str());
  }
  c1->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 0:
    PlotUsedVars();
    break;
  case 1:
    PlotUsedVars(*(new string(argv[1])));
    break;
  case 2:
    PlotUsedVars(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    PlotUsedVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
