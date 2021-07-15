//Script to perform a 2-D Reweighting from a reference sample. Custom binning must be specified with text files, 1 for each variable
//Reference and "apply" sample might have different variable naming. A ", " is used to separate reference from apply name
#include <string>
#include <iostream>
#include "TH2F.h"
#include "TTreeFormula.h"
#include "TFile.h"
#include "TTree.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/StringTools.h"
using namespace std;

void Reweight1D(string vars1, string reffile, string applyfile,
                string binfile1, string outfile, string refw = "", string applyw = "", string wname = "weight")
{
  //File open!
  TChain *refchain = GetChain(reffile);
  TChain *applychain = GetChain(applyfile);

  //Get those names!
  int Nvars1 = 0;
  string *varlist1 = SplitString(Nvars1, vars1, ", ");
  //Assign names
  string refvar1 = varlist1[0];
  string applyvar1 = (Nvars1 == 1) ? refvar1 : varlist1[1];

  //Get the binning!
  int NBins1 = 0;
  double *binning1 = ReadNumbers(NBins1, binfile1);

  //Time to get those histograms filled boi!
  //Yes, var2 goes first, this is how it works in ROOT
  //Don't ask
  TH1F *refhist = GetHistogram(refchain, refvar1, NBins1 - 1, binning1, "refhist", refw);
  refhist->Scale(1 / refhist->GetSumOfWeights());
  TH1F *applyhist = GetHistogram(applychain, applyvar1, NBins1 - 1, binning1, "applyhist", applyw);
  applyhist->Scale(1 / applyhist->GetSumOfWeights());
  refhist->Divide(applyhist);

  //Now we retreive the weights. In each bin, the weight is Nref[i]/Napp[i]
  //We do that for each entry in the ntuple, and see where it fall
  //The variable might be a formula, so....
  //Define TTreeFormula
  TTreeFormula *formulavar1 = new TTreeFormula(applyvar1.c_str(), applyvar1.c_str(), applychain);

  //Add new branch
  double wvalue, werror, value1;
  TFile *file = new TFile(Gridify(outfile).c_str(), "RECREATE");
  TTree *tree = applychain->CloneTree(0);
  tree->Branch(wname.c_str(), &wvalue, (wname + "/D").c_str());
  tree->Branch((wname + "err").c_str(), &werror, (wname + "err/D").c_str());

  //Loop over all events and get value
  for (int i = 0; i < applychain->GetEntries(); i++)
  {
    int bin1;
    applychain->GetEntry(i);
    tree->GetEntry(i);
    //Insert formula, if any
    value1 = formulavar1->EvalInstance();
    bin1 = refhist->GetXaxis()->FindBin(value1);
    wvalue = refhist->GetBinContent(bin1);
    werror = refhist->GetBinError(bin1);
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
int main(int argc, char **argv)
{
  switch (argc - 1)
  {
  case 5:
    Reweight1D(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    Reweight1D(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  case 7:
    Reweight1D(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
  case 8:
    Reweight1D(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
