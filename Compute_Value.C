//Function to compute the value of something, looped over a series of files
#include <string>
#include <iostream>
#include <fstream>
#include "TChain.h"
#include "Functions/Filereading.h"
#include "Functions/Dictreading.h"
#include "Functions/Interpol_exp.h"
#include "Functions/TreeTools.h"

using namespace std;

//Operation to do at each loop (editable)
Long64_t DoOperation(TChain* chain, string file)
{
  //string cuts_up = GetCuts(file) + " && (B_M012_Subst0_K2p > 5900) && (B_M012_Subst0_K2p < 6600)";
  //string cuts_down = GetCuts(file) + "&& (B_M012_Subst0_K2p < 5300) && (B_M012_Subst0_K2p > 4600)";

  //double all_evts = Interpol_exp(chain->GetEntries(cuts_down.c_str()), chain->GetEntries(cuts_up.c_str()));
  //return all_evts-chain->GetEntries(cuts_down.c_str())-chain->GetEntries(cuts_up.c_str());
  return chain->GetEntries(GetCuts(file).c_str());
}


void Compute_Value(string datadir, string filetoloop, string extracut = "1", string outfilename = "result_value.txt")
{
  int N_loops = 0;
  ofstream outfile;

  //Get thing to loop over (string of filenames by default)
  string* filename = ReadVariables(N_loops, filetoloop);

  //Get chain with data to use
  TChain* chain = GetChain(datadir);
  //Get common cuts for all (extra)
  string cuts = GetCuts(extracut);

  //Open file
  outfile.open(outfilename.c_str());
  //Loop over
  for (int i = 0; i < N_loops; i++)
  {
    //Do Operation defined on top (easily editable)
    outfile << filename[i] << " | " << DoOperation(chain, GetCuts(filename[i]) + " * " + cuts) << endl;
    if (i % 100 == 0)
    {
      cout << "Read file number " << i << endl;
    }
  }

  //Close file
  outfile.close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 2:
    Compute_Value(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    Compute_Value(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    Compute_Value(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
