//Script to compute trigger efficiency trough TISTOS method
//To start with, no binning is made
//Should only be used on MC, since it is just counting events. See TISTOS_data.C if the counting is done from a fit
//The Trigger efficiency is computed as eff(Trig)=N(TISTOS)/N(TIS)
//We assume the trigger requirements are TOS, and these same are used for N(TISTOS)
//For TIS we also assume the same set of lines is used

//One needs to provide a data sample to work with, another file containing the custom trigger selection and the weight variable (if applicable).
//The name of the tree can be specified, otherwise it will search on Dictionaries/treenames.dic in the execution folder (not here) and if not found it defaults to "DecayTree"
//Physical L0, HLT1 and HLT2 trigger lines are given some default names, but they can be overriden

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "TMath.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/TISTOS.h"
#include "Functions/TUncertainty.h"

void TISTOS_MC(std::string datadir, std::string L0fileTOS, std::string Hlt1fileTOS, std::string Hlt2fileTOS, std::string L0fileTIS, std::string Hlt1fileTIS, std::string Hlt2fileTIS, std::string outfile, std::string w_var = "1", std::string treename = "")
{
  //Get the data
  auto chain = GetChain(datadir, treename);

  //Get the L0 lines
  auto TOScutsL0 = ReadVariables(L0fileTOS);
  auto TIScutsL0 = ReadVariables(L0fileTIS);
  //Get the Hlt1 lines
  auto TOScutsHlt1 = ReadVariables(Hlt1fileTOS);
  auto TIScutsHlt1 = ReadVariables(Hlt1fileTIS);
  //Get the Hlt2 lines
  auto TOScutsHlt2 = ReadVariables(Hlt2fileTOS);
  auto TIScutsHlt2 = ReadVariables(Hlt2fileTIS);

  //Get the cuts
  auto TIScuts = JoinCutsAND({JoinCutsOR(TIScutsL0), JoinCutsOR(TIScutsHlt1), JoinCutsOR(TIScutsHlt2)});
  auto TOScuts = JoinCutsAND({JoinCutsOR(TOScutsL0), JoinCutsOR(TOScutsHlt1), JoinCutsOR(TOScutsHlt2)});
  auto TISTOScuts = JoinCutsAND({TIScuts, TOScuts});

  //Compute the relevant numbers
  auto N_TIS = GetEfficiency(chain, TIScuts, w_var);
  auto N_TISTOS = GetEfficiency(chain, TISTOScuts, w_var);
  std::cout << "N_TISTOS: ";
  N_TISTOS.Print();
  std::cout << "N_TIS: ";
  N_TIS.Print();

  TUncertainty eff_trig = TUncertainty((N_TISTOS / N_TIS).GetValue(), TISTOSUncertainty(N_TIS, N_TISTOS));

  CloseChain(chain);
  PrintOutput(datadir, outfile, TIScuts, TISTOScuts, eff_trig);
  delete chain;
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
  switch (argc - 1)
  {
  case 8:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])));
    break;
  case 9:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])), *(new std::string(argv[9])));
    break;
  case 10:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])), *(new std::string(argv[9])), *(new std::string(argv[10])));
    break;
  default:
    std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
    return (1);
    break;
  }
  return 0;
}
#endif
