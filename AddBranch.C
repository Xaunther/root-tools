//New AddBranch function. Plan is to remove old version and stick with this one as it is more general
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TBranch.h"
#include "../Functions/TreeTools.h"
#include "../Functions/StringTools.h"
using namespace std;
void AddBranch(string branchname, string tupleinfile, string tupleoutfile, string formula, string treename = "")
{
  double branchvalue; //Stick to a double output for now...

  //Data chain
  TChain* inchain = GetChain(tupleinfile, treename);

  //Define TTreeFormula
  TTreeFormula* formulavar = new TTreeFormula(formula.c_str(), formula.c_str(), inchain);

  //Add new branch
  TFile* file = new TFile(Gridify(tupleoutfile).c_str(), "RECREATE");
  TTree* tree = inchain->CloneTree(0);
  tree->Branch(branchname.c_str(), &branchvalue, (branchname + "/D").c_str());

  //Loop over all events and get value
  for (int i = 0; i < inchain->GetEntries(); i++)
  {
    inchain->GetEntry(i);
    tree->GetEntry(i);
    //Insert formula, if any
    branchvalue = formulavar->EvalInstance();
    tree->Fill();
    if (i % (inchain->GetEntries() / 10 + 1) == 0)
    {
      cout << "Processing event: " << i << " / " << inchain->GetEntries() << endl;
    }
  }
  tree->Print();
  tree->Write();

  file->Close();
  cout << "New branch added" << endl;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 4:
    AddBranch(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    AddBranch(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
