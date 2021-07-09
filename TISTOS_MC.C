//Script to compute trigger efficiency trough TISTOS method
//To start with, no binning is made
//Should only be used on MC, since it is just counting events. See TISTOS_data.C if the counting is done from a fit
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
#include "TMath.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/TISTOS.h"
#include "Functions/TUncertainty.h"

void TISTOS_MC(std::string datadir, std::string triggerfile, std::string outfile, std::string w_var = "1", std::string treename = "", std::string L0Phys = "B_L0Global", std::string HLT1Phys = "B_Hlt1Phys", std::string HLT2Phys = "B_Hlt2Phys")
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
  double N0 = chain->GetEntries();
  TUncertainty N_TOS = GetMeanEntries(chain, TOScuts, w_var);
  N_TOS = TUncertainty(N_TOS.GetValue(), TMath::Sqrt(N_TOS.GetValue() / N0));
  TUncertainty N_TIS = GetMeanEntries(chain, TIScuts, w_var);
  N_TIS = TUncertainty(N_TIS.GetValue(), TMath::Sqrt(N_TIS.GetValue() / N0));
  TUncertainty N_TISTOS = GetMeanEntries(chain, TISTOScuts, w_var);
  N_TISTOS = TUncertainty(N_TISTOS.GetValue(), TMath::Sqrt(N_TISTOS.GetValue() / N0));
  TUncertainty N_trig = GetMeanEntries(chain, triggercuts, w_var);
  N_trig = TUncertainty(N_trig.GetValue(), TMath::Sqrt(N_trig.GetValue() / N0));

  TUncertainty eff_trig = TUncertainty(((N_trig / N_TIS) * (N_TISTOS / N_TOS)).GetValue(), TISTOSUncertainty(N_TOS, N_TIS, N_TISTOS, N_trig));

  CloseChain(chain);
  PrintOutput(datadir, outfile, triggercuts, TIScuts, TOScuts, TISTOScuts, eff_trig);
  delete chain;
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
  switch (argc - 1)
  {
  case 3:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])));
    break;
  case 4:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])));
    break;
  case 5:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])));
    break;
  case 6:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])));
    break;
  case 7:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])));
    break;
  case 8:
    TISTOS_MC(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])));
    break;
  default:
    std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
    return (1);
    break;
  }
  return 0;
}
#endif
