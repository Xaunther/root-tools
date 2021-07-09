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

void TISTOS_data(std::string datadir, std::string triggerfilecuts, std::string outfile, std::string variablename, FitOption fitopt, std::string treename = "", std::string L0Phys = "B_L0Global", std::string HLT1Phys = "B_Hlt1Phys", std::string HLT2Phys = "B_Hlt2Phys", std::string opts = "")
{
    //Initialize fit options
    FitFunction *fitf = FitFunction_init();

    //Get the data
    auto chain = GetChain(datadir, treename);

    //Get the trigger cuts
    auto triggercuts = GetCuts(triggerfilecuts);

    //Get TISTOS'ed
    std::vector<std::string> Phys_list = {L0Phys, HLT1Phys, HLT2Phys};
    auto TIScuts = MakeTIS(Phys_list);
    auto TOScuts = MakeTOS(Phys_list);
    auto TISTOScuts = MakeTISTOS(Phys_list);

    //Cut the data in the 4 possible trigger configurations
    //TIS
    TTree *TIStree = new TTree();
    TFile *TISfile = new TFile("Tuples/temp_TIS.root", "recreate");
    TIStree = (TTree *)chain->CopyTree(TIScuts.c_str());
    TISfile->Write();
    //TOS
    TTree *TOStree = new TTree();
    TFile *TOSfile = new TFile("Tuples/temp_TOS.root", "recreate");
    TOStree = (TTree *)chain->CopyTree(TOScuts.c_str());
    TOSfile->Write();
    //TISTOS
    TTree *TISTOStree = new TTree();
    TFile *TISTOSfile = new TFile("Tuples/temp_TISTOS.root", "recreate");
    TISTOStree = (TTree *)chain->CopyTree(TISTOScuts.c_str());
    TISTOSfile->Write();
    //Trigger
    TTree *triggertree = new TTree();
    TFile *triggerfile = new TFile("Tuples/temp_trigger.root", "recreate");
    triggertree = (TTree *)chain->CopyTree(triggercuts.c_str());
    triggerfile->Write();

    //If opts not specified, use default
    if (opts == "")
        opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
    Names name_list(opts);

    //We must do 4 datasets to do 4 fits (TIS, TOS, TISTOS, Trigger)
    RooWorkspace *TISws = fitf[fitopt](variablename, TIStree, "", 0, 0, opts);
    RooWorkspace *TOSws = fitf[fitopt](variablename, TOStree, "", 0, 0, opts);
    RooWorkspace *TISTOSws = fitf[fitopt](variablename, TISTOStree, "", 0, 0, opts);
    RooWorkspace *triggerws = fitf[fitopt](variablename, triggertree, "", 0, 0, opts);
    //We also plot
    GoodPlot(TISws, variablename, "", "", opts, "TIS");
    GoodPlot(TOSws, variablename, "", "", opts, "TOS");
    GoodPlot(TISTOSws, variablename, "", "", opts, "TISTOS");
    GoodPlot(triggerws, variablename, "", "", opts, "trigger");
    //And save the parameters from the fit in txt files
    SaveRooVars(TISws, "output/" + variablename + "_RooYields_TIS.txt");
    SaveRooVars(TOSws, "output/" + variablename + "_RooYields_TOS.txt");
    SaveRooVars(TISTOSws, "output/" + variablename + "_RooYields_TISTOS.txt");
    SaveRooVars(triggerws, "output/" + variablename + "_RooYields_trigger.txt");

    //Compute the relevant numbers
    TUncertainty N_TOS = TUncertainty(TOSws->var(name_list.fcomp[0].c_str())->getValV(), TOSws->var(name_list.fcomp[0].c_str())->getError());
    TUncertainty N_TIS = TUncertainty(TISws->var(name_list.fcomp[0].c_str())->getValV(), TISws->var(name_list.fcomp[0].c_str())->getError());
    TUncertainty N_TISTOS = TUncertainty(TISTOSws->var(name_list.fcomp[0].c_str())->getValV(), TISTOSws->var(name_list.fcomp[0].c_str())->getError());
    TUncertainty N_trig = TUncertainty(triggerws->var(name_list.fcomp[0].c_str())->getValV(), triggerws->var(name_list.fcomp[0].c_str())->getError());

    TUncertainty eff_trig = TUncertainty(((N_trig / N_TIS) * (N_TISTOS / N_TOS)).GetValue(), TISTOSUncertainty(N_TOS, N_TIS, N_TISTOS, N_trig));

    //Close TChain
    CloseChain(chain);
    //Close TFiles
    TISfile->Close();
    TOSfile->Close();
    TISTOSfile->Close();
    triggerfile->Close();

    PrintOutput(datadir, outfile, triggercuts, TIScuts, TOScuts, TISTOScuts, eff_trig);
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
    FitOption fitopt = StringToFitOption(*(new string(argv[5])));
    switch (argc - 1)
    {
    case 5:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), fitopt);
        break;
    case 6:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), fitopt, *(new std::string(argv[6])));
        break;
    case 7:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), fitopt, *(new std::string(argv[6])), *(new std::string(argv[7])));
        break;
    case 8:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), fitopt, *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])));
        break;
    case 9:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), fitopt, *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])), *(new std::string(argv[9])));
        break;
    case 10:
        TISTOS_data(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), fitopt, *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])), *(new std::string(argv[9])), *(new std::string(argv[10])));
        break;
    default:
        std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
        return (1);
        break;
    }
    return 0;
}
#endif
