#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include <string>
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"

using namespace std;
void Compare(string filename1, string filename2, string variable, string cutfile, string treename1 = "", string treename2 = "", string wvar1 = "1", string wvar2 = "1")
{
  //Open files and tuples
  TChain* chain1 = GetChain(filename1, treename1);
  TChain* chain2 = GetChain(filename2, treename2);
  string cuts = GetCuts(cutfile);
  TCanvas* c1 = new TCanvas();
  
  chain1->Draw(variable.c_str(), ("("+cuts+")*"+wvar1).c_str(), "HISTO NORM");
  chain2->Draw(variable.c_str(), ("("+cuts+")*"+wvar2).c_str(), "HISTO NORM SAME");

  TH1* hist1 = chain1->GetHistogram();
  TH1* hist2 = chain2->GetHistogram();

  hist1->SetFillColorAlpha(kBlue, 1);
  hist2->SetFillColorAlpha(kRed, 1);

  hist1->SetLineColor(kBlue);
  hist2->SetLineColor(kRed);

  hist1->SetFillStyle(3004);
  hist2->SetFillStyle(3005);

  c1->SaveAs(("plots/compare_"+variable+".pdf").c_str());

}
