#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TTreeFormula.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <TXMLEngine.h>
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include <sstream>
#include "../Functions/Filereading.h"
using namespace std;

//BDTApply is the original function, which requires input  on which variables need to be saved. These must be listed in different files which are described below, classified by variable type. It also provides a way to use formulas for the BDT, setting excludeBDTvars = true
//BDTApply2 is a new function which uses TTreeFormulas for the BDT variables, and creates new ntuple with ALL PREVIOUS VARIABLES + BDT_response. So no need to provide lists, just the BDT vars
void BDTApply(string fileapplied, string outputfilename, bool excludeBDTvars = false, string filename = "Variables/BDTVariables.txt", string BDTweights = "TMVAClassification")
{
  int N_variables = 0;
  int N_extravars = 0;
  int N_extrauint = 0;
  int N_extraulong64 = 0;
  int N_extraint = 0;
  int N_extrashort = 0;
  int N_extrabool = 0;
  string filename2 = "Variables/BDTExtravars.txt";
  string filenameUInt = "Variables/BDTExtravarsUInt.txt";
  string filenameULong64 = "Variables/BDTExtravarsULong64.txt";
  string filenameInt = "Variables/BDTExtravarsInt.txt";
  string filenameShort = "Variables/BDTExtravarsShort.txt";
  string filenameBool = "Variables/BDTExtravarsBool.txt";
  string* variable_list = ReadVariables(N_variables, filename);
  string* extravar_list = ReadVariables(N_extravars, filename2);
  string* extrauint_list = ReadVariables(N_extrauint, filenameUInt);
  string* extraulong64_list = ReadVariables(N_extraulong64, filenameULong64);
  string* extraint_list = ReadVariables(N_extraint, filenameInt);
  string* extrashort_list = ReadVariables(N_extrashort, filenameShort);
  string* extrabool_list = ReadVariables(N_extrabool, filenameBool);

  TFile* data = new TFile(fileapplied.c_str());
  TTree* datatree = (TTree*)data->Get("DecayTree");

  //BDT Output
  TFile* target = new TFile(outputfilename.c_str(), "RECREATE");
  TTree* tree = new TTree("DecayTree", "treelibrated tree");

  TMVA::Tools::Instance();
  TMVA::Reader* reader =  new TMVA::Reader("V:Color:!Silent");

  Float_t* var = new Float_t[N_variables];
  //Variables used in training
  for (int i = 0; i < N_variables; i++)
  {
    reader->TMVA::Reader::AddVariable(variable_list[i].c_str(), &var[i]);
  }
  //TMVA method
  //The Folder is the one used by default
  reader->TMVA::Reader::BookMVA("BDT method", ("default/weights/" + BDTweights + "_BDT.weights.xml").c_str());

  //Variables from data
  TTreeFormula** formulavars = new TTreeFormula*[N_variables];
  Double_t* uservar = new Double_t[N_variables + N_extravars];
  UInt_t* useruint = new UInt_t[N_extrauint];
  ULong64_t* userulong64 = new ULong64_t[N_extraulong64];
  Int_t* userint = new Int_t[N_extraint];
  Short_t* usershort = new Short_t[N_extrashort];
  Bool_t* userbool = new Bool_t[N_extrabool];
  for (int i = 0; i < N_variables; i++)
  {
    if (!excludeBDTvars)
    {
      datatree->SetBranchAddress(variable_list[i].c_str(), &uservar[i]);
      tree->Branch(variable_list[i].c_str(), &uservar[i]);
    }
    else
    {
      formulavars[i] = new TTreeFormula(variable_list[i].c_str(), variable_list[i].c_str(), datatree);
    }
  }
  for (int i = 0; i < N_extravars; i++)
  {
    datatree->SetBranchAddress(extravar_list[i].c_str(), &uservar[N_variables + i]);
    tree->Branch(extravar_list[i].c_str(), &uservar[N_variables + i]);
  }
  for (int i = 0; i < N_extrauint; i++)
  {
    datatree->SetBranchAddress(extrauint_list[i].c_str(), &useruint[i]);
    tree->Branch(extrauint_list[i].c_str(), &useruint[i]);
  }
  for (int i = 0; i < N_extraulong64; i++)
  {
    datatree->SetBranchAddress(extraulong64_list[i].c_str(), &userulong64[i]);
    tree->Branch(extraulong64_list[i].c_str(), &userulong64[i]);
  }
  for (int i = 0; i < N_extraint; i++)
  {
    datatree->SetBranchAddress(extraint_list[i].c_str(), &userint[i]);
    tree->Branch(extraint_list[i].c_str(), &userint[i]);
  }
  for (int i = 0; i < N_extrashort; i++)
  {
    datatree->SetBranchAddress(extrashort_list[i].c_str(), &usershort[i]);
    tree->Branch(extrashort_list[i].c_str(), &usershort[i]);
  }
  for (int i = 0; i < N_extrabool; i++)
  {
    datatree->SetBranchAddress(extrabool_list[i].c_str(), &userbool[i]);
    tree->Branch(extrabool_list[i].c_str(), &userbool[i]);
  }

  //Add branch to store BDT response
  double BDT_response;
  tree->Branch("BDT_response", &BDT_response);

  //Apply BDT
  for (long k = 0; k < datatree->GetEntries(); k++)
  {
    //Some output to see it's still alive
    if (k % 100000 == 0)
    {
      cout << "--- ... Processing event: " << k << endl;
    }
    datatree->GetEntry(k);

    //Here we can perform operations if we made the log or smth
    for (int j = 0; j < N_variables; j++)
    {
      if (!excludeBDTvars)
      {
        var[j] = uservar[j];
      }
      else
      {
        var[j] = formulavars[j]->EvalInstance();
      }
    }

    BDT_response = reader->TMVA::Reader::EvaluateMVA("BDT method");
    cout << BDT_response << endl;
    tree->Fill();
  }
  tree->Write();

  data->Close();
  target->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  bool excludeBDTvars = false;
  switch (argc - 1)
  {
  case 2:
    BDTApply(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {excludeBDTvars = true;}
    BDTApply(*(new string(argv[1])), *(new string(argv[2])), excludeBDTvars);
    break;
  case 4:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {excludeBDTvars = true;}
    BDTApply(*(new string(argv[1])), *(new string(argv[2])), excludeBDTvars, *(new string(argv[4])));
    break;
  case 5:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {excludeBDTvars = true;}
    BDTApply(*(new string(argv[1])), *(new string(argv[2])), excludeBDTvars, *(new string(argv[4])), *(new string(argv[5])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
