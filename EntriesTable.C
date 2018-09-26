//Function to make a table with number of entries for a certain number of ntuples, provided by the user
//The string can either point to a directory file with the list of ntuples to run over, or a list of them, separated by commas
//A variable to scan has to be provided also by the user, as well as bottom and upper limits, and number of steps

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../Functions/StringTools.h"
#include "../Functions/TreeTools.h"
using namespace std;

#define col_width 7 //Spaces allowed for each number (max)
void EntriesTable(string samples, string varcut, double var_min, double var_max, int steps = 100)
{
  int N_samples = 0; //Number of samples (columns in output)
  string* files = SplitString(N_samples, samples, ", ");
  if(N_samples < 1)
    {
      cout << "No samples provided" << endl;
      return;
    }
  //Prepare chains and blah blah blah...
  TChain** chain = new TChain*[N_samples];
  for(int i=0;i<N_samples;i++)
    {
      chain[i] = GetChain(files[i], "", false);
    }

  //Output in screen
  for(int i=0;i<=steps;i++)
    {
      for(int j=0;j<N_samples;j++)
	{
	  double cut = var_min+(var_max-var_min)/steps*double(i);
	  stringstream ss;
	  ss << varcut << " " << cut;
	  cout << setfill(' ') << setw(col_width) << chain[j]->GetEntries(ss.str().c_str()) << " ";
	  ss.str("");
	}
      cout << endl;
    }

  return;
}
