//Script that reads histogram and computes area.
//Designed to compute ROC area from ROC Curve histogram from TMVA
#include <iostream>
#include <string>
#include "TH1D.h"
#include "TFile.h"
using namespace std;

void ROC_Area(string filename, string histname = "default/Method_BDT/BDT/MVA_BDT_trainingRejBvsS")
{
  TFile* file = new TFile(filename.c_str());
  TH1D* hist = (TH1D*)file->Get(histname.c_str());
  double area = 0;
  for(int i=1;i<=hist->GetEntries();i++)
    {
      area += hist->GetBinWidth(i)*hist->GetAt(i);
    }
  
  cout << endl << "Area of the curve: " << area << endl << endl;
  file->Close();
}
