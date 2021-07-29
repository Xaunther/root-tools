//Spin-off of AnalFits.cxx containing simultaneous fits used in NstG analysis
#include <string>
#include <vector>
#include "TTree.h"
#include "TFile.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"
#include "RooPolynomial.h"
#include "RooCBShape.h"
#include "RooExponential.h"
#include "RooGaussian.h"
#include "Dictionaries/Constants.h"
#include "Dictionaries/Names.h"
#include "Functions/Fits.h"
#include "Functions/SimultaneousFits_NstG.h"
#include "Functions/PlotTools.h"
#include "Functions/TreeTools.h"
#include "Functions/Filesaving.h"
#include "Functions/Dictreading.h"
#include "Functions/RooGaussExp.h"
#include "Functions/RooDoubleGaussExp.h"
#include "Functions/RooArgusGauss.h"
#include "Functions/RooCBExp.h"

using namespace std;

//Simultaneous fit to 3 channels: ppiG, KpiG and pKG
RooWorkspace *FitLb2NstG_Simult(string *variablename, TTree **tree, string opts)
{
    //Initialize constants
    Constants const_list(opts);
    Names name_list(opts);
    stringstream ss;
    //Initialize array of predefined fit functions
    FitFunction *fitf = FitFunction_init();
    //Initialize total multifit workspace
    RooWorkspace *ws = new RooWorkspace(name_list.workspace.c_str());
    //First of all, we need fits to MC. Evey MC is fitted 3 times: ppiG, KpiG and pKG mass
    //First, open the MC samples and read them trees
    std::vector<TFile *> MCfile0;
    std::vector<TTree *> MCtree0;
    //Stage 1 files
    std::vector<TFile *> MCfile1;
    std::vector<TTree *> MCtree1;
    //Stage 2 files
    std::vector<TFile *> MCfile2;
    std::vector<TTree *> MCtree2;
    //Final stage temp tuples
    std::vector<TFile *> MCfile;
    std::vector<TTree *> MCtree;
    //Initialize workspaces
    std::vector<RooWorkspace *> ws_ppiG_mass;
    std::vector<RooWorkspace *> ws_KpiG_mass;
    std::vector<RooWorkspace *> ws_pKG_mass;
    //KpiG, pKG ppiG, KpiG Ref, pKG Ref, KpipiG MCs
    std::vector<string> fileindex = {"0", "5", "4", "6", "9", "2"};
    //We also need to add the global weight variable to each PID weight...
    for (unsigned int i = 0; i < fileindex.size(); i++)
    {
        //Initial tree
        MCfile0.push_back(TFile::Open(("Tuples/temp" + fileindex[i] + ".root").c_str()));
        MCtree0.push_back((TTree *)MCfile0[i]->Get("DecayTree"));
        //Add normal pid branch and open
        AddTreeBranch(MCtree0[i], "Event_PIDCalibEff_global_weight", "Event_PIDCalibEff*Global_weight", "Tuples/temp_" + fileindex[i] + "_global.root");
        MCfile1.push_back(TFile::Open(("Tuples/temp_" + fileindex[i] + "_global.root").c_str()));
        MCtree1.push_back((TTree *)MCfile1[i]->Get("DecayTree"));
        //Add pbarpi pid branch and open
        AddTreeBranch(MCtree1[i], "Event_PIDCalibEff_pbarpi_global_weight", "Event_PIDCalibEff_pbarpi*Global_weight", "Tuples/temp_" + fileindex[i] + "_global_pbarpi.root");
        MCfile2.push_back(TFile::Open(("Tuples/temp_" + fileindex[i] + "_global_pbarpi.root").c_str()));
        MCtree2.push_back((TTree *)MCfile2[i]->Get("DecayTree"));
        //Add ppibar pid branch and open
        AddTreeBranch(MCtree2[i], "Event_PIDCalibEff_ppibar_global_weight", "Event_PIDCalibEff_ppibar*Global_weight", "Tuples/temp_" + fileindex[i] + "_global_ppibar.root");
        MCfile.push_back(TFile::Open(("Tuples/temp_" + fileindex[i] + "_global_ppibar.root").c_str()));
        MCtree.push_back((TTree *)MCfile[i]->Get("DecayTree"));
    }

    std::vector<string> massnames;
    massnames.push_back("KpiG");
    massnames.push_back("pKG");
    massnames.push_back("ppiG");
    massnames.push_back("KpiGRef");
    massnames.push_back("pKGRef");
    massnames.push_back("KpipiG");
    std::vector<string> latex_massnames;
    latex_massnames.push_back("K^{+}#pi^{-}#gamma");
    latex_massnames.push_back("pK^{-}#gamma");
    latex_massnames.push_back("p#pi^{-}#gamma");
    latex_massnames.push_back("K^{+}#pi^{-}#gamma Ref");
    latex_massnames.push_back("pK^{-}#gamma Ref");
    latex_massnames.push_back("K^{+}#pi^{+}#pi^{-}#gamma");
    //Let's start with fits to ppiG mass
    cout << "Starting MC fits to ppiG mass" << endl;
    cout << "-----------------------------" << endl;
    cout << "KpiG MC" << endl
         << "-------" << endl;
    ws_ppiG_mass.push_back(fitf[CBExp](variablename[0], MCtree[0], "Event_PIDCalibEff_global_weight", 0, 0, opts)); //KpiG MC
    cout << "pKG MC" << endl
         << "------" << endl;
    ws_ppiG_mass.push_back(fitf[DoubleGaussExp](variablename[0], MCtree[1], "Event_PIDCalibEff_global_weight", 0, 0, opts)); //pKG MC
    cout << "ppiG MC" << endl
         << "-------" << endl;
    ws_ppiG_mass.push_back(fitf[CBExp](variablename[0], MCtree[2], "Event_PIDCalibEff_global_weight", 0, 0, opts)); //ppiG MC
    cout << "KpiG MC Reflected" << endl
         << "-------" << endl;
    ws_ppiG_mass.push_back(fitf[CBExp](variablename[0], MCtree[3], "Event_PIDCalibEff_global_weight", 0, 0, opts + "_ppiG_KpiGRef")); //KpiG MC Reflected
    cout << "pKG MC Reflected" << endl
         << "-------" << endl;
    ws_ppiG_mass.push_back(fitf[DoubleGaussExp](variablename[0], MCtree[4], "Event_PIDCalibEff_global_weight", 0, 0, opts + "_ppiG_pKGRef")); //pKG MC Reflected

    //Let's continue with fits to KpiG mass
    cout << "Starting MC fits to KpiG mass" << endl;
    cout << "-----------------------------" << endl;
    cout << "KpiG MC" << endl
         << "-------" << endl;
    ws_KpiG_mass.push_back(fitf[CBExp](variablename[1], MCtree[0], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts)); //KpiG MC
    cout << "pKG MC" << endl
         << "------" << endl;
    ws_KpiG_mass.push_back(fitf[DoubleGaussExp](variablename[1], MCtree[1], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts + "_KpiG_pKG")); //pKG MC
    cout << "ppiG MC" << endl
         << "-------" << endl;
    ws_KpiG_mass.push_back(fitf[DoubleGaussExp](variablename[1], MCtree[2], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts)); //ppiG MC
    cout << "KpiG MC Reflected" << endl
         << "-------" << endl;
    ws_KpiG_mass.push_back(fitf[CBExp](variablename[1], MCtree[3], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts)); //KpiG MC Reflected
    cout << "pKG MC Reflected" << endl
         << "-------" << endl;
    ws_KpiG_mass.push_back(fitf[CBExp](variablename[1], MCtree[4], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts)); //pKG MC Reflected
    cout << "Partially reconstructed" << endl
         << "-------" << endl;
    ws_KpiG_mass.push_back(fitf[ArgusGauss](variablename[1], MCtree[5], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts)); //KpipiG part. reco

    //Let's finish with fits to pKG mass
    cout << "Starting MC fits to pKG mass" << endl;
    cout << "-----------------------------" << endl;
    cout << "KpiG MC" << endl
         << "-------" << endl;
    ws_pKG_mass.push_back(fitf[CBExp](variablename[2], MCtree[0], "Event_PIDCalibEff_ppibar_global_weight", 0, 0, opts)); //KpiG MC
    cout << "pKG MC" << endl
         << "------" << endl;
    ws_pKG_mass.push_back(fitf[CBExp](variablename[2], MCtree[1], "Event_PIDCalibEff_ppibar_global_weight", 0, 0, opts)); //pKG MC
    cout << "ppiG MC" << endl
         << "-------" << endl;
    ws_pKG_mass.push_back(fitf[CBExp](variablename[2], MCtree[2], "Event_PIDCalibEff_ppibar_global_weight", 0, 0, opts)); //ppiG MC
    cout << "KpiG MC Reflected" << endl
         << "-------" << endl;
    ws_pKG_mass.push_back(fitf[DoubleGaussExp](variablename[2], MCtree[3], "Event_PIDCalibEff_ppibar_global_weight", 0, 0, opts)); //KpiG MC Reflected
    cout << "pKG MC Reflected" << endl
         << "-------" << endl;
    ws_pKG_mass.push_back(fitf[DoubleGaussExp](variablename[2], MCtree[4], "Event_PIDCalibEff_ppibar_global_weight", 0, 0, opts)); //pKG MC Reflected

    //Time to plot and dump results in .txt files
    for (unsigned int i = 0; i < ws_ppiG_mass.size(); i++) //ppiG mass fits
    {
        GoodPlot(ws_ppiG_mass[i], variablename[0], "p #pi^{-} #gamma mass. " + latex_massnames[i] + " MC", "m_{p#pi^{-}#gamma}", opts + "_MC", "_MC_" + massnames[i], 60);
        SaveRooVars(ws_ppiG_mass[i], const_list.workingdir + variablename[0] + "_RooYields_MC_" + massnames[i] + ".txt");
    }
    for (unsigned int i = 0; i < ws_KpiG_mass.size(); i++) //KpiG mass fits
    {
        GoodPlot(ws_KpiG_mass[i], variablename[1], "K^{+} #pi^{-} #gamma mass. " + latex_massnames[i] + " MC", "m_{K^{+}#pi^{-}#gamma}", opts + "_MC", "_MC_" + massnames[i], 60);
        SaveRooVars(ws_KpiG_mass[i], const_list.workingdir + variablename[1] + "_RooYields_MC_" + massnames[i] + ".txt");
    }
    for (unsigned int i = 0; i < ws_pKG_mass.size(); i++) //pKG mass fits
    {
        GoodPlot(ws_pKG_mass[i], variablename[2], "p K^{-} #gamma mass. " + latex_massnames[i] + " MC", "m_{pK^{-}#gamma}", opts + "_MC", "_MC_" + massnames[i], 60);
        SaveRooVars(ws_pKG_mass[i], const_list.workingdir + variablename[2] + "_RooYields_MC_" + massnames[i] + ".txt");
    }

    //Nicely done with the MCs, now it is time to close them all, they are no longer needed. Also deallocate the arrays
    for (unsigned int i = 0; i < MCfile.size(); i++)
    {
        CloseTree(MCtree0[i]);
        CloseTree(MCtree1[i]);
        CloseTree(MCtree2[i]);
        CloseTree(MCtree[i]);
        MCtree0.erase(MCtree0.begin() + i);
        MCtree1.erase(MCtree1.begin() + i);
        MCtree2.erase(MCtree2.begin() + i);
        MCtree.erase(MCtree.begin() + i);
        MCfile0.erase(MCfile0.begin() + i);
        MCfile1.erase(MCfile1.begin() + i);
        MCfile2.erase(MCfile2.begin() + i);
        MCfile.erase(MCfile.begin() + i);
    }
    MCtree0.clear();
    MCtree1.clear();
    MCtree2.clear();
    MCtree.clear();
    MCfile.clear();

    Info("AnalFits.cxx", "MC deleted");
    //Time to construct the Simultaneous fit! We have 3 categories: ppiG, KpiG and pKG masses
    RooCategory cats(name_list.catname.c_str(), name_list.catname.c_str());
    for (int i = 0; i < 3; i++)
    {
        cats.defineType(variablename[i].c_str());
    }
    RooSimultaneous simpdf(name_list.pdfmodel[3].c_str(), name_list.pdfmodel[3].c_str(), cats);
    //Define the 3 mass variables and the dataset for each (it's the same tree but each variable has a different selection)
    std::vector<RooRealVar> b_masses;
    std::vector<RooDataSet> b_datasets;
    //Entries in each tree
    std::vector<long> entries;
    for (int i = 0; i < 3; i++)
    {
        //Only pass to dataset the variable we are going to use (Speeds up fit)
        Activate(tree[i], variablename[i]);
        b_masses.push_back(RooRealVar(variablename[i].c_str(), variablename[i].c_str(), const_list.xmin, const_list.xmax));
        b_datasets.push_back(RooDataSet(("dataset_" + variablename[i]).c_str(), ("dataset_" + variablename[i]).c_str(), tree[i], b_masses[i]));
        entries.push_back(tree[i]->GetEntries());
    }
    //Big dataset with all pieces put together
    Info("AnalFits.cxx", "Starting to define big dataset");
    RooDataSet data(name_list.dataset.c_str(), name_list.dataset.c_str(), RooArgSet(b_masses[0], b_masses[1], b_masses[2]), RooFit::Index(cats),
                    RooFit::Import(variablename[0].c_str(), b_datasets[0]),
                    RooFit::Import(variablename[1].c_str(), b_datasets[1]),
                    RooFit::Import(variablename[2].c_str(), b_datasets[2]));

    //So now it is time to define the PDF for each variable
    std::vector<RooAddPdf> model;
    //Dummy variable to store values of fit parameters
    double value;
    //ppiG mass
    //Define all components and then add
    Info("AnalFits.cxx", "Starting to define ppiG mass pdf");
    //This mean can be shifted. The shift is read from a fit to data, and the dictionary dictates whether it is used, added, or substracted (factor)
    value = ws_ppiG_mass[0]->var(name_list.mean[0].c_str())->getValV();
    if (const_list.shift1 != 0.)
    {
        value = value + const_list.shift1 * TMath::Abs(stod(GetValueFor("#mu_{K#pi#gamma}", "output/B_M012_RooYields_WrongMassFit.txt")) - stod(GetValueFor("#mu_{K#pi#gamma}", "output/" + variablename[0] + "_RooYields_MC0_Wrong.txt")));
    }
    RooRealVar mean_ppiGmass_KpiGMC(name_list.mean[0].c_str(), name_list.mean[0].c_str(), value);
    mean_ppiGmass_KpiGMC.setConstant();
    value = ws_ppiG_mass[0]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_ppiGmass_KpiGMC(name_list.width[0].c_str(), name_list.width[0].c_str(), value);
    sigma_ppiGmass_KpiGMC.setConstant();
    value = ws_ppiG_mass[0]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_ppiGmass_KpiGMC(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), value);
    alphaL_ppiGmass_KpiGMC.setConstant();
    value = ws_ppiG_mass[0]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_ppiGmass_KpiGMC(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), value);
    alphaR_ppiGmass_KpiGMC.setConstant();
    value = ws_ppiG_mass[0]->var(name_list.n.c_str())->getValV();
    RooRealVar n_ppiGmass_KpiGMC(name_list.nL[0].c_str(), name_list.nL[0].c_str(), value);
    n_ppiGmass_KpiGMC.setConstant();
    RooCBExp pdf_ppiGmass_KpiGMC(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), b_masses[0], mean_ppiGmass_KpiGMC, sigma_ppiGmass_KpiGMC, alphaL_ppiGmass_KpiGMC, n_ppiGmass_KpiGMC, alphaR_ppiGmass_KpiGMC);

    //This mean can be shifted. The shift is read from a fit to data, and the dictionary dictates whether it is used, added, or substracted (factor)
    value = ws_ppiG_mass[1]->var(name_list.mean[0].c_str())->getValV();
    if (const_list.shift2 != 0.)
    {
        value = value + const_list.shift2 * TMath::Abs(stod(GetValueFor("#mu_{pK#gamma}", "output/B_M012_Subst01_Kpi2pK_RooYields_WrongMassFit.txt")) - stod(GetValueFor("#mu_{pK#gamma}", "output/" + variablename[0] + "_RooYields_MC1_Wrong.txt")));
    }
    RooRealVar mean_ppiGmass_pKGMC(name_list.mean[1].c_str(), name_list.mean[1].c_str(), value);
    mean_ppiGmass_pKGMC.setConstant();
    value = ws_ppiG_mass[1]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_ppiGmass_pKGMC(name_list.width[1].c_str(), name_list.width[1].c_str(), value);
    sigma_ppiGmass_pKGMC.setConstant();
    value = ws_ppiG_mass[1]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_ppiGmass_pKGMC(name_list.alphaL[1].c_str(), name_list.alphaL[1].c_str(), value);
    alphaL_ppiGmass_pKGMC.setConstant();
    value = ws_ppiG_mass[1]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_ppiGmass_pKGMC(name_list.alphaR[1].c_str(), name_list.alphaR[1].c_str(), value);
    alphaR_ppiGmass_pKGMC.setConstant();
    RooDoubleGaussExp pdf_ppiGmass_pKGMC(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), b_masses[0], mean_ppiGmass_pKGMC, sigma_ppiGmass_pKGMC, alphaL_ppiGmass_pKGMC, alphaR_ppiGmass_pKGMC);

    value = ws_ppiG_mass[2]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_ppiGmass_ppiGMC(name_list.mean[2].c_str(), name_list.mean[2].c_str(), value, const_list.xmin, const_list.xmax); //mean_ppiGmass_ppiGMC.setConstant();
    //Free width, tie it to pKG MC width in pKG mass
    value = ws_pKG_mass[1]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_ppiGmass_ppiGMC(name_list.width[2].c_str(), name_list.width[2].c_str(), value, const_list.width_min, const_list.width_max);
    value = ws_ppiG_mass[2]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_ppiGmass_ppiGMC(name_list.alphaL[2].c_str(), name_list.alphaL[2].c_str(), value);
    alphaL_ppiGmass_ppiGMC.setConstant();
    value = ws_ppiG_mass[2]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_ppiGmass_ppiGMC(name_list.alphaR[2].c_str(), name_list.alphaR[2].c_str(), value);
    alphaR_ppiGmass_ppiGMC.setConstant();
    value = ws_ppiG_mass[2]->var(name_list.n.c_str())->getValV();
    RooRealVar n_ppiGmass_ppiGMC(name_list.nL[2].c_str(), name_list.nL[2].c_str(), value);
    n_ppiGmass_ppiGMC.setConstant();
    RooCBExp pdf_ppiGmass_ppiGMC(name_list.comppdf[2].c_str(), name_list.comppdf[2].c_str(), b_masses[0], mean_ppiGmass_ppiGMC, sigma_ppiGmass_ppiGMC, alphaL_ppiGmass_ppiGMC, n_ppiGmass_ppiGMC, alphaR_ppiGmass_ppiGMC);

    value = ws_ppiG_mass[3]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_ppiGmass_KpiRefGMC(name_list.mean[3].c_str(), name_list.mean[3].c_str(), value, const_list.xmin, const_list.xmax);
    mean_ppiGmass_KpiRefGMC.setConstant();
    value = ws_ppiG_mass[3]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_ppiGmass_KpiRefGMC(name_list.width[3].c_str(), name_list.width[3].c_str(), value, const_list.width_min, const_list.width_max);
    sigma_ppiGmass_KpiRefGMC.setConstant();
    value = ws_ppiG_mass[3]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_ppiGmass_KpiRefGMC(name_list.alphaL[3].c_str(), name_list.alphaL[3].c_str(), value);
    alphaL_ppiGmass_KpiRefGMC.setConstant();
    value = ws_ppiG_mass[3]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_ppiGmass_KpiRefGMC(name_list.alphaR[3].c_str(), name_list.alphaR[3].c_str(), value);
    alphaR_ppiGmass_KpiRefGMC.setConstant();
    value = ws_ppiG_mass[3]->var(name_list.n.c_str())->getValV();
    RooRealVar n_ppiGmass_KpiRefGMC(name_list.nL[3].c_str(), name_list.nL[3].c_str(), value);
    n_ppiGmass_KpiRefGMC.setConstant();
    RooCBExp pdf_ppiGmass_KpiRefGMC(name_list.comppdf[3].c_str(), name_list.comppdf[3].c_str(), b_masses[0], mean_ppiGmass_KpiRefGMC, sigma_ppiGmass_KpiRefGMC, alphaL_ppiGmass_KpiRefGMC, n_ppiGmass_KpiRefGMC, alphaR_ppiGmass_KpiRefGMC);

    value = ws_ppiG_mass[4]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_ppiGmass_pKRefGMC(name_list.mean[4].c_str(), name_list.mean[4].c_str(), value, const_list.xmin, const_list.xmax);
    mean_ppiGmass_pKRefGMC.setConstant();
    value = ws_ppiG_mass[4]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_ppiGmass_pKRefGMC(name_list.width[4].c_str(), name_list.width[4].c_str(), value, const_list.width_min, const_list.width_max);
    sigma_ppiGmass_pKRefGMC.setConstant();
    value = ws_ppiG_mass[4]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_ppiGmass_pKRefGMC(name_list.alphaL[4].c_str(), name_list.alphaL[4].c_str(), value);
    alphaL_ppiGmass_pKRefGMC.setConstant();
    value = ws_ppiG_mass[4]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_ppiGmass_pKRefGMC(name_list.alphaR[4].c_str(), name_list.alphaR[4].c_str(), value);
    alphaR_ppiGmass_pKRefGMC.setConstant();
    RooDoubleGaussExp pdf_ppiGmass_pKRefGMC(name_list.comppdf[4].c_str(), name_list.comppdf[4].c_str(), b_masses[0], mean_ppiGmass_pKRefGMC, sigma_ppiGmass_pKRefGMC, alphaL_ppiGmass_pKRefGMC, alphaR_ppiGmass_pKRefGMC);

    RooRealVar tau_ppiGmass_exp(name_list.exp_par[7].c_str(), name_list.exp_par[7].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
    RooExponential pdf_ppiGmass_exp(name_list.comppdf[7].c_str(), name_list.comppdf[7].c_str(), b_masses[0], tau_ppiGmass_exp);

    //We are going to define the yield variables in ppiG mass, and the others will be a function of these (using the efficiencies).
    //We also define the yields for the reflection pdfs, which are dependent
    double error; //Another dummy value to go step by step. This one will contain the error in the efficiency
    value = stod(GetValueFor("Global", "output/PreCutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_2hG.txt"));
    value = value * stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt"));
    value = value * stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "output/PreCutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/PreCutEff_KstG_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_KstG_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/KstG_Ref_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDEff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/KstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_ppiG_mass_KpiGRefMC("R_ppiG_mass_KpiGRefMC", "R_ppiG_mass_KpiGRefMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_ppiG_mass_KpiGRefMC("RG_ppiG_mass_KpiGRefMC", "RG_ppiG_mass_KpiGRefMC", R_ppiG_mass_KpiGRefMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Global", "output/PreCutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt"));
    value = value * stod(GetValueFor("Global", "output/CutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt"));
    value = value * stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "output/CutEff_pKG_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/PreCutEff_pKG_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_pKG_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_Ref_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDEff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_ppiG_mass_pKGRefMC("R_ppiG_mass_pKGRefMC", "R_ppiG_mass_pKGRefMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_ppiG_mass_pKGRefMC("RG_ppiG_mass_pKGRefMC", "RG_ppiG_mass_pKGRefMC", R_ppiG_mass_pKGRefMC, RooFit::RooConst(value), RooFit::RooConst(error));

    RooRealVar f_ppiG_mass_KpiGMC(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), 6, 0., double(entries[0]));
    RooRealVar f_ppiG_mass_pKGMC(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), 13, 0., double(entries[0]));
    RooRealVar f_ppiG_mass_ppiGMC(name_list.fcomp[2].c_str(), name_list.fcomp[2].c_str(), 30, 0., double(entries[0]));
    RooFormulaVar f_ppiG_mass_KpiGRefMC(name_list.fcomp[3].c_str(), name_list.fcomp[3].c_str(), "@0*@1", RooArgList(f_ppiG_mass_KpiGMC, R_ppiG_mass_KpiGRefMC));
    RooFormulaVar f_ppiG_mass_pKGRefMC(name_list.fcomp[4].c_str(), name_list.fcomp[4].c_str(), "@0*@1", RooArgList(f_ppiG_mass_pKGMC, R_ppiG_mass_pKGRefMC));
    RooRealVar f_ppiG_mass_exp(name_list.fcomp[7].c_str(), name_list.fcomp[7].c_str(), double(entries[0]) / 2., 0., double(entries[0]));
    model.push_back(RooAddPdf(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(),
                              RooArgList(pdf_ppiGmass_KpiGMC, pdf_ppiGmass_pKGMC, pdf_ppiGmass_ppiGMC, pdf_ppiGmass_KpiRefGMC, pdf_ppiGmass_pKRefGMC, pdf_ppiGmass_exp),
                              RooArgList(f_ppiG_mass_KpiGMC, f_ppiG_mass_pKGMC, f_ppiG_mass_ppiGMC, f_ppiG_mass_KpiGRefMC, f_ppiG_mass_pKGRefMC, f_ppiG_mass_exp)));

    //KpiG mass
    //Define all components and then add
    Info("AnalFits.cxx", "Starting to define KpiG mass pdf");
    value = ws_KpiG_mass[0]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_KpiGmass_KpiGMC(name_list.mean[8].c_str(), name_list.mean[8].c_str(), value, const_list.xmin, const_list.xmax); //mean_KpiGmass_KpiGMC.setConstant();
    value = ws_KpiG_mass[0]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_KpiGmass_KpiGMC(name_list.width[8].c_str(), name_list.width[8].c_str(), value, const_list.width_min, const_list.width_max); //sigma_KpiGmass_KpiGMC.setConstant();
    value = ws_KpiG_mass[0]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_KpiGmass_KpiGMC(name_list.alphaL[8].c_str(), name_list.alphaL[8].c_str(), value);
    alphaL_KpiGmass_KpiGMC.setConstant();
    value = ws_KpiG_mass[0]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_KpiGmass_KpiGMC(name_list.alphaR[8].c_str(), name_list.alphaR[8].c_str(), value);
    alphaR_KpiGmass_KpiGMC.setConstant();
    value = ws_KpiG_mass[0]->var(name_list.n.c_str())->getValV();
    RooRealVar n_KpiGmass_KpiGMC(name_list.nL[8].c_str(), name_list.nL[8].c_str(), value);
    n_KpiGmass_KpiGMC.setConstant();
    RooCBExp pdf_KpiGmass_KpiGMC(name_list.comppdf[8].c_str(), name_list.comppdf[8].c_str(), b_masses[1], mean_KpiGmass_KpiGMC, sigma_KpiGmass_KpiGMC, alphaL_KpiGmass_KpiGMC, n_KpiGmass_KpiGMC, alphaR_KpiGmass_KpiGMC);

    value = ws_KpiG_mass[1]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_KpiGmass_pKGMC(name_list.mean[9].c_str(), name_list.mean[9].c_str(), value);
    mean_KpiGmass_pKGMC.setConstant();
    value = ws_KpiG_mass[1]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_KpiGmass_pKGMC(name_list.width[9].c_str(), name_list.width[9].c_str(), value);
    sigma_KpiGmass_pKGMC.setConstant();
    value = ws_KpiG_mass[1]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_KpiGmass_pKGMC(name_list.alphaL[9].c_str(), name_list.alphaL[9].c_str(), value);
    alphaL_KpiGmass_pKGMC.setConstant();
    value = ws_KpiG_mass[1]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_KpiGmass_pKGMC(name_list.alphaR[9].c_str(), name_list.alphaR[9].c_str(), value);
    alphaR_KpiGmass_pKGMC.setConstant();
    RooDoubleGaussExp pdf_KpiGmass_pKGMC(name_list.comppdf[9].c_str(), name_list.comppdf[9].c_str(), b_masses[1], mean_KpiGmass_pKGMC, sigma_KpiGmass_pKGMC, alphaL_KpiGmass_pKGMC, alphaR_KpiGmass_pKGMC);

    value = ws_KpiG_mass[2]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_KpiGmass_ppiGMC(name_list.mean[10].c_str(), name_list.mean[10].c_str(), value);
    mean_KpiGmass_ppiGMC.setConstant();
    value = ws_KpiG_mass[2]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_KpiGmass_ppiGMC(name_list.width[10].c_str(), name_list.width[10].c_str(), value);
    sigma_KpiGmass_ppiGMC.setConstant();
    value = ws_KpiG_mass[2]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_KpiGmass_ppiGMC(name_list.alphaL[10].c_str(), name_list.alphaL[10].c_str(), value);
    alphaL_KpiGmass_ppiGMC.setConstant();
    value = ws_KpiG_mass[2]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_KpiGmass_ppiGMC(name_list.alphaR[10].c_str(), name_list.alphaR[10].c_str(), value);
    alphaR_KpiGmass_ppiGMC.setConstant();
    RooDoubleGaussExp pdf_KpiGmass_ppiGMC(name_list.comppdf[10].c_str(), name_list.comppdf[10].c_str(), b_masses[1], mean_KpiGmass_ppiGMC, sigma_KpiGmass_ppiGMC, alphaL_KpiGmass_ppiGMC, alphaR_KpiGmass_ppiGMC);

    value = ws_KpiG_mass[3]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_KpiGmass_KpiGRefMC(name_list.mean[11].c_str(), name_list.mean[11].c_str(), value);
    mean_KpiGmass_KpiGRefMC.setConstant();
    value = ws_KpiG_mass[3]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_KpiGmass_KpiGRefMC(name_list.width[11].c_str(), name_list.width[11].c_str(), value);
    sigma_KpiGmass_KpiGRefMC.setConstant();
    value = ws_KpiG_mass[3]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_KpiGmass_KpiGRefMC(name_list.alphaL[11].c_str(), name_list.alphaL[11].c_str(), value);
    alphaL_KpiGmass_KpiGRefMC.setConstant();
    value = ws_KpiG_mass[3]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_KpiGmass_KpiGRefMC(name_list.alphaR[11].c_str(), name_list.alphaR[11].c_str(), value);
    alphaR_KpiGmass_KpiGRefMC.setConstant();
    value = ws_KpiG_mass[3]->var(name_list.n.c_str())->getValV();
    RooRealVar n_KpiGmass_KpiGRefMC(name_list.nL[11].c_str(), name_list.nL[11].c_str(), value);
    n_KpiGmass_KpiGRefMC.setConstant();
    RooCBExp pdf_KpiGmass_KpiGRefMC(name_list.comppdf[11].c_str(), name_list.comppdf[11].c_str(), b_masses[1], mean_KpiGmass_KpiGRefMC, sigma_KpiGmass_KpiGRefMC, alphaL_KpiGmass_KpiGRefMC, n_KpiGmass_KpiGRefMC, alphaR_KpiGmass_KpiGRefMC);

    value = ws_KpiG_mass[4]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_KpiGmass_pKGRefMC(name_list.mean[12].c_str(), name_list.mean[12].c_str(), value);
    mean_KpiGmass_pKGRefMC.setConstant();
    value = ws_KpiG_mass[4]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_KpiGmass_pKGRefMC(name_list.width[12].c_str(), name_list.width[12].c_str(), value);
    sigma_KpiGmass_pKGRefMC.setConstant();
    value = ws_KpiG_mass[4]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_KpiGmass_pKGRefMC(name_list.alphaL[12].c_str(), name_list.alphaL[12].c_str(), value);
    alphaL_KpiGmass_pKGRefMC.setConstant();
    value = ws_KpiG_mass[4]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_KpiGmass_pKGRefMC(name_list.alphaR[12].c_str(), name_list.alphaR[12].c_str(), value);
    alphaR_KpiGmass_pKGRefMC.setConstant();
    value = ws_KpiG_mass[4]->var(name_list.n.c_str())->getValV();
    RooRealVar n_KpiGmass_pKGRefMC(name_list.nL[12].c_str(), name_list.nL[12].c_str(), value);
    n_KpiGmass_pKGRefMC.setConstant();
    RooCBExp pdf_KpiGmass_pKGRefMC(name_list.comppdf[12].c_str(), name_list.comppdf[12].c_str(), b_masses[1], mean_KpiGmass_pKGRefMC, sigma_KpiGmass_pKGRefMC, alphaL_KpiGmass_pKGRefMC, n_KpiGmass_pKGRefMC, alphaR_KpiGmass_pKGRefMC);

    value = ws_KpiG_mass[5]->var(name_list.c_Argus.c_str())->getValV();
    RooRealVar c_Argus_KpiGmass_KpipiGMC(name_list.c_Argus.c_str(), name_list.c_Argus.c_str(), value);
    c_Argus_KpiGmass_KpipiGMC.setConstant();
    value = ws_KpiG_mass[5]->var(name_list.p_Argus.c_str())->getValV();
    RooRealVar p_Argus_KpiGmass_KpipiGMC(name_list.p_Argus.c_str(), name_list.p_Argus.c_str(), value);
    p_Argus_KpiGmass_KpipiGMC.setConstant();
    value = ws_KpiG_mass[5]->var(name_list.m0_Argus.c_str())->getValV();
    RooRealVar m0_Argus_KpiGmass_KpipiGMC(name_list.m0_Argus.c_str(), name_list.m0_Argus.c_str(), value);
    m0_Argus_KpiGmass_KpipiGMC.setConstant();
    value = ws_KpiG_mass[5]->var(name_list.width_Argus.c_str())->getValV();
    RooRealVar width_Argus_KpiGmass_KpipiGMC(name_list.width_Argus.c_str(), name_list.width_Argus.c_str(), value);
    width_Argus_KpiGmass_KpipiGMC.setConstant();
    RooArgusGauss pdf_KpiGmass_KpipiGMC(name_list.comppdf[13].c_str(), name_list.comppdf[13].c_str(), b_masses[1], m0_Argus_KpiGmass_KpipiGMC, c_Argus_KpiGmass_KpipiGMC, p_Argus_KpiGmass_KpipiGMC, width_Argus_KpiGmass_KpipiGMC);

    RooRealVar tau_KpiGmass_exp(name_list.exp_par[14].c_str(), name_list.exp_par[14].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
    RooExponential pdf_KpiGmass_exp(name_list.comppdf[14].c_str(), name_list.comppdf[14].c_str(), b_masses[1], tau_KpiGmass_exp);

    RooRealVar slope_KpiGmass_line(name_list.slope.c_str(), name_list.slope.c_str(), const_list.slope_0, const_list.slope_min, const_list.slope_max);
    RooPolynomial pdf_KpiGmass_line(name_list.comppdf[15].c_str(), name_list.comppdf[15].c_str(), b_masses[1], slope_KpiGmass_line);

    //Yields in KpiG, depend on ppiG yields. This dependency takes into account the uncertainty on the PID efficiency
    value = stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "Systematics/PID/KstG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDInv1Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/KstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_KpiG_mass_KpiGMC("R_KpiG_mass_KpiGMC", "R_KpiG_mass_KpiGMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_KpiG_mass_KpiGMC("RG_KpiG_mass_KpiGMC", "RG_KpiG_mass_KpiGMC", R_KpiG_mass_KpiGMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv1Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_KpiG_mass_pKGMC("R_KpiG_mass_pKGMC", "R_KpiG_mass_pKGMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_KpiG_mass_pKGMC("RG_KpiG_mass_pKGMC", "RG_KpiG_mass_pKGMC", R_KpiG_mass_pKGMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "Systematics/PID/NstG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv1Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/NstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_KpiG_mass_ppiGMC("R_KpiG_mass_ppiGMC", "R_KpiG_mass_ppiGMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_KpiG_mass_ppiGMC("RG_KpiG_mass_ppiGMC", "RG_KpiG_mass_ppiGMC", R_KpiG_mass_ppiGMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Global", "output/PreCutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_2hG.txt"));
    value = value * stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt"));
    value = value * stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "output/PreCutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/PreCutEff_KstG_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_KstG_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/KstG_Ref_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDInv1Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/KstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_KpiG_mass_KpiGRefMC("R_KpiG_mass_KpiGRefMC", "R_KpiG_mass_KpiGRefMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_KpiG_mass_KpiGRefMC("RG_KpiG_mass_KpiGRefMC", "RG_KpiG_mass_KpiGRefMC", R_KpiG_mass_KpiGRefMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Global", "output/PreCutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt"));
    value = value * stod(GetValueFor("Global", "output/CutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt"));
    value = value * stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "output/PreCutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_pKG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/PreCutEff_pKG_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_pKG_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_Ref_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDInv1Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_KpiG_mass_pKGRefMC("R_KpiG_mass_pKGRefMC", "R_KpiG_mass_pKGRefMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_KpiG_mass_pKGRefMC("RG_KpiG_mass_pKGRefMC", "RG_KpiG_mass_pKGRefMC", R_KpiG_mass_pKGRefMC, RooFit::RooConst(value), RooFit::RooConst(error));

    RooFormulaVar f_KpiG_mass_KpiGMC(name_list.fcomp[8].c_str(), name_list.fcomp[8].c_str(), "@0*@1", RooArgList(f_ppiG_mass_KpiGMC, R_KpiG_mass_KpiGMC));
    RooFormulaVar f_KpiG_mass_pKGMC(name_list.fcomp[9].c_str(), name_list.fcomp[9].c_str(), "@0*@1", RooArgList(f_ppiG_mass_pKGMC, R_KpiG_mass_pKGMC));
    RooFormulaVar f_KpiG_mass_ppiGMC(name_list.fcomp[10].c_str(), name_list.fcomp[10].c_str(), "@0*@1", RooArgList(f_ppiG_mass_ppiGMC, R_KpiG_mass_ppiGMC));
    RooFormulaVar f_KpiG_mass_KpiGRefMC(name_list.fcomp[11].c_str(), name_list.fcomp[11].c_str(), "@0*@1", RooArgList(f_ppiG_mass_KpiGMC, R_KpiG_mass_KpiGRefMC));
    RooFormulaVar f_KpiG_mass_pKGRefMC(name_list.fcomp[12].c_str(), name_list.fcomp[12].c_str(), "@0*@1", RooArgList(f_ppiG_mass_pKGMC, R_KpiG_mass_pKGRefMC));
    RooRealVar f_KpiG_mass_KpipiGMC(name_list.fcomp[13].c_str(), name_list.fcomp[13].c_str(), double(entries[1]) / 4., 0., double(entries[1]));
    RooRealVar f_KpiG_mass_exp(name_list.fcomp[14].c_str(), name_list.fcomp[14].c_str(), double(entries[1]) / 2., 0., double(entries[1]));
    RooRealVar f_KpiG_mass_line(name_list.fcomp[15].c_str(), name_list.fcomp[15].c_str(), double(entries[1]) / 4., 0., double(entries[1]));
    model.push_back(RooAddPdf(name_list.pdfmodel[1].c_str(), name_list.pdfmodel[1].c_str(),
                              RooArgList(pdf_KpiGmass_KpiGMC, pdf_KpiGmass_pKGMC, pdf_KpiGmass_ppiGMC, pdf_KpiGmass_KpiGRefMC, pdf_KpiGmass_pKGRefMC, pdf_KpiGmass_KpipiGMC, pdf_KpiGmass_line, pdf_KpiGmass_exp),
                              RooArgList(f_KpiG_mass_KpiGMC, f_KpiG_mass_pKGMC, f_KpiG_mass_ppiGMC, f_KpiG_mass_KpiGRefMC, f_KpiG_mass_pKGRefMC, f_KpiG_mass_KpipiGMC, f_KpiG_mass_line, f_KpiG_mass_exp)));

    //pKG mass
    //Define all components and then add
    Info("AnalFits.cxx", "Starting to define pKG mass pdf");
    value = ws_pKG_mass[0]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_pKGmass_KpiGMC(name_list.mean[16].c_str(), name_list.mean[16].c_str(), value);
    mean_pKGmass_KpiGMC.setConstant();
    value = ws_pKG_mass[0]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_pKGmass_KpiGMC(name_list.width[16].c_str(), name_list.width[16].c_str(), value);
    sigma_pKGmass_KpiGMC.setConstant();
    value = ws_pKG_mass[0]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_pKGmass_KpiGMC(name_list.alphaL[16].c_str(), name_list.alphaL[16].c_str(), value);
    alphaL_pKGmass_KpiGMC.setConstant();
    value = ws_pKG_mass[0]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_pKGmass_KpiGMC(name_list.alphaR[16].c_str(), name_list.alphaR[16].c_str(), value);
    alphaR_pKGmass_KpiGMC.setConstant();
    value = ws_pKG_mass[0]->var(name_list.n.c_str())->getValV();
    RooRealVar n_pKGmass_KpiGMC(name_list.nL[16].c_str(), name_list.nL[16].c_str(), value);
    n_pKGmass_KpiGMC.setConstant();
    RooCBExp pdf_pKGmass_KpiGMC(name_list.comppdf[16].c_str(), name_list.comppdf[16].c_str(), b_masses[2], mean_pKGmass_KpiGMC, sigma_pKGmass_KpiGMC, alphaL_pKGmass_KpiGMC, n_pKGmass_KpiGMC, alphaR_pKGmass_KpiGMC);

    value = ws_pKG_mass[1]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_pKGmass_pKGMC(name_list.mean[17].c_str(), name_list.mean[17].c_str(), value, const_list.xmin, const_list.xmax); //mean_pKGmass_pKGMC.setConstant();
    value = ws_pKG_mass[1]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_pKGmass_pKGMC(name_list.alphaL[17].c_str(), name_list.alphaL[17].c_str(), value);
    alphaL_pKGmass_pKGMC.setConstant();
    value = ws_pKG_mass[1]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_pKGmass_pKGMC(name_list.alphaR[17].c_str(), name_list.alphaR[17].c_str(), value);
    alphaR_pKGmass_pKGMC.setConstant();
    value = ws_pKG_mass[1]->var(name_list.n.c_str())->getValV();
    RooRealVar n_pKGmass_pKGMC(name_list.nL[17].c_str(), name_list.nL[17].c_str(), value);
    n_pKGmass_pKGMC.setConstant();
    RooCBExp pdf_pKGmass_pKGMC(name_list.comppdf[17].c_str(), name_list.comppdf[17].c_str(), b_masses[2], mean_pKGmass_pKGMC, sigma_ppiGmass_ppiGMC, alphaL_pKGmass_pKGMC, n_pKGmass_pKGMC, alphaR_pKGmass_pKGMC);

    value = ws_pKG_mass[2]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_pKGmass_ppiGMC(name_list.mean[18].c_str(), name_list.mean[18].c_str(), value);
    mean_pKGmass_ppiGMC.setConstant();
    value = ws_pKG_mass[2]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_pKGmass_ppiGMC(name_list.width[18].c_str(), name_list.width[18].c_str(), value);
    sigma_pKGmass_ppiGMC.setConstant();
    value = ws_pKG_mass[2]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_pKGmass_ppiGMC(name_list.alphaL[18].c_str(), name_list.alphaL[18].c_str(), value);
    alphaL_pKGmass_ppiGMC.setConstant();
    value = ws_pKG_mass[2]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_pKGmass_ppiGMC(name_list.alphaR[18].c_str(), name_list.alphaR[18].c_str(), value);
    alphaR_pKGmass_ppiGMC.setConstant();
    value = ws_pKG_mass[2]->var(name_list.n.c_str())->getValV();
    RooRealVar n_pKGmass_ppiGMC(name_list.nL[18].c_str(), name_list.nL[18].c_str(), value);
    n_pKGmass_ppiGMC.setConstant();
    RooCBExp pdf_pKGmass_ppiGMC(name_list.comppdf[18].c_str(), name_list.comppdf[18].c_str(), b_masses[2], mean_pKGmass_ppiGMC, sigma_pKGmass_ppiGMC, alphaL_pKGmass_ppiGMC, n_pKGmass_ppiGMC, alphaR_pKGmass_ppiGMC);

    value = ws_pKG_mass[3]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_pKGmass_KpiGRefMC(name_list.mean[19].c_str(), name_list.mean[19].c_str(), value);
    mean_pKGmass_KpiGRefMC.setConstant();
    value = ws_pKG_mass[3]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_pKGmass_KpiGRefMC(name_list.width[19].c_str(), name_list.width[19].c_str(), value);
    sigma_pKGmass_KpiGRefMC.setConstant();
    value = ws_pKG_mass[3]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_pKGmass_KpiGRefMC(name_list.alphaL[19].c_str(), name_list.alphaL[19].c_str(), value);
    alphaL_pKGmass_KpiGRefMC.setConstant();
    value = ws_pKG_mass[3]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_pKGmass_KpiGRefMC(name_list.alphaR[19].c_str(), name_list.alphaR[19].c_str(), value);
    alphaR_pKGmass_KpiGRefMC.setConstant();
    RooDoubleGaussExp pdf_pKGmass_KpiGRefMC(name_list.comppdf[19].c_str(), name_list.comppdf[19].c_str(), b_masses[2], mean_pKGmass_KpiGRefMC, sigma_pKGmass_KpiGRefMC, alphaL_pKGmass_KpiGRefMC, alphaR_pKGmass_KpiGRefMC);

    value = ws_pKG_mass[4]->var(name_list.mean[0].c_str())->getValV();
    RooRealVar mean_pKGmass_pKGRefMC(name_list.mean[20].c_str(), name_list.mean[20].c_str(), value);
    mean_pKGmass_pKGRefMC.setConstant();
    value = ws_pKG_mass[4]->var(name_list.width[0].c_str())->getValV();
    RooRealVar sigma_pKGmass_pKGRefMC(name_list.width[20].c_str(), name_list.width[20].c_str(), value);
    sigma_pKGmass_pKGRefMC.setConstant();
    value = ws_pKG_mass[4]->var(name_list.alphaL[0].c_str())->getValV();
    RooRealVar alphaL_pKGmass_pKGRefMC(name_list.alphaL[20].c_str(), name_list.alphaL[20].c_str(), value);
    alphaL_pKGmass_pKGRefMC.setConstant();
    value = ws_pKG_mass[4]->var(name_list.alphaR[0].c_str())->getValV();
    RooRealVar alphaR_pKGmass_pKGRefMC(name_list.alphaR[20].c_str(), name_list.alphaR[20].c_str(), value);
    alphaR_pKGmass_pKGRefMC.setConstant();
    RooDoubleGaussExp pdf_pKGmass_pKGRefMC(name_list.comppdf[20].c_str(), name_list.comppdf[20].c_str(), b_masses[2], mean_pKGmass_pKGRefMC, sigma_pKGmass_pKGRefMC, alphaL_pKGmass_pKGRefMC, alphaR_pKGmass_pKGRefMC);

    RooRealVar tau_pKGmass_exp(name_list.exp_par[23].c_str(), name_list.exp_par[23].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
    RooExponential pdf_pKGmass_exp(name_list.comppdf[23].c_str(), name_list.comppdf[23].c_str(), b_masses[2], tau_pKGmass_exp);

    //Yields in pKG, depend on ppiG yields. This dependency takes into account the uncertainty on the PID efficiency
    value = stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "Systematics/PID/KstG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDInv2Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/KstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_pKG_mass_KpiGMC("R_pKG_mass_KpiGMC", "R_pKG_mass_KpiGMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_pKG_mass_KpiGMC("RG_pKG_mass_KpiGMC", "RG_pKG_mass_KpiGMC", R_pKG_mass_KpiGMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv2Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_pKG_mass_pKGMC("R_pKG_mass_pKGMC", "R_pKG_mass_pKGMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_pKG_mass_pKGMC("RG_pKG_mass_pKGMC", "RG_pKG_mass_pKGMC", R_pKG_mass_pKGMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "Systematics/PID/NstG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv2Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/NstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_pKG_mass_ppiGMC("R_pKG_mass_ppiGMC", "R_pKG_mass_ppiGMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_pKG_mass_ppiGMC("RG_pKG_mass_ppiGMC", "RG_pKG_mass_ppiGMC", R_pKG_mass_ppiGMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Global", "output/PreCutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_2hG.txt"));
    value = value * stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt"));
    value = value * stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "output/PreCutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/PreCutEff_KstG_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_KstG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_KstG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_KstG_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/KstG_Ref_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDInv2Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/KstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_pKG_mass_KpiGRefMC("R_pKG_mass_KpiGRefMC", "R_pKG_mass_KpiGRefMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_pKG_mass_KpiGRefMC("RG_pKG_mass_KpiGRefMC", "RG_pKG_mass_KpiGRefMC", R_pKG_mass_KpiGRefMC, RooFit::RooConst(value), RooFit::RooConst(error));

    value = stod(GetValueFor("Global", "output/PreCutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt"));
    value = value * stod(GetValueFor("Global", "output/CutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt"));
    value = value * stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
    error = pow(stod(GetValueFor("Error", "output/PreCutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_pKG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/PreCutEff_pKG_2hG.txt")) / stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_pKG_Ref_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_Ref_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "output/CutEff_pKG_2hG.txt")) / stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_Ref_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDInv2Eff.txt")), 2.);
    error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt")), 2.);
    error = sqrt(error) * value;
    RooRealVar R_pKG_mass_pKGRefMC("R_pKG_mass_pKGRefMC", "R_pKG_mass_pKGRefMC", value, TMath::Max(value - 5 * error, 0.), value + 5 * error);
    RooGaussian RG_pKG_mass_pKGRefMC("RG_pKG_mass_pKGRefMC", "RG_pKG_mass_pKGRefMC", R_pKG_mass_pKGRefMC, RooFit::RooConst(value), RooFit::RooConst(error));

    RooFormulaVar f_pKG_mass_KpiGMC(name_list.fcomp[16].c_str(), name_list.fcomp[16].c_str(), "@0*@1", RooArgList(f_ppiG_mass_KpiGMC, R_pKG_mass_KpiGMC));
    RooFormulaVar f_pKG_mass_pKGMC(name_list.fcomp[17].c_str(), name_list.fcomp[17].c_str(), "@0*@1", RooArgList(f_ppiG_mass_pKGMC, R_pKG_mass_pKGMC));
    RooFormulaVar f_pKG_mass_ppiGMC(name_list.fcomp[18].c_str(), name_list.fcomp[18].c_str(), "@0*@1", RooArgList(f_ppiG_mass_ppiGMC, R_pKG_mass_ppiGMC));
    RooFormulaVar f_pKG_mass_KpiGRefMC(name_list.fcomp[19].c_str(), name_list.fcomp[19].c_str(), "@0*@1", RooArgList(f_ppiG_mass_KpiGMC, R_pKG_mass_KpiGRefMC));
    RooFormulaVar f_pKG_mass_pKGRefMC(name_list.fcomp[20].c_str(), name_list.fcomp[20].c_str(), "@0*@1", RooArgList(f_ppiG_mass_pKGMC, R_pKG_mass_pKGRefMC));
    RooRealVar f_pKG_mass_exp(name_list.fcomp[23].c_str(), name_list.fcomp[23].c_str(), double(entries[2]) / 2., 0., double(entries[2]));
    model.push_back(RooAddPdf(name_list.pdfmodel[2].c_str(), name_list.pdfmodel[2].c_str(),
                              RooArgList(pdf_pKGmass_KpiGMC, pdf_pKGmass_pKGMC, pdf_pKGmass_ppiGMC, pdf_pKGmass_KpiGRefMC, pdf_pKGmass_pKGRefMC, pdf_pKGmass_exp),
                              RooArgList(f_pKG_mass_KpiGMC, f_pKG_mass_pKGMC, f_pKG_mass_ppiGMC, f_pKG_mass_KpiGRefMC, f_pKG_mass_pKGRefMC, f_pKG_mass_exp)));

    //Add them to the simultaneous fit
    for (int i = 0; i < 3; i++)
    {
        simpdf.addPdf(model[i], variablename[i].c_str());
    }

    //Time to fit
    Info("AnalFits.cxx", "Starting to fit");
    RooArgSet gaussconstraints;
    gaussconstraints.add(RG_ppiG_mass_KpiGRefMC);
    gaussconstraints.add(RG_ppiG_mass_pKGRefMC);
    gaussconstraints.add(RG_KpiG_mass_KpiGMC);
    gaussconstraints.add(RG_KpiG_mass_pKGMC);
    gaussconstraints.add(RG_KpiG_mass_ppiGMC);
    gaussconstraints.add(RG_KpiG_mass_KpiGRefMC);
    gaussconstraints.add(RG_KpiG_mass_pKGRefMC);
    gaussconstraints.add(RG_pKG_mass_KpiGMC);
    gaussconstraints.add(RG_pKG_mass_pKGMC);
    gaussconstraints.add(RG_pKG_mass_ppiGMC);
    gaussconstraints.add(RG_pKG_mass_KpiGRefMC);
    gaussconstraints.add(RG_pKG_mass_pKGRefMC);
    auto results = simpdf.fitTo(data, RooFit::ExternalConstraints(gaussconstraints), RooFit::Save());
    //Import to workspace the pdfs and the dataset
    ws->import(simpdf);
    ws->import(data);

    //Save yields in a file
    SaveRooVars(ws, const_list.workingdir + "NstG_MultiPlot_RooYields.txt", results);
    return ws;
}