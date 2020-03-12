//Script to perform a 2-D Reweighting from a reference sample. Custom binning must be specified with text files, 1 for each variable
//Reference and "apply" sample might have different variable naming. A ", " is used to separate reference from apply name
#include <string>
#include <iostream>
#include "TH2F.h"
#include "TTreeFormula.h"
#include "TFile.h"
#include "TTree.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/StringTools.h"
using namespace std;

void Reweight2D(string vars1, string vars2, string reffile, string applyfile, string outfile,
                string binfile1, string binfile2, string refw = "", string applyw = "", string wname = "weight")
{
  //File open!
  TChain* refchain = GetChain(reffile);
  TChain* applychain = GetChain(applyfile);

  //Get those names!
  int Nvars1 = 0; int Nvars2 = 0;
  string* varlist1 = SplitString(Nvars1, vars1, ", ");
  string* varlist2 = SplitString(Nvars2, vars2, ", ");
  //Assign names
  string refvar1 = varlist1[0];
  string applyvar1 = (Nvars1 == 1) ? refvar1 : varlist1[1];
  string refvar2 = varlist2[0];
  string applyvar2 = (Nvars2 == 1) ? refvar2 : varlist2[1];

  //Get the binning!
  int NBins1 = 0, NBins2 = 0;
  double* binning1 = ReadNumbers(NBins1, binfile1);
  double* binning2 = ReadNumbers(NBins2, binfile2);

  //Time to get those histograms filled boi!
  //Yes, var2 goes first, this is how it works in ROOT
  //Don't ask
  TH2F* refhist = GetHistogram2D(refchain, refvar2 + ":" + refvar1, NBins1 - 1, binning1, NBins2 - 1, binning2, "refhist", refw);
  refhist->Scale(1 / refhist->GetSumOfWeights());
  TH2F* applyhist = GetHistogram2D(applychain, applyvar2 + ":" + applyvar1, NBins1 - 1, binning1, NBins2 - 1, binning2, "applyhist", applyw);
  applyhist->Scale(1 / applyhist->GetSumOfWeights());
  refhist->Divide(applyhist);
  refhist->Sumw2();

  //Now we retreive the weights. In each bin, the weight is Nref[i]/Napp[i]
  //We do that for each entry in the ntuple, and see where it fall
  //The variable might be a formula, so....
  //Define TTreeFormula
  TTreeFormula* formulavar1 = new TTreeFormula(applyvar1.c_str(), applyvar1.c_str(), applychain);
  TTreeFormula* formulavar2 = new TTreeFormula(applyvar2.c_str(), applyvar2.c_str(), applychain);

  //Add new branch
  double wvalue, werror, value1, value2;
  TFile* file = new TFile(outfile.c_str(), "RECREATE");
  TTree* tree = applychain->CloneTree(0);
  tree->Branch(wname.c_str(), &wvalue, (wname + "/D").c_str());
  tree->Branch((wname + "err").c_str(), &werror, (wname + "err/D").c_str());

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
    bin2 = refhist->GetYaxis()->FindBin(value2);
    wvalue = refhist->GetBinContent(bin1, bin2);
    werror = refhist->GetBinError(bin1, bin2);
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
