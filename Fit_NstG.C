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

#define Nbkgs 5
/************************************************************************************************************************************************/
//Fill Option arrays. Try to modularize parts of the scripts
void Fill_Opts(FitOption* fitopt, string* opts_MC, string* variablename, string* w_var, string varnamedata, bool use_weights)
{
  //Also define PID weight variable
  if (varnamedata == "B_M012") //Kpigamma
  {
    fitopt[0] = DoubleCB; //KstG - like
    fitopt[1] = Nothing; //LambdaG - like
    fitopt[2] = ArgusGauss; //KpipiG - like
    fitopt[3] = Nothing; //Kpipi0X - like
    fitopt[4] = Nothing; //NstG
    for (int i = 0; i < Nbkgs; i++)
    {
      opts_MC[i] = "NstG_KpiG_MC";
      variablename[i] = varnamedata;
      if (use_weights)
      {
        w_var[i] = "Event_PIDCalibEff_pbarpi_global_weight";
      }
    }
  }
  else if (varnamedata == "B_M012_Subst0_K2p") //ppigamma
  {
    fitopt[0] = CBExp;
    fitopt[1] = CBExp;
    fitopt[2] = ArgusGauss;
    fitopt[3] = Line;
    fitopt[4] = CBExp;
    for (int i = 0; i < Nbkgs; i++)
    {
      opts_MC[i] = "NstGamma_MC";
      variablename[i] = varnamedata;
      if (use_weights)
      {
        w_var[i] = "Event_PIDCalibEff_global_weight";
      }
    }
  }
  else if (varnamedata == "B_M012_Subst01_Kpi2pK") //pKgamma
  {
    fitopt[0] = CBExp;
    fitopt[1] = CBExp;
    fitopt[2] = ArgusGauss;
    fitopt[3] = Line;
    fitopt[4] = DoubleCB;
    for (int i = 0; i < Nbkgs; i++)
    {
      opts_MC[i] = "NstG_pKG_MC";
      variablename[i] = varnamedata;
      if (use_weights)
      {
        w_var[i] = "Event_PIDCalibEff_ppibar_global_weight";
      }
    }
  }
  return;
}
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
//Function used to do NstG mass fits. It esentially does fits to MC backgrounds and picks up the parameter values to be used on the final datafit
void Fit_NstG(bool use_weights, string varnamedata, string filedirdata, string cutfiledata = "", string opts = "", bool plotMC = false);

