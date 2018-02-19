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
#include "Dictionaries/Constants.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
using namespace std;

void BDTTrain(string massvar, string extracuts_sig = "", bool HMonly = false, bool logdira = false)
{
  int N_variables = 0;
  string filename = "Variables/BDTVariables.txt";
  string* variable_list = ReadVariables(N_variables, filename);
  
  TMVA::Tools::Instance();
  
  //Output file to store the results
  TFile* outfile = TFile::Open("Tuples/BDT-results.root", "RECREATE");

  //Declare factory :mgalletas:
  TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outfile, "V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification");
  
  double signalW = 1;
  double backgroundW = 1;

  TFile* signal = new TFile("Tuples/BDTcuttreeMC.root");
  TTree* sigtree = (TTree*)signal->Get("DecayTree");

  TFile* data = new TFile("Tuples/BDTcuttreebkg.root");
  TTree* datatree = (TTree*)data->Get("DecayTree");

  //Create bkg cut
  stringstream ss;
  if (HMonly)
    {
      ss << massvar << " > " << Constants::xHM;
    }
  else
    {
      ss << massvar << " < " << Constants::xLM << " || " << massvar << " > " << Constants::xHM;
    }

  cout << "----------------------------------------------------------------------------" << endl;
  cout << "Starting training with " << sigtree->GetEntries() << " signal events and " << datatree->GetEntries(ss.str().c_str()) << " background events" << endl;
  cout << "----------------------------------------------------------------------------" << endl;

  TFile* addedsigtree = new TFile("Tuples/BDTsigtest", "RECREATE");
  factory->AddTree(sigtree, "Signal", signalW, TCut(extracuts_sig.c_str()), TMVA::Types::kMaxTreeType);
  TFile* addedbkgtree = new TFile("Tuples/BDTbkgtest", "RECREATE");
  factory->AddTree(datatree, "Background", backgroundW, TCut(ss.str().c_str()), TMVA::Types::kMaxTreeType);

  //Add Variables
  for(int i=0;i<N_variables;i++)
    {
      if (variable_list[i] == "B_DIRA_OWNPV" && logdira)
	{
	  string logvarname = "log_B_DIRA_OWNPV := log(1-B_DIRA_OWNPV)";
	  factory->TMVA::Factory::AddVariable(logvarname.c_str(), 'D');
	}
      else
	{
	  factory->TMVA::Factory::AddVariable(variable_list[i].c_str(), 'D');
	}
    }
  
  //Prepare training
  factory->TMVA::Factory::PrepareTrainingAndTestTree("", "", Constants::BDT_Prepare_options.c_str());

  //TMVA method
  factory->BookMethod(TMVA::Types::kBDT, "BDT", Constants::BDT_Method_options.c_str());
  
  //Train and test
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}
