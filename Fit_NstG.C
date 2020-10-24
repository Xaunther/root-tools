#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "RooRealVar.h"
#include "TObject.h"
#include "Functions/Fits.h"
#include "Functions/AnalFits.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Dictionaries/Names.h"
#include "Functions/Dictreading.h"
#include "Functions/PlotTools.h"
#include "Functions/Extract_Var.h"
using namespace std;

/************************************************************************************************************************************************/
//Function used to do NstG mass fits. It esentially does fits to MC backgrounds and picks up the parameter values to be used on the final datafit
void Fit_NstG(string varnamedata, string filedirdata, string cutfiledata = "", string opts = "");

void Fit_NstG(string varnamedata, string filedirdata, string cutfiledata, string opts)
{
  //Initialize data stuff
  //Load TChain
  string cutsdata = GetCuts(cutfiledata);
  TChain* chain = GetChain(filedirdata);
  //Apply cuts and save in a temporary root file
  TFile* tempfile = new TFile("Tuples/temp.root", "recreate");
  TTree* temptree = (TTree*)chain->CopyTree(cutsdata.c_str());
  tempfile->Write();

  RooWorkspace* Final_ws;
  //This function is used to channel the 3 mass variable fits for NstG: ppiG, KpiG, pKG
  if (opts == "NstGamma")
  {
    Final_ws = FitLb2NstG(varnamedata, temptree, opts);
  }
  else if (opts == "NstG_KpiG")
  {
    Final_ws = FitLb2NstG_Kpi(varnamedata, temptree, opts);
  }
  else if (opts == "NstG_pKG")
  {
    Final_ws = FitLb2NstG_pK(varnamedata, temptree, opts);
  }
  else if (opts == "NstG_KpiG_Wrong")
  {
    Final_ws = FitLb2NstG_Kpi_Wrong(varnamedata, temptree, opts);
  }
  else if (opts == "NstG_pKG_Wrong")
  {
    Final_ws = FitLb2NstG_pK_Wrong(varnamedata, temptree, opts);
  }
  else if (opts == "NstGamma_minmu")
  {
    Final_ws = FitLb2NstG(varnamedata, temptree, opts);
  }
  else if (opts == "NstGamma_maxmu")
  {
    Final_ws = FitLb2NstG(varnamedata, temptree, opts);
  }
  else //Unknown mass variable to fit
  {
    cout << "Fit for " + opts + " not implemented" << endl;
    exit(1);
  }
  //Plot with linear scale
  GoodPlot(Final_ws, varnamedata, "", "", opts);
  //Get log options (is this safe?)
  string logopts = opts + "_log";
  if (opts == GetValueFor("Project_name", "Dictionaries/Project_variables.txt"))
  {
    logopts = "NstG_ppiG_log";
  }
  //Plot with log scale
  GoodPlot(Final_ws, varnamedata, "", "", logopts, "_log");

  //Clean up
  delete Final_ws;
  tempfile->Close();
  CloseChain(chain);
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 2:
    Fit_NstG(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    Fit_NstG(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    Fit_NstG(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
