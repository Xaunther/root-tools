//Does the same thing as Compare.C, which is comparing a variable between 2 given samples
//But instead it takes a file with a list of variables to compare, so it produces MANY! comparisons

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TLegend.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/StringTools.h"

using namespace std;
void CompareList(string filename1, string filename2, string var1, string var2, string cutfile1, string cutfile2, string treename1 = "", string treename2 = "", string wvar1 = "1", string wvar2 = "1", string outputfolder = "", string title = "", string opts = "HISTO NORM", bool compare_weights2 = false)
{
  //Open files and tuples
  TChain *chain1 = GetChain(filename1, treename1);
  TChain *chain2 = GetChain(filename2, treename2);
  TChain *chain2_no_w = GetChain(filename2, treename2);
  string cuts1 = GetCuts(cutfile1);
  string cuts2 = GetCuts(cutfile2);
  TCanvas *c1 = new TCanvas();

  //Open the variable lists
  int var_N1 = 0;
  string *varname1 = ReadVariables(var_N1, var1);
  int var_N2 = 0;
  string *varname2 = ReadVariables(var_N2, var2);

  //If the sizes of the arrays are different, raise error and exit
  if (var_N1 != var_N2)
  {
    throw std::invalid_argument("Number of variables in each list is different. " + std::to_string(var_N1) + " vs " + std::to_string(var_N2));
  }

  //Loop for each variable
  for (int i = 0; i < var_N1; i++)
  {
    gStyle->SetOptStat(0);
    chain1->Draw(varname1[i].c_str(), ("(" + cuts1 + ")*" + wvar1).c_str(), opts.c_str());
    chain2->Draw(varname2[i].c_str(), ("(" + cuts2 + ")*" + wvar2).c_str(), (opts + " SAME").c_str());

    TH1 *hist1 = chain1->GetHistogram();
    TH1 *hist2 = chain2->GetHistogram();

    hist1->SetFillColorAlpha(kBlue, 1);
    hist2->SetFillColorAlpha(kRed, 1);

    hist1->SetLineColor(kBlue);
    hist2->SetLineColor(kRed);

    hist1->SetFillStyle(3004);
    hist2->SetFillStyle(3005);

    hist1->SetTitle(title.c_str());

    //Include unweighted MC, if it's the case
    TLegend *legend;
    if (compare_weights2)
    {
      chain2_no_w->Draw(varname2[i].c_str(), ("(" + cuts2 + ")").c_str(), (opts + " SAME").c_str());
      TH1 *hist2_no_w = chain2_no_w->GetHistogram();
      hist2_no_w->SetFillColorAlpha(kGreen, 1);
      hist2_no_w->SetLineColor(kGreen);
      hist2->SetFillStyle(3006);
      legend = new TLegend(0.8, 0.7, 0.99, 0.9);
      legend->AddEntry(hist1, "sWeighted data", "pl");
      legend->AddEntry(hist2, "Reweighted MC", "pl");
      legend->AddEntry(hist2_no_w, "Unweighted MC", "pl");
      legend->Draw();
    }

    c1->SaveAs((outputfolder + "var" + std::to_string(i) + ".pdf").c_str());
  }
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
  switch (argc - 1)
  {
  case 6:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  case 7:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
                *(new string(argv[7])));
    break;
  case 8:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
                *(new string(argv[7])), *(new string(argv[8])));
    break;
  case 9:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
                *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])));
    break;
  case 10:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
                *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])));
    break;
  case 11:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
                *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])));
    break;
  case 12:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
                *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])));
    break;
  case 13:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
                *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])));
    break;
  case 14:
    CompareList(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])),
                *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])), (int)argv[14][0] - 48);
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
