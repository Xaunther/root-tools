#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
include <sstream>
#include "TMath.h"
#include "Functions/misc.h"
using namespace std;
void Merge(string outfile, string filename = "Variables/VariablesUsed.txt", string tupledir = "Directories/Bu2KstG_tuples.dir")
{
  int N_variables = 0;
  int N_files = 0;

  string* variable_list = ReadVariables(N_variables, filename);
  string* filenames = ReadVariables(N_files, tupledir);

  //Data chain
  string treename = GetTreeName(tupledir); 
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
  cout << "Merging ... " << endl;
  chain->Merge(outfile.c_str());
  cout << "Data merged" << endl;
}
