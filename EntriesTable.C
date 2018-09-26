//Function to make a table with number of entries for a certain number of ntuples, provided by the user
//The string can either point to a directory file with the list of ntuples to run over, or a list of them, separated by commas
//A variable to scan has to be provided also by the user, as well as bottom and upper limits, and number of steps

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "../Functions/StringTools.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
using namespace std;

#define col_width 7 //Spaces allowed for each number (max)
void EntriesTable(string samples, string varcut, double var_min, double var_max, int steps = 100, string output = "output.txt")
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

  //Get cuts
  string cuts = GetCuts(varcut);
  if(cuts.substr(cuts.size()-2, 1) == ")")
    {
      cuts = cuts.substr(0, cuts.size()-1); //Remove the last parenthesis if has been read from cutfile!
    }
  //Output in file
  ofstream outf;
  outf.open(output.c_str());
  for(int i=0;i<=steps;i++)
    {
      for(int j=0;j<N_samples;j++)
	{
	  double cut = var_min+(var_max-var_min)/steps*double(i);
	  stringstream ss;
	  ss << cuts << " " << cut;
	  outf << setfill(' ') << setw(col_width) << GetEvents(chain[j], ss.str()) << " ";
	  ss.str("");
	}
      outf << endl;
    }
  outf.close();
  return;
}
