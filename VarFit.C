#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "TChain.h"
#include "RooWorkspace.h"
#include "Functions/Fits.h"
using namespace std;

void VarFit(string variablename, FitOption fitopt, string filedir, string cutfile = "")
{
  FitFunction* fitf = FitFunction_init();
  RooWorkspace* ws = new RooWorkspace();

  int N_files = 0;
  string* filenames = ReadVariables(N_files, filedir);
  //Load TChain
  string cuts = GetCuts(cutfile);
  string treename = GetTreeName(filedir); 

  TChain* chain = new TChain(treename.c_str());
  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }

  //Cut chain into new TChain in a temp root file
  tempfile = new TFile("Tuples/temp.root", "recreate");
  temptree = (TTree*)chain->CopyTree(cuts.c_str());
  tempfile->Write();

  //Do fit depending on request
  ws = fitf[fitopt](variablename, temptree);

  //Proceed to the plot
  GoodPlot(ws, variablename);

  cout << temptree->GetEntries() << " events plotted" << endl;
}
