#include <string>
#include <sstream>
#include <iostream>
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

#define Nbkgs 6
//Use 1: default
//Use 2: is needCuts, default. If not, use already created temp files
void Fit_NstG(string, string, string = "");
void Fit_NstG(bool, string, string, string = "");

void Fit_NstG(bool needCuts, string varnamedata, string filedirdata, string cutfiledata)
{
  if(needCuts)
    {
      Fit_NstG(varnamedata, filedirdata, cutfiledata);
    }
  else
    {
      //RooFit
      FitFunction* fitf = FitFunction_init();
      RooWorkspace** ws = new RooWorkspace*[Nbkgs];
      RooWorkspace* Param_ws = new RooWorkspace("Parameter WS");
      
      //Names dict
      Names name_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));
      
      string variablename[Nbkgs];
      variablename[0] = varnamedata;
      variablename[2] = varnamedata;
      variablename[3] = varnamedata;
      variablename[4] = varnamedata;
      variablename[5] = varnamedata;
      if(varnamedata == "B_M012")
	{
	  variablename[1] = "B_M012_Subst0_pi2K";
	}
      else if(varnamedata == "B_M012_Subst01_Kpi2pK")
	{
	  variablename[1] = "B_M012_Subst01_pipi2pK";
	}
      else if(varnamedata == "B_M012_Subst0_K2p")
	{
	  variablename[1] = "B_M012_Subst0_pi2p";
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
	  ss.str("");
	  ws[i] = fitf[DoubleCB](variablename[i], tree[i]);
	  //Now we retrieve the values of the parameters and save them in our new workspace
	  RooRealVar* dummy = new RooRealVar(name_list.alphaL[i+1].c_str(),name_list.alphaL[i+1].c_str(), ws[i]->var(name_list.alphaL[0].c_str())->getValV(), ws[i]->var(name_list.alphaL[0].c_str())->getValV(), ws[i]->var(name_list.alphaL[0].c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.alphaR[i+1].c_str(),name_list.alphaR[i+1].c_str(), ws[i]->var(name_list.alphaR[0].c_str())->getValV(), ws[i]->var(name_list.alphaR[0].c_str())->getValV(), ws[i]->var(name_list.alphaR[0].c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.nL[i+1].c_str(),name_list.nL[i+1].c_str(), ws[i]->var(name_list.nL[0].c_str())->getValV(), ws[i]->var(name_list.nL[0].c_str())->getValV(), ws[i]->var(name_list.nL[0].c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.nR[i+1].c_str(),name_list.nR[i+1].c_str(), ws[i]->var(name_list.nR[0].c_str())->getValV(), ws[i]->var(name_list.nR[0].c_str())->getValV(), ws[i]->var(name_list.nR[0].c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.mean[i+1].c_str(),name_list.mean[i+1].c_str(), ws[i]->var(name_list.mean[0].c_str())->getValV(), ws[i]->var(name_list.mean[0].c_str())->getValV(), ws[i]->var(name_list.mean[0].c_str())->getValV());
	  Param_ws->import(*dummy);
	  dummy = new RooRealVar(name_list.width[i+1].c_str(),name_list.width[i+1].c_str(), ws[i]->var(name_list.width[0].c_str())->getValV(), ws[i]->var(name_list.width[0].c_str())->getValV(), ws[i]->var(name_list.width[0].c_str())->getValV());
	  Param_ws->import(*dummy);
	} 
      
      //Initialize data stuff
      //Load TChain
      string cutsdata = GetCuts(cutfiledata);
      TChain* chain = GetChain(filedirdata);
      TFile* tempfile = new TFile("Tuples/temp.root", "recreate");
      TTree* temptree = (TTree*)chain->CopyTree(cutsdata.c_str());
      tempfile->Write();
      
      RooWorkspace* Final_ws = FitLb2NstG(varnamedata, temptree, Param_ws);
      GoodPlot(Final_ws, varnamedata, true);
    }
}

void Fit_NstG(string varnamedata, string filedirdata, string cutfiledata)
{
  //Put dirs in an array
  string filedir[Nbkgs];
  filedir[0] = "../Directories/Bd2KstG_tuples.dir";
  filedir[1] = "../Directories/Bd2Rho0G_tuples.dir";
  filedir[2] = "../Directories/Lb2L1520G_tuples.dir";
  filedir[3] = "../Directories/Lb2L1670G_tuples.dir";
  filedir[4] = "../Directories/Lb2L1820G_tuples.dir";
  filedir[5] = "../Directories/Lb2L1830G_tuples.dir";
  //It happens, that the cuts are different for some MC...
  string cutfile[Nbkgs];
  cutfile[0] = "Variables/Cuts_Kpigamma.txt";
  cutfile[1] = "Variables/Cuts_pipigamma.txt";
  cutfile[2] = "Variables/Cuts_Kpigamma.txt";
  cutfile[3] = "Variables/Cuts_Kpigamma.txt";
  cutfile[4] = "Variables/Cuts_Kpigamma.txt";
  cutfile[5] = "Variables/Cuts_Kpigamma.txt";
  string cuts[Nbkgs];
  //Root stuff
  TChain** chain = new TChain*[Nbkgs];
  TTree** tree = new TTree*[Nbkgs];
  TFile** file = new TFile*[Nbkgs];

  for(int i=0;i<Nbkgs;i++)
    {
      //Get tree i and fit the variable
      stringstream ss;
      ss << i;
      cuts[i] = GetCuts(cutfile[i]);
      chain[i] = GetChain(filedir[i]);
      file[i] = new TFile(("Tuples/temp"+ss.str()+".root").c_str(), "recreate");
      tree[i] = (TTree*)chain[i]->CopyTree(cuts[i].c_str());
      file[i]->Write();
    }
  Fit_NstG(0, varnamedata, filedirdata, cutfiledata);
}
