/*******************************************************************************
*Code to make mass substitutions (when the tupletool makes you angry)          *
*It is designed to work with 2hG final states (K pi gamma).                    *
*One make improve it to make it more general, though...                        *
*******************************************************************************/
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include <string>
#include "TMath.h"
#include "../Functions/TreeTools.h"
#include "../Functions/StringTools.h"
using namespace std;

void AddETA(string infile, string outfile, string varlist)
{
  //Data chain
  TChain* inchain = GetChain(infile);
  //Get list of particle names
  int N = 0;
  string* plist = SplitString(N, varlist, ",");
  //Array of P's and PZ's
  double* P = new double[N];
  double* PZ = new double[N];
  double eta;
  //Load the 2*N variables
  for(int i=0;i<N;i++)
    {
      inchain->SetBranchAddress((plist[i]+"_P").c_str(), &P[i]);
      inchain->SetBranchAddress((plist[i]+"_PZ").c_str(), &PZ[i]);
    }

  //Add new branches for each particle
  TFile* file = new TFile(outfile.c_str(), "RECREATE");
  TTree* tree = inchain->CloneTree();
  TBranch** newbranch = new TBranch*[N];
  for(int i=0;i<N;i++)
    {
      newbranch[i] = tree->Branch((plist[i]+"_eta").c_str(), &eta, (plist[i]+"_eta/D").c_str());
    }

  //FILL THY NTUPLE!
  for(int i=0;i<inchain->GetEntries();i++)
    {
      inchain->GetEntry(i);
      tree->GetEntry(i);
      for(int k=0;k<N;k++)
	{
	  //Pseudorapidity (eta = -0.5*log((1-x)/(1+x)), x = PZ/P) var1 = P, var2 = PZ
	  eta = -TMath::Log((1-PZ[k]/P[k])/(1+PZ[k]/P[k]))/2.;
	  newbranch[k]->Fill();
	}
    }
  tree->Write();

  file->Close();
  cout << "Pseudorapidity added" << endl;
}
