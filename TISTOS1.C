//TISTOS.C uses both the data and MC part
//We split that script into two, so that one can run the data part (Super slow) once and then run only the MC part once for each sample
//Like in TISTOS.C, we are assuming that the TIS efficiency is independent of the MC sample, and taking it to the next level.

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

void TISTOS1(string dirfile_data, string trigger_lines, string precutsfilename_data = "1", string outfile = "TISTOS1_results.txt")
{
    //Precuts we might want to provide
    auto precuts_data = GetCuts(precutsfilename_data);

    //Get chains
    auto chain_data = GetChain(dirfile_data);

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
    TUncertainty eff_TIS;
    double N0;

    fout.open(outfile.c_str());
    fout << "TIS efficiency over data " << dirfile_data << " :" << endl
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

    fout << "TIS_eff = ";
    eff_TIS.Print(fout);
    fout << endl;

    fout.close();
    CloseChain(chain_data);
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
    switch (argc - 1)
    {
    case 2:
        TISTOS1(*(new string(argv[1])), *(new string(argv[2])));
        break;
    case 3:
        TISTOS1(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
        break;
    case 4:
        TISTOS1(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
        break;
    default:
        cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
        return (1);
        break;
    }
    return 0;
}
#endif
