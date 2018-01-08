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

void VarFit(string variablename, string fitopt, string filedir, string cutfile = "")
{
  RooWorkspace* ws();

  int N_files = 0;
  string* filenames = ReadVariables(N_files, filedir);
  //Load TChain
  string cuts = GetCuts(cutfile);
  string treename = GetTreeName(filedir); 
  int N_part = GetNPart(filedir);
  int N_part_plot = GetNPartPlot(variablename);
  if(N_part_plot==0)
    {
      N_part_plot = N_part;
    }
  //If not a common opt, add manually here
  //treename=""

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
  if(fitopt=="Gauss-exp") //Gaussian signal with exp bkg
    {
      ws = FitGauss_Exp(variablename, temptree);
      GoodPlot(ws, variablename, 1, N_part, N_part_plot);
    }
  else if(fitopt=="CB") //CB 1 sided
    {
      ws = FitCB(variablename, temptree);
      GoodPlot(ws, variablename, 0, N_part, N_part_plot);
    }
  else if(fitopt=="DoubleCB") //CB 2 sided
    {
      ws = FitDoubleCB(variablename, temptree);
      GoodPlot(ws, variablename, 0, N_part, N_part_plot);
    }
  else if(fitopt=="Lb2NstG") //Lb2NstG fit
    {
      ws = FitLb2NstG(variablename, temptree);
      GoodPlot(ws, variablename, 100, N_part, N_part_plot);
    }
  else
    {
      FitNothing(variablename, temptree, N_part, N_part_plot);
    }
  cout << temptree->GetEntries() << " events plotted" << endl;
}
