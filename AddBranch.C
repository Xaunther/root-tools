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
void AddBranch(string branchname, string tupleinfile, string tupleoutfile, string treename, string var1, string var2 = "", string var3 = "", string var4 = "", string var5 = "")
{
  double branchvalue; //One should adapt the variable type to the branch requested Kplus_ProbNNp

  //Data chain
  TFile* infile = new TFile(tupleinfile.c_str());
  TTree* intree = (TTree*)infile->Get(treename.c_str());

  string var[5];
  var[0] = var1;
  var[1] = var2;
  var[2] = var3;
  var[3] = var4;
  var[4] = var5;

  int N_vars = 0;
  double varvalue[5];

  for(int i=0;i<5;i++)
    {
      if(var[i]!="")
	{
	  intree->SetBranchAddress(var[i].c_str(),&varvalue[N_vars]);
	  N_vars++;
	}
    }

  //Add new branch with year
  //For data
  TFile* file = new TFile(tupleoutfile.c_str(), "RECREATE");
  TTree* tree = intree->CloneTree();
  TBranch* newbranch = tree->Branch(branchname.c_str(), &branchvalue, (branchname+"/D").c_str());

  for(int i=0;i<intree->GetEntries();i++)
    {
      intree->GetEntry(i);
      tree->GetEntry(i);
      //Insert formula, if any
      branchvalue = -TMath::Log((1-varvalue[1]/varvalue[0])/(1+varvalue[1]/varvalue[0]))/2;
      newbranch->Fill();
      if(i%500000==0)
	{
	  cout << "Processing event: " << i << " / " << intree->GetEntries() << endl;
	}
    }
  tree->Print();
  tree->Write();

  file->Close();
  infile->Close();
  cout << "New branch added" << endl;
}
