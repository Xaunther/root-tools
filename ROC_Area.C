#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TFile.h"
using namespace std;

void ROC_Area(string treename, int steps = 100, string filename = "Tuples/BDT-results_OP.root")
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get(treename.c_str());

  int N_sig_init = tree->GetEntries("classID==0");
  int N_bkg_init = tree->GetEntries("classID==1");

  double x0 = -1.;
  double xf = 1.;
  double w = (xf-x0)/double(steps);
  double area = 0;
  for(int i=0;i<steps;i++)
    {
      stringstream cut0, cutf;
      cut0 << "BDT > " << x0 + w*i;
      cutf << "BDT > " << x0 + w*(i+1);
      
      int N_sig0 = tree->GetEntries(("classID==0 && " + cut0.str()).c_str());
      int N_sigf = tree->GetEntries(("classID==0 && " + cutf.str()).c_str());
      int N_bkg0 = tree->GetEntries(("classID==1 && " + cut0.str()).c_str());
      int N_bkgf = tree->GetEntries(("classID==1 && " + cutf.str()).c_str());

      area += (double(-N_sigf+N_sig0)/double(N_sig_init))*(2-double(N_bkgf)/double(N_bkg_init)-double(N_bkg0)/double(N_bkg_init))/2.;
      cut0.str("");
      cutf.str("");
    }
  
  cout << endl << "Area of the curve: " << area << endl << endl;
  file->Close();
}
