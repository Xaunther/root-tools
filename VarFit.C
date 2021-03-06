#include <iostream>
#include <string>
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "RooWorkspace.h"
#include "Functions/Fits.h"
#include "Functions/Dictreading.h"
#include "Functions/Filereading.h"
#include "Functions/TreeTools.h"
#include "Functions/PlotTools.h"
using namespace std;

void VarFit(string variablename, FitOption fitopt, string filedir, string cutfile = "", string w_var = "", string title = "", string Xtitle = "", string opts = "")
{
  FitFunction *fitf = FitFunction_init();
  RooWorkspace *ws = new RooWorkspace();

  //Load TChain
  string cuts = GetCuts(cutfile);

  TChain *chain = GetChain(filedir);
  TTree *temptree = new TTree();
  TFile *tempfile = new TFile();

  //Cut chain into new TChain in a temp root file
  tempfile = new TFile("Tuples/temp.root", "recreate");
  temptree = (TTree *)chain->CopyTree(cuts.c_str());
  tempfile->Write();

  //If opts not specified, use default
  if (opts == "")
  {
    opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
  }
  //Do fit depending on request
  ws = fitf[fitopt](variablename, temptree, w_var, 0, 0, opts);

  GoodPlot(ws, variablename, title, Xtitle, opts);

  cout << temptree->GetEntries() << " events plotted" << endl;

  CloseChain(chain);
  delete temptree;
  delete tempfile;
  delete ws;
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
  FitOption fitopt = StringToFitOption(*(new string(argv[2])));
  switch (argc - 1)
  {
  case 3:
    VarFit(*(new string(argv[1])), fitopt, *(new string(argv[3])));
    break;
  case 4:
    VarFit(*(new string(argv[1])), fitopt, *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    VarFit(*(new string(argv[1])), fitopt, *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    VarFit(*(new string(argv[1])), fitopt, *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  case 7:
    VarFit(*(new string(argv[1])), fitopt, *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
  case 8:
    VarFit(*(new string(argv[1])), fitopt, *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
