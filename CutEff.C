#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
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
  string* cuts = SplitString(N_cuts, allcuts, " && ");
  int N_precuts = 0;
  string* precuts = SplitString(N_precuts, allprecuts, " && ");

  if(allprecuts == "")
    {
      allprecuts = "1";
    }
  int* N_final = new int[N_cuts+1];
  int N0;

  //Data chain
  TChain* chain = GetChain(dirfile);

  //If weights are requested
  double* w_f = new double[N_cuts+1];
  double w_0 = 1.;
  if(weight != "")
    {
      w_0 = GetMean(dirfile, weight);
    }
  //Simply compute #of evts before and after
  N0 = chain->GetEntries(allprecuts.c_str());
  for(int i=0;i<N_cuts;i++)
    {
      N_final[i] = chain->GetEntries((allprecuts+" && "+cuts[i]).c_str());
      //If weights are requested, we need the mean after the cuts
      if(weight != "")
	{
	  w_f[i] = GetMean(dirfile, weight, "", allprecuts+" && "+cuts[i]);
	}
      //Otherwise, just 1
      else
	{
	  w_f[i] = 1.;
	}
    }
  N_final[N_cuts] =chain->GetEntries((allprecuts+" && "+allcuts).c_str());
  if(weight != "")
    {
      w_f[N_cuts] = GetMean(dirfile, weight, "", allprecuts+" && "+allcuts);
    }
  //Otherwise, just 1
  else
    {
      w_f[N_cuts] = 1.;
    }
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
      fout << cuts[i] << setw(maxL+5-int(cuts[i].size())) << "  |  " << (w_f[i]/w_0)*(N_final[i]/double(N0)) << endl;
    }
  fout << "Global" << setw(maxL+5-6) << "  |  " << (w_f[N_cuts]/w_0)*(N_final[N_cuts]/double(N0)) << endl;
  fout.close();

}
