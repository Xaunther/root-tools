#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
#include "Functions/Filereading.h"
#include "Functions/Dictreading.h"
#include "Functions/TreeTools.h"
using namespace std;

void BDTCut(string tupledir = "Directories/Alltuples.dir", string tupledirbkg = "Directories/tuples.dir", string tupledirMC = "Directories/tuplesMCAll.dir", string cutsfilename = "Variables/BDTCuts.txt", string cutsbkgfilename = "Variables/BDTCutsbkg.txt")
{
  string cuts = GetCuts(cutsfilename);
  string cutsbkg = GetCuts(cutsbkgfilename);
  cout << cuts << endl;

  TChain* chain = GetChain(tupledir);
  TChain* chainbkg = GetChain(tupledirbkg);
  TChain* chainMC = GetChain(tupledirMC);

  TFile* newFile = new TFile("Tuples/BDTcuttree.root", "recreate");
  TTree* tree = chain->CopyTree(cuts.c_str()); //<===new line
  tree->Write();

  TFile* newFilebkg = new TFile("Tuples/BDTcuttreebkg.root", "recreate");
  TTree* treebkg = chainbkg->CopyTree(cutsbkg.c_str()); //<===new line
  treebkg->Write();

  TFile* newFileMC = new TFile("Tuples/BDTcuttreeMC.root", "recreate");
  TTree* treeMC = chainMC->CopyTree(cuts.c_str()); //<===new line
  treeMC->Write();

  newFile->Close();
  newFilebkg->Close();
  newFileMC->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
    case 0:
    BDTCut();
    break;
    case 1:
    BDTCut(*(new string(argv[1])));
    break;
    case 2:
    BDTCut(*(new string(argv[1])), *(new string(argv[2])));
    break;
    case 3:
    BDTCut(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
    case 4:
    BDTCut(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
    case 5:
    BDTCut(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
    default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
