#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TMath.h"
using namespace std;

void Vodka(string varname, bool binned = false)
{
  TFile* file = TFile::Open("Tuples/BDT-results.root");
  TTree* testtree = (TTree*)file->Get("TestTree");
  TTree* traintree = (TTree*)file->Get("TrainTree");

  if(!binned)
    {
      // UNBINNED VODKA TEST  
      //   ONLY IF YOU HAVE AN ORDERED DATASET, OTHERWISE THIS WILL GIVE ZERO OF COURSARS
      
      Double_t* testsig = new Double_t[testtree->GetEntries("classID==0")];
      Double_t* testbkg = new Double_t[testtree->GetEntries("classID==1")];    
      Double_t* trainsig = new Double_t[traintree->GetEntries("classID==0")];
      Double_t* trainbkg = new Double_t[traintree->GetEntries("classID==1")];  
      
      int N_S = 0;
      int N_B = 0;
      float basura;
      int classID;
      testtree->SetBranchAddress(varname.c_str(), &basura);
      testtree->SetBranchAddress("classID", &classID);  
      for(int i=0;i<testtree->GetEntries();i++)
	{
	  testtree->GetEntry(i);
	  if(classID == 0)
	    {
	      testsig[N_S] = basura;
	      N_S++;
	    }
	  else
	    {
	      testbkg[N_B] = basura;
	      N_B++;
	    }
	}
      N_S = 0; N_B = 0;
      traintree->SetBranchAddress(varname.c_str(), &basura);
      traintree->SetBranchAddress("classID", &classID);  
      for(int i=0;i<traintree->GetEntries();i++)
	{
	  traintree->GetEntry(i);
	  if(classID == 0)
	    {
	      trainsig[N_S] = basura;
	      N_S++;
	    }
	  else
	    {
	      trainbkg[N_B] = basura;
	      N_B++;
	    }
	}
      
      //Ordenar
      testsig = Ordenar(testtree->GetEntries("classID==0"), testsig);
      testbkg = Ordenar(testtree->GetEntries("classID==1"),testbkg);
      trainsig = Ordenar(traintree->GetEntries("classID==0"), trainsig);
      trainbkg = Ordenar(traintree->GetEntries("classID==1"), trainbkg);
      
      TMath::KolmogorovTest(testtree->GetEntries("classID==0"), testsig, traintree->GetEntries("classID==0"), trainsig, "D");
      TMath::KolmogorovTest(testtree->GetEntries("classID==1"), testbkg, traintree->GetEntries("classID==1"), trainbkg, "D");
    }
  else
    {
      // BINNED VODKA TEST
      TCanvas* c2 = new TCanvas();
      TCanvas* c1 = new TCanvas();
  
      testtree->Draw(varname.c_str(), "classID == 0");
      c2->cd();
      traintree->Draw(varname.c_str(), "classID == 0");
      
      cout << "Signal test: " << testtree->GetHistogram()->KolmogorovTest(traintree->GetHistogram()) << endl;

      TCanvas* c4 = new TCanvas();
      TCanvas* c3 = new TCanvas();

      testtree->Draw( varname.c_str(), "classID == 1");
      c4->cd();
      traintree->Draw(varname.c_str(), "classID == 1");
      
      cout << "Background test: " << testtree->GetHistogram()->KolmogorovTest(traintree->GetHistogram()) << endl;
    }
}
