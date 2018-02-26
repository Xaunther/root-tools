#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TFile.h"
using namespace std;

void Err_Frac(int steps = 100, string filename = "Tuples/BDT-results_OP.root", string treename = "TestTree")
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get(treename.c_str());

  int N_sig_init = tree->GetEntries("classID==0");
  int N_bkg_init = tree->GetEntries("classID==1");

  int N_sig;
  int N_bkg;

  double x0 = -0.2;
  double xf = 0.2;
  double w = (xf-x0)/double(steps);
  for(int i=0;i<steps+1;i++)
    {
      stringstream cut;
      cut << "BDT > " << x0 + w*i;
      
      N_sig = tree->GetEntries(("classID==0 && " + cut.str()).c_str());
      N_bkg = tree->GetEntries(("classID==1 && " + cut.str()).c_str());

      //Check if already traversed middle point
      if(double(N_bkg)/N_bkg_init + double(N_sig)/N_sig_init < 1.)
	{
	  break;
	}
      cut.str("");
    }
  
  double errf = double(N_bkg+N_sig_init-N_sig)/double(N_sig_init+N_bkg_init);
  cout << endl << filename << " | " << errf << endl << endl;
  file->Close();
}
