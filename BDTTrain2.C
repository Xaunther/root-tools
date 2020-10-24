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
#include "Functions/TreeTools.h"
#include "Functions/StringTools.h"
#include "TMVA/Tools.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Factory.h"
using namespace std;

//BDTTrain es la función original, se usa en algunos análisis y no es demasiado general, pero es dificil ahora tocar todo para que cuadre
//BDTTrain2 generaliza la función anterior, reduciendo los argumentos a cosas mas necesarias
//Why in Spanish? IDK
void BDTTrain2(string sig_file, string bkg_filelist, string sig_cutfile = "", string bkg_cutfile = "", string varfile = "Variables/BDTVariables.txt", string factoryname = "TMVAClassification", string bkgweighlist = "1", string sigw = "1");

void BDTTrain2(string sig_file, string bkg_filelist, string sig_cutfile, string bkg_cutfile, string varfile, string factoryname, string bkgweightlist, string sigw)
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

  //Get list of files and weights. Space separated
  int Nfiles = 0, Nw = 0;
  string* bkg_file = SplitString(Nfiles, bkg_filelist, " ");
  string* bkg_w_str = SplitString(Nw, bkgweightlist, " ");
  //Weights for signal and background
  double signalW = 1;
  double* bkg_w = new double[Nfiles];
  for (int i = 0; i < Nw; i++) {bkg_w[i] = stod(bkg_w_str[i]);}
  for (int i = Nw; i < Nfiles; i++) {bkg_w[i] = 1.;}

  //Cuts
  string sig_cuts = GetCuts(sig_cutfile);
  string bkg_cuts = GetCuts(bkg_cutfile);

  //Open input MC and signal files
  TTree* sigtree  = GetTree(sig_file);
  TTree** datatree = new TTree*[Nfiles];
  for (int i = 0; i < Nfiles; i++) {datatree[i] = GetTree(bkg_file[i]);}

  cout << "----------------------------------------------------------------------------" << endl;
  cout << "Starting training with " << sigtree->GetEntries(sig_cuts.c_str()) << " signal events and ";
  for (int i = 0; i < Nfiles; i++)
  {
    cout << datatree[i]->GetEntries(bkg_cuts.c_str()) << ", ";
  }
  cout << " background events" << endl;
  cout << "----------------------------------------------------------------------------" << endl;

  TFile* addedsigtree = new TFile("Tuples/BDTsigtest.root", "RECREATE");
  addedsigtree->cd();
  dl->AddTree(sigtree, "Signal", signalW, sig_cuts.c_str(), TMVA::Types::kMaxTreeType);
  TFile* addedbkgtree = new TFile("Tuples/BDTbkgtest.root", "RECREATE");
  addedbkgtree->cd();
  for (int i = 0; i < Nfiles; i++)
  {
    dl->AddTree(datatree[i], "Background", bkg_w[i], bkg_cuts.c_str() , TMVA::Types::kMaxTreeType);
  }

  //Add Variables
  for (int i = 0; i < N_variables; i++)
  {
    dl->AddVariable(variable_list[i].c_str(), 'D');
  }

  //Add signal weight
  dl->SetSignalWeightExpression(sigw.c_str());
  //Prepare training
  dl->PrepareTrainingAndTestTree("", "", (const_list.BDT_Prepare_options).c_str());

  //TMVA method
  factory->BookMethod(dl, TMVA::Types::kBDT, "BDT", const_list.BDT_Method_options.c_str());
  //Train and test
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 2:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  case 7:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
  case 8:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
              *(new string(argv[8])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
