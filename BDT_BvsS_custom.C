#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <string>
using namespace std;

void BDT_BvsS_custom(string signame, string bkgname, double N0 = 1, string output = "plots/BDT_BvsS_custom.pdf")
{
  TFile* sigfile = TFile::Open(signame.c_str());
  TFile* bkgfile = TFile::Open(bkgname.c_str());
  TTree* sigtree = (TTree*)_sigfile->Get("DecayTree");
  TTree* bkgtree = (TTree*)_bkgfile->Get("DecayTree");

  UInt_t ncand;
  int Nsig = 0;
  sigtree->SetBranchAddress("nCandidate", &ncand);
  for(int i=0;i<sigtree->GetEntries();i++)
    {
      sigtree->GetEntry(i);
      if(ncand==0)
	{
	  Nsig0++;
	}
    }

  for(int i=0;i<101;i++)
    {
      double bdtcut = -1.0+double(i*2)/100.0;
    }



  for(int i=0;i<histo->GetNbinsX()+1;i++)
    {
      histo->SetBinContent(i, histo->GetBinContent(i)*yfactor);
    }
  //X, Y axis titles
  histo->GetYaxis()->SetTitle("Rate (kHz)");
  //histo->SetTitle("Signal average");
  histo->SetTitle("Bu2K1gamma");

  //Canvas
  TCanvas* c1 = new TCanvas();
  c1->SetLogy();
  c1->SetGrid();

  //Draw and save
  gStyle->SetOptStat(0);
  histo->Draw();
  c1->SaveAs(output.c_str());
}
