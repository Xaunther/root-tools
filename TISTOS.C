//Trigger efficiency, assuming one is accepting TOS events

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TChain.h"
#include "../Functions/TISTOS.h"
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/ArrayTools.h"
using namespace std;

void TISTOS(string dirfile, string cutsfilename, string outfile = "TISTOS_results.txt", string L0_lines = "Variables/L0_TISTOS.txt", string Hlt1_lines = "Variables/Hlt1_TISTOS.txt", string Hlt2_lines = "Variables/Hlt2_TISTOS.txt")
{
  //Obtain cuts (in case some cuts must be applied before TISTOSing)
  string cuts = GetCuts(cutsfilename);
  if (cuts == "") {cuts = "1";}

  //Get chain
  TChain* chain = GetChain(dirfile);

  //Trigger lines used for TISTOS
  int N_L0 = 0, N_Hlt1 = 0, N_Hlt2 = 0;
  string* L0_list = ReadVariables(N_L0, L0_lines);
  string* Hlt1_list = ReadVariables(N_Hlt1, Hlt1_lines);
  string* Hlt2_list = ReadVariables(N_Hlt2, Hlt2_lines);
  //Clean suffixes
  L0_list = CleanTISTOS(L0_list, N_L0);
  Hlt1_list = CleanTISTOS(Hlt1_list, N_Hlt1);
  Hlt2_list = CleanTISTOS(Hlt2_list, N_Hlt2);
  //Perform TIS and TISTOS Cuts
  string L0_TIS = MakeTIS(L0_list, N_L0);
  string L0_TISTOS = MakeTISTOS(L0_list, N_L0);
  string Hlt1_TIS = MakeTIS(Hlt1_list, N_Hlt1);
  string Hlt1_TISTOS = MakeTISTOS(Hlt1_list, N_Hlt1);
  string Hlt2_TIS = MakeTIS(Hlt2_list, N_Hlt2);
  string Hlt2_TISTOS = MakeTISTOS(Hlt2_list, N_Hlt2);

  //Part where the output is stored into outfile
  //We compute L0 efficiency
  //Then, Hlt1 efficiency, having passed L0
  ofstream fout;
  double eff_TIS, eff_TISTOS;

  fout.open(outfile.c_str());
  fout << "TISTOS method applied on tuples defined at " << dirfile << ":" << endl << endl;
  //L0
  fout << "         L0 lines used         " << endl;
  fout << "-------------------------------" << endl;
  for (int i = 0; i < N_L0; i++)
  {
    fout << L0_list[i] << endl;
  }
  fout << MakeTOS(L0_list, N_L0);
  fout << endl;
  cout << "Going for L0 TISTOS" << endl;
  //Compute Numbers
  eff_TIS = GetMean(chain, L0_TIS, cuts);
  eff_TISTOS = GetMean(chain, L0_TISTOS, cuts);

  fout << "L0_Trigger_eff = " << eff_TISTOS / eff_TIS << endl;
  fout << endl;


  //Hlt1
  //Include L0 cuts into cuts
  cuts = cuts + " * (" + MakeTOS(L0_list, N_L0) + ")";
  fout << "        Hlt1 lines used        " << endl;
  fout << "-------------------------------" << endl;
  for (int i = 0; i < N_Hlt1; i++)
  {
    fout << Hlt1_list[i] << endl;
  }
  fout << MakeTOS(Hlt1_list, N_Hlt1);
  fout << endl;
  cout << "Going for Hlt1 TISTOS" << endl;
  //Compute numbers
  eff_TIS = GetMean(chain, Hlt1_TIS, cuts);
  eff_TISTOS = GetMean(chain, Hlt1_TISTOS, cuts);

  fout << "Hlt1_Trigger_eff = " << eff_TISTOS / eff_TIS << endl;
  fout << endl;


  //Hlt2
  //Include Hlt1 cuts into cuts
  cuts = cuts + " * (" + MakeTOS(Hlt1_list, N_Hlt1) + ")";
  fout << "        Hlt2 lines used        " << endl;
  fout << "-------------------------------" << endl;
  for (int i = 0; i < N_Hlt2; i++)
  {
    fout << Hlt2_list[i] << endl;
  }
  fout << MakeTOS(Hlt2_list, N_Hlt2);
  fout << endl;
  cout << "Going for Hlt2 TISTOS" << endl;
  //Compute numbers
  eff_TIS = GetMean(chain, Hlt2_TIS, cuts);
  eff_TISTOS = GetMean(chain, Hlt2_TISTOS, cuts);

  fout << "Hlt2_Trigger_eff = " << eff_TISTOS / eff_TIS << endl;


  fout.close();
  CloseChain(chain);
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
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
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
