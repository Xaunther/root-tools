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
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Dictionaries/Names.h"
#include "../Functions/Dictreading.h"
#include "../Functions/PlotTools.h"
using namespace std;

#define Nbkgs 3
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
  //Also define PID weight variable
  if(varnamedata == "B_M012") //Kpigamma
    {
      fitopt[0] = DoubleCB;
      fitopt[1] = CBExp;
      fitopt[2] = CBExp;
      for(int i=0;i<Nbkgs;i++)
	{
	  opts_MC[i] = "NstG_KpiG";
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
	}
      if(use_weights){w_var = "Event_PIDCalibEff_ppibar";}
    }
  //ELSE: LET IT DIE
  //Variable to fit. Could be that for some sample the naming is different!
  string variablename[Nbkgs];
  for(int i=0;i<Nbkgs;i++)
    {
      variablename[i] = varnamedata;
    }
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
      ws[i] = fitf[fitopt[i]](variablename[i], tree[i], w_var, 0, 0, opts_MC[i]);
      //Plot MC if requested
      if(plotMC)
	{
	  GoodPlot(ws[i], variablename[i], true, "", "", opts_MC[i], "_MC"+ss.str());
	}
      ss.str("");
      //Now we retrieve the values of the parameters and save them in our new workspace
      RooRealVar* dummy;
      //Depending on what pdf we chose... (so far only 1 Argus is defined, should define array of constants/names if more are needed)
      if(fitopt[i] == ArgusGauss)
	{
	  dummy = new RooRealVar(name_list.m0_Argus.c_str(),name_list.m0_Argus.c_str(),
				 ws[i]->var(name_list.m0_Argus.c_str())->getValV(), ws[i]->var(name_list.m0_Argus.c_str())->getValV(), ws[i]->var(name_list.m0_Argus.c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.c_Argus.c_str(),name_list.c_Argus.c_str(),
				 ws[i]->var(name_list.c_Argus.c_str())->getValV(), ws[i]->var(name_list.c_Argus.c_str())->getValV(), ws[i]->var(name_list.c_Argus.c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.p_Argus.c_str(),name_list.p_Argus.c_str(),
				 ws[i]->var(name_list.p_Argus.c_str())->getValV(), ws[i]->var(name_list.p_Argus.c_str())->getValV(), ws[i]->var(name_list.p_Argus.c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.width_Argus.c_str(),name_list.width_Argus.c_str(),
				 ws[i]->var(name_list.width_Argus.c_str())->getValV(), ws[i]->var(name_list.width_Argus.c_str())->getValV(), ws[i]->var(name_list.width_Argus.c_str())->getValV());
	  Param_ws->import(*dummy);
	}
      else if(fitopt[i] == Exp){} //Only exponential
      else if(fitopt[i] == Line){} //Straight line
      //Family of Gaussian core and some exponential/power-law tails
      else
	{
	  if(fitopt[i] == GaussExp)
	    {
	      dummy = new RooRealVar(name_list.alphaL[i+1].c_str(),name_list.alphaL[i+1].c_str(),
				     ws[i]->var(name_list.alpha.c_str())->getValV(), ws[i]->var(name_list.alpha.c_str())->getValV(), ws[i]->var(name_list.alpha.c_str())->getValV());
	      Param_ws->import(*dummy);	      
	    }
	  else
	    {
	      dummy = new RooRealVar(name_list.alphaL[i+1].c_str(),name_list.alphaL[i+1].c_str(), 
				     ws[i]->var(name_list.alphaL[0].c_str())->getValV(), ws[i]->var(name_list.alphaL[0].c_str())->getValV(), ws[i]->var(name_list.alphaL[0].c_str())->getValV());
	      Param_ws->import(*dummy);
	      dummy = new RooRealVar(name_list.alphaR[i+1].c_str(),name_list.alphaR[i+1].c_str(), 
				     ws[i]->var(name_list.alphaR[0].c_str())->getValV(), ws[i]->var(name_list.alphaR[0].c_str())->getValV(), ws[i]->var(name_list.alphaR[0].c_str())->getValV());
	      Param_ws->import(*dummy);
	    }
	  if(fitopt[i] == DoubleCB)
	    {
	      dummy = new RooRealVar(name_list.nL[i+1].c_str(),name_list.nL[i+1].c_str(), 
				     ws[i]->var(name_list.nL[0].c_str())->getValV(), ws[i]->var(name_list.nL[0].c_str())->getValV(), ws[i]->var(name_list.nL[0].c_str())->getValV());
	      Param_ws->import(*dummy);
	    }
	  else if(fitopt[i] == CBExp)
	    {
	      dummy = new RooRealVar(name_list.nL[i+1].c_str(),name_list.nL[i+1].c_str(), 
				     ws[i]->var(name_list.n.c_str())->getValV(), ws[i]->var(name_list.n.c_str())->getValV(), ws[i]->var(name_list.n.c_str())->getValV());
	      Param_ws->import(*dummy);
	    }
	  if(fitopt[i] == DoubleCB)
	    {
	      dummy = new RooRealVar(name_list.nR[i+1].c_str(),name_list.nR[i+1].c_str(), 
				     ws[i]->var(name_list.nR[0].c_str())->getValV(), ws[i]->var(name_list.nR[0].c_str())->getValV(), ws[i]->var(name_list.nR[0].c_str())->getValV());
	      Param_ws->import(*dummy);
	    }
	  dummy = new RooRealVar(name_list.mean[i+1].c_str(),name_list.mean[i+1].c_str(), 
				 ws[i]->var(name_list.mean[0].c_str())->getValV(), ws[i]->var(name_list.mean[0].c_str())->getValV(), ws[i]->var(name_list.mean[0].c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.width[i+1].c_str(),name_list.width[i+1].c_str(), 
				 ws[i]->var(name_list.width[0].c_str())->getValV(), ws[i]->var(name_list.width[0].c_str())->getValV(), ws[i]->var(name_list.width[0].c_str())->getValV());
	  Param_ws->import(*dummy);
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
  cout << "THere" << endl;
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
