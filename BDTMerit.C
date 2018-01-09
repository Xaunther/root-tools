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

void BDTMerit(int run_number, double init_value, double final_value, int steps = 100)
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
  double* N_bkg = new double[steps+1];
  double* N_MC = new double[steps+1];
  int* N_MC_raw = new int[steps+1];
  double* eff = new double[steps+1];
  double* sig = new double[steps+1];
  string cut;
  stringstream ss_HM;
  stringstream ss_LM;
  stringstream ss;
  int maxpos;
  for(int i=0;i<=steps;i++)
    {
      ss_HM << HMcut << " && BDT_response > " << init_value+double(i*(final_value-init_value))/double(steps);
      ss_LM << LMcut << " && BDT_response > " << init_value+double(i*(final_value-init_value))/double(steps);
      ss << "BDT_response > " << init_value+double(i*(final_value-init_value))/double(steps);
      N_HM = datatree->GetEntries(ss_HM.str().c_str());
      N_LM = datatree->GetEntries(ss_LM.str().c_str());
      //Interpolate from sidebands
      if(N_HM != 0 && N_LM != 0 && N_HM != N_LM)
	{
	  N_bkg[i] = double(N_LM)*(1-pow(double(N_LM)/double(N_HM),(Constants::xmin-Constants::xmax)/(Constants::xHM-Constants::xmin)))/(1-pow(double(N_LM)/double(N_HM),(Constants::xmin-Constants::xLM)/(Constants::xHM-Constants::xmin)));
	}
      else if(N_HM != 0 && N_LM != 0)
	{
	  N_bkg[i] = N_LM/double(Constants::xLM-Constants::xmin)*(Constants::xmax-Constants::xmin);
	}
      else
	{
	  N_bkg[i] = N_HM + N_LM;
	}
      //N_MC[i] = 975.863316563*double(MCtree->GetEntries(ss.str().c_str()))/1740.0; //LL R-II
      //N_MC[i] = 483.103294757*double(MCtree->GetEntries(ss.str().c_str()))/6546.0; //LL R-I
      if(run_number==1)
	{
	  N_MC[i] = Constants::N_I*double(MCtree->GetEntries(ss.str().c_str()))/Constants::N_I_MC;
	}
      else if(run_number==2)
	{
	  N_MC[i] = Constants::N_II*double(MCtree->GetEntries(ss.str().c_str()))/Constants::N_II_MC;
	}
      //Compute significance
      if(N_bkg[i] == 0 && MCtree->GetEntries(ss.str().c_str()) == 0)
	{
	  cout << 1 << endl;
	  sig[i] == 0;
	}
      else
	{
	  cout << 2 << " " << N_LM << " " << N_HM << endl;;
	  sig[i] = double(N_MC[i])/sqrt(double(N_MC[i]+N_bkg[i]));
	}

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
  //Output the line with max significance (saves time!)
  maxpos = GetMaxPos(sig, steps+1);
  cout << endl << "Max significance:" << endl << endl;
  cout << setfill(' ') << setw(7) << init_value+double(maxpos*(final_value-init_value))/double(steps) << "|" << setw(14) << sig[maxpos] << "|" << setw(9) << eff[maxpos] << "|" << setw(7) << N_MC_raw[maxpos] << "|" << setw(7) << N_bkg[maxpos] << endl;

}
