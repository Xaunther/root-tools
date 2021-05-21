//Trigger efficiency, assuming one is accepting TOS events

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TChain.h"
#include <stdio.h>
#include "Functions/TISTOS.h"
#include "Functions/Dictreading.h"
#include "Functions/Filereading.h"
#include "Functions/TreeTools.h"
#include "Functions/ArrayTools.h"
#include "Functions/TUncertainty.h"
using namespace std;

void TISTOS(string dirfile_data, string dirfile_MC, string trigger_lines, string precutsfilename_data = "1", string precutsfilename_MC = "1", string outfile = "TISTOS_results.txt")
{
  //Precuts we might want to provide
  auto precuts_data = GetCuts(precutsfilename_data);
  auto precuts_MC = GetCuts(precutsfilename_MC);

  //Get chains
  auto chain_data = GetChain(dirfile_data);
  auto chain_MC = GetChain(dirfile_MC);

  //Trigger lines used for TISTOS
  int N_lines = 0;
  auto trigger_list = ReadVariables(N_lines, trigger_lines);
  //Clean suffixes
  trigger_list = CleanTISTOS(trigger_list, N_lines);
  //Perform TIS and TISTOS Cuts
  auto trigger_TIS = MakeTIS(trigger_list, N_lines);
  auto trigger_TOS = MakeTOS(trigger_list, N_lines);
  auto trigger_TISTOS = MakeTISTOS(trigger_list, N_lines);

  //Part where the output is stored into outfile
  //We compute trigger efficiency
  ofstream fout;
  TUncertainty eff_TIS, eff_MC;
  double N0;
  fout.open(outfile.c_str());
  fout << "TISTOS method applied on tuples defined at " << dirfile_data << " (data) and " << dirfile_MC << " (MC):" << endl
       << endl;
  //List trigger lines
  fout << "         Trigger lines used         " << endl;
  fout << "------------------------------------" << endl;
  for (int i = 0; i < N_lines; i++)
    fout << trigger_list[i] << endl;

  //Compute efficiency part from data
  N0 = chain_data->GetEntries(precuts_data.c_str());
  eff_TIS = GetMean(chain_data, trigger_TISTOS, precuts_data) / GetMean(chain_data, trigger_TOS, precuts_data);
  eff_TIS = TUncertainty(eff_TIS.GetValue(), sqrt(eff_TIS.GetValue() * (1 - eff_TIS.GetValue()) / N0));

  //Compute efficiency part from MC
  N0 = chain_MC->GetEntries(precuts_MC.c_str());
  eff_MC = GetMean(chain_MC, trigger_TOS, precuts_MC) / GetMean(chain_MC, trigger_TIS, precuts_MC);
  eff_MC = TUncertainty(eff_MC.GetValue(), sqrt(eff_MC.GetValue() * (1 - eff_MC.GetValue()) / N0));

  fout << "Trigger_eff = ";
  (eff_MC * eff_TIS).Print(fout, "rel");
  fout << endl;

  fout.close();
  CloseChain(chain_data);
  CloseChain(chain_MC);
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
  switch (argc - 1)
  {
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
