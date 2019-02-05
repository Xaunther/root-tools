//Does the same as AddBranch (more or less) but it's designed to add PID efficiencies when inverting some PID cuts
#include <iostream>
#include <string>
#include "TBranch.h"
#include "TMath.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "../Functions/TreeTools.h"
using namespace std;

void AddPIDBranch(string tupleinfile, string tupleoutfile, string treename = "", string p1 = "Kplus", string p2 = "piminus")
{
  float eff;
  float eff1;
  float eff2;
  double inv1;
  double inv2;

  //Data chain
  TChain* inchain = GetChain(tupleinfile, treename);
  //Efficiencies of each particle and total
  inchain->SetBranchAddress((p1+"_PIDCalibEff").c_str(),&eff1);
  inchain->SetBranchAddress((p2+"_PIDCalibEff").c_str(),&eff2);
  inchain->SetBranchAddress("Event_PIDCalibEff",&eff);

  //For datafile
  TFile* file = new TFile(tupleoutfile.c_str(), "RECREATE");
  TTree* tree = inchain->CloneTree();
  //Create new branches
  TBranch* binv1 = tree->Branch("Event_PIDCalibEff_pbarpi", &inv1, "Event_PIDCalibEff_pbarpi/D");
  TBranch* binv2 = tree->Branch("Event_PIDCalibEff_ppibar", &inv2, "Event_PIDCalibEff_pbarpi/D");

  for(int i=0;i<inchain->GetEntries();i++)
    {
      inchain->GetEntry(i);
      tree->GetEntry(i);
      //Insert formula, if any
      inv1 = eff2-eff;
      inv2 = eff1-eff;
      binv1->Fill();
      binv2->Fill();
      if(i%(inchain->GetEntries()/10)==0)
	{
	  cout << "Processing event: " << i << " / " << inchain->GetEntries() << endl;
	}
    }
  tree->Write();

  file->Close();
  cout << "New branch added" << endl;
}
