#include <iostream>
#include <string>
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "RooWorkspace.h"
#include "../Functions/Fits.h"
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/PlotTools.h"
using namespace std;

void VarFit(string variablename, FitOption fitopt, string filedir, string cutfile = "", string w_var = "", string title = "", string Xtitle = "")
{
  FitFunction* fitf = FitFunction_init();
  RooWorkspace* ws = new RooWorkspace();

  int N_files = 0;
  string* filenames = ReadVariables(N_files, filedir);
  //Load TChain
  string cuts = GetCuts(cutfile);

  TChain* chain = GetChain(filedir);
  TTree* temptree = new TTree();
  TFile* tempfile = new TFile();

  //Cut chain into new TChain in a temp root file
  tempfile = new TFile("Tuples/temp.root", "recreate");
  temptree = (TTree*)chain->CopyTree(cuts.c_str());
  tempfile->Write();

  //Do fit depending on request
  ws = fitf[fitopt](variablename, temptree, w_var, 0, 0);

  //Proceed to the plot
  GoodPlot(ws, variablename, true, title, Xtitle);

  cout << temptree->GetEntries() << " events plotted" << endl;
}
