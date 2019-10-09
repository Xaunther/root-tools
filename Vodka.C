#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TMath.h"
#include "../Functions/Ordenar.h"
using namespace std;

void Vodka(string filename1, string filename2, string treename1, string treename2, string varname, bool binned = false)
{
  //Open files and tuples                                                                                                                                                           
  TFile* file1 = TFile::Open(filename1.c_str());
  TFile* file2 = TFile::Open(filename2.c_str());
  TTree* testtree = (TTree*)file1->Get(treename1.c_str());
  TTree* traintree = (TTree*)file2->Get(treename2.c_str());

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
      testbkg = Ordenar(testtree->GetEntries("classID==1"), testbkg);
      trainsig = Ordenar(traintree->GetEntries("classID==0"), trainsig);
      trainbkg = Ordenar(traintree->GetEntries("classID==1"), trainbkg);
      cout << "Signal:" << endl;
      TMath::KolmogorovTest(testtree->GetEntries("classID==0"), testsig, traintree->GetEntries("classID==0"), trainsig, "D");
      cout << "Background:" << endl;
      TMath::KolmogorovTest(testtree->GetEntries("classID==1"), testbkg, traintree->GetEntries("classID==1"), trainbkg, "D");
    }
  else
    {
      // BINNED VODKA TEST
      TCanvas* c1 = new TCanvas();
      TCanvas* c2 = new TCanvas();
      TCanvas* c3 = new TCanvas();
      TCanvas* c4 = new TCanvas();

      c1->cd();
      testtree->Draw(varname.c_str(), "classID == 0");
      c2->cd();
      traintree->Draw(varname.c_str(), "classID == 0");
      
      cout << "Signal test: " << testtree->GetHistogram()->KolmogorovTest(traintree->GetHistogram()) << endl;
      
      c3->cd();
      testtree->Draw( varname.c_str(), "classID == 1");
      c4->cd();
      traintree->Draw(varname.c_str(), "classID == 1");
      
      cout << "Background test: " << testtree->GetHistogram()->KolmogorovTest(traintree->GetHistogram()) << endl;
    }
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  bool binned = false;
  switch(argc-1)
    {
    case 5:
      Vodka(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
      break;
    case 6:
      if(*(new string(argv[6]))=="true" || *(new string(argv[6]))=="1"){binned = true;}
      Vodka(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), binned);
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for Vodka" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
