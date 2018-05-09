#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "../Dictionaries/Constants.h"
using namespace std;

#define TAM 1000

void CutSig(string dirfile_data, string dirfile_MC, string cutfiledir, double MC_exp_0, string precutfile_LM = "", string precutfile_HM = "", string precutfile_MC = "", string outfile_prefix = "outputs/CutEff_results_")
{
  //Initialize constants
  Constants const_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  int N_cutfiles = 0;
  string* cutfiles = ReadVariables(N_cutfiles, cutfiledir);
  string allprecuts_LM = GetCuts(precutfile_LM);
  string allprecuts_HM = GetCuts(precutfile_HM);
  string allprecuts_MC = GetCuts(precutfile_MC);

  int N_precuts_LM = 0;
  int N_precuts_HM = 0;
  int N_precuts_MC = 0;
  //  string* precuts_LM = ReadVariables(N_precuts_LM, precutfile_LM);
  //  string* precuts_HM = ReadVariables(N_precuts_HM, precutfile_HM);
  //  string* precuts_MC = ReadVariables(N_precuts_MC, precutfile_MC);

  ofstream fout;
  if(N_precuts_LM == 0)
    {
      allprecuts_LM = "1";
    }
  if(N_precuts_HM == 0)
    {
      allprecuts_HM = "1";
    }
  if(N_precuts_MC == 0)
    {
      allprecuts_MC = "1";
    }

  double significances[TAM];
  //int* N_final = new int[N_cuts+1];

  //Read the tuples
  int N_files_data = 0;
  string* filenames_data = ReadVariables(N_files_data, dirfile_data);

  int N_files_MC = 0;
  string* filenames_MC = ReadVariables(N_files_MC, dirfile_MC);

  //MC chain
  string treename_MC = GetTreeName(dirfile_MC);
  cout << "Reading Tree " << treename_MC << endl;
  TChain* chain_MC = new TChain(treename_MC.c_str());

  //Data chain
  string treename_data = GetTreeName(dirfile_data);
  cout << "Reading Tree " << treename_data << endl;
  TChain* chain_data = new TChain(treename_data.c_str());

  //Add to chain and get N of entries
  for(int i=0;i<N_files_MC;i++)
    {
      chain_MC->Add(filenames_MC[i].c_str());
    }
  for(int i=0;i<N_files_data;i++)
    {
      chain_data->Add(filenames_data[i].c_str());
    }

  //Loop over different variables to cut
  for(int i=0;i<N_cutfiles;i++)
    {
      int N0 = chain_MC->GetEntries(allprecuts_MC.c_str());
      int N_cuts = 0;
      string* cuts = ReadVariables(N_cuts, cutfiles[i]);
      //Output File headers
      fout.open((outfile_prefix+cutfiles[i].substr(cutfiles[i].find("/")+1)).c_str());
      fout << "Selection efficiencies applied on " << dirfile_data << ", " << dirfile_MC << ": " << endl << endl;
      fout << "                     Preselection                     " << endl;
      fout << "------------------------------------------------------" << endl;
      fout << allprecuts_MC << endl;
      for(int j=0;j<N_cuts;j++)
	{
	  int N_LM = chain_data->GetEntries((allprecuts_LM+" && ("+cuts[j]+")").c_str());
	  int N_HM = chain_data->GetEntries((allprecuts_HM+" && ("+cuts[j]+")").c_str());
	  int N_MC = chain_MC->GetEntries((allprecuts_MC+" && ("+cuts[j]+")").c_str());
	  double N_bkg;
	  //Compute number of bkg events (interpolate)
	  if(N_HM != 0 && N_LM != 0 && N_HM != N_LM)
	    {
	      N_bkg = double(N_LM)*(1-pow(double(N_LM)/double(N_HM),(const_list.xmin-const_list.xmax)/(const_list.xHM-const_list.xmin)))/(1-pow(double(N_LM)/double(N_HM),(const_list.xmin-const_list.xLM)/(const_list.xHM-const_list.xmin)));
	    }
	  else if(N_HM != 0 && N_LM != 0)
	    {
	      N_bkg = N_LM/double(const_list.xLM-const_list.xmin)*(const_list.xmax-const_list.xmin);
	    }
	  else
	    {
	      N_bkg = 0;
	    }
	  //Compute significance
	  if(N_LM == 0)
	    {
	      significances[j] = 0;
	    }
	  else
	    {
	      significances[j] = (double(MC_exp_0*N_MC)/double(N0))/sqrt(double(MC_exp_0*N_MC)/double(N0)+N_bkg);
	    }
	  int maxL = GetMaxLength(cuts,N_cuts);
	  //Output
	  fout << setw(maxL) << cuts[j] << "  |  " << setw(8) << N_MC/double(N0) << " | " << setw(8) << N_LM << " | " << setw(8) << N_HM << " | " << setw(8) << N_bkg << " | " << setw(8) << significances[j] << endl;
	}
      fout.close();
      int best = GetMaxPos(significances, N_cuts);
      //Add that cut to precuts
      allprecuts_LM += (" && ("+cuts[best]+")").c_str();
      allprecuts_HM += (" && ("+cuts[best]+")").c_str();
      allprecuts_MC += (" && ("+cuts[best]+")").c_str();
      //Update expected signal events
      MC_exp_0 = double(MC_exp_0*chain_MC->GetEntries(allprecuts_MC.c_str()))/double(N0);

    }
}
