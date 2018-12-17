#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
#include "../Dictionaries/Constants.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Functions/TreeTools.h"
#include "TMVA/Tools.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Factory.h"
#include "TMVA/MethodBase.h"
using namespace std;

//BDTTrain es la función original, se usa en algunos análisis y no es demasiado general, pero es dificil ahora tocar todo para que cuadre
//BDTTrain2 generaliza la función anterior, reduciendo los argumentos a cosas mas necesarias
void BDTTrain(string massvar, string sample, string extracuts_sig = "", bool HMonly = false, bool logdira = false, string seed = "", string filename = "Variables/BDTVariables.txt");
void BDTTrain2(string sig_file, string bkg_file, string varfile = "Variables/BDTVariables.txt", string factoryname = "TMVAClassification");

void BDTTrain(string massvar, string sample, string extracuts_sig, bool HMonly, bool logdira, string seed, string filename)
{
  //Initialize constants
  Constants const_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  //Variables
  int N_variables = 0;
  string* variable_list = ReadVariables(N_variables, filename);
  
  //Instantiate class
  TMVA::Tools::Instance();
  
  //Output file to store the results
  TFile* outfile = TFile::Open("Tuples/BDT-results.root", "RECREATE");

  //Declare factory :mgalletas:
  TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outfile, "V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification");
  TMVA::DataLoader *dl = new TMVA::DataLoader();
  
  //Weights for signal and background
  double signalW = 1;
  double backgroundW = 1;

  //Open input MC and signal files
  TFile* signal = new TFile("Tuples/BDTcuttreeMC.root");
  TTree* sigtree = (TTree*)signal->Get("DecayTree");

  TFile* data = new TFile(("Tuples/BDTcuttreebkg"+sample+".root").c_str());
  TTree* datatree = (TTree*)data->Get("DecayTree");

  //Create bkg cut
  stringstream ss;
  if (HMonly)
    {
      ss << massvar << " > " << const_list.xHM;
    }
  else
    {
      ss << massvar << " < " << const_list.xLM << " || " << massvar << " > " << const_list.xHM;
    }

  cout << "----------------------------------------------------------------------------" << endl;
  cout << "Starting training with " << sigtree->GetEntries() << " signal events and " << datatree->GetEntries(ss.str().c_str()) << " background events" << endl;
  cout << "----------------------------------------------------------------------------" << endl;

  TFile* addedsigtree = new TFile("Tuples/BDTsigtest.root", "RECREATE");
  addedsigtree->cd();
  dl->AddTree(sigtree, "Signal", signalW, TCut(extracuts_sig.c_str()), TMVA::Types::kMaxTreeType);
  TFile* addedbkgtree = new TFile("Tuples/BDTbkgtest.root", "RECREATE");
  addedbkgtree->cd();
  dl->AddTree(datatree, "Background", backgroundW, TCut(ss.str().c_str()), TMVA::Types::kMaxTreeType);

  //Add Variables
  for(int i=0;i<N_variables;i++)
    {
      if (variable_list[i] == "B_DIRA_OWNPV" && logdira)
	{
	  string logvarname = "log_B_DIRA_OWNPV := log(1-B_DIRA_OWNPV)";
	  dl->AddVariable(logvarname.c_str(), 'D');
	}
      else
	{
	  dl->AddVariable(variable_list[i].c_str(), 'D');
	}
    }
  
  //Prepare training
  dl->PrepareTrainingAndTestTree("", "", (const_list.BDT_Prepare_options+seed).c_str());

  //TMVA method
  factory->BookMethod(dl, TMVA::Types::kBDT, "BDT", const_list.BDT_Method_options.c_str());
  
  //Train and test
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
  
  //Leave open
  //  addedsigtree->Close();
  //  addedbkgtree->Close();
}

void BDTTrain2(string sig_file, string bkg_file, string sig_cutfile, string bkg_cutfile, string varfile, string factoryname)
{
  //Initialize constants
  Constants const_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  //Variables
  int N_variables = 0;
  string* variable_list = ReadVariables(N_variables, varfile);
  
  //Instantiate class
  TMVA::Tools::Instance();

  //Output file to store the results
  TFile* outfile = TFile::Open("Tuples/BDT-results.root", "RECREATE");

  //Declare factory :mgalletas:
  TMVA::Factory *factory = new TMVA::Factory(factoryname.c_str(), outfile, "V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification");
  TMVA::DataLoader *dl = new TMVA::DataLoader();
  
  //Weights for signal and background
  double signalW = 1;
  double backgroundW = 1;

  //Cuts
  string sig_cuts = GetCuts(sig_cutfile);
  string bkg_cuts = GetCuts(bkg_cutfile);

  //Open input MC and signal files
  TTree* sigtree  = GetTree(sig_file);
  TTree* datatree = GetTree(bkg_file);

  cout << "----------------------------------------------------------------------------" << endl;
  cout << "Starting training with " << sigtree->GetEntries(sig_cuts.c_str()) << " signal events and " << datatree->GetEntries(bkg_cutfile.c_str()) << " background events" << endl;
  cout << "----------------------------------------------------------------------------" << endl;

  TFile* addedsigtree = new TFile("Tuples/BDTsigtest.root", "RECREATE");
  addedsigtree->cd();
  dl->AddTree(sigtree, "Signal", signalW, sig_cuts.c_str(), TMVA::Types::kMaxTreeType);
  TFile* addedbkgtree = new TFile("Tuples/BDTbkgtest.root", "RECREATE");
  addedbkgtree->cd();
  dl->AddTree(datatree, "Background", backgroundW, bkg_cuts.c_str() , TMVA::Types::kMaxTreeType);

  //Add Variables
  for(int i=0;i<N_variables;i++)
    {
      dl->AddVariable(variable_list[i].c_str(), 'D');
    }
  
  //Prepare training
  dl->PrepareTrainingAndTestTree("", "", (const_list.BDT_Prepare_options).c_str());

  //TMVA method
  TMVA::MethodBase* mb = factory->BookMethod(dl, TMVA::Types::kBDT, "BDT", const_list.BDT_Method_options.c_str());
  //Train and test
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}
