#include "TH1D.h"
#include <iostream>
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TArrayD.h"
#include <string>
#include "../Dictionaries/Constants.h"
#include "../Functions/Dictreading.h"
using namespace std;

void BDT_BvsS(string filename, double yfactor = 1, double xfactor = 1, string output = "plots/BDT_BvsS.pdf")
{
  TFile* file = TFile::Open(filename.c_str());
  TH1D* histo = (TH1D*)file->Get("Method_BDT/BDT/MVA_BDT_effBvsS");

  Constants const_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  for(int i=0;i<histo->GetNbinsX()+1;i++)
    {
      histo->SetBinContent(i, histo->GetBinContent(i)*yfactor);
    }
  //X, Y axis titles
  histo->GetYaxis()->SetTitle("Rate (kHz)");
  int Nbins = histo->GetXaxis()->GetNbins();
  double* xbin = new double[Nbins+1];
  Double_t* xbin0 = new Double_t[Nbins+1];
  histo->GetXaxis()->GetLowEdge(xbin0);
  for(int i=0;i<Nbins;i++)
    {
      xbin[i] = xbin0[i]*xfactor;
    }
  xbin[Nbins] = 1*xfactor;
  histo->GetXaxis()->Set(Nbins, xbin);
  //histo->SetTitle("Signal average");
  histo->SetTitle("HHgamma Line");

  //Canvas
  TCanvas* c1 = new TCanvas();
  c1->SetLogy(const_list.logY);
  c1->SetGrid();

  //Draw and save
  gStyle->SetOptStat(0);
  histo->Draw();
  c1->SaveAs(output.c_str());
}
