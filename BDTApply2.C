#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TTreeFormula.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include "TBranch.h"
#include <TXMLEngine.h>
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include <sstream>
#include "../Functions/Filereading.h"
#include "../Functions/StringTools.h"
using namespace std;

//BDTApply is the original function, which requires input  on which variables need to be saved. These must be listed in different files which are described below, classified by variable type. It also provides a way to use formulas for the BDT, setting excludeBDTvars = true
//BDTApply2 is a new function which uses TTreeFormulas for the BDT variables, and creates new ntuple with ALL PREVIOUS VARIABLES + BDT_response. So no need to provide lists, just the BDT vars
void BDTApply2(string fileapplied, string outputfilename, string filename = "Variables/BDTVariables.txt", string BDTweights = "TMVAClassification", string BDTvarname = "BDT_response")
{
  //Read Variables
  int N_variables = 0;
  string* variable_list = ReadVariables(N_variables, filename);

  //Read input dataset
  TFile* data = new TFile(Gridify(fileapplied).c_str());
  TTree* datatree = (TTree*)data->Get("DecayTree");

  //Open output file (clone input tree)
  TFile* target = new TFile(Gridify(outputfilename).c_str(), "RECREATE");
  TTree* tree = datatree->CloneTree(0);

  //Instance TMVA READER
  TMVA::Tools::Instance();
  TMVA::Reader* reader =  new TMVA::Reader("V:Color:!Silent");

  //Link reader to array of floats (input)
  Float_t* var = new Float_t[N_variables];
  //Variables used in training
  for (int i = 0; i < N_variables; i++)
  {
    reader->TMVA::Reader::AddVariable(variable_list[i].c_str(), &var[i]);
  }
  //TMVA method
  //The Folder is the one used by default
  reader->TMVA::Reader::BookMVA("BDT method", ("default/weights/" + BDTweights + "_BDT.weights.xml").c_str());

  //Variables from data. Retrieve formulas
  TTreeFormula** formulavars = new TTreeFormula*[N_variables];
  for (int i = 0; i < N_variables; i++)
  {
    formulavars[i] = new TTreeFormula(variable_list[i].c_str(), variable_list[i].c_str(), datatree);
  }

  //Add branch to store BDT response
  double BDT_response;
  tree->Branch(BDTvarname.c_str(), &BDT_response, (BDTvarname + "/D").c_str());

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
      var[j] = formulavars[j]->EvalInstance();
    }
    //Evaluate BDT
    BDT_response = reader->TMVA::Reader::EvaluateMVA("BDT method");
    tree->Fill();
  }
  tree->Write();

  target->Close();
  data->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 2:
    BDTApply2(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    BDTApply2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    BDTApply2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    BDTApply2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
