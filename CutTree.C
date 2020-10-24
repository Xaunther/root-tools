#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "Functions/Dictreading.h"
#include "Functions/Filereading.h"
#include "Functions/TreeTools.h"
#include "Functions/StringTools.h"
using namespace std;

void CutTree(string outputfile, string cutsfilename = "", string tupledir = "", string variablefile = "", int fentries = 1, int initentries = 1)
{
  string cuts = GetCuts(cutsfilename);
  cout << cuts << endl;

  //Variables Used
  int N_variables = 0;
  string* variable_list = ReadVariables(N_variables, variablefile);

  //Data chain
  TChain* chain = GetChain(tupledir);
  TFile* cutfile = new TFile(Gridify(outputfile).c_str(), "recreate");
  if (chain->GetEntries() == 0)
  {
    exit(0);
  }
  //Select only desired variables if any are selected. Else, select all
  if (N_variables > 0)
  {
    chain->SetBranchStatus("*", 0);
    for (int i = 0; i < N_variables; i++)
    {
      chain->SetBranchStatus(variable_list[i].c_str(), 1);
    }
  }

  //Cut chain into new TChain in a temp root file
  TTree* cuttree = (TTree*)chain->CopyTree(cuts.c_str(), "", long(chain->GetEntries() / double(fentries)), long(double((initentries - 1) * chain->GetEntries()) / fentries));
  cutfile->cd();
  cuttree->Write();

  cout << "Tree cut" << endl;
  cutfile->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 1:
    CutTree(*(new string(argv[1])));
    break;
  case 2:
    CutTree(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    CutTree(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    CutTree(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    CutTree(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), stoi(*(new string(argv[5]))));
    break;
  case 6:
    CutTree(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), stoi(*(new string(argv[5]))), stoi(*(new string(argv[6]))));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
