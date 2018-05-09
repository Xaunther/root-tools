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
using namespace std;
void PlotCorrVar(string drawopt = "", string varfile = "Variables/VariablesCorr.txt", string cutfile="Variables/Cuts.txt", string tupledir = "Directories/Cuttuples2.dir")
{
  int N_files = 0;
  int N_vars = 0;
  string* filenames = ReadVariables(N_files, tupledir);
  string* variables = ReadVariables(N_vars, varfile);

  string cuts = GetCuts(cutfile);
  //Data chain
  TChain* chain = new TChain("DecayTree");

  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }
  //Plot and save bro
  TCanvas* c1 = new TCanvas();
  cout << "Plotting..." << endl;
  for(int i=0;i<N_vars;i++)
    {
      for(int j=i+1;j<N_vars;j++)
	{
	  chain->Draw((variables[i]+":"+variables[j]).c_str(), cuts.c_str(), drawopt.c_str());
	  c1->SaveAs(("plots/"+variables[i]+"_VS_"+variables[j]+".pdf").c_str());
	}
    }
}
