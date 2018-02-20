#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "Functions/misc.h"
using namespace std;

void CutTree(string outputfile = "Tuples/cuttree.root", string cutsfilename = "Variables/PreCuts.txt", string tupledir = "Directories/Alltuples.dir", string variablefile = "")
{
  string cuts = GetCuts(cutsfilename);
  cout << cuts << endl;

  int N_files = 0;
  string* filenames = ReadVariables(N_files, tupledir);

  //Variables Used
  int N_variables = 0;
  string* variable_list = ReadVariables(N_variables, variablefile);

  //Data chain
  string treename = GetTreeName(tupledir);
  cout << "Reading Tree " << treename << endl;
  TChain* chain = new TChain(treename.c_str());

  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
    }

  //Select only desired variables if any are selected. Else, select all
  if (N_variables > 0)
    {
      chain->SetBranchStatus("*",0);
      for(int i=0;i<N_variables;i++)
	{
	  chain->SetBranchStatus(variable_list[i].c_str(), 1);
	}
    }

  //Cut chain into new TChain in a temp root file
  TFile* cutfile = new TFile(outputfile.c_str(), "recreate");
  TTree* cuttree = (TTree*)chain->CopyTree(cuts.c_str());
  cutfile->Write();
  
  cout << "Tree cut" << endl;
  cutfile->Close();
}
