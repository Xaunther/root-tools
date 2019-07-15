//This used to be some crappy code I assembled long ago... USELESS PIECE OF SH
//So, I will try to generate some distribution, which mihgt be useful to understand some dependency of the efficiencies on the mass of the resonance. I'll see
#include <iostream>
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "../Functions/StringTools.h"
using namespace std;

void PhaseSpace(double mothermass, string daughtermasses, string pnames, string outfilename, const int N_evts = 100000)
{
  //Masses involved (GeV)
  int N_daughters = 0;
  string* d_mass_str = SplitString(N_daughters, daughtermasses, " ");
  Double_t* d_mass = new Double_t[N_daughters];
  for(int i=0;i<N_daughters;i++)
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
  double caca[3] = {1.,1.,0.};
  event.SetDecay(mother, N_daughters, d_mass);

  //Set up the ntuple
  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  TTree* tree = new TTree("DecayTree", "DecayTree");

  //I need to book 3*N_daughters variables, 3-momentum of each. An extra one for the weight
  double** p = new double*[N_daughters];
  for(int i=0;i<N_daughters;i++)
    {
      p[i] = new double[3];
    }
  //Initialize branches
  TBranch** branch = new TBranch*[3*N_daughters];
  for(int i=0;i<N_daughters;i++)
    {
      for(int j=0;j<3;j++)
	{
	  branch[i*3+j] = tree->Branch((pname[i]+"_"+axname[j]).c_str(), &p[i][j], (pname[i]+"_"+axname[j]+"/D").c_str());
	}
    }
  double weight;
  TBranch* br_weight = tree->Branch("evt_weight", &weight, "evt_weight/D");
  //Generate events, izy. Save them in tree
  for (int n=0;n<N_evts;n++)
    {
      weight = event.Generate();
      for(int i=0;i<N_daughters;i++)
	{
	  event.GetDecay(i)->Vect().GetXYZ(p[i]);
	}
      tree->Fill();
    }
  tree->Write();
  outfile->Close();
}
