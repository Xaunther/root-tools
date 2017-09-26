#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
using namespace std;

void BDTMerit(double init_value, double final_value, int steps = 100)
{
  //Going to plot 1 variable and then extend it with a loop
  
  TFile* tuplefile = new TFile("Tuples/data-mva_output.root");
  TTree* tree = (TTree*)tuplefile->Get("tree"); //Get Tree from corresponding directory
  //2012 Data
  double* merit = new double[steps+1];
  double* cutx = new double[steps+1];
  double* fsigarray = new double[steps+1];
  //Cut the trees
  TFile* cutfile = TFile::Open("Tuples/BDToutputtree.root", "RECREATE");
  for(int i=0;i<=steps;i++)
    {
      stringstream ss;
      cutx[i] = init_value + double(i)*(final_value-init_value)/steps;
      ss << "BDT_response > " << cutx[i];
      TTree* cuttree = tree->CopyTree(ss.str().c_str());
      //Initialize all the stuff needed for the fit
      RooRealVar B_M("B_M", "B_M", 4300, 6300);
      RooRealVar mean("mean", "mean", 5288, 5200, 5400);
      RooRealVar width("width","width", 100, 10, 200);
      RooGaussian signal("signal","signal",B_M,mean,width);
      RooRealVar tau("tau", "tau", -0.00027, -0.004, -0.0001);
      RooExponential bkg("bkg","bkg", B_M, tau);
      RooRealVar fsig("fsig","fsig",0.1, 0 ,1);
      RooAddPdf model("model", "model", RooArgList(signal,bkg),fsig);
      RooDataSet data("data", "data", cuttree, RooArgSet(B_M));
      model.fitTo(data);
      merit[i] = fsig.getValV()*TMath::Sqrt(cuttree->GetEntries());
      fsigarray[i] = fsig.getValV();
    }
  //Before making a plot, output the values
  cout << "  Cut       Merit           Frac     " << endl;
  cout << "-------|--------------|--------------" << endl;
  for(int i=0;i<=steps;i++)
    {
      cout << setfill(' ') << setw(7) << cutx[i] << "|" << setw(14) << merit[i] << "|" << setw(14) << fsigarray[i] << endl;
    }
}
