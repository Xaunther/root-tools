#include <string>
#include <iostream>
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
using namespace std;

void CheckMultiplicity(string filedir, string cutfile = "")
{
  TChain* chain = GetChain(filedir);
  string cuts = GetCuts(cutfile);
  TTree* temptree = (TTree*)chain->CopyTree(cuts.c_str());
  ULong64_t evtnumber;
  UInt_t ncand;  temptree->SetBranchAddress("nCandidate", &ncand);
  temptree->SetBranchAddress("eventNumber", &evtnumber);
  ULong64_t currentevt = 0;
  int* repeated_evtnumber = new int[temptree->GetEntries()];
  int N = 0;
  for(int i=0;i<temptree->GetEntries();i++)
    {
      repeated_evtnumber[i] = 0;
      temptree->GetEntry(i);
      if(i==0){currentevt=evtnumber;}
      if(currentevt != evtnumber)
	{
	currentevt = evtnumber;
	N++;
      }
      repeated_evtnumber[N]++;
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
  for(int i=0;i<N;i++)
    {
      if(nCandidates[i]>0)
	{
	  cout << i+1 << "  |  " << nCandidates[i] << endl;
	}
    }
  cout << "Found " << N_repeated << " events out of " << N << "  with multiplicity higher than 1 " << endl;
}
