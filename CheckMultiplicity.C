#include <string>
#include <iostream>
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
using namespace std;

void CheckMultiplicity(string filedir, string cutfile = "")
{
  TChain* chain = GetChain(filedir);
  string cuts = GetCuts(cutfile);
  TFile* file = TFile::Open("Tuples/temp.root", "recreate");
  TTree* temptree = (TTree*)chain->CopyTree(cuts.c_str());
  ULong64_t evtnumber;
  temptree->SetBranchAddress("eventNumber", &evtnumber);
  ULong64_t currentevt = 0;
  int* repeated_evtnumber = new int[temptree->GetEntries()];
  int N = 0;
  for(int i=0;i<temptree->GetEntries();i++)
    {
      repeated_evtnumber[i] = 0;
      temptree->GetEntry(i);
      if(i==0){currentevt=evtnumber; N++;}
      if(currentevt != evtnumber)
	{
	currentevt = evtnumber;
	N++;
      }
      repeated_evtnumber[N-1]++;
    }
  int N_repeated = 0;
  int* nCandidates = new int[N]; for(int i=0;i<N;i++){nCandidates[i]=0;}
  for(int i=0;i<N;i++)
    {
      if(repeated_evtnumber[i] > 1)
	{
	  N_repeated++;
	}
      nCandidates[repeated_evtnumber[i]-1]++;
    }
  double multiplicity = 0;
  for(int i=0;i<N;i++)
    {
      if(nCandidates[i]>0)
	{
	  cout << i+1 << "  |  " << nCandidates[i] << endl;
	  multiplicity += nCandidates[i]*(i+1);
	}
    }
  cout << "Found " << N_repeated << " events out of " << N << "  with multiplicity higher than 1 " << endl;
  cout << "Multiplicity: " << multiplicity/double(N) << endl;
  file->Close();
}
