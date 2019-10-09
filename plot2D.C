//Function to do a 2D plot from a file with data
//For now, hard-coded bin range and width

#include "../Functions/Styles.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStyle.h"
#include "TH2F.h"
#include <string>
#include <iostream>
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
#include "../Functions/StringTools.h"
using namespace std;

//Constantitllsos
#define NxBins 15
#define NyBins 15

void plot2D(string datafile, MyStyles::palette palette, string title = "")
{
  //Central value of the bins
  double xBins[NxBins+1] = {-0.05, 0.05, 0.15, 0.25, 0.35, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.95};
  double yBins[NyBins+1] = {-0.05, 0.05, 0.15, 0.25, 0.35, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.95};

  //Set Style
  MyStyles::SetPalette(palette);

  TH2F* h = new TH2F("caca1", "Particle ID figure of merit", NxBins, xBins, NyBins, yBins);
  
  int Nentries = 0;
  int Ncols = 3;

  double** data = GetData(datafile, Nentries, Ncols);

  for(int i=0;i<Nentries;i++)
    {
      h->Fill(data[i][0], data[i][1], data[i][2]);
    }
  cout << Nentries << " events" << endl;
  //Deal with the titles
  int title_index = 0;
  string* _titles = SplitString(title_index, title, ";");
  string* titles = new string[4];
  int max_index = (title_index > 4) ? 4 : title_index;
  for(int i=0;i<max_index;i++)
    {
      //Only the first three
      
      titles[i] = _titles[i];
    }
  TLine l;
  TCanvas* c1 = new TCanvas();
  gStyle->SetOptStat(0);
  c1->cd();
  c1->SetRightMargin(0.15);
  h->Draw("COLZ");
  h->SetTitle (titles[0].c_str());
  h->SetXTitle(titles[1].c_str());
  h->SetYTitle(titles[2].c_str());
  h->SetZTitle(titles[3].c_str());
  h->GetZaxis()->SetTitleOffset(1.);

  for (int i=1;i<NxBins;i++)
    {
      l.DrawLine((xBins[i]+xBins[i])/2,-0.05,(xBins[i]+xBins[i])/2,0.95);
    }
  for (int i=1;i<NyBins+1;i++)
    {
      l.DrawLine(-0.05,(yBins[i]+yBins[i])/2,0.95,(yBins[i]+yBins[i])/2);
    }

  //Extract filename from directory
  int dir_size = 0;
  string* dirname = SplitString(dir_size, datafile, "/");
  c1->SaveAs(("plots/"+dirname[dir_size-1]+".pdf").c_str());
  return;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  MyStyles::palette palette = MyStyles::StringToPalette(*(new string(argv[2])));
  switch(argc-1)
    {
    case 2:
      plot2D(*(new string(argv[1])), palette);
      break;
    case 3:
      plot2D(*(new string(argv[1])), palette, *(new string(argv[3])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for plot2D" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
