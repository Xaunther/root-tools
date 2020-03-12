//Function to perform unbinned reweighting of a sample, to match the distribution of a second sample
//What it does is to assign weights to each event of the first sample
#include <string>
#include <iostream>
#include "TH2F.h"
#include "TTreeFormula.h"
#include "TFile.h"
#include "TTree.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/StringTools.h"
#include "../Functions/Ordenar.h"
using namespace std;

//Script to do unbinned reweight. It follows the same scheme as Reweight2D but for 1D, and no bins!(well, actually each entry is a bin)
void UnbinnedReweight(string vars, string reffile, string applyfile, string outfile, string refw = "", string applyw = "", string wname = "weight")
{
  //File open!
  TChain* refchain = GetChain(reffile);
  TChain* applychain = GetChain(applyfile);

  //Get those names!
  int Nvars = 0;
  string* varlist = SplitString(Nvars, vars, ", ");
  //Assign names
  string refvar = varlist[0];
  string applyvar = (Nvars == 1) ? refvar : varlist[1];

  //Now we retreive the weights. In each bin, the weight is Nref[i]/Napp[i]
  //We do that for each entry in the ntuple, and see where it falls
  //The variable might be a formula, so....
  //Define formulas
  TTreeFormula* applyformula = new TTreeFormula(applyvar.c_str(), applyvar.c_str(), applychain);
  TTreeFormula* applywformula = new TTreeFormula(applyw.c_str(), applyw.c_str(), applychain);
  TTreeFormula* refformula = new TTreeFormula(refvar.c_str(), refvar.c_str(), refchain);
  TTreeFormula* refwformula = new TTreeFormula(refw.c_str(), refw.c_str(), refchain);

  //So first we fo through the reference sample to get the (un)binning. Must order the entries
  int applyentries = applychain->GetEntries();
  int refentries = refchain->GetEntries();

  //Arrays with the entries, it will be ordered. It is a table with necessary stuff
  double** refarray = new double*[refentries];
  double** applyarray = new double*[applyentries];
  for (int i = 0; i < refentries; i++)
  {
    refarray[i] = new double[2];
    refchain->GetEntry(i);
    refarray[i][0] = refformula->EvalInstance();
    refarray[i][1] = refwformula->EvalInstance();
  }
  for (int i = 0; i < applyentries; i++)
  {
    applyarray[i] = new double[3];
    applychain->GetEntry(i);
    applyarray[i][0] = applyformula->EvalInstance();
    applyarray[i][1] = applywformula->EvalInstance();
    applyarray[i][2] = i;
  }
  refarray = Ordenar(refentries, 2, 0, refarray);
  applyarray = Ordenar(applyentries, 3, 0, applyarray);
  ////////////////////////////////////////
  //Loop over the reference sample to get the weight for each event in the sample we want to apply them
  int j = 0;
  for (int i = 0; i < refentries; i++)
  {
    int N_i = 0;
    double sum = 0;
    //For each one of them, find when to stop in sample
    for (; refarray[i][0] > applyarray[j][0]; j++) {N_i++; sum += applyarray[j][1];}
    for (int k = j - N_i; k < j; k++)
    {
      if (sum == 0)
      {
        applyarray[k][2] = 0;
      }
      else
      {
        applyarray[k][2] = refarray[i][1] / double(sum); //Weights overwrite original mass array
      }
    }
  }
//Order back to original
  applyarray = Ordenar(applyentries, 3, 2, applyarray);

  //Add new branch
  double wvalue;
  TFile* file = new TFile(outfile.c_str(), "RECREATE");
  TTree* tree = applychain->CloneTree(0);
  tree->Branch(wname.c_str(), &wvalue, (wname + "/D").c_str());

  //Loop over all events and save
  for (int i = 0; i < applyentries; i++)
  {
    tree->GetEntry(i);
    wvalue = applyarray[i][2];
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
  case 4:
    UnbinnedReweight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    UnbinnedReweight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    UnbinnedReweight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])),
                     *(new string(argv[6])));
    break;
  case 7:
    UnbinnedReweight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])),
                     *(new string(argv[6])), *(new string(argv[7])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for UnbinnedReweight" << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
