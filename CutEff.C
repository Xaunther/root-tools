//Take a ntuple, or a file containing a list of them, and calculate the efficiency of each cut given in cutfile, starting from a previous set of cuts, precutfile
//The output is given by outfile. Event weights can be included using weight

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "TChain.h"
#include "TMath.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/ArrayTools.h"
#include "../Functions/StringTools.h"
using namespace std;

void CutEff(string dirfile, string cutfile, string precutfile = "", string outfile = "CutEff_results.txt", string weight = "", string treename = "")
{
  //Load list of cuts to apply and previous cuts
  string allcuts = GetCuts(cutfile);
  string allprecuts = GetCuts(precutfile);

  //Split the cuts and precuts for each " * " found, it's risky if we have provided some strange logical combination...
  int N_cuts = 0;
  string* cuts = SplitString(N_cuts, allcuts, " * ");
  int N_precuts = 0;
  string* precuts = SplitString(N_precuts, allprecuts, " * ");

  //To prevent bad things to happen
  if(allprecuts == ""){allprecuts = "1";}

  //Declare array where we'll save the final number of events for each cut, as well as the initial number
  double* N_final = new double[N_cuts+1];
  double N0;

  //We'll need the square of the weight for later
  string w2 = "";
  if(weight != ""){w2 = weight+"*"+weight;}

  //Open chain here
  TChain* chain = GetChain(dirfile, treename);

  //Simply compute #of evts before and after
  N0 = GetMeanEntries(chain, allprecuts, weight);
  for(int i=0;i<N_cuts;i++)
  {
    N_final[i] = GetMeanEntries(chain, allprecuts+" * "+cuts[i], weight);
  }
  N_final[N_cuts] =GetMeanEntries(chain, allprecuts+" * "+allcuts, weight);

  //////////////Now, produce a gorgeous output #4dalulz/////////////
  ofstream fout;
  fout.open(outfile.c_str());
  fout << "Selection efficiencies applied on " << dirfile << ": " << endl;
  fout << "Event weight: " << weight << endl << endl;
  fout << "                     Preselection                     " << endl;
  fout << "------------------------------------------------------" << endl;
  for(int i=0;i<N_precuts;i++)
  {
    fout << precuts[i] << endl;
  }
  fout << endl;
  fout << "          Efficiency for each cut, and global          " << endl;
  fout << "-------------------------------------------------------" << endl;

  int maxL = GetMaxLength(cuts, N_cuts);
  for(int i=0;i<N_cuts;i++)
  {
    fout << cuts[i] << setw(maxL+5-int(cuts[i].size())) << "  |  " << N_final[i]/N0 << endl;
  }
  fout << "Global" << setw(maxL+5-6) << "  |  " << fixed << N_final[N_cuts]/N0 << endl;
  fout << "Error" << setw(maxL+5-5) << "  |  " << TMath::Sqrt(N_final[N_cuts]/N0*(1-N_final[N_cuts]/N0)/chain->GetEntries()*GetMeanEntries(chain, allprecuts, w2))/N0 << endl;

  //Close files and clean memory
  fout.close();
  CloseChain(chain);
  delete[] N_final;
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
  {
  case 2:
    CutEff(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    CutEff(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    CutEff(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    CutEff(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    CutEff(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return(1);
    break;
  }
  return 0;
}
#endif
