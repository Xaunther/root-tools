//This used to be some crappy code I assembled long ago... USELESS PIECE OF SH
//So, I will try to generate some distribution, which mihgt be useful to understand some dependency of the efficiencies on the mass of the resonance. I'll see
//Added a new option to obtain Lambda_b momentum distribution from an ntuple
#include <iostream>
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "../Functions/TreeTools.h"
#include "../Functions/StringTools.h"
using namespace std;

void PhaseSpace(double mothermass, string daughtermasses, string pnames, string outfilename, int N_evts = 100000, string mother_P_filename = "", string mother_P_prefix = "B_P")
{
  //Masses involved (GeV)
  int N_daughters = 0;
  string* d_mass_str = SplitString(N_daughters, daughtermasses, " ");
  Double_t* d_mass = new Double_t[N_daughters];
  for (int i = 0; i < N_daughters; i++)
  {
    d_mass[i] = stod(d_mass_str[i]);
  }
  //Names of the daughters
  N_daughters = 0;
  string* pname = SplitString(N_daughters, pnames, " ");
  //Names of the axes
  string* axname = new string[3];
  axname[0] = "PX";
  axname[1] = "PY";
  axname[2] = "PZ";

  //////////////////////////////////////

  //Decaying particle
  TLorentzVector mother(0., 0., 0., mothermass);
  //Set up the decay
  TGenPhaseSpace event;
  event.SetDecay(mother, N_daughters, d_mass);

  //Set up the ntuple
  TFile* outfile = new TFile(Gridify(outfilename).c_str(), "RECREATE");
  TTree* tree = new TTree("DecayTree", "DecayTree");

  //I need to book 3*N_daughters variables, 3-momentum of each. An extra one for the weight
  double** p = new double*[N_daughters];
  for (int i = 0; i < N_daughters; i++)
  {
    p[i] = new double[3];
  }
  //Initialize branches
  TBranch** branch = new TBranch*[3 * N_daughters];
  for (int i = 0; i < N_daughters; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      branch[i * 3 + j] = tree->Branch((pname[i] + "_" + axname[j]).c_str(), &p[i][j], (pname[i] + "_" + axname[j] + "/D").c_str());
    }
  }
  double weight;
  tree->Branch("evt_weight", &weight, "evt_weight/D");

  //Set up mother momentum
  TChain* mother_P_chain;
  double* mother_P = new double[3];
  if (mother_P_filename != "")
  {
    mother_P_chain = GetChain(mother_P_filename);
    mother_P_chain->SetBranchAddress((mother_P_prefix + "X").c_str(), &mother_P[0]);
    mother_P_chain->SetBranchAddress((mother_P_prefix + "Y").c_str(), &mother_P[1]);
    mother_P_chain->SetBranchAddress((mother_P_prefix + "Z").c_str(), &mother_P[2]);
    //Can only generate either the minimum of the number requested and the events in the mother P distribution
    if (mother_P_chain->GetEntries() < N_evts)
    {
      N_evts = mother_P_chain->GetEntries();
    }
  }
  else
  {
    //Remove warning...
    mother_P_chain = new TChain();
  }
  //Generate events, izy. Save them in tree.
  for (int n = 0; n < N_evts; n++)
  {
    //Re-roll momentum
    if (mother_P_filename != "")
    {
      mother_P_chain->GetEntry(n);
      mother.SetXYZM(mother_P[0], mother_P[1], mother_P[2], mothermass);
    }
    weight = event.Generate();
    for (int i = 0; i < N_daughters; i++)
    {
      event.GetDecay(i)->Vect().GetXYZ(p[i]);
    }
    tree->Fill();
  }
  tree->Write();
  outfile->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 4:
    PhaseSpace(stod(*(new string(argv[1]))), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    PhaseSpace(stod(*(new string(argv[1]))), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), stoi(*(new string(argv[5]))));
    break;
  case 6:
    PhaseSpace(stod(*(new string(argv[1]))), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), stoi(*(new string(argv[5]))), *(new string(argv[6])));
    break;
  case 7:
    PhaseSpace(stod(*(new string(argv[1]))), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), stoi(*(new string(argv[5]))), *(new string(argv[6])), *(new string(argv[7])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
