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
void AddBranch(string branchname, string tupledir = "tuples.dir")
{
  int N_files = 0;
  double branchvalue; //One should adapt the variable type to the branch requested Kplus_ProbNNp
  string* filenames = ReadVariables(N_files, tupledir);

  //Data chain
  TChain* chain = new TChain("kstGTuple/DecayTree");

  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
    }

  chain->SetBranchAddress(branchname.c_str(),&branchvalue);

  //Add new branch with year
  //For data
  TFile* file = new TFile("Tuples/Data.root", "UPDATE");
  TTree* tree = (TTree*)file->Get("DecayTree");
  TBranch* newbranch = tree->Branch(branchname.c_str(), &branchvalue, (branchname+"/D").c_str());

  for(int i=0;i<tree->GetEntries();i++)
    {
      tree->GetEntry(i);
      chain->GetEntry(i);
      newbranch->Fill();
      if(i%500000==0)
	{
	  cout << "Processing event: " << i << " / " << tree->GetEntries() << endl;
	}
    }
  tree->Print();
  tree->Write();

  file->Close();
  cout << "New branch added" << endl;
}
