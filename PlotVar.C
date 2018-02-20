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
#include "Functions/misc.h"
using namespace std;
void PlotVar(string plotvar, string plotopt = "", string tupledir = "Directories/Bd2KstG_tuples.dir", string cutfile="Variables/Cuts.txt")
{
  int N_files = 0;
  string* filenames = ReadVariables(N_files, tupledir);
      
  string cuts = GetCuts(cutfile);
  //Data chain
  string treename = GetTreeName(tupledir);
  TChain* chain = new TChain(treename.c_str());

  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }
  cout << chain->GetEntries() << endl;
  //Plot and save bro
  TCanvas* c1 = new TCanvas();
  cout << "Plotting..." << endl;
  chain->Draw(plotvar.c_str(), cuts.c_str(), plotopt.c_str());
  c1->SaveAs(("plots/"+plotvar+".pdf").c_str());
  //cout << htemp->GetEntries() << endl;
}
