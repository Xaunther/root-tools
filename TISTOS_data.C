//Script to compute trigger efficiency trough TISTOS method
//To start with, no binning is made
//It is thought to be used for data, where the number of signal events is extracted from fits
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

#include "TFile.h"
#include "RooRealVar.h"
#include "TMath.h"

#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/Filesaving.h"
#include "Functions/Dictreading.h"
#include "Functions/TISTOS.h"
#include "Functions/Fits.h"
#include "Functions/PlotTools.h"
#include "Functions/TUncertainty.h"

#include "Dictionaries/Names.h"

void TISTOS_data(std::string datadir, std::string L0fileTOS, std::string Hlt1fileTOS, std::string Hlt2fileTOS, std::string L0fileTIS, std::string Hlt1fileTIS, std::string Hlt2fileTIS, std::string outfile, std::string variablename, FitOption fitopt, std::string treename = "", std::string opts = "")
{
    //Initialize fit options
    FitFunction *fitf = FitFunction_init();

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

    std::cout << TIScuts << std::endl;
    std::cout << TISTOScuts << std::endl;
    //Cut the data in the 4 possible trigger configurations
    //TIS
    TTree *TIStree = new TTree();
    TFile *TISfile = new TFile("Tuples/temp_TIS.root", "recreate");
    TIStree = (TTree *)chain->CopyTree(TIScuts.c_str());
    TISfile->Write();
    //TISTOS
    TTree *TISTOStree = new TTree();
    TFile *TISTOSfile = new TFile("Tuples/temp_TISTOS.root", "recreate");
    TISTOStree = (TTree *)chain->CopyTree(TISTOScuts.c_str());
    TISTOSfile->Write();

    //If opts not specified, use default
    if (opts == "")
        opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
    Names name_list(opts);

    //We must do 4 datasets to do 4 fits (TIS, TOS, TISTOS, Trigger)
    RooWorkspace *TISws = fitf[fitopt](variablename, TIStree, "", 0, 0, opts);
    RooWorkspace *TISTOSws = fitf[fitopt](variablename, TISTOStree, "", 0, 0, opts);
    //We also plot
    GoodPlot(TISws, variablename, "", "", opts, "TIS");
    GoodPlot(TISTOSws, variablename, "", "", opts, "TISTOS");
    //And save the parameters from the fit in txt files
    SaveRooVars(TISws, "output/" + variablename + "_RooYields_TIS.txt");
    SaveRooVars(TISTOSws, "output/" + variablename + "_RooYields_TISTOS.txt");

    //Compute the relevant numbers
    TUncertainty N_TIS = TUncertainty(TISws->var(name_list.fcomp[0].c_str())->getValV(), TISws->var(name_list.fcomp[0].c_str())->getError());
    TUncertainty N_TISTOS = TUncertainty(TISTOSws->var(name_list.fcomp[0].c_str())->getValV(), TISTOSws->var(name_list.fcomp[0].c_str())->getError());

    TUncertainty eff_trig = TUncertainty((N_TISTOS / N_TIS).GetValue(), TISTOSUncertainty(N_TIS, N_TISTOS));

    //Close TChain
    CloseChain(chain);
    //Close TFiles
    TISfile->Close();
    TISTOSfile->Close();

    PrintOutput(datadir, outfile, TIScuts, TISTOScuts, eff_trig);
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
    FitOption fitopt = StringToFitOption(*(new string(argv[10])));
    switch (argc - 1)
    {
    case 10:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), fitopt);
        break;
    case 11:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), fitopt, *(new std::string(argv[11])));
        break;
    case 12:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new string(argv[7])), *(new string(argv[8])), *(new string(argv[9])), fitopt, *(new std::string(argv[11])), *(new std::string(argv[12])));
        break;
    default:
        std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
        return (1);
        break;
    }
    return 0;
}
#endif
