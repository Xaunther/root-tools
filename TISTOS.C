//Script to compute trigger efficiency trough TISTOS method
//To start with, no binning is made
//Can be used both on MC or data
//The Trigger efficiency is computed as eff(Trig)=N(trig)/N(TIS)*N(TISTOS)/N(TOS)
//Trig refers to our trigger selection of choice
//TIS, TOS and TISTOS, refer to L0, HLT1 and HLT2 Physical lines all either TIS, TOS or TISTOS

//One needs to provide a data sample to work with, another file containing the custom trigger selection and the weight variable (if applicable).
//The name of the tree can be specified, otherwise it will search on Dictionaries/treenames.dic in the execution folder (not here) and if not found it defaults to "DecayTree"
//Physical L0, HLT1 and HLT2 trigger lines are given some default names, but they can be overriden

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/TISTOS.h"

void PrintOutput(const std::string &datadir, const std::string &outfile, const std::string &triggercuts, const std::string &TIScuts, const std::string &TOScuts, const std::string &TISTOScuts, const double &eff_trig)
{
  ofstream outf;
  outf.open(outfile);
  if (!outf)
  {
    std::cout << "Cannot open " << outfile << ". Exiting" << std::endl;
    exit(1);
  }
  outf << "Trigger efficiency on " << datadir << std::endl;
  outf << "----------------------------------------------------------------------" << std::endl;
  outf << "TIS cuts: " << TIScuts << std::endl;
  outf << "TOS cuts: " << TOScuts << std::endl;
  outf << "TISTOS cuts: " << TISTOScuts << std::endl;
  outf << "Trigger cuts: " << triggercuts << std::endl;
  outf << "Efficiency: " << eff_trig << std::endl;
  outf.close();
}

void TISTOS(std::string datadir, std::string triggerfile, std::string outfile, std::string w_var = "1", std::string treename = "", std::string L0Phys = "B_L0Global", std::string HLT1Phys = "B_Hlt1Phys", std::string HLT2Phys = "B_Hlt2Phys")
{
  //Get the data
  auto chain = GetChain(datadir, treename);

  //Get the trigger cuts
  auto triggercuts = GetCuts(triggerfile);

  //Get TISTOS'ed
  std::vector<std::string> Phys_list = {L0Phys, HLT1Phys, HLT2Phys};
  auto TIScuts = MakeTIS(Phys_list);
  auto TOScuts = MakeTOS(Phys_list);
  auto TISTOScuts = MakeTISTOS(Phys_list);

  //Compute the relevant numbers
  double N_TOS = GetMeanEntries(chain, TOScuts, w_var);
  double N_TIS = GetMeanEntries(chain, TIScuts, w_var);
  double N_TISTOS = GetMeanEntries(chain, TISTOScuts, w_var);
  double N_trig = GetMeanEntries(chain, triggercuts, w_var);

  double eff_trig = (N_trig / N_TIS) * (N_TISTOS / N_TOS);

  CloseChain(chain);
  PrintOutput(datadir, outfile, triggercuts, TIScuts, TOScuts, TISTOScuts, eff_trig);
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
  switch (argc - 1)
  {
  case 3:
    TISTOS(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])));
    break;
  case 4:
    TISTOS(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])));
    break;
  case 5:
    TISTOS(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])));
    break;
  case 6:
    TISTOS(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])));
    break;
  case 7:
    TISTOS(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])));
    break;
  case 8:
    TISTOS(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])));
    break;
  default:
    std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
    return (1);
    break;
  }
  return 0;
}
#endif
