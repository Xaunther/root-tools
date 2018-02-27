#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include <string>

using namespace std;

void Compare(string filename1, string filename2, string variable, string cuts)
{
  //Open files and tuples
  TFile* file1 = TFile::Open(filename1.c_str());
  TFile* file2 = TFile::Open(filename2.c_str());
  TTree* tree1 = (TTree*)file1->Get("TrainTree");
  TTree* tree2 = (TTree*)file2->Get("TestTree");

  TCanvas* c1 = new TCanvas();
  tree1->Draw(variable.c_str(), cuts.c_str(), "HISTO");
  tree2->Draw(variable.c_str(), cuts.c_str(), "HISTO SAME");

  TH1* hist1 = tree1->GetHistogram();
  TH1* hist2 = tree2->GetHistogram();

  hist1->SetFillColorAlpha(kBlue, 1);
  hist2->SetFillColorAlpha(kRed, 1);

  hist1->SetLineColor(kBlue);
  hist2->SetLineColor(kRed);

  hist1->SetFillStyle(3004);
  hist2->SetFillStyle(3005);

  c1->SaveAs(("plots/compare_"+variable+".pdf").c_str());

  file1->Close();
  file2->Close();
}