void Fit_NstG(bool use_weights, string varnamedata, string filedirdata, string cutfiledata, string opts, bool plotMC)
{
  //Initialize data stuff
  //Load TChain
  string cutsdata = GetCuts(cutfiledata);
  TChain* chain = GetChain(filedirdata);
  //Apply cuts and save in a temporary root file
  TFile* tempfile = new TFile("Tuples/temp.root", "recreate");
  TTree* temptree = (TTree*)chain->CopyTree(cutsdata.c_str());
  tempfile->Write();

  //This function is used to channel the 3 mass variable fits for NstG: ppiG, KpiG, pKG
  if (varnamedata == "B_M012_Subst0_K2p")
  {
    Final_ws = FitLb2NstG(varnamedata, temptree, opts);
  }
  else if (varnamedata == "B_M012")
  {
    Final_ws = FitLb2NstG_Kpi(varnamedata, temptree, opts);
  }
  else if (varnamedata == "B_M012_Subst01_Kpi2pK")
  {
    Final_ws = FitLb2NstG_pK(varnamedata, temptree, opts);
  }
  else //Unknown mass variable to fit
  {
    cout << "Mass variable " + varnamedata + " not implemented" << endl;
    exit(1);
  }  
  if (opts == "")
  {
    opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
  }
  string w_var[Nbkgs];
  //RooFit
  FitFunction* fitf = FitFunction_init();
  RooWorkspace* ws[Nbkgs];
  RooWorkspace* Param_ws = new RooWorkspace("Parameter WS");

  //Names dict
  Names name_list(opts);

  //Type of fit for each sample
  FitOption fitopt[Nbkgs];
  FitOption fitopt_trimmed[Nbkgs];
  //Which set of constants should be used in each MC fit
  string opts_MC[Nbkgs];
  string variablename[Nbkgs];
  //Initialize options
  Fill_Opts(fitopt, opts_MC, variablename, w_var, varnamedata, use_weights);
  //Root stuff
  TTree** tree = new TTree*[Nbkgs];
  TFile** file = new TFile*[Nbkgs];

  int fitcounter = 0;
  for (int i = 0; i < Nbkgs; i++)
  {
    if(fitopt[i] == Nothing){continue;} //Only fit when requested
    fitopt_trimmed[fitcounter] = fitopt[i];
    //Get tree i and fit the variable
    stringstream ss;
    ss << i;
    file[i] = TFile::Open(("Tuples/temp" + ss.str() + ".root").c_str());
    tree[i] = (TTree*)file[i]->Get("DecayTree");
    cout << endl << "Starting MC fit number " << i << endl;
    cout << "------------------------" << endl << endl;
    ws[i] = fitf[fitopt[i]](variablename[i], tree[i], w_var[i], 0, 0, opts_MC[i]);
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
      Extract_Var(ws[i], Param_ws, name_list.exp_par[0], name_list.exp_par[fitcounter]);
      break;
    case Line: //Straight line fit
      Extract_Var(ws[i], Param_ws, name_list.slope, name_list.slope);
      break;
    case GaussExp: //Gaussian with one exponential tail
      Extract_Var(ws[i], Param_ws, name_list.alpha, name_list.alphaL[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[fitcounter]);
      break;
    case CB: //Gaussian with power-law tail
      Extract_Var(ws[i], Param_ws, name_list.alpha, name_list.alphaL[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.n, name_list.nL[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[fitcounter]);
      break;
    case CBExp: //Gaussian with power-law, exponential tails
      Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.n, name_list.nL[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[fitcounter]);
      break;
    case DoubleGaussExp: //Gaussian with 2 exponential tails
      Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[fitcounter]);
      break;
    case DoubleCB: //Gaussian with 2 power-law tails
      Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.nL[0], name_list.nL[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.nR[0], name_list.nR[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[fitcounter]);
      Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[fitcounter]);
      break;
    default: //Any other thing. Output a disclaimer here
      cout << "Fit option not implemented. Doing nothing" << endl;
      break;
    }
    fitcounter++;
  }
  //Maybe the fit for the signal channel clearly differs from the PID missID ones
  RooWorkspace* Final_ws;
  cout << endl << "Starting data fit for " << varnamedata << endl;
  cout << "-----------------------------------" << endl << endl;
  if (varnamedata == "B_M012_Subst0_K2p")
  {
    Final_ws = FitLb2NstG(varnamedata, temptree, Param_ws, "", 0, 0, opts, fitopt_trimmed, fitcounter);
  }
  else if (varnamedata == "B_M012")
  {
    Final_ws = FitLb2NstG_Kpi(varnamedata, temptree, Param_ws, "", 0, 0, opts, fitopt_trimmed, fitcounter);
  }
  else if (varnamedata == "B_M012_Subst01_Kpi2pK")
  {
    Final_ws = FitLb2NstG_pK(varnamedata, temptree, Param_ws, "", 0, 0, opts, fitopt_trimmed, fitcounter);
  }
  else //Unknown mass variable to fit
  {
    cout << "Mass variable " + varnamedata + " not implemented" << endl;
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
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  bool use_weights = false;
  bool plotMC = false;
  switch (argc - 1)
  {
  case 3:
    if (*(new string(argv[1])) == "true" || *(new string(argv[1])) == "1") {use_weights = true;}
    Fit_NstG(use_weights, *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    if (*(new string(argv[1])) == "true" || *(new string(argv[1])) == "1") {use_weights = true;}
    Fit_NstG(use_weights, *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    if (*(new string(argv[1])) == "true" || *(new string(argv[1])) == "1") {use_weights = true;}
    Fit_NstG(use_weights, *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    if (*(new string(argv[1])) == "true" || *(new string(argv[1])) == "1") {use_weights = true;}
    if (*(new string(argv[6])) == "true" || *(new string(argv[6])) == "1") {plotMC = true;}
    Fit_NstG(use_weights, *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), plotMC);
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for Fit_NstG" << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
