//Function to compute the value of something, looped over a series of files
#include <string>
#include <fstream>
#include "../Functions/misc.h"
#include "TChain.h"

using namespace std;

//Operation to do at each loop (editable)
Long64_t DoOperation(TChain* chain, string file)
{
  string cuts = GetCuts(file);
  return chain->GetEntries(cuts.c_str());
}


void Compute_Value(string datadir, string filetoloop, string outfilename = "result_value.txt")
{
  int N_loops = 0;
  int N_tuples = 0:
  ofstream outfile;

  //Get thing to loop over (string of filenames by default)
  string* filename = ReadVariables(N_loops, filetoloop);
  string* dataname = ReadVariables(N_tuples, datadir);

  //Get chain with data to use
  TChain* chain = GetChain(datadir);

  //Open file
  outfile.open(outfilename.c_str());
  //Loop over
  for(int i=0;i<N_loops;i++)
    {
      //Do Operation defined on top (easily editable)
      outfile << filename[i] << " | " << DoOperation(chain, filename[i]) << endl;
    }

  //Close file
  outfile.close();
}
