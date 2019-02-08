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
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
using namespace std;

void CutTree(string outputfile = "Tuples/cuttree.root", string cutsfilename = "Variables/PreCuts.txt", string tupledir = "Directories/Alltuples.dir", string variablefile = "", int fentries = 1, int initentries = 1)
{
  string cuts = GetCuts(cutsfilename);
  cout << cuts << endl;

  int N_files = 0;
  string* filenames = ReadVariables(N_files, tupledir);

  //Variables Used
  int N_variables = 0;
  string* variable_list = ReadVariables(N_variables, variablefile);

  //Data chain
  TChain* chain = GetChain(tupledir);
  TFile* cutfile = new TFile(outputfile.c_str(), "recreate");
  if(chain->GetEntries()==0)
    {
      exit(0);
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
  TTree* cuttree = (TTree*)chain->CopyTree(cuts.c_str(), "", long(chain->GetEntries()/double(fentries)), long(double((initentries-1)*chain->GetEntries())/fentries));
  cutfile->cd();
  cuttree->Write();

  cout << "Tree cut" << endl;
  cutfile->Close();
}
