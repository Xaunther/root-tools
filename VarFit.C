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
using namespace std;

void VarFit(string variablename, FitOption fitopt, string filedir, string cutfile = "")
{
  //I create an array of pointers to the fitting functions, so no need to change code in VarFit.C if we add more ;)
  FitFunction fitf[] = 
    {
      FitGauss_Exp,
      FitCB,
      FitDoubleCB,
      FitLb2NstG,
      FitNothing
    };
  /************************************
  *Function that returns this thing!  
  *Encode number of bkgs somewhere in workspace
  ************************************/
  RooWorkspace* ws = new RooWorkspace();

  int N_files = 0;
  string* filenames = ReadVariables(N_files, filedir);
  //Load TChain
  string cuts = GetCuts(cutfile);
  string treename = GetTreeName(filedir); 
  int N_bkgs;

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
  N_bkgs = -1;

  //Proceed to the plot
  GoodPlot(ws, variablename, N_bkgs);

  cout << temptree->GetEntries() << " events plotted" << endl;
}
