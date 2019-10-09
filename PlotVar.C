#include <iostream>
#include <string>
#include "TChain.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/StringTools.h"
using namespace std;
void PlotVar(string plotvar, string plotopt = "", string tupledir = "Directories/Bd2KstG_tuples.dir", string cutfile="Variables/Cuts.txt", string title = "")
{
  string cuts = GetCuts(cutfile);
  //Data chain  
  TChain* chain = GetChain(tupledir);

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

  //Plot and save bro
  TCanvas* c1 = new TCanvas();
  cout << "Plotting..." << endl;
  chain->Draw(plotvar.c_str(), cuts.c_str(), plotopt.c_str());

  //Set the titles, then save
  TH1* hist = chain->GetHistogram();
  hist->SetTitle (titles[0].c_str());
  hist->SetXTitle(titles[1].c_str());
  hist->SetYTitle(titles[2].c_str());
  
  c1->SaveAs(("plots/"+plotvar+".pdf").c_str());

}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc)
    {
    case 1:
      PlotVar(*(new string(argv[1])));
      break;
    case 2:
      PlotVar(*(new string(argv[1])), *(new string(argv[2])));
      break;
    case 3:
      PlotVar(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    case 4:
      PlotVar(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
      break;
    case 5:
      PlotVar(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for PlotVar" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
