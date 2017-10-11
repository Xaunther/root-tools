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
using namespace std;

void BDTTrain(bool logdira = false, string cutsfilename = "Variables/BDTCuts.txt")
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
  cout << "----------------------------------------------------------------------------" << endl;
  cout << "Starting training with " << sigtree->GetEntries() << " signal events and " << datatree->GetEntries() << " background events" << endl;
  cout << "----------------------------------------------------------------------------" << endl;

  factory->AddSignalTree(sigtree,signalW);
  factory->AddBackgroundTree(datatree, backgroundW);

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
  factory->TMVA::Factory::PrepareTrainingAndTestTree("", "", "random");

  //TMVA method
  factory->BookMethod(TMVA::Types::kBDT, "BDT", "NTrees=400:MaxDepth=2");
  
  //Train and test
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}
