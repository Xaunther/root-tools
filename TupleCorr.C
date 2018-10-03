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

void TupleCorr(string tuplefile, string treelist, string cutfile = "")
{
  int N_trees = 0;
  string* treename = SplitString(N_trees, treelist, ", ");
  if(N_trees < 1)
    {
      cout << "No trees provided" << endl;
      return;
    }
  string cuts = GetCuts(cutfile);
  //Define array of TChains for each tree
  TChain** chain = new TChain*[N_trees];
  //Define correlation matrix
  int** corr_matrix = new int*[N_trees];
  for(int i=0;i<N_trees;i++)
    {
      corr_matrix[i] = new int[N_trees];
      for(int j=0;j<N_trees;j++)
	{
	  corr_matrix[i][j] = 0;
	}
    }
  //Cannot do TChain for all the ntuples, because this variable resets for each subjob (subtuple)
  int N_tuples = 0;
  string* filename = ReadVariables(N_tuples, tuplefile);

  //But we loop over each subtuple :)
  for(int i=0;i<N_tuples;i++)
    {
      for(int j=0;j<N_trees;j++)
	{
	  chain[j] = GetChain(filename[i], treename[j], false);
	}
      //Read the trees in this file
      for(int j=0;j<N_trees;j++)
	{
	  //Compare all vs all
	  for(int k=j;k<N_trees;k++)
	    {
	      if(chain[j]->GetEntries(cuts.c_str())!=0 && chain[k]->GetEntries(cuts.c_str())!=0)
		{
		  if(j==k)
		    {
		      corr_matrix[j][k]+=GetEvents(chain[j], cuts);
		    }
		  else
		    {
		      corr_matrix[j][k]+=GetCorrEvents(chain[j], chain[k], cuts);
		      corr_matrix[k][j] = corr_matrix[j][k]; //Symmetric!
		    }
		}
	    }
	}
    }
  
  //Output matrix!!
  for(int i=0;i<N_trees;i++)
    {
      for(int j=0;j<N_trees;j++)
	{
	  cout << setfill(' ') << setw(col_width) << corr_matrix[i][j] << " ";
	}
      cout << endl;
    }
  return;
}
