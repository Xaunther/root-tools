#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TLegend.h"
#include <string>
#include <iostream>
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/StringTools.h"

using namespace std;
void Compare(string filename1, string filename2, string var1, string var2, string cutfile1, string cutfile2, string treename1 = "", string treename2 = "", string wvar1 = "1", string wvar2 = "1", string title = "", string outputname = "", string opts = "HISTO NORM", bool compare_weights2 = false, string legend_labels = "sWeighted data; Reweighted MC; Unweighted MC")
{
  //Open files and tuples
  TChain *chain1 = GetChain(filename1, treename1);
  TChain *chain2 = GetChain(filename2, treename2);
  TChain *chain2_no_w = GetChain(filename2, treename2);
  string cuts1 = GetCuts(cutfile1);
  string cuts2 = GetCuts(cutfile2);
  TCanvas *c1 = new TCanvas();

  //Deal with the titles
  int title_index = 0;
  string *_titles = SplitString(title_index, title, ";");
  string *titles = new string[3];
  int max_index = (title_index > 3) ? 3 : title_index;
  for (int i = 0; i < max_index; i++)
  {
    //Only the first three
    titles[i] = _titles[i];
  }

  //Deal with the legends
  int legend_index = 0;
  string *_legends = SplitString(legend_index, legend_labels, ";");
  string *legends = new string[3];
  int max_legend_index = (legend_index > 3) ? 3 : legend_index;
  for (int i = 0; i < max_legend_index; i++)
  {
    //Only the first three
    legends[i] = _legends[i];
  }

  gStyle->SetOptStat(0);
  chain1->Draw(var1.c_str(), ("(" + cuts1 + ")*" + wvar1).c_str(), opts.c_str());
  chain2->Draw(var2.c_str(), ("(" + cuts2 + ")*" + wvar2).c_str(), (opts + " SAME").c_str());

  TH1 *hist1 = chain1->GetHistogram();
  TH1 *hist2 = chain2->GetHistogram();

  hist1->SetFillColorAlpha(kBlue, 1);
  hist2->SetFillColorAlpha(kRed, 1);

  hist1->SetLineColor(kBlue);
  hist2->SetLineColor(kRed);

  hist1->SetFillStyle(3004);
  hist2->SetFillStyle(3005);

  hist1->SetTitle(titles[0].c_str());
  hist1->SetXTitle(titles[1].c_str());
  hist1->SetYTitle(titles[2].c_str());

  //Include unweighted MC, if it's the case
  TLegend *legend;
  if (compare_weights2)
  {
    chain2_no_w->Draw(var2.c_str(), ("(" + cuts2 + ")").c_str(), (opts + " SAME").c_str());
    TH1 *hist2_no_w = chain2_no_w->GetHistogram();
    hist2_no_w->SetFillColorAlpha(kGreen, 1);
    hist2_no_w->SetLineColor(kGreen);
    hist2->SetFillStyle(3006);
    legend = new TLegend(0.8, 0.7, 0.99, 0.9);
    legend->AddEntry(hist1, legends[0].c_str(), "pl");
    legend->AddEntry(hist2, legends[1].c_str(), "pl");
    legend->AddEntry(hist2_no_w, legends[2].c_str(), "pl");
    legend->Draw();
  }

  if (outputname == "")
    c1->SaveAs(("plots/compare_" + var1 + "_" + var2 + ".pdf").c_str());
  else
    c1->SaveAs(outputname.c_str());
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
  switch (argc - 1)
  {
  case 6:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  case 7:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])));
    break;
  case 8:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])), *(new string(argv[8])));
    break;
  case 9:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])));
    break;
  case 10:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])));
    break;
  case 11:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])));
    break;
  case 12:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])));
    break;
  case 13:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])));
    break;
  case 14:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])), (int)argv[14][0] - 48);
    break;
  case 15:
    Compare(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
            *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])), (int)argv[14][0] - 48,
            *(new string(argv[15])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
