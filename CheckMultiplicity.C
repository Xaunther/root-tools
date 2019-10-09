#include <string>
#include <iostream>
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/Probability.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
using namespace std;

//Function to check multiplicity in an ntuple. It identifies consecutive entries with same evtnumber as coming from the same event. If they are not consecutive they are marked as coming from different events (as an ntuple might be formed of several subjobs)
void CheckMultiplicity(string filedir, string cutfile = "")
{
  TChain* chain = GetChain(filedir);
  string cuts = GetCuts(cutfile);
  TFile* file = TFile::Open("Tuples/temp.root", "recreate");
  TTree* temptree = (TTree*)chain->CopyTree(cuts.c_str()); //Copy all events with weight > 0
  ULong64_t evtnumber;
  temptree->SetBranchAddress("eventNumber", &evtnumber);
  ULong64_t currentevt = 0;
  int* repeated_evtnumber = new int[temptree->GetEntries()];
  int N = 0;

  //Define TTreeFormula. Returns weight of each event
  TTreeFormula* formulavar = new TTreeFormula(cuts.c_str(), cuts.c_str(), temptree);

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
  double multiplicity = 0;
  //Now we want to keep track of the weight of each event, like this
  int N_entry = 0;
  int j;
  for(int i=0;i<N;i++)
    {
      //For each event we have to compute the joint probability of each of its candidates. Call a function to do so
      double* w_array = new double[repeated_evtnumber[i]];
      for(j = N_entry;j<N_entry+repeated_evtnumber[i];j++)
	{
	  temptree->GetEntry(j);
	  w_array[j-N_entry] = formulavar->EvalInstance();
	  //Make sure it is between 0 and 1
	  if(w_array[j-N_entry]<0)
	    {
	      w_array[j-N_entry] = 0.;
	    }
	  else if(w_array[j-N_entry]>1)
	    {
	      w_array[j-N_entry] = 1.;
	    }
	}
      N_entry = j;
      multiplicity += JointProbIndependent(w_array, repeated_evtnumber[i]);
      
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
  cout << "Multiplicity: " << GetMeanEntries(temptree, cuts)*double(temptree->GetEntries())/multiplicity << endl;
  file->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 1:
      CheckMultiplicity(*(new string(argv[1])));
      break;
    case 2:
      CheckMultiplicity(*(new string(argv[1])), *(new string(argv[2])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for CheckMultiplicity" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
