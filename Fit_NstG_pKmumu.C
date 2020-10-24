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

#define Nbkgs_pKmumu 1
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
//Function used to do NstG mass fits. It esentially does fits to MC backgrounds and picks up the parameter values to be used on the final datafit
void Fit_NstG_pKmumu(string varnamedata, string filedirdata, string cutfiledata = "", string opts = "", bool plotMC = false);

void Fit_NstG_pKmumu(string varnamedata, string filedirdata, string cutfiledata, string opts, bool plotMC)
{
  if (opts == "")
  {
    opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
  }
  string w_var = "";
  //RooFit
  FitFunction* fitf = FitFunction_init();
  RooWorkspace** ws = new RooWorkspace*[Nbkgs_pKmumu];
  RooWorkspace* Param_ws = new RooWorkspace("Parameter WS");

  //Names dict
  Names name_list(opts);

  //Type of fit for each sample
  FitOption fitopt[Nbkgs_pKmumu];
  //Which set of constants should be used in each MC fit
  string opts_MC[Nbkgs_pKmumu];
  string variablename[Nbkgs_pKmumu];
  //Initialize options
  fitopt[0] = DoubleGaussExp;
  opts_MC[0] = "NstG_pKmumu";
  variablename[0] = varnamedata;
  //Root stuff
  TTree** tree = new TTree*[Nbkgs_pKmumu];
  TFile** file = new TFile*[Nbkgs_pKmumu];

  for (int i = 0; i < Nbkgs_pKmumu; i++)
  {
    //Get tree i and fit the variable
    stringstream ss;
    ss << i;
    file[i] = TFile::Open(("Tuples/temp_pKmumu" + ss.str() + ".root").c_str());
    tree[i] = (TTree*)file[i]->Get("h1");
    cout << endl << "Starting MC fit number " << i << endl;
    cout << "------------------------" << endl << endl;
    ws[i] = fitf[fitopt[i]](variablename[i], tree[i], w_var, 0, 0, opts_MC[i]);
    /************************/
    //Plot MC if requested
    if (plotMC) {GoodPlot(ws[i], variablename[i], "", "", opts_MC[i], "_MC" + ss.str());}
    ss.str("");
    file[i]->Close();
  }
  //Initialize data stuff
  //Load TChain
  string cutsdata = GetCuts(cutfiledata);
  TChain* chain = GetChain(filedirdata);
  TFile* tempfile = new TFile("Tuples/temp.root", "recreate");
  TTree* temptree = (TTree*)chain->CopyTree(cutsdata.c_str());
  tempfile->Write();
  //Maybe the fit for the signal channel clearly differs from the PID missID ones
  RooWorkspace* Final_ws;
  cout << endl << "Starting data fit for " << varnamedata << endl;
  cout << "-----------------------------------" << endl << endl;
  Final_ws = FitLb2pKJPsi(varnamedata, temptree, "", 0, 0, opts);
  //Plot with linear scale
  GoodPlot(Final_ws, varnamedata, "", "", opts);
  //Get log options (is this safe?)
  string logopts = opts + "_log";
  //Plot with log scale
  GoodPlot(Final_ws, varnamedata, "", "", logopts, "_log");

  /*-------------Close up------------------*/
  tempfile->Close();
  CloseChain(chain);
  delete[] ws;
  delete Param_ws;
  delete[] tree;
  delete[] file;
  delete tempfile;
  delete fitf;
  /*---------------------------------------*/
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  bool plotMC = false;
  switch (argc - 1)
  {
  case 2:
    Fit_NstG_pKmumu(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    Fit_NstG_pKmumu(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    Fit_NstG_pKmumu(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    if (*(new string(argv[5])) == "true" || *(new string(argv[5])) == "1") {plotMC = true;}
    Fit_NstG_pKmumu(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), plotMC);
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
