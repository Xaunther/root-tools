#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
#include "TMath.h"
#include "Functions/misc.h"
using namespace std;
void CompareVar(string plotvar, string plotopt = "", string tupledir1 = "Directories/tuples.dir", string tupledir2 = "Directories/tuples2016.dir", string cutfile="Variables/Cuts.txt")
{
  int N_files1 = 0;
  int N_files2 = 0;
  string* filenames1 = ReadVariables(N_files1, tupledir1);
  string* filenames2 = ReadVariables(N_files2, tupledir2);
      
  string cuts = GetCuts(cutfile);
  //Data chain
  string treename1 = GetTreeName(tupledir1);
  string treename2 = GetTreeName(tupledir2);
  
  TChain* chain1 = new TChain(treename1.c_str());
  TChain* chain2 = new TChain(treename2.c_str());

  //Add to chain and get N of entries
  for(int i=0;i<N_files1;i++)
    {
      chain1->Add(filenames1[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }
  cout << chain1->GetEntries() << endl;
  //Add to chain and get N of entries
  for(int i=0;i<N_files2;i++)
    {
      chain2->Add(filenames2[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }
  cout << chain2->GetEntries() << endl;

  //Plot and save bro
  TCanvas* c1 = new TCanvas();
  cout << "Plotting..." << endl;
  chain1->SetLineColor(kRed);
  chain1->Draw(plotvar.c_str(), cuts.c_str(), plotopt.c_str());
  chain2->SetLineColor(kBlue);
  chain2->Draw(plotvar.c_str(), cuts.c_str(), (plotopt + " same").c_str());
  c1->SaveAs(("plots/"+plotvar+".pdf").c_str());
  //cout << htemp->GetEntries() << endl;
}
