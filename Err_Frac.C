#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TFile.h"
#include "../Functions/StringTools.h"
using namespace std;

void Err_Frac(int steps = 100, string filename = "Tuples/BDT-results_OP.root", string treename = "TestTree")
{
  TFile* file = new TFile(Gridify(filename).c_str());
  TTree* tree = (TTree*)file->Get(treename.c_str());

  int N_sig_init = tree->GetEntries("classID==0");
  int N_bkg_init = tree->GetEntries("classID==1");

  int N_sig = 0;
  int N_bkg = 0;

  double x0 = -0.2;
  double xf = 0.2;
  double w = (xf - x0) / double(steps);
  for (int i = 0; i < steps + 1; i++)
  {
    stringstream cut;
    cut << "BDT > " << x0 + w*i;

    N_sig = tree->GetEntries(("classID==0 && " + cut.str()).c_str());
    N_bkg = tree->GetEntries(("classID==1 && " + cut.str()).c_str());

    //Check if already traversed middle point
    if (double(N_bkg) / N_bkg_init + double(N_sig) / N_sig_init < 1.)
    {
      break;
    }
    cut.str("");
  }

  double errf = double(N_bkg + N_sig_init - N_sig) / double(N_sig_init + N_bkg_init);
  cout << endl << filename << " | " << errf << endl << endl;
  file->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 0:
    Err_Frac();
    break;
  case 1:
    Err_Frac(stoi(*(new string(argv[1]))));
    break;
  case 2:
    Err_Frac(stoi(*(new string(argv[1]))), *(new string(argv[2])));
    break;
  case 3:
    Err_Frac(stoi(*(new string(argv[1]))), *(new string(argv[2])), *(new string(argv[3])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
