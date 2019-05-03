/*******************************************************************************
*Code to make mass substitutions (when the tupletool makes you angry)          *
*It is designed to work with 2hG final states (K pi gamma).                    *
*One make improve it to make it more general, though...                        *
*******************************************************************************/
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include <string>
#include "TMath.h"
#include "../Functions/TreeTools.h"
using namespace std;

#define Kmass 493.677
#define pimass 139.57
#define pmass 938.272
#define gammamass 0.
#define pi0mass 134.977
#define mumass 105.658
//List of functions
void MassSub        (string dirfile, string outfile = "", string deccode = "NstG");
void MassSub_NstG   (string dirfile, string outfile = "");
void MassSub_ppimumu(string dirfile, string outfile = "");


//Main function just leads to corresponding mass sub config
void MassSub(string dirfile, string outfile, string deccode)
{
  if(deccode == "NstG"){MassSub_NstG(dirfile, outfile);}
  else if(deccode == "NstG_ppimumu"){MassSub_ppimumu(dirfile, outfile);}
  else{cout << deccode << " decay code not implemented" << endl;}
  return;
}

//Default for NstG analysis (output of Beauty2XGamma stripping line)
void MassSub_NstG(string dirfile, string outfile)
{
  //Data chain
  TChain* inchain = GetChain(dirfile);

  //Particle names
  string part_names[3];
  part_names[0] = "Kplus";
  part_names[1] = "piminus";
  part_names[2] = "gamma";
  //Component names
  string comp_names[3];
  comp_names[0] = "PX";
  comp_names[1] = "PY";
  comp_names[2] = "PZ";
  //Mass possibilities (particle i hypothesis j)
  double m[3][3] =
    {
      {Kmass, pimass, pmass},
      {Kmass, pimass, pmass},
      {gammamass, pi0mass, 0.}
    };
  //List of mass combinations (3*3*2 = 18)
  string mass_names[3][3][2];
  //gamma mass
  mass_names[0][0][0] = "_Subst1_pi2K";     mass_names[0][1][0] = "";            mass_names[0][2][0] = "_Subst1_pi2p";      //Kaon X
  mass_names[1][0][0] = "_Subst01_Kpi2piK"; mass_names[1][1][0] = "_Subst0_K2pi"; mass_names[1][2][0] = "_Subst01_Kpi2pip~"; //Pion X
  mass_names[2][0][0] = "_Subst01_Kpi2pK";  mass_names[2][1][0] = "_Subst0_K2p";  mass_names[2][2][0] = "_Subst01_Kpi2pp~";  //Proton X
  //pi0 mass
  mass_names[0][0][1] = "_Subst12_pigamma2Kpi0";     mass_names[0][1][1] = "_Subst2_gamma2pi0";     mass_names[0][2][1] = "_Subst12_pigamma2ppi0";     //Kaon X
  mass_names[1][0][1] = "_Subst123_Kpigamma2piKpi0"; mass_names[1][1][1] = "_Subst02_Kgamma2pipi0"; mass_names[1][2][1] = "_Subst123_Kpigamma2pippi0"; //Pion X
  mass_names[2][0][1] = "_Subst123_Kpigamma2pKpi0";  mass_names[2][1][1] = "_Subst02_Kgamma2ppi0";  mass_names[2][2][1] = "_Subst123_Kpigamma2pppi0";  //Proton X

  //Mass values
  double _m;
  double m_01[3][3];
  double m_012[3][3][2];
  //4-momentum of p1, p2, p3 (p[i]
  TLorentzVector* p = new TLorentzVector[3];
  //3-momentum of p1, p2, p3 (particle i component j)
  double _p[3][3];

  for(int i=0;i<3;i++)
    {
      for(int j=0;j<3;j++)
	{
	  inchain->SetBranchAddress((part_names[i]+"_"+comp_names[j]).c_str(), &_p[i][j]);
	}
    }

  //Add new branches for each substitution
  TFile* file = new TFile(outfile.c_str(), "RECREATE");
  TTree* tree = inchain->CloneTree();
  TBranch**** newbranch012 = new TBranch***[3];
  TBranch*** newbranch01 = new TBranch**[3];
  for(int i=0;i<3;i++)
    {
      newbranch012[i] = new TBranch**[3];
      newbranch01[i] = new TBranch*[3];
      for(int j=0;j<3;j++)
	{
	  newbranch012[i][j] = new TBranch*[3];
	  newbranch01[i][j] = tree->Branch(("B_M01"+mass_names[i][j][0]).c_str(), &_m, ("B_M01"+mass_names[i][j][0]+"/D").c_str());
	  for(int k=0;k<2;k++)
	    {
	      newbranch012[i][j][k] = tree->Branch(("B_M012"+mass_names[i][j][k]).c_str(), &_m, ("B_M012"+mass_names[i][j][k]+"/D").c_str());	      
	    }
	}
    }

  //So right now we have 18 branches for m012 and 9 branches for m01. Now we need to compute event per event...
  for(int ii=0;ii<inchain->GetEntries();ii++)
    {
      inchain->GetEntry(ii);
      tree->GetEntry(ii);
      //Loop over all mass combinations to obtain the corresponding energy
      for(int i=0;i<3;i++)
	{
	  p[0].SetXYZM(_p[0][0],_p[0][1],_p[0][2],m[0][i]);
	  for(int j=0;j<3;j++)
	    {
	      p[1].SetXYZM(_p[1][0],_p[1][1],_p[1][2],m[1][j]);
	      //B_M01 calculations
	      _m = (p[0]+p[1]).M();
	      newbranch01[i][j]->Fill();
	      for(int k=0;k<2;k++)
		{
		  p[2].SetXYZM(_p[2][0],_p[2][1],_p[2][2],m[2][k]);
		  //B_M012 calculations
		  _m = (p[0]+p[1]+p[2]).M();
		  newbranch012[i][j][k]->Fill();
		}
	    }
	}
    }
  tree->Write();

  file->Close();
  cout << "New branches added" << endl;
}
/*********************************************************************************************************************************************************/
//Mass sub for NstG reweighting (output of B2Xmumu stripping line)
void MassSub_ppimumu(string dirfile, string outfile)
{
  //Data chain
  TChain* inchain = GetChain(dirfile);

  //We have 4 particles in the final state, but only mass subs will be done to the first two (hadrons)
  //Particle names
  string part_names[4];
  part_names[0] = "pplus";
  part_names[1] = "piminus";
  part_names[2] = "muplus";
  part_names[3] = "muminus";
  //Component names
  string comp_names[3];
  comp_names[0] = "PX";
  comp_names[1] = "PY";
  comp_names[2] = "PZ";
  //Mass possibilities (particle i hypothesis j)
  double m[4][3] =
    {
      {Kmass, pimass, pmass},
      {Kmass, pimass, pmass},
      {mumass, 0., 0.},
      {mumass, 0., 0.}
    };
  //List of mass combinations (3*3*1*1 = 9)
  string mass_names[3][3][1][1];
  //Muons mass
  mass_names[0][0][0][0] = "_Subst1_pi2K";     mass_names[0][1][0][0] = "";             mass_names[0][2][0][0] = "_Subst1_pi2p";      //Kaon X
  mass_names[1][0][0][0] = "_Subst01_Kpi2piK"; mass_names[1][1][0][0] = "_Subst0_K2pi"; mass_names[1][2][0][0] = "_Subst01_Kpi2pip~"; //Pion X
  mass_names[2][0][0][0] = "_Subst01_Kpi2pK";  mass_names[2][1][0][0] = "_Subst0_K2p";  mass_names[2][2][0][0] = "_Subst01_Kpi2pp~";  //Proton X

  //Mass values
  double _m;
  double m_01[3][3];
  double m_012[3][3][1]; //Only interested in dimuon
  //4-momentum of p1, p2, p3 p4(p[i])
  TLorentzVector* p = new TLorentzVector[4];
  //3-momentum of p1, p2, p3 (particle i component j)
  double _p[4][3];

  for(int i=0;i<4;i++)
    {
      for(int j=0;j<3;j++)
	{
	  inchain->SetBranchAddress((part_names[i]+"_"+comp_names[j]).c_str(), &_p[i][j]);
	}
    }

  //Add new branches for each substitution
  TFile* file = new TFile(outfile.c_str(), "RECREATE");
  TTree* tree = inchain->CloneTree();
  TBranch**** newbranch012 = new TBranch***[3];
  TBranch*** newbranch01 = new TBranch**[3];
  for(int i=0;i<3;i++)
    {
      newbranch012[i] = new TBranch**[3];
      newbranch01[i] = new TBranch*[3];
      for(int j=0;j<3;j++)
	{
	  newbranch012[i][j] = new TBranch*[1];
	  newbranch01[i][j] = tree->Branch(("B_M01"+mass_names[i][j][0][0]).c_str(), &_m, ("B_M01"+mass_names[i][j][0][0]+"/D").c_str());
	  for(int k=0;k<1;k++)
	    {
	      newbranch012[i][j][k] = tree->Branch(("B_M012"+mass_names[i][j][k][0]).c_str(), &_m, ("B_M012"+mass_names[i][j][k][0]+"/D").c_str());	      
	    }
	}
    }

  //So right now we have 9 branches for m012 and 9 branches for m01. Now we need to compute event per event...
  for(int ii=0;ii<inchain->GetEntries();ii++)
    {
      inchain->GetEntry(ii);
      tree->GetEntry(ii);
      //Loop over all mass combinations to obtain the corresponding energy
      for(int i=0;i<3;i++)
	{
	  p[0].SetXYZM(_p[0][0],_p[0][1],_p[0][2],m[0][i]);
	  for(int j=0;j<3;j++)
	    {
	      p[1].SetXYZM(_p[1][0],_p[1][1],_p[1][2],m[1][j]);
	      //B_M01 calculations
	      _m = (p[0]+p[1]).M();
	      newbranch01[i][j]->Fill();
	      for(int k=0;k<1;k++)
		{
		  p[2].SetXYZM(_p[2][0],_p[2][1],_p[2][2],m[2][k]);
		  p[3].SetXYZM(_p[3][0],_p[3][1],_p[3][2],m[3][k]);
		  //B_M012 calculations
		  _m = (p[0]+p[1]+p[2]+p[3]).M();
		  newbranch012[i][j][k]->Fill();
		}
	    }
	}
    }
  tree->Write();

  file->Close();
  cout << "New branches added" << endl;
}
