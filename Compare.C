#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include <string>
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/StringTools.h"

using namespace std;
void Compare(string filename1, string filename2, string var1, string var2, string cutfile1, string cutfile2, string treename1 = "", string treename2 = "", string wvar1 = "1", string wvar2 = "1", string title = "")
{
  //Open files and tuples
  TChain* chain1 = GetChain(filename1, treename1);
  TChain* chain2 = GetChain(filename2, treename2);
  string cuts1 = GetCuts(cutfile1);
  string cuts2 = GetCuts(cutfile2);
  TCanvas* c1 = new TCanvas();

  //Deal with the titles
  int title_index = 0;
  string* _titles = SplitString(title_index, title, ";");
  string* titles = new string[3];
  int max_index = (title_index > 3) ? 3 : title_index;
  for(int i=0;i<max_index;i++)
    {
      //Only the first three
      
      titles[i] = _titles[i];
    }

  gStyle->SetOptStat(0);
  chain1->Draw(var1.c_str(), ("("+cuts1+")*"+wvar1).c_str(), "HISTO NORM");
  chain2->Draw(var2.c_str(), ("("+cuts2+")*"+wvar2).c_str(), "HISTO NORM SAME");

  TH1* hist1 = chain1->GetHistogram();
  TH1* hist2 = chain2->GetHistogram();

  hist1->SetFillColorAlpha(kBlue, 1);
  hist2->SetFillColorAlpha(kRed, 1);

  hist1->SetLineColor(kBlue);
  hist2->SetLineColor(kRed);

  hist1->SetFillStyle(3004);
  hist2->SetFillStyle(3005);

  hist1->SetTitle (titles[0].c_str());
  hist1->SetXTitle(titles[1].c_str());
  hist1->SetYTitle(titles[2].c_str());

  c1->SaveAs(("plots/compare_"+var1+"_"+var2+".pdf").c_str());

}
