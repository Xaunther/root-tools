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
using namespace std;

#define Nbkgs 3
/************************************************************************************************************************************************/
//Fill Option arrays. Try to modularize parts of the scripts
void Fill_Opts(FitOption* fitopt, string* opts_MC, string* variablename, string w_var, string varnamedata, bool use_weights)
{
  //Also define PID weight variable
  if(varnamedata == "B_M012") //Kpigamma
    {
      fitopt[0] = DoubleCB;
      fitopt[1] = Line;
      fitopt[2] = ArgusGauss;
      for(int i=0;i<Nbkgs;i++)
	{
	  opts_MC[i] = "NstG_KpiG";
	  variablename[i] = varnamedata;
	}
      if(use_weights){w_var = "Event_PIDCalibEff_pbarpi";}
    }
  else if(varnamedata == "B_M012_Subst0_K2p") //ppigamma
    {
      fitopt[0] = DoubleCB;
      fitopt[1] = CBExp;
      fitopt[2] = ArgusGauss;
      for(int i=0;i<Nbkgs;i++)
	{
	  opts_MC[i] = "NstGamma";
	  variablename[i] = varnamedata;
	}
      if(use_weights){w_var = "Event_PIDCalibEff";}
    }
  else if (varnamedata == "B_M012_Subst01_Kpi2pK") //pKgamma
    {
      fitopt[0] = CBExp;
      fitopt[1] = CBExp;
      fitopt[2] = ArgusGauss;
      for(int i=0;i<Nbkgs;i++)
	{
	  opts_MC[i] = "NstG_pKG";
	  variablename[i] = varnamedata;
	}
      if(use_weights){w_var = "Event_PIDCalibEff_ppibar";}
    }
  return;
}
/************************************************************************************************************************************************/
//Pick up the parameter requested (name_0) and add it to RooWorkspace with the name provided (name_f)
void Extract_Var(RooWorkspace* ws, RooWorkspace* Param_ws, string name_0, string name_f)
{
  RooRealVar* dummy = new RooRealVar(name_f.c_str(), name_f.c_str(),
				     ws->var(name_0.c_str())->getValV(), ws->var(name_0.c_str())->getValV(), ws->var(name_0.c_str())->getValV());
  Param_ws->import(*dummy);
  return;
}
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
/************************************************************************************************************************************************/
//Function used to do NstG mass fits. It esentially does fits to MC backgrounds and picks up the parameter values to be used on the final datafit
void Fit_NstG(bool use_weights, string varnamedata, string filedirdata, string cutfiledata = "", string opts = "", bool plotMC = false);

void Fit_NstG(bool use_weights, string varnamedata, string filedirdata, string cutfiledata, string opts, bool plotMC)
{
  if(opts == "")
    {
      opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");      
    }
  string w_var = "";
  //RooFit
  FitFunction* fitf = FitFunction_init();
  RooWorkspace** ws = new RooWorkspace*[Nbkgs];
  RooWorkspace* Param_ws = new RooWorkspace("Parameter WS");
  
  //Names dict
  Names name_list(opts);
  
  //Type of fit for each sample
  FitOption fitopt[Nbkgs];
  //Which set of constants should be used in each MC fit
  string opts_MC[Nbkgs];
  string variablename[Nbkgs];
  //Initialize options
  Fill_Opts(fitopt, opts_MC, variablename, w_var, varnamedata, use_weights);
  //Root stuff
  TTree** tree = new TTree*[Nbkgs];
  TFile** file = new TFile*[Nbkgs];
  
  for(int i=0;i<Nbkgs;i++)
    {
      //Get tree i and fit the variable
      stringstream ss;
      ss << i;
      file[i] = TFile::Open(("Tuples/temp"+ss.str()+".root").c_str());
      tree[i] = (TTree*)file[i]->Get("DecayTree");
      cout << endl << "Starting MC fit number " << i << endl;
      cout << "------------------------" << endl << endl;
      ws[i] = fitf[fitopt[i]](variablename[i], tree[i], w_var, 0, 0, opts_MC[i]);
      /************************/
      //Plot MC if requested
      if(plotMC){GoodPlot(ws[i], variablename[i], true, "", "", opts_MC[i], "_MC"+ss.str());}
      ss.str("");
      //Now we retrieve the values of the parameters and save them in our new workspace
      //Depending on what pdf we chose... (so far only 1 Argus is defined, should define array of constants/names if more are needed)
      switch(fitopt[i])
	{
	case ArgusGauss: //ArgusGauss fit
	  Extract_Var(ws[i], Param_ws, name_list.m0_Argus, name_list.m0_Argus);
	  Extract_Var(ws[i], Param_ws, name_list.c_Argus, name_list.c_Argus);
	  Extract_Var(ws[i], Param_ws, name_list.p_Argus, name_list.p_Argus);
	  Extract_Var(ws[i], Param_ws, name_list.width_Argus, name_list.width_Argus);
	  break;
	case Exp: //Exponential fit
	  Extract_Var(ws[i], Param_ws, name_list.exp_par, name_list.exp_par);
	  break;
	case Line: //Straight line fit
	  Extract_Var(ws[i], Param_ws, name_list.slope, name_list.slope);
	  break;
	case GaussExp: //Gaussian with one exponential tail
	  Extract_Var(ws[i], Param_ws, name_list.alpha, name_list.alphaL[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i+1]);
	  break;
	case CB: //Gaussian with power-law tail
	  Extract_Var(ws[i], Param_ws, name_list.alpha, name_list.alphaL[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.n, name_list.nL[i+1]);	  
          Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i+1]);
          Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i+1]);
	  break;
	case CBExp: //Gaussian with power-law, exponential tails
	  Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.n, name_list.nL[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i+1]);
	  break;
	case DoubleGaussExp: //Gaussian with 2 exponential tails
	  Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i+1]);
	  break;
	case DoubleCB: //Gaussian with 2 power-law tails
	  Extract_Var(ws[i], Param_ws, name_list.alphaL[0], name_list.alphaL[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.alphaR[0], name_list.alphaR[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.nL[0], name_list.nL[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.nR[0], name_list.nR[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.mean[0], name_list.mean[i+1]);
	  Extract_Var(ws[i], Param_ws, name_list.width[0], name_list.width[i+1]);
	  break;
	default: //Any other thing. Output a disclaimer here
	  cout << "Fit option not implemented. Doing nothing" << endl;
	  break;
	}
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
  if(varnamedata=="B_M012_Subst0_K2p")
    {
      Final_ws = FitLb2NstG(varnamedata, temptree, Param_ws, "", 0, 0, opts, fitopt, Nbkgs);
    }
  else if(varnamedata=="B_M012")
    {
      Final_ws = FitLb2NstG(varnamedata, temptree, Param_ws, "", 0, 0, opts, fitopt, Nbkgs);
    }
  else if(varnamedata=="B_M012_Subst01_Kpi2pK")
    {
      Final_ws = FitGauss_Exp(varnamedata, temptree, "", 0, 0, opts);
    }
  else //Unknown mass variable to fit
    {
      cout << "Mass variable " + varnamedata + " not implemented" << endl;
      exit(1);
    }
  //Plot with linear scale
  GoodPlot(Final_ws, varnamedata, true, "", "", opts);
  //Get log options (is this safe?)
  string logopts = opts+"_log";
  if(opts==GetValueFor("Project_name", "Dictionaries/Project_variables.txt"))
    {
      logopts = "NstG_ppiG_log";
    }
  //Plot with log scale
  GoodPlot(Final_ws, varnamedata, true, "", "", logopts, "_log");
}
