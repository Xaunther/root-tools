#include <string>
#include <iostream>
#include <stdlib.h>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "../Functions/AnalFits.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/PlotTools.h"
#include "../Functions/StringTools.h"
using namespace std;

/************************************************************************************************************************************************/
//Function used to do Simultaneous fit in NstG analysis. It is designed to fit ppiG, KpiG and pKG masses (IN THAT ORDER!), each one having different PID cuts.
//It also fits and plots the MC to extract the parameters of the PDFs that aren't (or can't) be fitted directly on the data
void Fit_NstG_Simult(string varnamelist, string filedirdata, string cutfilelist, string opts = "");

void Fit_NstG_Simult(string varnamelist, string filedirdata, string cutfilelist, string opts)
{
  //First of all, varnamelist containts tha names of the 3 variables, separated with white spaces
  int N_vars = 0;
  string* varnames = SplitString(N_vars, varnamelist, " ");
  //We do the same with the cutfiles (we should have as many as N_vars)
  int N_cuts = 0;
  string* cutfiles = SplitString(N_cuts, cutfilelist, " ");
  if (N_vars != N_cuts)
  {
    cout << "There must be same number of mass variables (" + std::to_string(N_vars) + ") as cutfiles (" + std::to_string(N_cuts) + ")" << endl;
    exit(1);
  }
  string* cuts = new string[N_cuts];
  for (int i = 0; i < N_cuts; i++) {cuts[i] = GetCuts(cutfiles[i]);}
  //So now what we must do is to open the TChain and save 3 instances of it, 1 with each PID cut.
  //Load TChain
  TChain* chain = GetChain(filedirdata);
  TFile** pidcutfile = new TFile*[N_cuts];
  TTree** pidcuttree = new TTree*[N_cuts];
  //Apply cuts and save
  for (int i = 0; i < N_cuts; i++)
  {
    pidcutfile[i] = TFile::Open(("Tuples/temp" + std::to_string(i) + "_Simult.root").c_str(), "RECREATE");
    pidcutfile[i]->cd();
    pidcuttree[i] = (TTree*)chain->CopyTree(cuts[i].c_str());
    pidcuttree[i]->Write();
    cout << "Saved tree with cuts from " << cutfiles[i] << endl;
  }

  //Do the fit
  RooWorkspace* Final_ws = FitLb2NstG_Simult(varnames, pidcuttree, opts);

  //Plot with linear scale
  GoodPlot_Simult(Final_ws, varnames, N_vars, "", "", opts);
  //Maybe repeat plots in log scale?

  //Clean up
  CloseChain(chain);
  for (int i = 0; i < N_vars; i++) {CloseTree(pidcuttree[i]);}
  delete[] varnames; delete[] cutfiles; delete[] cuts; delete[] pidcutfile; delete[] pidcuttree;
  delete chain; delete Final_ws;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 3:
    Fit_NstG_Simult(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    Fit_NstG_Simult(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
