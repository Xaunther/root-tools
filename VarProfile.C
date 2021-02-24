//Script to plot a profile of some given value
//Binning must be specified (for example some homogeneous binning from HomoBins.C)
//It gives the average value of Yvar in each bin of Xvar, using TProfile.
//No weights can be assigned
//If output file is not specified, it defaults to "plots/TProfile_{varX}_{varY}.pdf"
#include <iostream>
#include <string>
#include "TProfile.h"
#include "TChain.h"
#include "TTreeFormula.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TTree.h"
#include "Math/ProbFuncMathCore.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/StringTools.h"

void FillProfileFromChain(TProfile *profile, TTree *chain, std::string varX, std::string varY)
{
    //Define the formulas for the two variables
    auto formulaX = new TTreeFormula(varX.c_str(), varX.c_str(), chain);
    auto formulaY = new TTreeFormula(varY.c_str(), varY.c_str(), chain);

    //Loop over entries
    for (auto i = 0; i < chain->GetEntries(); i++)
    {
        chain->GetEntry(i);
        profile->Fill(formulaX->EvalInstance(), formulaY->EvalInstance());
    }
    delete formulaX;
    delete formulaY;
}

void VarProfile(std::string tupledir, std::string varX, std::string varY, std::string binfileX, std::string treename = "DecayTree", std::string cutfile = "", std::string plotopt = "", std::string title = "", std::string outputfile = "")
{
    //Open ntuple
    auto raw_chain = GetChain(tupledir, treename);
    //Get cuts
    std::string cuts = GetCuts(cutfile);
    //Only use events passing the given selection
    TTree *chain = raw_chain->CopyTree(cuts.c_str());
    //Get the binning!
    int NBins = 0;
    auto binning = ReadNumbers(NBins, binfileX);

    //Now define the TProfile
    auto profile = new TProfile("profile", "profile", NBins - 1, binning);

    //This must be done manually, go through the ntuple and fill the profile
    FillProfileFromChain(profile, chain, varX, varY);

    //Deal with the titles
    int title_index = 0;
    auto _titles = SplitString(title_index, title, ";");
    auto titles = new string[3];
    int max_index = (title_index > 3) ? 3 : title_index;
    for (int i = 0; i < max_index; i++)
    {
        //Only the first three
        titles[i] = _titles[i];
    }
    gStyle->SetOptStat(0);

    //Plot and save
    auto c1 = new TCanvas();
    cout << "Plotting..." << endl;
    profile->Draw(plotopt.c_str());

    //Set the titles, then save
    profile->SetTitle(titles[0].c_str());
    profile->SetXTitle(titles[1].c_str());
    profile->SetYTitle(titles[2].c_str());

    if (outputfile == "")
        outputfile = "plots/TProfile_" + varX + "_" + varY + ".pdf";
    c1->SaveAs(outputfile.c_str());

    //Some extra thingy. Perform a CHI2 test wrt a flat distribution and provide output on screen
    double chi2 = 0;
    double entries = 0;
    double mean = 0;
    //Loop to the the entries inside the range
    for (int i = 1; i <= profile->GetNbinsX(); i++)
        entries += profile->GetBinEntries(i);
    //Loop to get the mean inside the range
    for (int i = 1; i <= profile->GetNbinsX(); i++)
        mean += profile->GetBinEntries(i) / entries * profile->GetBinContent(i);
    //Loop once more to get the chi2
    for (int i = 1; i <= profile->GetNbinsX(); i++)
        chi2 += TMath::Power((profile->GetBinContent(i) - mean) / profile->GetBinError(i), 2.);

    std::cout << "Result of CHI2 test (w.r.t. uniform distribution) p-value: " << ROOT::Math::chisquared_cdf_c(chi2, profile->GetNbinsX() - 1) << std::endl;
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
    switch (argc - 1)
    {
    case 4:
        VarProfile(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])));
        break;
    case 5:
        VarProfile(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])));
        break;
    case 6:
        VarProfile(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])));
        break;
    case 7:
        VarProfile(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])));
        break;
    case 8:
        VarProfile(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])));
        break;
    case 9:
        VarProfile(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])), *(new std::string(argv[7])), *(new std::string(argv[8])), *(new std::string(argv[9])));
        break;
    default:
        std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
        return (1);
        break;
    }
    return 0;
}
#endif