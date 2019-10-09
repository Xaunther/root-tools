#include "TH1D.h"
#include <iostream>
#include "TFile.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStyle.h"
#include <string>
#include "../Dictionaries/Constants.h"
#include "../Functions/Dictreading.h"
using namespace std;

void BDT_BvsS(string filename, double yfactor = 1, double xfactor = 1, double oldrate = 0, double oldeff = 0, string output = "plots/BDT_BvsS.pdf")
{
  TFile* file = TFile::Open(filename.c_str());
  TH1D* histo = (TH1D*)file->Get("default/Method_BDT/BDT/MVA_BDT_effBvsS");

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
  histo->SetTitle(const_list.plot_title.c_str());

  //Canvas
  TCanvas* c1 = new TCanvas();
  c1->SetLogy(const_list.logY);
  c1->SetGrid();
  //Draw and save
  gStyle->SetOptStat(0);
  histo->Draw();
  //Draw old performance
  TLine* RateLine = new TLine(histo->GetXaxis()->GetXmin(), oldrate, histo->GetXaxis()->GetXmax(), oldrate);
  TLine* EffLine = new TLine(oldeff, histo->GetYaxis()->GetXmin()*yfactor, oldeff, histo->GetBinContent(histo->GetNbinsX())*21/20);
  RateLine->SetLineColor(kRed);
  EffLine->SetLineColor(kRed);
  RateLine->Draw("same");
  EffLine->Draw("same");
  c1->SaveAs(output.c_str());
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 1:
      BDT_BvsS(*(new string(argv[1])));
      break;
    case 2:
      BDT_BvsS(*(new string(argv[1])), stod(*(new string(argv[2]))));
      break;
    case 3:
      BDT_BvsS(*(new string(argv[1])), stod(*(new string(argv[2]))), stod(*(new string(argv[3]))));
      break;      
    case 4:
      BDT_BvsS(*(new string(argv[1])), stod(*(new string(argv[2]))), stod(*(new string(argv[3]))), stod(*(new string(argv[4]))));
      break;
    case 5:
      BDT_BvsS(*(new string(argv[1])), stod(*(new string(argv[2]))), stod(*(new string(argv[3]))), stod(*(new string(argv[4]))), stod(*(new string(argv[5]))));
      break;
    case 6:
      BDT_BvsS(*(new string(argv[1])), stod(*(new string(argv[2]))), stod(*(new string(argv[3]))), stod(*(new string(argv[4]))), stod(*(new string(argv[5]))), *(new string(argv[6])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for BDT_BvsS" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
