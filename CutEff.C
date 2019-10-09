#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "TChain.h"
#include "TMath.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/ArrayTools.h"
#include "../Functions/StringTools.h"
using namespace std;

void CutEff(string dirfile, string cutfile, string precutfile = "", string outfile = "CutEff_results.txt", string weight = "")
{
  string allcuts = GetCuts(cutfile);
  string allprecuts = GetCuts(precutfile);

  int N_cuts = 0;
  string* cuts = SplitString(N_cuts, allcuts, " * ");
  int N_precuts = 0;
  string* precuts = SplitString(N_precuts, allprecuts, " * ");

  if(allprecuts == "")
    {
      allprecuts = "1";
    }
  double* N_final = new double[N_cuts+1];
  double N0;

  //Open chain here
  TChain* chain = GetChain(dirfile);
  //Simply compute #of evts before and after
  N0 = GetMeanEntries(chain, allprecuts, weight);
  for(int i=0;i<N_cuts;i++)
    {
      N_final[i] = GetMeanEntries(chain, allprecuts+" * "+cuts[i], weight);
    }
  N_final[N_cuts] =GetMeanEntries(chain, allprecuts+" * "+allcuts, weight);
  //Now, produce a gorgeous output #4dalulz
  ofstream fout;
  fout.open(outfile.c_str());
  fout << "Selection efficiencies applied on " << dirfile << ": " << endl << endl;
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
  fout << "Global" << setw(maxL+5-6) << "  |  " << N_final[N_cuts]/N0 << endl;
  fout << "Error" << setw(maxL+5-5) << "  |  " << TMath::Sqrt(N_final[N_cuts]/N0*(1-N_final[N_cuts]/N0)/(N0*chain->GetEntries())) << endl;
  fout.close();

}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc)
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
    default:
      cout << "Wrong number of arguments (" << argc << ") for CutEff" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
