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
  temptree->SetBranchAddress("EventInSequence", &evtnumber);
  ULong64_t* id_evtnumber = new ULong64_t[temptree->GetEntries()];
  int* repeated_evtnumber = new int[temptree->GetEntries()];
  int N = 0;
  for(int i=0;i<temptree->GetEntries();i++)
    {
      bool wasadded = false;
      repeated_evtnumber[i] = 0;
      temptree->GetEntry(i);
      for(int j=0;j<N;j++)
	{
	  if(id_evtnumber[j]==evtnumber)
	    {
	      repeated_evtnumber[j]++;
	      wasadded = true;
	    }
	}
      if(!wasadded)
	{
	  id_evtnumber[N] = evtnumber;
	  repeated_evtnumber[N]++;
	  N++;
	}

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
