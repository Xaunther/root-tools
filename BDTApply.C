#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <TXMLEngine.h>
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include <sstream>
#include "Functions/misc.h"
using namespace std;

void BDTApply(string fileapplied, string outputfilename, bool logdira = false)
{
  int N_variables = 0;
  int N_extravars = 0;
  int N_extrauint = 0;
  int N_extrashort = 0;
  string filename = "Variables/BDTVariables.txt";
  string filename2= "Variables/BDTExtravars.txt";
  string filenameInt = "Variables/BDTExtravarsUInt.txt";
  string filenameShort = "Variables/BDTExtravarsShort.txt";
  string* variable_list = ReadVariables(N_variables, filename);
  string* extravar_list = ReadVariables(N_extravars, filename2);
  string* extrauint_list = ReadVariables(N_extrauint, filenameInt);
  string* extrashort_list = ReadVariables(N_extrashort, filenameShort);

  TFile* data = new TFile(fileapplied.c_str());
  TTree* datatree = (TTree*)data->Get("DecayTree");

  //BDT Output
  TFile* target = new TFile(outputfilename.c_str(), "RECREATE");
  TTree* tree = new TTree("DecayTree", "treelibrated tree");

  TMVA::Tools::Instance();
  TMVA::Reader* reader =  new TMVA::Reader("V:Color:!Silent");

  Float_t* var = new Float_t[N_variables];
  //Variables used in training
  for(int i=0;i<N_variables;i++)
    {
      if (variable_list[i] == "B_DIRA_OWNPV" && logdira)
	{
	  string logvarname = "log_B_DIRA_OWNPV := log(1-B_DIRA_OWNPV)";
	  reader->TMVA::Reader::AddVariable(logvarname.c_str(), &var[i]);
	}
      else
	{
	  reader->TMVA::Reader::AddVariable(variable_list[i].c_str(), &var[i]);
	}
    }
  //TMVA method
  reader->TMVA::Reader::BookMVA("BDT method", "weights/TMVAClassification_BDT.weights.xml");

  //Variables from data
  Double_t* uservar = new Double_t[N_variables + N_extravars];
  UInt_t* userint = new UInt_t[N_extrauint];
  Short_t* usershort = new Short_t[N_extrashort];
  for(int i=0;i<N_variables;i++)
    {
      datatree->SetBranchAddress(variable_list[i].c_str(), &uservar[i]);
      tree->Branch(variable_list[i].c_str(), &uservar[i]);
    }
  for(int i=0;i<N_extravars;i++)
    {
      datatree->SetBranchAddress(extravar_list[i].c_str(), &uservar[N_variables + i]);
      tree->Branch(extravar_list[i].c_str(), &uservar[N_variables + i]);
    }
  for(int i=0;i<N_extrauint;i++)
    {
      datatree->SetBranchAddress(extrauint_list[i].c_str(), &userint[i]);
      tree->Branch(extrauint_list[i].c_str(), &userint[i]);
    }
  for(int i=0;i<N_extrashort;i++)
    {
      datatree->SetBranchAddress(extrashort_list[i].c_str(), &usershort[i]);
      tree->Branch(extrashort_list[i].c_str(), &usershort[i]);
    }


  //Add branch to store BDT response
  double BDT_response;
  tree->Branch("BDT_response", &BDT_response);

  //Apply BDT
  for(long k=0; k<datatree->GetEntries(); k++)
    {
      //Some output to see it's still alive
      if (k%100000 == 0)
	{
	  cout << "--- ... Processing event: " << k << endl;
	}
      datatree->GetEntry(k);

      //Here we can perform operations if we made the log or smth
      for (int j=0;j<N_variables;j++)
	{
	  var[j] = uservar[j];
	}

      BDT_response = reader->TMVA::Reader::EvaluateMVA("BDT method");
      tree->Fill();
    }
  tree->Write();
  
  data->Close();
  target->Close();
}

