#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "Functions/misc.h"
using namespace std;

void CutTree(string outputfile = "Tuples/cuttree.root", string cutsfilename = "Variables/PreCuts.txt", string tupledir = "Directories/Alltuples.dir")
{
  string cuts = GetCuts(cutsfilename);
  cout << cuts << endl;

  int N_files = 0;
  string* filenames = ReadVariables(N_files, tupledir);

  //Data chain
  string treename = GetTreeName(tupledir);
  TChain* chain = new TChain(treename.c_str());

  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
    }

  //Cut chain into new TChain in a temp root file
  TFile* cutfile = new TFile(outputfile.c_str(), "recreate");
  TTree* cuttree = (TTree*)chain->CopyTree(cuts.c_str());
  cutfile->Write();
  
  cout << "Tree cut" << endl;
  cutfile->Close();
}
