//Take a ntuple, or a file containing a list of them, and calculate the efficiency of each cut given in cutfile, starting from a previous set of cuts, precutfile
//The output is given by outfile. Event weights can be included using weight

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TChain.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLine.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Functions/TreeTools.h"
using namespace std;

void EffPerBin(string dirfile, string cutfile, string varname, string binfile, string outfile, string plotfile, string precutfile = "1", string weight = "1", string title = "")
{
  //Get the cuts and the precuts
  string cuts = GetCuts(cutfile);
  string precuts = GetCuts(precutfile);

  //Get the tree, it might use some precuts
  TChain* chain = GetChain(dirfile);

  //Get the binning!
  int NBins=0;
  double* binning = ReadNumbers(NBins, binfile);
  //Construct the array for bin-efficiency
  double* bineff = new double[NBins-1];
  double* errbineff = new double[NBins-1];
  //Total efficiency and error
  double eff;
  //double erreff;
  //Get total efficiency and error, total and per bin
  stringstream bincut;
  bincut << " * (" << varname << " > " << binning[0] << ") * (" << varname << " < " << binning[NBins-1] << ")";
  eff = GetMeanEntries(chain, cuts+bincut.str(), weight)/GetMeanEntries(chain, precuts+bincut.str(), weight);
  //erreff = TMath::Sqrt(eff*(1-eff)/chain->GetEntries()*GetMeanEntries(chain, precuts+bincut.str(), weight+ " * "+weight))/GetMeanEntries(chain, precuts+bincut.str(), weight);
  bincut.str("");
  for(int i=0;i<NBins-1;i++)
  {
    bincut << " * (" << varname << " > " << binning[i] << ") * (" << varname << " < " << binning[i+1] << ")";
    bineff[i] = GetMeanEntries(chain, cuts+bincut.str(), weight)/GetMeanEntries(chain, precuts+bincut.str(), weight);
    errbineff[i] = TMath::Sqrt(bineff[i]*(1-bineff[i])/chain->GetEntries()*GetMeanEntries(chain, precuts+bincut.str(), weight+ " * "+weight))/GetMeanEntries(chain, precuts+bincut.str(), weight);
    bincut.str("");
  }

  //Save array of efficiencies in desired file
  ofstream fout;
  fout.open(outfile.c_str());
  for(int i=0;i<NBins-1;i++)
  {
    fout << (binning[i]+binning[i+1])/2. << "  |  " << bineff[i] << " \u00B1 " <<  errbineff[i] << endl;
  }

  //Time to create our graph. We have the Y axis but we need to build the X too
  double* bin = new double[NBins-1];
  double* errbin = new double[NBins-1];
  for(int i =0;i<NBins-1;i++)
  {
    bin[i] = (binning[i]+binning[i+1])/2.;
    errbin[i] = (binning[i+1] - binning[i])/2.;
  }
  //Construct graph
  TGraphErrors* graph = new TGraphErrors(NBins-1, bin, bineff, errbin, errbineff);
  if(title==""){title=";"+varname+";Efficiency";} //Set some default title
  graph->SetTitle(title.c_str());
  graph->SetMarkerStyle(kFullCircle);
  graph->SetMinimum(0.); graph->SetMaximum(1.);

  //Define canvas, plot and save.
  TCanvas* c1 = new TCanvas();
  c1->cd();
  graph->Draw("ap");
  //Construct lines to show average
  TLine* meanline = new TLine(graph->GetXaxis()->GetXmin(), eff, graph->GetXaxis()->GetXmax(), eff);
  meanline->SetLineWidth(1); meanline->SetLineColor(kRed); meanline->SetLineStyle(kDashed);
  //TLine* toperrorline = new TLine(graph->GetXaxis()->GetXmin(), eff+erreff, graph->GetXaxis()->GetXmax(), eff+erreff);
  //TLine* boterrorline = new TLine(graph->GetXaxis()->GetXmin(), eff-erreff, graph->GetXaxis()->GetXmax(), eff-erreff);
  //toperrorline->SetLineColor(kRed); toperrorline->SetLineStyle(kDashed);
  //boterrorline->SetLineColor(kRed); boterrorline->SetLineStyle(kDashed);

  meanline->Draw("same");
  //toperrorline->Draw("same");
  //boterrorline->Draw("same");
  c1->SaveAs(plotfile.c_str());

  //Close files and clean memory
  fout.close();
  CloseChain(chain);
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
  {
    case 6:
    EffPerBin(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
    case 7:
    EffPerBin(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
    case 8:
    EffPerBin(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])));
    break;
    case 9:
    EffPerBin(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])));
    break;
    default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return(1);
    break;
  }
  return 0;
}
#endif
