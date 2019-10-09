#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "../Functions/TISTOS.h"
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
using namespace std;

void TISTOS(string dirfile, string cutsfilename, string outfile = "TISTOS_results.txt", string L0_lines = "Variables/L0_TISTOS.txt", string Hlt1_lines = "Variables/Hlt1_TISTOS.txt", string Hlt2_lines = "Variables/Hlt2_TISTOS.txt")
{
  string cuts = GetCuts(cutsfilename);
  cout << cuts << endl;

  int N_files = 0;
  string* filenames = ReadVariables(N_files, dirfile);

  //Trigger lines used for TISTOS
  int N_L0 = 0, N_Hlt1 = 0, N_Hlt2 = 0;
  string* L0_list = ReadVariables(N_L0, L0_lines);
  string* Hlt1_list = ReadVariables(N_Hlt1, Hlt1_lines);
  string* Hlt2_list = ReadVariables(N_Hlt2, Hlt2_lines);
  //Perform TIS and TISTOS Cuts
  string L0_TIS = MakeTIS(L0_list, N_L0);
  string L0_TOS = MakeTOS(L0_list, N_L0);
  string L0_TISTOS = MakeTISTOS(L0_list, N_L0);
  string L0_TISorTOS = MakeTISorTOS(L0_list, N_L0);
  string Hlt1_TIS = MakeTIS(Hlt1_list, N_Hlt1);
  string Hlt1_TOS = MakeTOS(Hlt1_list, N_Hlt1);
  string Hlt1_TISTOS = MakeTISTOS(Hlt1_list, N_Hlt1);
  string Hlt1_TISorTOS = MakeTISorTOS(Hlt1_list, N_Hlt1);
  string Hlt2_TIS = MakeTIS(Hlt2_list, N_Hlt2);
  string Hlt2_TOS = MakeTOS(Hlt2_list, N_Hlt2);
  string Hlt2_TISTOS = MakeTISTOS(Hlt2_list, N_Hlt2);
  string Hlt2_TISorTOS = MakeTISorTOS(Hlt2_list, N_Hlt2);

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
  TTree* cuttree = (TTree*)chain->CopyTree(cuts.c_str());
  cutfile->Write();
  
  //Part where the output is stored into outfile
  ofstream fout;
  int N_TIS, N_TOS, N_TISTOS, N_TISorTOS;

  fout.open(outfile.c_str());
  fout << "TISTOS method applied on tuples defined at " << dirfile << ":" << endl << endl;
  //L0
  fout << "         L0 lines used         " << endl;
  fout << "-------------------------------" << endl;
  for(int i=0;i<N_L0;i++)
    {
      fout << L0_list[i] << endl;
    }
  fout << endl;
  //Compute Numbers
  N_TIS = cuttree->GetEntries(L0_TIS.c_str());
  N_TISTOS = cuttree->GetEntries(L0_TISTOS.c_str());
  N_TISorTOS = cuttree->GetEntries(L0_TISorTOS.c_str());
  N_TOS = cuttree->GetEntries(L0_TOS.c_str());

  fout << "Trigger eff: " << double(N_TISTOS)/N_TIS << endl;
  fout << endl;
  //Hlt1
  fout << "        Hlt1 lines used        " << endl;
  fout << "-------------------------------" << endl;
  for(int i=0;i<N_Hlt1;i++)
    {
      fout << Hlt1_list[i] << endl;
    }
  fout << endl;
  //Compute numbers
  N_TIS = cuttree->GetEntries(("("+L0_TIS+") && ("+Hlt1_TIS+")").c_str());
  N_TISTOS = cuttree->GetEntries(("("+L0_TISTOS+") && ("+Hlt1_TISTOS+")").c_str());
  N_TISorTOS = cuttree->GetEntries(("("+L0_TISorTOS+") && ("+Hlt1_TISorTOS+")").c_str());
  N_TOS = cuttree->GetEntries(("("+L0_TOS+") && ("+Hlt1_TOS+")").c_str());

  fout << "Trigger eff: " << double(N_TISTOS)/N_TIS << endl;
  fout << endl;
  //Hlt2
  fout << "        Hlt2 lines used        " << endl;
  fout << "-------------------------------" << endl;
  for(int i=0;i<N_Hlt2;i++)
    {
      fout << Hlt2_list[i] << endl;
    }
  fout << endl;
  //Compute numbers
  N_TIS = cuttree->GetEntries(("("+L0_TIS+") && ("+Hlt1_TIS+") && ("+Hlt2_TIS+")").c_str());
  N_TISTOS = cuttree->GetEntries(("("+L0_TISTOS+") && ("+Hlt1_TISTOS+") && ("+Hlt2_TISTOS+")").c_str());
  N_TISorTOS = cuttree->GetEntries(("("+L0_TISorTOS+") && ("+Hlt1_TISorTOS+") && ("+Hlt2_TISorTOS+")").c_str());
  N_TOS = cuttree->GetEntries(("("+L0_TOS+") && ("+Hlt1_TOS+") && ("+Hlt2_TOS+")").c_str());

  fout << "Trigger eff: " << double(N_TISTOS)/N_TIS << endl;
  fout << endl;

  fout.close();
  
  cutfile->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc)
    {
    case 2:
      TISTOS(*(new string(argv[1])), *(new string(argv[2])));
      break;
    case 3:
      TISTOS(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    case 4:
      TISTOS(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
      break;
    case 5:
      TISTOS(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
      break;
    case 6:
      TISTOS(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for TISTOS" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
