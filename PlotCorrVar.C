#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
#include "TMath.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
using namespace std;
void PlotCorrVar(string histopt = "", string drawopt = "", string varfile = "Variables/VariablesCorr.txt", string cutfile = "Variables/Cuts.txt", string tupledir = "Directories/Cuttuples2.dir", string fileext = ".pdf")
{
  int N_vars = 0;
  string* variables = ReadVariables(N_vars, varfile);

  string cuts = GetCuts(cutfile);
  //Data chain
  TChain* chain = GetChain(tupledir);
  //Plot and save bro
  TCanvas* c1 = new TCanvas();
  cout << "Plotting..." << endl;
  for (int i = 0; i < N_vars; i++)
  {
    for (int j = i + 1; j < N_vars; j++)
    {
      chain->Draw((variables[i] + ":" + variables[j] + histopt).c_str(), cuts.c_str(), drawopt.c_str());
      c1->SaveAs(("plots/" + variables[i] + "_VS_" + variables[j] + fileext).c_str());
    }
  }
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
    case 0:
    PlotCorrVar();
    break;
    case 1:
    PlotCorrVar(*(new string(argv[1])));
    break;
    case 2:
    PlotCorrVar(*(new string(argv[1])), *(new string(argv[2])));
    break;
    case 3:
    PlotCorrVar(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
    case 4:
    PlotCorrVar(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
    case 5:
    PlotCorrVar(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
    case 6:
    PlotCorrVar(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
    default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
