#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "RooRealVar.h"
#include "TObject.h"
#include "../Functions/Fits.h"
#include "../Functions/AnalFits.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Dictionaries/Names.h"
#include "../Functions/Dictreading.h"
#include "../Functions/PlotTools.h"
#include "../Functions/Extract_Var.h"
using namespace std;

#define Nbkgs_ppimumu 1
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
//Function used to do NstG mass fits. It esentially does fits to MC backgrounds and picks up the parameter values to be used on the final datafit
void Fit_NstG_ppimumu(string varnamedata, string filedirdata, string cutfiledata = "", string opts = "", bool plotMC = false);

void Fit_NstG_ppimumu(string varnamedata, string filedirdata, string cutfiledata, string opts, bool plotMC)
{
  if (opts == "")
  {
    opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
  }
  string w_var = "";
  //RooFit
  FitFunction* fitf = FitFunction_init();
  RooWorkspace** ws = new RooWorkspace*[Nbkgs_ppimumu];
  RooWorkspace* Param_ws = new RooWorkspace("Parameter WS");

  //Names dict
  Names name_list(opts);

  //Type of fit for each sample
  FitOption fitopt[Nbkgs_ppimumu];
  //Which set of constants should be used in each MC fit
  string opts_MC[Nbkgs_ppimumu];
  string variablename[Nbkgs_ppimumu];
  //Initialize options
  fitopt[0] = DoubleCB;
  opts_MC[0] = "NstG_ppimumu";
  variablename[0] = varnamedata;
  //Root stuff
  TTree** tree = new TTree*[Nbkgs_ppimumu];
  TFile** file = new TFile*[Nbkgs_ppimumu];

  for (int i = 0; i < Nbkgs_ppimumu; i++)
  {
    //Get tree i and fit the variable
    stringstream ss;
    ss << i;
    file[i] = TFile::Open(("Tuples/temp_ppimumu" + ss.str() + ".root").c_str());
    tree[i] = (TTree*)file[i]->Get("DecayTree");
    cout << endl << "Starting MC fit number " << i << endl;
    cout << "------------------------" << endl << endl;
    ws[i] = fitf[fitopt[i]](variablename[i], tree[i], w_var, 0, 0, opts_MC[i]);
    /************************/
    //Plot MC if requested
    if (plotMC) {GoodPlot(ws[i], variablename[i], "", "", opts_MC[i], "_MC" + ss.str());}
    ss.str("");
    //Now we retrieve the values of the parameters and save them in our new workspace
    //Depending on what pdf we chose... (so far only 1 Argus is defined, should define array of constants/names if more are needed)
    switch (fitopt[i])
    {
    case ArgusGauss: //ArgusGauss fit
      Extract_Var(ws[i], Param_ws, name_list.m0_Argus, name_list.m0_Argus);
      Extract_Var(ws[i], Param_ws, name_list.c_Argus, name_list.c_Argus);
      Extract_Var(ws[i], Param_ws, name_list.p_Argus, name_list.p_Argus);
      Extract_Var(ws[i], Param_ws, name_list.width_Argus, name_list.width_Argus);
      break;
    case Exp: //Exponential fit
      Extract_Var(ws[i], Param_ws, name_list.exp_par[0], name_list.exp_par[i + 1]);
      break;
    case Line: //Straight line fit
      Extract_Var(ws[i], Param_ws, name_list.slope, name_list.slope);
      break;
    case GaussExp: //Gaussian with one exponential tail
      Extract_Var(ws[i], Param_ws, name_list.alpha, name_list.alphaL[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i + 1]);
      break;
    case CB: //Gaussian with power-law tail
      Extract_Var(ws[i], Param_ws, name_list.alpha, name_list.alphaL[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.n, name_list.nL[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i + 1]);
      break;
    case CBExp: //Gaussian with power-law, exponential tails
      Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.n, name_list.nL[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i + 1]);
      break;
    case DoubleGaussExp: //Gaussian with 2 exponential tails
      Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i + 1]);
      break;
    case DoubleCB: //Gaussian with 2 power-law tails
      Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.nL[0], name_list.nL[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.nR[0], name_list.nR[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i + 1]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i + 1]);
      break;
    default: //Any other thing. Output a disclaimer here
      cout << "Fit option not implemented. Doing nothing" << endl;
      break;
    }
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
  Final_ws = FitLb2ppiJPsi(varnamedata, temptree, Param_ws, "", 0, 0, opts, fitopt, Nbkgs_ppimumu);
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
    Fit_NstG_ppimumu(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    Fit_NstG_ppimumu(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    Fit_NstG_ppimumu(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    if (*(new string(argv[5])) == "true" || *(new string(argv[5])) == "1") {plotMC = true;}
    Fit_NstG_ppimumu(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), plotMC);
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for Fit_NstG_ppimumu" << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
