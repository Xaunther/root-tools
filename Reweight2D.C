//Script to perform a 2-D Reweighting from a reference sample. Custom binning must be specified with text files, 1 for each variable
//It is assumed that reference and sample being reweighted have the same names for these 2 variables
#include <string>
#include <iostream>
#include "TH2F.h"
#include "TTreeFormula.h"
#include "TFile.h"
#include "TTree.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
using namespace std;

void Reweight2D(string var1, string var2, string reffile, string applyfile, string outfile,
                string binfile1, string binfile2, string refw = "", string applyw = "", string wname = "weight")
{
  //File open!
  TChain* refchain = GetChain(reffile);
  TChain* applychain = GetChain(applyfile);

  //Get the binning!
  int NBins1 = 0, NBins2 = 0;
  double* binning1 = ReadNumbers(NBins1, binfile1);
  double* binning2 = ReadNumbers(NBins2, binfile2);

  //Time to get those histograms filled boi!
  TH2F* refhist = GetHistogram2D(refchain, var1 + ":" + var2, NBins1 - 1, binning1, NBins2 - 1, binning2, "refhist", refw);
  TH2F* applyhist = GetHistogram2D(applychain, var1 + ":" + var2, NBins1 - 1, binning1, NBins2 - 1, binning2, "applyhist", applyw);
  TH2F* divhist = new TH2F(*applyhist);
  divhist->SetNameTitle("divhist", "divhist");
  divhist->Divide(refhist, applyhist, applyhist->GetSumOfWeights(), refhist->GetSumOfWeights());

  //Now we retreive the weights. In each bin, the weight is Nref[i]/Napp[i]
  //We do that for each entry in the ntuple, and see where it fall
  //The variable might be a formula, so....
  //Define TTreeFormula
  TTreeFormula* formulavar1 = new TTreeFormula(var1.c_str(), var1.c_str(), applychain);
  TTreeFormula* formulavar2 = new TTreeFormula(var2.c_str(), var2.c_str(), applychain);

  //Add new branch
  double wvalue, value1, value2;
  TFile* file = new TFile(outfile.c_str(), "RECREATE");
  TTree* tree = applychain->CloneTree(0);
  tree->Branch(wname.c_str(), &wvalue, (wname + "/D").c_str());

  //Loop over all events and get value
  for (int i = 0; i < applychain->GetEntries(); i++)
  {
    int bin1, bin2;
    applychain->GetEntry(i);
    tree->GetEntry(i);
    //Insert formula, if any
    value1 = formulavar1->EvalInstance();
    value2 = formulavar2->EvalInstance();
    bin1 = refhist->GetXaxis()->FindBin(value1);
    bin2 = refhist->GetXaxis()->FindBin(value2);
    wvalue = divhist->GetBinContent(bin1, bin2);
    tree->Fill();
    if (i % (applychain->GetEntries() / 10 + 1) == 0)
    {
      cout << "Processing event: " << i << " / " << applychain->GetEntries() << endl;
    }
  }
  tree->Write();

  //File close!
  file->Close();
  CloseChain(refchain);
  CloseChain(applychain);
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 7:
    Reweight2D(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
  case 8:
    Reweight2D(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])));
    break;
  case 9:
    Reweight2D(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])));
    break;
  case 10:
    Reweight2D(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif