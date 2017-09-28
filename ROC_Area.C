#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TH1F.h"
#include "TFile.h"
using namespace std;

void ROC_Area(string filename = "Tuples/BDT-results.root", string histname = "Method_BDT/BDT/MVA_BDT_rejBvsS")
{
  TFile* file = new TFile(filename.c_str());
  TH1F* hist = (TH1F*)file->Get(histname.c_str());
  double w = 0.01;
  double area = 0;
  for(int i=1;i<100;i++)
    {
      area += w*(hist->GetBinContent(i)+hist->GetBinContent(i+1))/double(2);
    }
  
  cout << endl << "Area of the curve: " << area << endl << endl;
  file->Close();
}
