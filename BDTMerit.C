#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
using namespace std;

void BDTMerit(double init_value, double final_value, int steps = 100)
{
  //Going to plot 1 variable and then extend it with a loop
  
  TFile* datafile = new TFile("Tuples/data-mva_output.root");
  TTree* datatree = (TTree*)datafile->Get("DecayTree"); //Get Tree from corresponding directory
  TFile* MCfile = new TFile("Tuples/data-mva_outputMC.root");
  TTree* MCtree = (TTree*)MCfile->Get("DecayTree");
  string HMcut = "B_M > 5600";
  string LMcut = "B_M < 5000";
  int N0_MC = MCtree->GetEntries();
  int N_HM, N_LM;
  int xmin = 4300, xmax = 6300, xlow = 5000, xhigh = 5600;
  int* N_bkg = new int[steps+1];
  int* N_MC  = new int[steps+1];
  int* N_MC_raw = new int[steps+1];
  double* eff= new double[steps+1];
  double* sig= new double[steps+1];
  string cut;
  stringstream ss_HM;
  stringstream ss_LM;
  stringstream ss;
  for(int i=0;i<=steps;i++)
    {
      ss_HM << HMcut << " && BDT_response > " << init_value+double(i*(final_value-init_value))/double(steps);
      ss_LM << LMcut << " && BDT_response > " << init_value+double(i*(final_value-init_value))/double(steps);
      ss << "BDT_response > " << init_value+double(i*(final_value-init_value))/double(steps);
      N_HM = datatree->GetEntries(ss_HM.str().c_str());
      N_LM = datatree->GetEntries(ss_LM.str().c_str());
      N_bkg[i] = N_LM*(1-TMath::Power(double(N_LM)/double(N_HM),double(xmin-xmax)/double(xhigh-xmin)))/(1-TMath::Power(double(N_LM)/double(N_HM),double(xmin-xlow)/double(xhigh-xmin)));
      //N_MC[i] = 975.863316563*double(MCtree->GetEntries(ss.str().c_str()))/1740.0; //LL R-II
      //N_MC[i] = 483.103294757*double(MCtree->GetEntries(ss.str().c_str()))/6546.0; //LL R-I
      N_MC[i] = 645.4811*double(MCtree->GetEntries(ss.str().c_str()))/1722.0; //DD R-I
      sig[i] = double(N_MC[i])/TMath::Sqrt(double(N_MC[i]+N_bkg[i]));
      eff[i] = double(MCtree->GetEntries(ss.str().c_str()))/double(N0_MC);
      N_MC_raw[i] = MCtree->GetEntries(ss.str().c_str());
      ss_HM.str("");
      ss_LM.str("");
      ss.str("");
    }
  
  //Output the values
  cout << "  Cut       Merit         Eff     N_sig   N_bkg " << endl;
  cout << "-------|--------------|---------|-------|-------" << endl;
  for(int i=0;i<=steps;i++)
    {
      cout << setfill(' ') << setw(7) << init_value+double(i*(final_value-init_value))/double(steps) << "|" << setw(14) << sig[i] << "|" << setw(9) << eff[i] << "|" << setw(7) << N_MC_raw[i] << "|" << setw(7) << N_bkg[i] << endl;
    }
}
