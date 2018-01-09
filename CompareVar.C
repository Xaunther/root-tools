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
using namespace std;

void CompareVar(string variablename, string filedir1, string filedir2, string cutfile1 = "", string cutfile2 = "")
{
  int N_files1 = 0;
  int N_files2 = 0;
  string* filenames1 = ReadVariables(N_files1, filedir1);
  string* filenames2 = ReadVariables(N_files2, filedir2);
  //Load TChain
  string cuts1 = GetCuts(cutfile1);
  string cuts2 = GetCuts(cutfile2);
  string treename1 = GetTreeName(filedir1); 
  string treename2 = GetTreeName(filedir2); 
  int N_part = GetNPart(filedir1);
  int N_part_plot = GetNPartPlot(variablename);
  if(N_part_plot==0)
    {
      N_part_plot = N_part;
    }
  //If not a common opt, add manually here
  //treename=""

  TChain* chain1 = new TChain(treename1.c_str());
  TChain* chain2 = new TChain(treename2.c_str());
  //Add to chain and get N of entries
  for(int i=0;i<N_files1;i++)
    {
      chain1->Add(filenames1[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }
  for(int i=0;i<N_files2;i++)
    {
      chain2->Add(filenames2[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }
  //Cut chain into new TChain in a temp root file
  TFile* tempfile1 = new TFile("Tuples/temp1.root", "recreate");
  TTree* temptree1 = (TTree*)chain1->CopyTree(cuts1.c_str());

  TFile* tempfile2 = new TFile("Tuples/temp2.root", "recreate");
  TTree* temptree2 = (TTree*)chain2->CopyTree(cuts2.c_str());

  //Do fit depending on request
  MultiPlot(variablename, temptree1, temptree2);
  cout << temptree1->GetEntries() << " events plotted" << endl;
  cout << temptree2->GetEntries() << " events plotted" << endl;
}
