//Function to compute the event correlation (events present in both trees) of a list of TTrees (comma separated), defined in the same file (ntuple)
//The file can be just one or a list, given by a dictionary

#include <string>
#include <iostream>
#include <iomanip>
#include "../Functions/StringTools.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
using namespace std;

#define col_width 5

void TupleCorr(string tuplelist, string treelist, string cutlist = "")
{
  //Files
  /************/
  //Can do TChain for all the ntuples, because yes
  int N_tuplefiles = 0;
  string* tuplefile = SplitString(N_tuplefiles, tuplelist, ", ");
  if(N_tuplefiles < 1)
    {
      cout << "No trees provided" << endl;
      return;
    }
  //Define array of TChains for each tree
  TChain** chain = new TChain*[N_tuplefiles];

  //Treenames
  /************/
  int N_trees = 0;
  string* treename = SplitString(N_trees, treelist, ", ");

  for(int i=0;i<N_tuplefiles;i++)
    {
      if(i>=N_trees)
	{
	  chain[i] = GetChain(tuplefile[i], "DecayTree", false);
	}
      else
	{
	  chain[i] = GetChain(tuplefile[i], treename[i], false);
	}
    }

  //Cuts
  /***************/
  int N_cuts = 0;
  string* cutfile = SplitString(N_cuts, cutlist, ", ");
  string* cuts = new string[N_tuplefiles];
  //If not enough cuts provided, assume none were given
  for(int i=0;i<N_tuplefiles;i++)
    {
      if(i>=N_cuts)
	{
	  cuts[i] = "";
	}
      else
	{
	  cuts[i] = GetCuts(cutfile[i]);
	}
    }

  //Define correlation matrix
  int** corr_matrix = new int*[N_tuplefiles];
  for(int i=0;i<N_tuplefiles;i++)
    {
      corr_matrix[i] = new int[N_tuplefiles];
    }

  //Loop over different chains
  for(int j=0;j<N_tuplefiles;j++)
    {
      //Compare all vs all
      for(int k=j;k<N_tuplefiles;k++)
	{
	  if(chain[j]->GetEntries(cuts[j].c_str())!=0 && chain[k]->GetEntries(cuts[k].c_str())!=0)
	    {
	      if(j==k)
		{
		  corr_matrix[j][k] = GetEvents(chain[j], cuts[j]);
		}
	      else
		{
		  corr_matrix[j][k] = GetCorrEvents(chain[j], chain[k], cuts[j], cuts[k]);
		  corr_matrix[k][j] = corr_matrix[j][k]; //Symmetric!
		}
	    }
	}
    }
  
  //Output matrix!!
  for(int i=0;i<N_tuplefiles;i++)
    {
      for(int j=0;j<N_tuplefiles;j++)
	{
	  cout << setfill(' ') << setw(col_width) << corr_matrix[i][j] << " ";
	}
      cout << endl;
    }
  return;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 2:
      TupleCorr(*(new string(argv[1])), *(new string(argv[2])));
      break;
    case 3:
      TupleCorr(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for TupleCorr" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
