#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
using namespace std;

void CutEff(string dirfile, string cutfile, string precutfile = "", string outfile = "CutEff_results.txt")
{
  string allcuts = GetCuts(cutfile);
  string allprecuts = GetCuts(precutfile);

  int N_cuts = 0;
  string* cuts = ReadVariables(N_cuts, cutfile);
  int N_precuts = 0;
  string* precuts = ReadVariables(N_precuts, cutfile);

  int* N_final = int[N_cuts+1];
  int N0;

  int N_files = 0;
  string* filenames = ReadVariables(N_files, dirfile);

  //Data chain
  string treename = GetTreeName(dirfile);
  cout << "Reading Tree " << treename << endl;
  TChain* chain = new TChain(treename.c_str());

  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
    }

  //Cut chain into new TChain in a temp root file
  TFile* cutfile = new TFile("Tuples/tmp.root", "recreate");
  TTree* cuttree = (TTree*)chain->CopyTree(allprecuts.c_str());
  cutfile->Write();
  
  //Simply compute #of evts before and after
  N0 = cuttree->GetEntries();
  for(int i=0;i<N_cuts;i++)
    {
      N_final[i] = cuttree->GetEntries(cuts[i].c_str());
    }
  N_final[N_cuts] = cuttree->GetEntries(allcuts.c_str());

  //Now, produce a gorgeous output #4dalulz
  ofstream fout;
  fout.open(outfile.c_str());
  fout << "Selection efficiencies applied on " << dirfile << ": " << endl << endl;
  fout << "                     Preselection                     " << endl;
  fout << "------------------------------------------------------" << endl;
  for(int i=0;i<N_precuts;i++)
    {
      fout << precuts[i] << endl;
    }
  fout << endl;
  fout << "          Efficiency for each cut, and global          " << endl;
  fout << "-------------------------------------------------------" << endl;
  for(int i=0;i<N_cuts;i++)
    {
      fout << setw(80) << cuts[i] << N_final[i]/double(N0) << endl;
    }
  fout << setw(80) << "Global: " << N_final[N_cuts]/double(N0) << endl;
  fout.close();
}
