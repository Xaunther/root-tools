#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
#include "Functions/misc.h"
//#include "Functions/fitvartable.h"
using namespace std;

void Merit(string variablename, string filename = "Variables/VariablesMerit.txt", string cutsfilename = "Variables/CutsSwapped.txt", string varsfilename = "Variables/Variations.txt", string datafile = "Tuples/cuttreevars.root")
{
  int N_variables = 0;
  ofstream meritfile;
  string* variable_list = ReadVariables(N_variables, filename);
  fitvartable fitparam("Variables/FitParameters.txt");

  meritfile.open("merit.txt");
  meritfile << "                     Variable    Merit 1    Merit 2    Merit 3    Merit 4    Merit 5" << endl;
  meritfile << "------------------------------------------------------------------------------------" << endl;
  //Going to plot 1 variable and then extend it with a loop
  
  TFile* tuplefile = new TFile(datafile.c_str());
  TTree* tree = (TTree*)tuplefile->Get("DecayTree"); //Get Tree from corresponding directory

  for(int i=0;i<N_variables;i++)
    {
      if(1)
	{
      meritfile << setfill(' ') << setw(29) << variable_list[i] << " ";
      for(int j=0;j<5;j++)
	{
	  string cuts = GetCutsVar(cutsfilename, varsfilename, N_variables, i, j);
	  TFile* cutfile = TFile::Open("Tuples/cuttree2.root", "RECREATE");
	  TTree* cuttree = tree->CopyTree(cuts.c_str());
	  //Initialize all the stuff needed for the fit
	  double entries = cuttree->GetEntries();
	  RooRealVar B_M(variablename.c_str(), variablename.c_str(), fitparam.Value("B_M_min"), fitparam.Value("B_M_max"));
	  RooRealVar mean("mean", "mean", fitparam.Value("mean_mid"), fitparam.Value("mean_min"), fitparam.Value("mean_max"));
	  RooRealVar width("width","width", fitparam.Value("width_mid"), fitparam.Value("width_min"), fitparam.Value("width_max"));
	  RooGaussian signal("signal","signal", B_M, mean, width);
	  RooRealVar bkgpar("bkgpar", "bkgpar", fitparam.Value("bkgpar_mid"), fitparam.Value("bkgpar_min"), fitparam.Value("bkgpar_max"));
	  RooExponential bkg("bkg","bkg", B_M, bkgpar);
	  //RooPolynomial bkg("bkg", "bkg", B_M, RooFit::RooConst(1), 0);
	  RooRealVar fsig("fsig","fsig", entries/10, 0, entries);
	  RooRealVar fbkg("fbkg","fbkg", entries/10, 0, entries);
	  RooAddPdf model("model", "model", RooArgList(signal,bkg),RooArgList(fsig,fbkg));
	  
	  RooDataSet data("data", "data", cuttree, RooArgSet(B_M));
	  model.fitTo(data);
	  double merit = fsig.getValV()/TMath::Sqrt(fsig.getValV()+fbkg.getValV());
	  meritfile << setfill(' ') << setw(10) << merit << " ";
	  cutfile->Close();
	}
      meritfile << endl;
	}
    }

  meritfile.close();
  tuplefile->Close();
}
