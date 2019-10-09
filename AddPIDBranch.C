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
  //For datafile
  TFile* file = new TFile(tupleoutfile.c_str(), "RECREATE");
  if(inchain->GetEntries()==0)
    {
      exit(0);
    }
  TTree* tree = inchain->CloneTree();
  //Efficiencies of each particle and total
  inchain->SetBranchAddress((p1+"_PIDCalibEff").c_str(),&eff1);
  inchain->SetBranchAddress((p2+"_PIDCalibEff").c_str(),&eff2);
  inchain->SetBranchAddress("Event_PIDCalibEff",&eff);
  //Create new branches
  TBranch* binv1 = tree->Branch("Event_PIDCalibEff_pbarpi", &inv1, "Event_PIDCalibEff_pbarpi/D");
  TBranch* binv2 = tree->Branch("Event_PIDCalibEff_ppibar", &inv2, "Event_PIDCalibEff_ppibar/D");

  for(int i=0;i<inchain->GetEntries();i++)
    {
      inchain->GetEntry(i);
      tree->GetEntry(i);
      //Insert formula, if any
      inv1 = eff2-eff;
      inv2 = eff1-eff;
      binv1->Fill();
      binv2->Fill();
      if(i%(inchain->GetEntries()/10+1)==0)
	{
	  cout << "Processing event: " << i+1 << " / " << inchain->GetEntries() << endl;
	}
    }
  tree->Write();

  file->Close();
  cout << "New branch added" << endl;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 2:
      AddPIDBranch(*(new string(argv[1])), *(new string(argv[2])));
      break;
    case 3:
      AddPIDBranch(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    case 4:
      AddPIDBranch(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
      break;
    case 5:
      AddPIDBranch(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for AddPIDBranch" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
