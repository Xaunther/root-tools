#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
#include "Dictionaries/Constants.h"
#include "Functions/Filereading.h"
#include "Functions/Dictreading.h"
#include "TMVA/Tools.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Factory.h"
using namespace std;

//BDTTrain es la función original, se usa en algunos análisis y no es demasiado general, pero es dificil ahora tocar todo para que cuadre
//BDTTrain2 generaliza la función anterior, reduciendo los argumentos a cosas mas necesarias
void BDTTrain(string massvar, string sample, string extracuts_sig = "", bool HMonly = false, bool logdira = false, string seed = "", string filename = "Variables/BDTVariables.txt");

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

  TFile* data = new TFile(("Tuples/BDTcuttreebkg" + sample + ".root").c_str());
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
  for (int i = 0; i < N_variables; i++)
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
  dl->PrepareTrainingAndTestTree("", "", (const_list.BDT_Prepare_options + seed).c_str());

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

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  bool HMonly = false;
  bool logdira = false;
  switch (argc - 1)
  {
  case 2:
    BDTTrain(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    BDTTrain(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {HMonly = true;}
    BDTTrain(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), HMonly);
    break;
  case 5:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {HMonly = true;}
    if (*(new string(argv[5])) == "true" || *(new string(argv[5])) == "1") {logdira = true;}
    BDTTrain(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), HMonly, logdira);
    break;
  case 6:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {HMonly = true;}
    if (*(new string(argv[5])) == "true" || *(new string(argv[5])) == "1") {logdira = true;}
    BDTTrain(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), HMonly, logdira, *(new string(argv[6])));
    break;
  case 7:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {HMonly = true;}
    if (*(new string(argv[5])) == "true" || *(new string(argv[5])) == "1") {logdira = true;}
    BDTTrain(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), HMonly, logdira, *(new string(argv[6])), *(new string(argv[7])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
