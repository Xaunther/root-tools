#include <iostream>
#include <iomanip>
#include <fstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include <string>
#include "TCanvas.h"
#include "TLeaf.h"
#include <sstream>
#include "../Dictionaries/Constants.h"
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
#include "../Functions/ArrayTools.h"
#include "../Functions/TreeTools.h"
#include "../Functions/RunInfo.h"
using namespace std;

void BDTMerit(RunNumber run_number, double init_value, double final_value, int steps = 100, string filename = "Tuples/data-mva_outputMC.root", string BDTvarname = "BDT_response")
{
  //Initialize constants
  Constants const_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  //Going to plot 1 variable and then extend it with a loop
  TChain* datatree = GetChain("../Directories/BDToutput.dir", "DecayTree");
  TChain* MCtree = GetChain(filename);
  string HMcut = "B_M > 5600";
  string LMcut = "B_M < 5000";
  int N0_MC, N_HM, N_LM, N0_bkg;
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
  //Run number cut to look only at certain part of the data
  string runcut = GetRunCut(run_number, "data");
  string runcut_MC = GetRunCut(run_number, "MC");
  //Initial values
  N0_MC = MCtree->GetEntries(runcut_MC.c_str());
  N_HM = datatree->GetEntries((runcut + " && " + HMcut).c_str());
  N_LM = datatree->GetEntries((runcut + " && " + LMcut).c_str());

  //Interpolate from sidebands
  if(N_HM != 0 && N_LM != 0 && N_HM != N_LM)
    {
      N0_bkg = double(N_LM)*(1-pow(double(N_LM)/double(N_HM),(const_list.xmin-const_list.xmax)/(const_list.xHM-const_list.xmin)))/(1-pow(double(N_LM)/double(N_HM),(const_list.xmin-const_list.xLM)/(const_list.xHM-const_list.xmin)));
    }
  else if(N_HM != 0 && N_LM != 0)
    {
      N0_bkg = N_LM/double(const_list.xLM-const_list.xmin)*(const_list.xmax-const_list.xmin);
    }
  else
    {
      N0_bkg = N_HM + N_LM;
    }

  for(int i=0;i<=steps;i++)
    {
      ss_HM << runcut << " && " << HMcut << " && " + BDTvarname + " > " << init_value+double(i*(final_value-init_value))/double(steps);
      ss_LM << runcut << " && " << LMcut << " && " + BDTvarname + " > " << init_value+double(i*(final_value-init_value))/double(steps);
      ss << runcut_MC << " && " + BDTvarname + " > " << init_value+double(i*(final_value-init_value))/double(steps);
      N_HM = datatree->GetEntries(ss_HM.str().c_str());
      N_LM = datatree->GetEntries(ss_LM.str().c_str());
      //Interpolate from sidebands
      if(N_HM != 0 && N_LM != 0 && N_HM != N_LM)
	{
	  N_bkg[i] = double(N_LM)*(1-pow(double(N_LM)/double(N_HM),(const_list.xmin-const_list.xmax)/(const_list.xHM-const_list.xmin)))/(1-pow(double(N_LM)/double(N_HM),(const_list.xmin-const_list.xLM)/(const_list.xHM-const_list.xmin)));
	}
      else if(N_HM != 0 && N_LM != 0)
	{
	  N_bkg[i] = N_LM/double(const_list.xLM-const_list.xmin)*(const_list.xmax-const_list.xmin);
	}
      else
	{
	  N_bkg[i] = N_HM + N_LM;
	}
      //N_MC[i] = 975.863316563*double(MCtree->GetEntries(ss.str().c_str()))/1740.0; //LL R-II
      //N_MC[i] = 483.103294757*double(MCtree->GetEntries(ss.str().c_str()))/6546.0; //LL R-I
      if(run_number==I)
	{
	  N_MC[i] = const_list.N_I*double(MCtree->GetEntries(ss.str().c_str()))/const_list.N_I_MC;
	}
      else if(run_number==II)
	{
	  N_MC[i] = const_list.N_II*double(MCtree->GetEntries(ss.str().c_str()))/const_list.N_II_MC;
	}
      else if(run_number==All)
	{
	  //Need to refine this (naive efficiency)
	  N_MC[i] = const_list.N_I*double(MCtree->GetEntries((ss.str()+" && "+GetRunCut(I,"MC")).c_str()))/const_list.N_I_MC +
	    const_list.N_II*double(MCtree->GetEntries((ss.str()+" && "+GetRunCut(II,"MC")).c_str()))/const_list.N_II_MC;
	}
      //Compute significance
      if(N_bkg[i] == 0 && MCtree->GetEntries(ss.str().c_str()) == 0)
	{
	  sig[i] = 0;
	}
      else
	{
	  sig[i] = double(N_MC[i])/sqrt(double(N_MC[i]+N_bkg[i]));
	}

      eff[i] = double(MCtree->GetEntries(ss.str().c_str()))/double(N0_MC);
      N_MC_raw[i] = MCtree->GetEntries(ss.str().c_str());
      ss_HM.str("");
      ss_LM.str("");
      ss.str("");
    }
  
  //Output the values
  cout << "  Cut       Merit       Eff (S)  1-Eff (B)   N_sig   N_bkg   N_MC " << endl;
  cout << "-------|--------------|---------|---------|-------|-------|------" << endl;
  for(int i=0;i<=steps;i++)
    {
      cout << setfill(' ') << setw(7) << init_value+double(i*(final_value-init_value))/double(steps) << "|" << setw(14) << sig[i] << "|" << setw(9) << eff[i] << "|" << setw(9) << 1-N_bkg[i]/N0_bkg << "|" << setw(7) << N_MC[i] << "|" << setw(7) << N_bkg[i] << "|" << setw(6) << N_MC_raw[i] << endl;
    }
  //Output the line with max significance (saves time!)
  maxpos = GetMaxPos(sig, steps+1);
  cout << endl << "Max significance:" << endl << endl;
  cout << setfill(' ') << setw(7) << init_value+double(maxpos*(final_value-init_value))/double(steps) << "|" << setw(14) << sig[maxpos] << "|" << setw(9) << eff[maxpos] << "|" << setw(7) << N_MC[maxpos] << "|" << setw(7) << N_bkg[maxpos] << "|" << setw(6) << N_MC_raw[maxpos] << endl;


  //Also output into a file to instantly fit for that cut! ;)
  ofstream fout;
  fout.open("Variables/BDTOutCut.txt");
  fout << BDTvarname + " > " << init_value+double(maxpos*(final_value-init_value))/double(steps) << endl;
  if(run_number == I)
    {
      fout << "runNumber < " << const_list.Max_Run_Number[1] << endl;
    }
  else if(run_number == II)
    {
      fout << "runNumber > " << const_list.Max_Run_Number[1] << endl;      
    }
  fout.close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  RunNumber run_number = StringToRunNumber(*(new string(argv[1])));
  switch(argc-1)
    {
    case 3:
      BDTMerit(run_number, stod(*(new string(argv[2]))), stod(*(new string(argv[3]))));
      break;
    case 4:
      BDTMerit(run_number, stod(*(new string(argv[2]))), stod(*(new string(argv[3]))), stoi(*(new string(argv[4]))));
      break;
    case 5:
      BDTMerit(run_number, stod(*(new string(argv[2]))), stod(*(new string(argv[3]))), stoi(*(new string(argv[4]))), *(new string(argv[5])));
      break;
    case 6:
      BDTMerit(run_number, stod(*(new string(argv[2]))), stod(*(new string(argv[3]))), stoi(*(new string(argv[4]))), *(new string(argv[5])), *(new string(argv[6])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for BDTMerit" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
