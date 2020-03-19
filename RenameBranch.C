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
#include "TMath.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
using namespace std;
void RenameBranch(string outname, string tupledir = "Directories/Lb2016DataMagUp.dir")
{
  //Data chain
  TChain* chain = GetChain(tupledir);

  //Manually here, implement branch renaming
  chain->GetLeaf("B_M01_Subst1_pi2p~")->SetTitle("B_M01_Subst1_pi2p");
  chain->GetLeaf("B_M01_Subst1_pi2p~")->SetName("B_M01_Subst1_pi2p");

  chain->GetLeaf("B_M012_Subst1_pi2p~")->SetTitle("B_M012_Subst1_pi2p");
  chain->GetLeaf("B_M012_Subst1_pi2p~")->SetName("B_M012_Subst1_pi2p");

  chain->GetLeaf("Kst_892_0_M01_Subst1_pi2p~")->SetTitle("Kst_892_0_M01_Subst1_pi2p");
  chain->GetLeaf("Kst_892_0_M01_Subst1_pi2p~")->SetName("Kst_892_0_M01_Subst1_pi2p");
  //*********************************************************************

  cout << "Merging ... " << endl;
  chain->Merge(outname.c_str());
  cout << "Data merged" << endl;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
    case 1:
    RenameBranch(*(new string(argv[1])));
    break;
    case 2:
    RenameBranch(*(new string(argv[1])), *(new string(argv[2])));
    break;
    default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
