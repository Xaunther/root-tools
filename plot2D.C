//Function to do a 2D plot from a file with data
//For now, hard-coded bin range and width

#include "../Functions/Styles.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStyle.h"
#include "TH2F.h"
#include <string>
#include <iostream>
using namespace std;

//Constantitllsos
#define NxBins 15
#define NyBins 15

void plot2D(string datafile, MyStyles::palette palette)
{
  //Central value of the bins
  double xBins[NxBins+1] = {-0.05, 0.05, 0.15, 0.25, 0.35, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.95};
  double yBins[NyBins+1] = {-0.05, 0.05, 0.15, 0.25, 0.35, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.95};

  //Set Style
  MyStyles::SetPalette(palette);

  TH2F* h = new TH2F("caca1", "Combinatorial background yield", NxBins, xBins, NyBins, yBins);
  
  int Nentries = 0;
  int Ncols = 3;

  double** data = GetData(datafile, Nentries, Ncols);

  for(int i=0;i<Nentries;i++)
    {
      h->Fill(data[i][0], data[i][1], data[i][2]);
    }
  cout << Nentries << " events" << endl;
  TLine l;
  TCanvas* c1 = new TCanvas();
  gStyle->SetOptStat(0);
  c1->cd();
  c1->SetRightMargin(0.15);
  h->Draw("COLZ");
  h->GetXaxis()->SetTitle("ProbNNpi(1-ProbNNp)(1-ProbNNk) > x");
  h->GetYaxis()->SetTitle("ProbNNp(1-ProbNNpi)(1-ProbNNk) > y");
  h->GetZaxis()->SetTitle("log(B_{comb})");
  h->GetZaxis()->SetTitleOffset(1.);

  for (int i=1;i<NxBins;i++)
    {
      l.DrawLine((xBins[i]+xBins[i])/2,-0.05,(xBins[i]+xBins[i])/2,0.95);
    }
  for (int i=1;i<NyBins+1;i++)
    {
      l.DrawLine(-0.05,(yBins[i]+yBins[i])/2,0.95,(yBins[i]+yBins[i])/2);
    }

  c1->SaveAs(("plots/"+datafile+".pdf").c_str());
  return;
}
