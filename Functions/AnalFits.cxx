#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooWorkspace.h"
#include "RooFormulaVar.h"
#include "RooAbsReal.h"
#include "RooPolynomial.h"
#include "RooCBShape.h"
#include "RooExponential.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"
#include "TFile.h"
#include "TError.h"
#include "Functions/AnalFits.h"
#include "Functions/BifurcatedCB.h"
#include "Functions/Dictreading.h"
#include "Functions/Filereading.h"
#include "Functions/Filesaving.h"
#include "Functions/PlotTools.h"
#include "Functions/RooCBExp.h"
#include "Functions/Roo2Exp.h"
#include "Functions/RooGaussExp.h"
#include "Functions/RooDoubleGaussExp.h"
#include "Functions/RooArgusGauss.h"
#include "Functions/TreeTools.h"
#include "Dictionaries/Constants.h"
#include "Dictionaries/Names.h"

using namespace std;
string *Create_Parlist_NstG(FitOption fitopt, Names *name_list, int i, int &N)
{
  //Elaborate array with varnames depending on each pdf

  switch (fitopt)
  {
  case ArgusGauss: //ArgusGauss fit
  {
    N = 4;
    string *var_list = new string[N];
    var_list[0] = name_list->m0_Argus;
    var_list[1] = name_list->c_Argus;
    var_list[2] = name_list->p_Argus;
    var_list[3] = name_list->width_Argus;
    return var_list;
  }
  case Exp: //Exponential fit
  {
    N = 1;
    string *var_list = new string[N];
    var_list[0] = name_list->exp_par[i];
    return var_list;
  }
  case Line: //Straight line fit
  {
    N = 1;
    string *var_list = new string[N];
    var_list[0] = name_list->slope;
    return var_list;
  }
  case GaussExp: //Gaussian with one exponential tail
  {
    N = 3;
    string *var_list = new string[N];
    var_list[0] = name_list->mean[i];
    var_list[1] = name_list->width[i];
    var_list[2] = name_list->alphaL[i];
    return var_list;
  }
  case CB: //Gaussian with power-law tail
  {
    N = 4;
    string *var_list = new string[N];
    var_list[0] = name_list->mean[i];
    var_list[1] = name_list->width[i];
    var_list[2] = name_list->alphaL[i];
    var_list[3] = name_list->nL[i];
    return var_list;
  }
  case CBExp: //Gaussian with power-law, exponential tails
  {
    N = 5;
    string *var_list = new string[N];
    var_list[0] = name_list->mean[i];
    var_list[1] = name_list->width[i];
    var_list[2] = name_list->alphaL[i];
    var_list[3] = name_list->nL[i];
    var_list[4] = name_list->alphaR[i];
    return var_list;
  }
  case DoubleGaussExp: //Gaussian with 2 exponential tails
  {
    N = 4;
    string *var_list = new string[N];
    var_list[0] = name_list->mean[i];
    var_list[1] = name_list->width[i];
    var_list[2] = name_list->alphaL[i];
    var_list[3] = name_list->alphaR[i];
    return var_list;
  }
  case DoubleCB: //Gaussian with 2 power-law tails
  {
    N = 6;
    string *var_list = new string[N];
    var_list[0] = name_list->mean[i];
    var_list[1] = name_list->width[i];
    var_list[2] = name_list->alphaL[i];
    var_list[3] = name_list->nL[i];
    var_list[4] = name_list->alphaR[i];
    var_list[5] = name_list->nR[i];
    return var_list;
  }
  default: //Any other thing. Output a disclaimer here
    cout << "Fit option not implemented. Doing nothing" << endl;
    return nullptr;
  }
}

RooAbsPdf *Create_FitPDF_NstG(FitOption fitopt, string pdfname, RooRealVar *B_M, RooAbsReal **param_list)
{
  //Define pdf with corresponding function
  switch (fitopt)
  {
  case ArgusGauss: //ArgusGaussian function
  {
    for (int i = 0; i < 4; i++)
    {
      ((RooRealVar *)param_list[i])->setConstant();
    }
    return (new RooArgusGauss(pdfname.c_str(), pdfname.c_str(), *B_M, *param_list[0], *param_list[1], *param_list[2], *param_list[3]));
  }
  case Line: //Straight line function. FREE!
  {
    for (int i = 0; i < 1; i++)
    {
      ((RooRealVar *)param_list[i])->setConstant();
    }
    return (new RooPolynomial(pdfname.c_str(), pdfname.c_str(), *B_M, RooArgList(*param_list[0])));
  }
  case Exp: //Exponential function
  {
    for (int i = 0; i < 4; i++)
    {
      ((RooRealVar *)param_list[i])->setConstant();
    }
    return (new RooExponential(pdfname.c_str(), pdfname.c_str(), *B_M, *param_list[0]));
  }
  case GaussExp: //Gaussian with one exponential tail
  {
    for (int i = 0; i < 3; i++)
    {
      ((RooRealVar *)param_list[i])->setConstant();
    }
    return (new RooGaussExp(pdfname.c_str(), pdfname.c_str(), *B_M, *param_list[0], *param_list[1], *param_list[2]));
  }
  case CB: //Gaussian with power-law tail
  {
    for (int i = 0; i < 4; i++)
    {
      ((RooRealVar *)param_list[i])->setConstant();
    }
    return (new RooCBShape(pdfname.c_str(), pdfname.c_str(), *B_M, *param_list[0], *param_list[1], *param_list[2], *param_list[3]));
  }
  case CBExp: //Gaussian with power-law, exponential tails
  {
    for (int i = 0; i < 5; i++)
    {
      ((RooRealVar *)param_list[i])->setConstant();
    }
    return (new RooCBExp(pdfname.c_str(), pdfname.c_str(), *B_M, *param_list[0], *param_list[1], *param_list[2], *param_list[3], *param_list[4]));
  }
  case DoubleGaussExp: //Gaussian with 2 exponential tails
  {
    for (int i = 0; i < 4; i++)
    {
      ((RooRealVar *)param_list[i])->setConstant();
    }
    return (new RooDoubleGaussExp(pdfname.c_str(), pdfname.c_str(), *B_M, *param_list[0], *param_list[1], *param_list[2], *param_list[3]));
  }
  case DoubleCB: //Gaussian with 2 power-law tails
  {
    for (int i = 0; i < 6; i++)
    {
      ((RooRealVar *)param_list[i])->setConstant();
    }
    return (new BifurcatedCB(pdfname.c_str(), pdfname.c_str(), *B_M, *param_list[0], *param_list[1], *param_list[2], *param_list[3], *param_list[4], *param_list[5]));
  }
  default: //Otherwise... well
  {
    cout << "This will probably crash the code. HA" << endl;
    return nullptr;
  }
  }
}
RooWorkspace *FitLb2NstG(string variablename, TTree *chain, string opts)
{
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);
  //Initialize fit workspace
  RooWorkspace *ws = new RooWorkspace(name_list.workspace.c_str());
  //Deactivate all unused variables (speeds up the fit)
  Activate(chain, 0, 0);
  chain->SetBranchStatus(variablename.c_str(), 1);
  //Get Entries.
  int entries = chain->GetEntries();
  //Initialize mass varibale
  RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

  //Initialize array of predefined fit functions
  FitFunction *fitf = FitFunction_init();
  //For this fit we have several components:
  //Gaussian for signal
  //CBExp for the KpiG component, the tails are characterized from a fit to MC
  //CBExp for the pKG component, the tails are characterized from a fit to MC
  //Exponential for combinatorial. Directly obtained from data

  /** Therefore, we can start with the fits to MC **/
  //Fit CBExp for KpiG
  stringstream ss;
  TFile *MCprefile = TFile::Open("Tuples/temp0.root");
  TTree *MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_global_weight", "Event_PIDCalibEff*Global_weight", "Tuples/temp0_global.root");
  TFile *MCfile = TFile::Open("Tuples/temp0_global.root");
  TTree *MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl;
  cout << "Starting MC fit number 0" << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws0 = fitf[CBExp](variablename, MCtree, "Event_PIDCalibEff_global_weight", 0, 0, opts + "_MC");
  GoodPlot(MCws0, variablename, "", "", opts + "_MC", "_MC0");
  SaveRooVars(MCws0, "output/" + variablename + "_RooYields_MC0.txt");
  //Close file
  MCprefile->Close();
  MCfile->Close();

  //Fit DoubleGaussExp for pKG
  MCprefile = TFile::Open("Tuples/temp1.root");
  MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_global_weight", "Event_PIDCalibEff*Global_weight", "Tuples/temp1_global.root");
  MCfile = TFile::Open("Tuples/temp1_global.root");
  MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl;
  cout << "Starting MC fit number 1" << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws1 = fitf[DoubleGaussExp](variablename, MCtree, "Event_PIDCalibEff_global_weight", 0, 0, opts + "_MC");
  GoodPlot(MCws1, variablename, "", "", opts + "_MC", "_MC1");
  SaveRooVars(MCws1, "output/" + variablename + "_RooYields_MC1.txt");
  //Close file
  MCprefile->Close();
  MCfile->Close();

  //Fit CBExp for ppiG
  MCprefile = TFile::Open("Tuples/temp4.root");
  MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_global_weight", "Event_PIDCalibEff*Global_weight", "Tuples/temp4_global.root");
  MCfile = TFile::Open("Tuples/temp4_global.root");
  MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl;
  cout << "Starting MC fit number 4" << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws4 = fitf[CBExp](variablename, MCtree, "Event_PIDCalibEff_global_weight", 0, 0, opts + "_MC");
  GoodPlot(MCws4, variablename, "", "", opts + "_MC", "_MC4");
  SaveRooVars(MCws4, "output/" + variablename + "_RooYields_MC4.txt");
  //Close file
  MCfile->Close();
  MCprefile->Close();

  //Save Variables in text files
  /*************************************************/
  //Retrieve the yields from the other fits (saved in text files)
  //K*G
  double yields_KpiG = 0;
  double yields_KpiG_error = 0;
  yields_KpiG += stod(GetColFor("KstG_ppi", "output/Yield_Contamination.txt", 4));
  yields_KpiG += stod(GetColFor("K2stG_ppi", "output/Yield_Contamination.txt", 4));
  yields_KpiG_error = pow(stod(GetColFor("KstG_ppi", "output/Yield_Contamination.txt", 6)), 2.);
  yields_KpiG_error = pow(stod(GetColFor("K2stG_ppi", "output/Yield_Contamination.txt", 6)), 2.);
  yields_KpiG_error = sqrt(yields_KpiG_error);
  //LambdaG. I am commenting the old way of doing it, which was using different LG resonances
  double yields_pKG = 0;
  double yields_pKG_error = 0;
  yields_pKG = stod(GetColFor("pKG_ppi", "output/Yield_Contamination.txt", 4));
  yields_pKG_error = stod(GetColFor("pKG_ppi", "output/Yield_Contamination.txt", 6));
  //yields_pKG += stod(GetColFor("L1520G_ppi", "output/Yield_Contamination.txt", 4));
  //yields_pKG += stod(GetColFor("L1670G_ppi", "output/Yield_Contamination.txt", 4));
  //yields_pKG += stod(GetColFor("L1820G_ppi", "output/Yield_Contamination.txt", 4));
  //yields_pKG += stod(GetColFor("L1830G_ppi", "output/Yield_Contamination.txt", 4));
  //yields_pKG_error += pow(stod(GetColFor("L1520G_ppi", "output/Yield_Contamination.txt", 6)), 2.);
  //yields_pKG_error += pow(stod(GetColFor("L1670G_ppi", "output/Yield_Contamination.txt", 6)), 2.);
  //yields_pKG_error += pow(stod(GetColFor("L1820G_ppi", "output/Yield_Contamination.txt", 6)), 2.);
  //yields_pKG_error += pow(stod(GetColFor("L1830G_ppi", "output/Yield_Contamination.txt", 6)), 2.);
  //yields_pKG_error = sqrt(yields_pKG_error);
  //Once this is done, we can define all the components of the fit
  /*************************************************/
  //We obtain the shift (Difference data and MC) It will be multiplied by a factor to say either right shift, left shift, or no shift.
  double shift1 = abs(stod(GetColFor("#mu_{K#pi#gamma}", "output/B_M012_RooYields_WrongMassFit.txt", 2)) - stod(GetColFor("#mu_{K#pi#gamma}", "output/B_M012_Subst0_K2p_RooYields_MC0_Wrong.txt", 2)));
  double shift2 = abs(stod(GetColFor("#mu_{pK#gamma}", "output/B_M012_Subst01_Kpi2pK_RooYields_WrongMassFit.txt", 2)) - stod(GetColFor("#mu_{pK#gamma}", "output/B_M012_Subst0_K2p_RooYields_MC1_Wrong.txt", 2)));
  //We shall introduce a scale for the width of all the means
  RooRealVar sigma_scale("#sigma_{scale}", "#sigma_{scale}", 1., 1.0, 1.);
  /*************************************************/

  double value;
  //Signal (ppiG)
  //Initialize CBExp for signal
  value = MCws4->var(name_list.mean[0].c_str())->getValV();
  RooRealVar ppiG_mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), value, const_list.mean_min, const_list.mean_max);
  value = stod(GetColFor("#sigma_{pK#gamma}", "output/B_M012_Subst01_Kpi2pK_RooYields.txt", 2)); //Use width from data!!
  RooRealVar ppiG_width(name_list.width[0].c_str(), name_list.width[0].c_str(), value, const_list.width_min, const_list.width_max);
  ppiG_width.setConstant();
  //RooFormulaVar ppiG_width(name_list.width[0].c_str(), name_list.width[0].c_str(), (ss.str()+"*@0").c_str(), RooArgList(sigma_scale));
  value = MCws4->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar ppiG_alphaL(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), value);
  ppiG_alphaL.setConstant();
  value = MCws4->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar ppiG_alphaR(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), value);
  ppiG_alphaR.setConstant();
  value = MCws4->var(name_list.n.c_str())->getValV();
  RooRealVar ppiG_nL(name_list.nL[0].c_str(), name_list.nL[0].c_str(), value);
  ppiG_nL.setConstant();
  RooCBExp ppiG_pdf(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, ppiG_mean, ppiG_width, ppiG_alphaL, ppiG_nL, ppiG_alphaR);
  RooRealVar ppiG_f(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), const_list.fsig_0, 0., double(entries));

  //Initialize Exponential for combinatorial
  RooRealVar comb_tau(name_list.exp_par[1].c_str(), name_list.exp_par[1].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
  RooExponential comb_pdf(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, comb_tau);
  RooRealVar comb_f(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), const_list.fbkg_0, 0., double(entries));

  //Initialize CBExp for KpiG
  value = MCws0->var(name_list.mean[0].c_str())->getValV() + shift1 * (const_list.shift1);
  RooRealVar KpiG_mean(name_list.mean[2].c_str(), name_list.mean[2].c_str(), value);
  KpiG_mean.setConstant();
  value = MCws0->var(name_list.width[0].c_str())->getValV();
  RooRealVar KpiG_width(name_list.width[2].c_str(), name_list.width[2].c_str(), value);
  KpiG_width.setConstant();
  value = MCws0->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar KpiG_alphaL(name_list.alphaL[2].c_str(), name_list.alphaL[2].c_str(), value);
  KpiG_alphaL.setConstant();
  value = MCws0->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar KpiG_alphaR(name_list.alphaR[2].c_str(), name_list.alphaR[2].c_str(), value);
  KpiG_alphaR.setConstant();
  value = MCws0->var(name_list.n.c_str())->getValV();
  RooRealVar KpiG_nL(name_list.nL[2].c_str(), name_list.nL[2].c_str(), value);
  KpiG_nL.setConstant();
  RooCBExp KpiG_pdf(name_list.comppdf[2].c_str(), name_list.comppdf[2].c_str(), B_M, KpiG_mean, KpiG_width, KpiG_alphaL, KpiG_nL, KpiG_alphaR);
  //Set-up gaussian constraint for the expected yield
  RooRealVar KpiG_f(name_list.fcomp[2].c_str(), name_list.fcomp[2].c_str(), yields_KpiG, yields_KpiG - 5. * yields_KpiG_error, yields_KpiG + 5. * yields_KpiG_error);
  RooGaussian KpiG_f_gc((name_list.fcomp[2] + "_gc").c_str(), (name_list.fcomp[2] + "_gc").c_str(), KpiG_f, RooFit::RooConst(yields_KpiG), RooFit::RooConst(yields_KpiG_error));

  //Initialize DoubleGaussExp for pKG
  //ss << MCws1->var(name_list.mean[0].c_str())->getValV();
  //RooFormulaVar pKG_mean(name_list.mean[3].c_str(), name_list.mean[3].c_str(), (ss.str()+"+@0").c_str(), RooArgList(shift));
  //ss.str("");
  value = MCws1->var(name_list.mean[0].c_str())->getValV() + shift2 * (const_list.shift2);
  RooRealVar pKG_mean(name_list.mean[3].c_str(), name_list.mean[3].c_str(), value);
  pKG_mean.setConstant();
  value = MCws1->var(name_list.width[0].c_str())->getValV();
  RooRealVar pKG_width(name_list.width[3].c_str(), name_list.width[3].c_str(), value);
  pKG_width.setConstant();
  value = MCws1->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar pKG_alphaL(name_list.alphaL[3].c_str(), name_list.alphaL[3].c_str(), value);
  pKG_alphaL.setConstant();
  value = MCws1->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar pKG_alphaR(name_list.alphaR[3].c_str(), name_list.alphaR[3].c_str(), value);
  pKG_alphaR.setConstant();
  RooDoubleGaussExp pKG_pdf(name_list.comppdf[3].c_str(), name_list.comppdf[3].c_str(), B_M, pKG_mean, pKG_width, pKG_alphaL, pKG_alphaR);
  //Set-up gaussian constraint for the expected yield
  RooRealVar pKG_f(name_list.fcomp[3].c_str(), name_list.fcomp[3].c_str(), yields_pKG, yields_pKG - 5. * yields_pKG_error, yields_pKG + 5. * yields_pKG_error);
  RooGaussian pKG_f_gc((name_list.fcomp[3] + "_gc").c_str(), (name_list.fcomp[3] + "_gc").c_str(), pKG_f, RooFit::RooConst(yields_pKG), RooFit::RooConst(yields_pKG_error));

  RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(ppiG_pdf, comb_pdf, KpiG_pdf, pKG_pdf), RooArgList(ppiG_f, comb_f, KpiG_f, pKG_f));
  RooDataSet data(name_list.dataset.c_str(), name_list.dataset.c_str(), chain, B_M);
  //Perform the fit
  model.fitTo(data, RooFit::ExternalConstraints(RooArgSet(KpiG_f_gc, pKG_f_gc)));
  //Import to workspace
  ws->import(model);
  ws->import(data);

  //Save yields in a file
  SaveRooVars(ws, "output/" + variablename + "_RooYields" + name_list.RooYield_suffix + ".txt");
  return ws;
}

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
  ws_ppiG_mass.push_back(fitf[CBExp](variablename[0], MCtree[3], "Event_PIDCalibEff_global_weight", 0, 0, opts)); //KpiG MC Reflected
  cout << "pKG MC Reflected" << endl
       << "-------" << endl;
  ws_ppiG_mass.push_back(fitf[CBExp](variablename[0], MCtree[4], "Event_PIDCalibEff_global_weight", 0, 0, opts)); //pKG MC Reflected

  //Let's continue with fits to KpiG mass
  cout << "Starting MC fits to KpiG mass" << endl;
  cout << "-----------------------------" << endl;
  cout << "KpiG MC" << endl
       << "-------" << endl;
  ws_KpiG_mass.push_back(fitf[CBExp](variablename[1], MCtree[0], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts)); //KpiG MC
  cout << "pKG MC" << endl
       << "------" << endl;
  ws_KpiG_mass.push_back(fitf[CBExp](variablename[1], MCtree[1], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts)); //pKG MC
  cout << "ppiG MC" << endl
       << "-------" << endl;
  ws_KpiG_mass.push_back(fitf[CBExp](variablename[1], MCtree[2], "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts)); //ppiG MC
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
  ws_pKG_mass.push_back(fitf[CBExp](variablename[2], MCtree[3], "Event_PIDCalibEff_ppibar_global_weight", 0, 0, opts)); //KpiG MC Reflected
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
  //Get the width from pKG fit
  value = ws_pKG_mass[1]->var(name_list.width[0].c_str())->getValV();
  RooRealVar sigma_ppiGmass_ppiGMC(name_list.width[2].c_str(), name_list.width[2].c_str(), value, const_list.width_min, const_list.width_max);
  sigma_ppiGmass_ppiGMC.setConstant();
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
  value = ws_ppiG_mass[4]->var(name_list.n.c_str())->getValV();
  RooRealVar n_ppiGmass_pKRefGMC(name_list.nL[4].c_str(), name_list.nL[4].c_str(), value);
  n_ppiGmass_pKRefGMC.setConstant();
  RooCBExp pdf_ppiGmass_pKRefGMC(name_list.comppdf[4].c_str(), name_list.comppdf[4].c_str(), b_masses[0], mean_ppiGmass_pKRefGMC, sigma_ppiGmass_pKRefGMC, alphaL_ppiGmass_pKRefGMC, n_ppiGmass_pKRefGMC, alphaR_ppiGmass_pKRefGMC);

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
  RooRealVar R_ppiG_mass_KpiGRefMC("R_ppiG_mass_KpiGRefMC", "R_ppiG_mass_KpiGRefMC", value, value - 5 * error, value + 5 * error);
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
  RooRealVar R_ppiG_mass_pKGRefMC("R_ppiG_mass_pKGRefMC", "R_ppiG_mass_pKGRefMC", value, value - 5 * error, value + 5 * error);
  RooGaussian RG_ppiG_mass_pKGRefMC("RG_ppiG_mass_pKGRefMC", "RG_ppiG_mass_pKGRefMC", R_ppiG_mass_pKGRefMC, RooFit::RooConst(value), RooFit::RooConst(error));

  RooRealVar f_ppiG_mass_KpiGMC(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), 25, 0., double(entries[0]));
  RooRealVar f_ppiG_mass_pKGMC(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), 25, 0., double(entries[0]));
  RooRealVar f_ppiG_mass_ppiGMC(name_list.fcomp[2].c_str(), name_list.fcomp[2].c_str(), 50, 0., double(entries[0]));
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
  value = ws_KpiG_mass[1]->var(name_list.n.c_str())->getValV();
  RooRealVar n_KpiGmass_pKGMC(name_list.nL[9].c_str(), name_list.nL[9].c_str(), value);
  n_KpiGmass_pKGMC.setConstant();
  RooCBExp pdf_KpiGmass_pKGMC(name_list.comppdf[9].c_str(), name_list.comppdf[9].c_str(), b_masses[1], mean_KpiGmass_pKGMC, sigma_KpiGmass_pKGMC, alphaL_KpiGmass_pKGMC, n_KpiGmass_pKGMC, alphaR_KpiGmass_pKGMC);

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
  value = ws_KpiG_mass[2]->var(name_list.n.c_str())->getValV();
  RooRealVar n_KpiGmass_ppiGMC(name_list.nL[10].c_str(), name_list.nL[10].c_str(), value);
  n_KpiGmass_ppiGMC.setConstant();
  RooCBExp pdf_KpiGmass_ppiGMC(name_list.comppdf[10].c_str(), name_list.comppdf[10].c_str(), b_masses[1], mean_KpiGmass_ppiGMC, sigma_KpiGmass_ppiGMC, alphaL_KpiGmass_ppiGMC, n_KpiGmass_ppiGMC, alphaR_KpiGmass_ppiGMC);

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
  RooRealVar R_KpiG_mass_KpiGMC("R_KpiG_mass_KpiGMC", "R_KpiG_mass_KpiGMC", value, value - 5 * error, value + 5 * error);
  RooGaussian RG_KpiG_mass_KpiGMC("RG_KpiG_mass_KpiGMC", "RG_KpiG_mass_KpiGMC", R_KpiG_mass_KpiGMC, RooFit::RooConst(value), RooFit::RooConst(error));

  value = stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
  error = pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv1Eff.txt")), 2.);
  error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt")), 2.);
  error = sqrt(error) * value;
  RooRealVar R_KpiG_mass_pKGMC("R_KpiG_mass_pKGMC", "R_KpiG_mass_pKGMC", value, value - 5 * error, value + 5 * error);
  RooGaussian RG_KpiG_mass_pKGMC("RG_KpiG_mass_pKGMC", "RG_KpiG_mass_pKGMC", R_KpiG_mass_pKGMC, RooFit::RooConst(value), RooFit::RooConst(error));

  value = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt"));
  error = pow(stod(GetValueFor("Error", "Systematics/PID/NstG_PIDInv1Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv1Eff.txt")), 2.);
  error += pow(stod(GetValueFor("Error", "Systematics/PID/NstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt")), 2.);
  error = sqrt(error) * value;
  RooRealVar R_KpiG_mass_ppiGMC("R_KpiG_mass_ppiGMC", "R_KpiG_mass_ppiGMC", value, value - 5 * error, value + 5 * error);
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
  RooRealVar R_KpiG_mass_KpiGRefMC("R_KpiG_mass_KpiGRefMC", "R_KpiG_mass_KpiGRefMC", value, value - 5 * error, value + 5 * error);
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
  RooRealVar R_KpiG_mass_pKGRefMC("R_KpiG_mass_pKGRefMC", "R_KpiG_mass_pKGRefMC", value, value - 5 * error, value + 5 * error);
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
  value = ws_pKG_mass[1]->var(name_list.width[0].c_str())->getValV();
  RooRealVar sigma_pKGmass_pKGMC(name_list.width[17].c_str(), name_list.width[17].c_str(), value, const_list.width_min, const_list.width_max); //sigma_pKGmass_pKGMC.setConstant();
  value = ws_pKG_mass[1]->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar alphaL_pKGmass_pKGMC(name_list.alphaL[17].c_str(), name_list.alphaL[17].c_str(), value);
  alphaL_pKGmass_pKGMC.setConstant();
  value = ws_pKG_mass[1]->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar alphaR_pKGmass_pKGMC(name_list.alphaR[17].c_str(), name_list.alphaR[17].c_str(), value);
  alphaR_pKGmass_pKGMC.setConstant();
  value = ws_pKG_mass[1]->var(name_list.n.c_str())->getValV();
  RooRealVar n_pKGmass_pKGMC(name_list.nL[17].c_str(), name_list.nL[17].c_str(), value);
  n_pKGmass_pKGMC.setConstant();
  RooCBExp pdf_pKGmass_pKGMC(name_list.comppdf[17].c_str(), name_list.comppdf[17].c_str(), b_masses[2], mean_pKGmass_pKGMC, sigma_pKGmass_pKGMC, alphaL_pKGmass_pKGMC, n_pKGmass_pKGMC, alphaR_pKGmass_pKGMC);

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
  value = ws_pKG_mass[3]->var(name_list.n.c_str())->getValV();
  RooRealVar n_pKGmass_KpiGRefMC(name_list.nL[19].c_str(), name_list.nL[19].c_str(), value);
  n_pKGmass_KpiGRefMC.setConstant();
  RooCBExp pdf_pKGmass_KpiGRefMC(name_list.comppdf[19].c_str(), name_list.comppdf[19].c_str(), b_masses[2], mean_pKGmass_KpiGRefMC, sigma_pKGmass_KpiGRefMC, alphaL_pKGmass_KpiGRefMC, n_pKGmass_KpiGRefMC, alphaR_pKGmass_KpiGRefMC);

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
  RooRealVar R_pKG_mass_KpiGMC("R_pKG_mass_KpiGMC", "R_pKG_mass_KpiGMC", value, value - 5 * error, value + 5 * error);
  RooGaussian RG_pKG_mass_KpiGMC("RG_pKG_mass_KpiGMC", "RG_pKG_mass_KpiGMC", R_pKG_mass_KpiGMC, RooFit::RooConst(value), RooFit::RooConst(error));

  value = stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
  error = pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv2Eff.txt")), 2.);
  error += pow(stod(GetValueFor("Error", "Systematics/PID/pKG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt")), 2.);
  error = sqrt(error) * value;
  RooRealVar R_pKG_mass_pKGMC("R_pKG_mass_pKGMC", "R_pKG_mass_pKGMC", value, value - 5 * error, value + 5 * error);
  RooGaussian RG_pKG_mass_pKGMC("RG_pKG_mass_pKGMC", "RG_pKG_mass_pKGMC", R_pKG_mass_pKGMC, RooFit::RooConst(value), RooFit::RooConst(error));

  value = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt"));
  error = pow(stod(GetValueFor("Error", "Systematics/PID/NstG_PIDInv2Eff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv2Eff.txt")), 2.);
  error += pow(stod(GetValueFor("Error", "Systematics/PID/NstG_PIDEff.txt")) / stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt")), 2.);
  error = sqrt(error) * value;
  RooRealVar R_pKG_mass_ppiGMC("R_pKG_mass_ppiGMC", "R_pKG_mass_ppiGMC", value, value - 5 * error, value + 5 * error);
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
  RooRealVar R_pKG_mass_KpiGRefMC("R_pKG_mass_KpiGRefMC", "R_pKG_mass_KpiGRefMC", value, value - 5 * error, value + 5 * error);
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
  RooRealVar R_pKG_mass_pKGRefMC("R_pKG_mass_pKGRefMC", "R_pKG_mass_pKGRefMC", value, value - 5 * error, value + 5 * error);
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
  simpdf.fitTo(data, RooFit::ExternalConstraints(gaussconstraints));
  //Import to workspace the pdfs and the dataset
  ws->import(simpdf);
  ws->import(data);

  //Save yields in a file
  SaveRooVars(ws, const_list.workingdir + "NstG_MultiPlot_RooYields.txt");
  return ws;
}

//This is going to change. All the fits and steps will be done in here
RooWorkspace *FitLb2NstG_Kpi(string variablename, TTree *chain, string opts)
{
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);
  //Initialize fit workspace
  RooWorkspace *ws = new RooWorkspace(name_list.workspace.c_str());
  //Deactivate all unused variables (speeds up the fit)
  Activate(chain, 0, 0);
  chain->SetBranchStatus(variablename.c_str(), 1);
  //Get Entries.
  int entries = chain->GetEntries();
  //Initialize mass varibale
  RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

  //Initialize array of predefined fit functions
  FitFunction *fitf = FitFunction_init();
  //For this fit we have several components:
  //DoubleCB for the KpiG component, the tails are characterized from a fit to MC
  //ArgusGauss for the KpipiG component. All parameters fixed from MC except the yield
  //Exponential for Kpipi0X component. Directly obtained from data
  //Straight line for combinatorial. Directly obtained from data

  /** Therefore, we can start with the fits to MC **/
  //Fit DoubleCB for KpiG
  stringstream ss;
  TFile *MCprefile = TFile::Open("Tuples/temp0.root");
  TTree *MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_pbarpi_global_weight", "Event_PIDCalibEff_pbarpi*Global_weight", "Tuples/temp0_global_pbarpi.root");
  TFile *MCfile = TFile::Open("Tuples/temp0_global_pbarpi.root");
  TTree *MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl
       << "Starting MC fit number 0"
       << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws0 = fitf[DoubleCB](variablename, MCtree, "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts + "_MC");
  GoodPlot(MCws0, variablename, "", "", opts + "_MC", "_MC0");
  SaveRooVars(MCws0, "output/" + variablename + "_RooYields_MC0.txt");
  //Close file
  MCfile->Close();
  MCprefile->Close();

  //Fit ArgusGauss for KpipiG
  MCprefile = TFile::Open("Tuples/temp2.root");
  MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_pbarpi_global_weight", "Event_PIDCalibEff_pbarpi*Global_weight", "Tuples/temp2_global_pbarpi.root");
  MCfile = TFile::Open("Tuples/temp2_global_pbarpi.root");
  MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl
       << "Starting MC fit number 2"
       << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws2 = fitf[ArgusGauss](variablename, MCtree, "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts + "_MC2");
  GoodPlot(MCws2, variablename, "", "", opts + "_MC2", "_MC2");
  SaveRooVars(MCws2, "output/" + variablename + "_RooYields_MC2.txt");
  //Close file
  MCfile->Close();
  MCprefile->Close();
  /*************************************************/

  //Once this is done, we can define all the components of the fit
  double value;
  //Signal (KpiG)
  //Initialize DoubleCB for signal
  RooRealVar KpiG_mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);       //From fit
  RooRealVar KpiG_width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max); //From fit
  value = MCws0->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar KpiG_alphaL(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), value);
  KpiG_alphaL.setConstant();
  value = MCws0->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar KpiG_alphaR(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), value);
  KpiG_alphaR.setConstant();
  value = MCws0->var(name_list.nL[0].c_str())->getValV();
  RooRealVar KpiG_nL(name_list.nL[0].c_str(), name_list.nL[0].c_str(), value);
  KpiG_nL.setConstant();
  value = MCws0->var(name_list.nR[0].c_str())->getValV();
  RooRealVar KpiG_nR(name_list.nR[0].c_str(), name_list.nR[0].c_str(), value);
  KpiG_nR.setConstant();
  BifurcatedCB KpiG_pdf(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, KpiG_mean, KpiG_width, KpiG_alphaL, KpiG_nL, KpiG_alphaR, KpiG_nR);
  RooRealVar KpiG_f(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), 8000., 0., double(entries));
  //Constantize automatically if we set equal limits
  KpiG_mean.setConstant(!(const_list.mean_min - const_list.mean_max));
  KpiG_width.setConstant(!(const_list.width_min - const_list.width_max));

  //Initialize Exponential for Kpipi0
  RooRealVar Kpipi0X_tau(name_list.exp_par[1].c_str(), name_list.exp_par[1].c_str(), -0.0019, -1., -0.0001);
  RooExponential Kpipi0X_pdf(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, Kpipi0X_tau);
  RooRealVar Kpipi0X_f(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), 41000., 0., double(entries));

  //Initialize Line for combinatorial
  RooRealVar comb_slope(name_list.slope.c_str(), name_list.slope.c_str(), const_list.slope_0, const_list.slope_min, const_list.slope_max);
  RooPolynomial comb_pdf(name_list.comppdf[2].c_str(), name_list.comppdf[2].c_str(), B_M, comb_slope);
  RooRealVar comb_f(name_list.fcomp[2].c_str(), name_list.fcomp[2].c_str(), 11500., 0., double(entries));

  //Part reco (KpipiG)
  //Initialize ArgusGauss
  value = MCws2->var(name_list.c_Argus.c_str())->getValV();
  RooRealVar KpipiG_c(name_list.c_Argus.c_str(), name_list.c_Argus.c_str(), value);
  KpipiG_c.setConstant();
  value = MCws2->var(name_list.p_Argus.c_str())->getValV();
  RooRealVar KpipiG_p(name_list.p_Argus.c_str(), name_list.p_Argus.c_str(), value);
  KpipiG_p.setConstant();
  value = MCws2->var(name_list.m0_Argus.c_str())->getValV();
  RooRealVar KpipiG_m0(name_list.m0_Argus.c_str(), name_list.m0_Argus.c_str(), value);
  KpipiG_m0.setConstant();
  value = MCws2->var(name_list.width_Argus.c_str())->getValV();
  RooRealVar KpipiG_sigma(name_list.width_Argus.c_str(), name_list.width_Argus.c_str(), value);
  KpipiG_sigma.setConstant();
  RooArgusGauss KpipiG_pdf(name_list.comppdf[3].c_str(), name_list.comppdf[3].c_str(), B_M, KpipiG_m0, KpipiG_c, KpipiG_p, KpipiG_sigma);
  RooRealVar KpipiG_f(name_list.fcomp[3].c_str(), name_list.fcomp[3].c_str(), 4000., 0., double(entries));

  RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(KpiG_pdf, Kpipi0X_pdf, comb_pdf, KpipiG_pdf), RooArgList(KpiG_f, Kpipi0X_f, comb_f, KpipiG_f));
  RooDataSet data(name_list.dataset.c_str(), name_list.dataset.c_str(), chain, B_M);
  //Perform the fit
  model.fitTo(data /*, RooFit::ExternalConstraints(gc_list)*/);
  //Import to workspace
  ws->import(model);
  ws->import(data);

  //Save yields in a file
  SaveRooVars(ws, "output/" + variablename + "_RooYields.txt");
  return ws;
}

RooWorkspace *FitLb2NstG_pK(string variablename, TTree *chain, string opts)
{
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);
  //Initialize fit workspace
  RooWorkspace *ws = new RooWorkspace(name_list.workspace.c_str());
  //Deactivate all unused variables (speeds up the fit)
  Activate(chain, 0, 0);
  chain->SetBranchStatus(variablename.c_str(), 1);
  //Get Entries.
  int entries = chain->GetEntries();
  //Initialize mass varibale
  RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

  //Initialize array of predefined fit functions
  FitFunction *fitf = FitFunction_init();
  //For this fit we have several components:
  //CBExp (Crystal-ball with exponential tail on the other tail) for the pKG (signal). Tails fixed from MC
  //Exponential for combinatorial. Directly obtained from data

  /** Therefore, we can start with the fits to MC **/
  //Fit CBExp for pKG
  stringstream ss;
  TFile *MCprefile = TFile::Open("Tuples/temp1.root");
  TTree *MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_ppibar_global_weight", "Event_PIDCalibEff_ppibar*Global_weight", "Tuples/temp1_global_ppibar.root");
  TFile *MCfile = TFile::Open("Tuples/temp1_global_ppibar.root");
  TTree *MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl
       << "Starting MC fit number 1"
       << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws1 = fitf[CBExp](variablename, MCtree, "Event_PIDCalibEff_ppibar_global_weight", 0, 0, opts + "_MC");
  GoodPlot(MCws1, variablename, "", "", opts + "_MC", "_MC1");
  SaveRooVars(MCws1, "output/" + variablename + "_RooYields_MC1.txt");
  //Close file
  MCfile->Close();
  MCprefile->Close();
  /*************************************************/

  //Once this is done, we can define all the components of the fit
  double value;
  //Signal (pKG)
  //Initialize CBExp for signal
  RooRealVar pKG_mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);       //From fit
  RooRealVar pKG_width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max); //From fit
  value = MCws1->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar pKG_alphaL(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), value);
  pKG_alphaL.setConstant();
  value = MCws1->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar pKG_alphaR(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), value);
  pKG_alphaR.setConstant();
  value = MCws1->var(name_list.n.c_str())->getValV();
  RooRealVar pKG_nL(name_list.nL[0].c_str(), name_list.nL[0].c_str(), value);
  pKG_nL.setConstant();
  //  value = MCws1->var(name_list.nR[0].c_str())->getValV();
  //  RooRealVar pKG_nR(name_list.nR[0].c_str(), name_list.nR[0].c_str(), value); pKG_nR.setConstant();
  RooCBExp pKG_pdf(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, pKG_mean, pKG_width, pKG_alphaL, pKG_nL, pKG_alphaR);
  RooRealVar pKG_f(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), 2700., 0., double(entries));
  //Constantize automatically if we set equal limits
  pKG_mean.setConstant(!(const_list.mean_min - const_list.mean_max));
  pKG_width.setConstant(!(const_list.width_min - const_list.width_max));

  //Initialize Exponential for combinatorial
  RooRealVar comb_tau(name_list.exp_par[1].c_str(), name_list.exp_par[1].c_str(), -0.0019, -1., -0.0001);
  RooExponential comb_pdf(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, comb_tau);
  RooRealVar comb_f(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), 3900., 0., double(entries));

  RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(pKG_pdf, comb_pdf), RooArgList(pKG_f, comb_f));
  RooDataSet data(name_list.dataset.c_str(), name_list.dataset.c_str(), chain, B_M);
  //Perform the fit
  model.fitTo(data /*, RooFit::ExternalConstraints(gc_list)*/);
  //Import to workspace
  ws->import(model);
  ws->import(data);

  //Save yields in a file
  SaveRooVars(ws, "output/" + variablename + "_RooYields.txt");
  return ws;
}

RooWorkspace *FitLb2ppiJPsi(string variablename, TTree *chain, RooWorkspace *param_ws, string w_var, string *act_vars, int N, string opts, FitOption *fitopt, int Nbkgs)
{
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);
  //Number of extra components
  const int Nextra = 1;
  //Initialize fit workspace
  RooWorkspace *ws = new RooWorkspace(name_list.workspace.c_str());
  //Establish number of bkgs (some may come as extra)
  //Deactivate all unused variables (speeds up the fit)
  Activate(chain, act_vars, N);
  chain->SetBranchStatus(variablename.c_str(), 1);
  //Get Entries
  int entries = chain->GetEntries();
  //Initialize all the stuff needed for the fit
  RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
  //Aray with pdfs and array with each relative contribution
  RooAbsPdf **comp = new RooAbsPdf *[Nbkgs + Nextra];
  RooRealVar **f_comp = new RooRealVar *[Nbkgs + Nextra];
  //Array of parameters. Is actually a 2-D array of pointers to the parameters (first dimension is bkg index and second is parameter index)
  int N_par;
  string *var_list = Create_Parlist_NstG(fitopt[0], &name_list, 1, N_par);

  //Initialize Exponential1 for Comb
  RooRealVar tauComb_bkg(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
  comp[1] = new RooExponential(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, tauComb_bkg);
  f_comp[1] = new RooRealVar(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), const_list.fbkg_0 * double(entries), 0., double(entries));

  //Initialize gaussian for signal
  RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
  mean.setConstant(!(const_list.mean_min - const_list.mean_max));
  RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
  width.setConstant(!(const_list.width_min - const_list.width_max));
  RooRealVar alphaL(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), param_ws->var(var_list[2].c_str())->getValV());
  RooRealVar alphaR(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), param_ws->var(var_list[4].c_str())->getValV());
  RooRealVar nL(name_list.nL[0].c_str(), name_list.nL[0].c_str(), param_ws->var(var_list[3].c_str())->getValV());
  RooRealVar nR(name_list.nR[0].c_str(), name_list.nR[0].c_str(), param_ws->var(var_list[5].c_str())->getValV());
  comp[0] = new BifurcatedCB(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width, alphaL, nL, alphaR, nR);
  f_comp[0] = new RooRealVar(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), double(entries) * const_list.fsig_0, 0., double(entries));
  f_comp[0]->setConstant(!(const_list.fsig_0));

  //Create lists with components for the RooAddPdf
  RooArgList pdf_list;
  RooArgList f_list;
  for (int i = 0; i < Nbkgs + Nextra; i++)
  {
    pdf_list.add(*comp[i]);
    f_list.add(*f_comp[i]);
  }
  //Define the model
  RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), pdf_list, f_list);
  //Deal with weights, if requested
  RooArgSet *args;
  RooRealVar *wvar;
  if (w_var != "")
  {
    wvar = new RooRealVar(w_var.c_str(), w_var.c_str(), 0, 10000);
    args = new RooArgSet(B_M, *wvar);
    chain->SetBranchStatus(w_var.c_str(), 1);
  }
  else
  {
    args = new RooArgSet(B_M);
  }
  RooDataSet data(name_list.dataset.c_str(), name_list.dataset.c_str(), chain, *args, 0, w_var.c_str());
  //Perform the fit
  if (w_var == "")
  {
    model.fitTo(data);
  }
  else
  {
    model.fitTo(data, RooFit::SumW2Error(const_list.SumW2Error));
  }
  //Import to workspace
  model.getComponents()->Print();
  ws->import(model);
  ws->import(data);

  //Save yields in a file
  SaveRooVars(ws, "output/" + variablename + "_RooYields.txt");
  return ws;
}

//Fit for normalization pKJPsi
RooWorkspace *FitLb2pKJPsi(string variablename, TTree *chain, string w_var, string *act_vars, int N, string opts)
{
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);
  //Initialize fit workspace
  RooWorkspace *ws = new RooWorkspace(name_list.workspace.c_str());
  //Establish number of bkgs (some may come as extra)
  //Deactivate all unused variables (speeds up the fit)
  Activate(chain, act_vars, N);
  chain->SetBranchStatus(variablename.c_str(), 1);
  //Get Entries
  int entries = chain->GetEntries();
  //Initialize all the stuff needed for the fit
  RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

  //Initialize Exponential1 for Comb
  RooRealVar tauComb_bkg(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
  RooExponential comb(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, tauComb_bkg);
  RooRealVar f_comb(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), const_list.fbkg_0 * double(entries), 0., double(entries));

  //Initialize gaussian for signal
  RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
  mean.setConstant(!(const_list.mean_min - const_list.mean_max));
  RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
  width.setConstant(!(const_list.width_min - const_list.width_max));
  RooRealVar alphaL(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), const_list.alpha1_0, const_list.alpha1_min, const_list.alpha1_max);
  RooRealVar alphaR(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), const_list.alpha2_0, const_list.alpha2_min, const_list.alpha2_max);
  RooDoubleGaussExp signal(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width, alphaL, alphaR);
  RooRealVar f_signal(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), double(entries) * const_list.fsig_0, 0., double(entries));
  f_signal.setConstant(!(const_list.fsig_0));

  //Define the model
  RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(signal, comb), RooArgList(f_signal, f_comb));
  //Deal with weights, if requested
  RooArgSet *args;
  RooRealVar *wvar;
  if (w_var != "")
  {
    wvar = new RooRealVar(w_var.c_str(), w_var.c_str(), 0, 10000);
    args = new RooArgSet(B_M, *wvar);
    chain->SetBranchStatus(w_var.c_str(), 1);
  }
  else
  {
    args = new RooArgSet(B_M);
  }
  RooDataSet data(name_list.dataset.c_str(), name_list.dataset.c_str(), chain, *args, 0, w_var.c_str());
  //Perform the fit
  if (w_var == "")
  {
    model.fitTo(data);
  }
  else
  {
    model.fitTo(data, RooFit::SumW2Error(const_list.SumW2Error));
  }
  //Import to workspace
  model.getComponents()->Print();
  ws->import(model);
  ws->import(data);

  //Save yields in a file
  SaveRooVars(ws, "output/" + variablename + "_RooYields.txt");
  return ws;
}

//This is going to change. All the fits and steps will be done in here
RooWorkspace *FitLb2NstG_Kpi_Wrong(string variablename, TTree *chain, string opts)
{
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);
  //Initialize fit workspace
  RooWorkspace *ws = new RooWorkspace(name_list.workspace.c_str());
  //Deactivate all unused variables (speeds up the fit)
  Activate(chain, 0, 0);
  chain->SetBranchStatus(variablename.c_str(), 1);
  //Get Entries.
  int entries = chain->GetEntries();
  //Initialize mass varibale
  RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

  //Initialize array of predefined fit functions
  FitFunction *fitf = FitFunction_init();
  //For this fit we have several components:
  //DoubleCB for the KpiG component, the tails are characterized from a fit to MC
  //ArgusGauss for the KpipiG component. All parameters fixed from MC except the yield
  //Exponential for Kpipi0X component. Directly obtained from data
  //Straight line for combinatorial. Directly obtained from data

  /** Therefore, we can start with the fits to MC **/
  //Fit DoubleCB for KpiG
  stringstream ss;
  TFile *MCprefile = TFile::Open("Tuples/Wrong1_temp0.root");
  TTree *MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_pbarpi_global_weight", "Event_PIDCalibEff_pbarpi*Global_weight", "Tuples/temp0_global_pbarpi.root");
  TFile *MCfile = TFile::Open("Tuples/temp0_global_pbarpi.root");
  TTree *MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl
       << "Starting MC fit number 0"
       << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws0 = fitf[DoubleCB](variablename, MCtree, "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts + "_MC");
  GoodPlot(MCws0, variablename, "", "", opts + "_MC", "_MC0_Wrong");
  SaveRooVars(MCws0, "output/" + variablename + "_RooYields_MC0_Wrong.txt");
  //Close file
  MCfile->Close();
  MCprefile->Close();

  //Fit ArgusGauss for KpipiG
  MCprefile = TFile::Open("Tuples/Wrong1_temp2.root");
  MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_pbarpi_global_weight", "Event_PIDCalibEff_pbarpi*Global_weight", "Tuples/temp2_global_pbarpi.root");
  MCfile = TFile::Open("Tuples/temp2_global_pbarpi.root");
  MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl
       << "Starting MC fit number 2"
       << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws2 = fitf[DoubleGaussExp](variablename, MCtree, "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts + "_MC");
  GoodPlot(MCws2, variablename, "", "", opts + "_MC", "_MC2_Wrong");
  SaveRooVars(MCws2, "output/" + variablename + "_RooYields_MC2_Wrong.txt");
  //Close file
  MCfile->Close();
  MCprefile->Close();
  /*************************************************/

  //Once this is done, we can define all the components of the fit
  double value;
  //Signal (KpiG)
  //Initialize DoubleCB for signal
  RooRealVar KpiG_mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);       //From fit
  RooRealVar KpiG_width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max); //From fit
  value = MCws0->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar KpiG_alphaL(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), value);
  KpiG_alphaL.setConstant();
  value = MCws0->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar KpiG_alphaR(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), value);
  KpiG_alphaR.setConstant();
  value = MCws0->var(name_list.nL[0].c_str())->getValV();
  RooRealVar KpiG_nL(name_list.nL[0].c_str(), name_list.nL[0].c_str(), value);
  KpiG_nL.setConstant();
  value = MCws0->var(name_list.nR[0].c_str())->getValV();
  RooRealVar KpiG_nR(name_list.nR[0].c_str(), name_list.nR[0].c_str(), value);
  KpiG_nR.setConstant();
  BifurcatedCB KpiG_pdf(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, KpiG_mean, KpiG_width, KpiG_alphaL, KpiG_nL, KpiG_alphaR, KpiG_nR);
  RooRealVar KpiG_f(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), 8000., 0., double(entries));
  //Constantize automatically if we set equal limits
  KpiG_mean.setConstant(!(const_list.mean_min - const_list.mean_max));
  KpiG_width.setConstant(!(const_list.width_min - const_list.width_max));

  //Initialize Exponential for Kpipi0
  RooRealVar Kpipi0X_tau(name_list.exp_par[1].c_str(), name_list.exp_par[1].c_str(), -0.0019, -1., -0.0001);
  RooExponential Kpipi0X_pdf(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, Kpipi0X_tau);
  RooRealVar Kpipi0X_f(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), 41000., 0., double(entries));

  //Initialize Line for combinatorial
  RooRealVar comb_slope(name_list.slope.c_str(), name_list.slope.c_str(), const_list.slope_0, const_list.slope_min, const_list.slope_max);
  RooPolynomial comb_pdf(name_list.comppdf[2].c_str(), name_list.comppdf[2].c_str(), B_M, comb_slope);
  RooRealVar comb_f(name_list.fcomp[2].c_str(), name_list.fcomp[2].c_str(), 11500., 0., double(entries));

  //Part reco (KpipiG)
  //Initialize DoubleGaussExp
  value = MCws2->var(name_list.mean[0].c_str())->getValV();
  RooRealVar KpipiG_mean(name_list.mean[3].c_str(), name_list.mean[3].c_str(), value);
  KpipiG_mean.setConstant();
  value = MCws2->var(name_list.width[0].c_str())->getValV();
  RooRealVar KpipiG_width(name_list.width[3].c_str(), name_list.width[3].c_str(), value);
  KpipiG_width.setConstant();
  value = MCws2->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar KpipiG_alphaL(name_list.alphaL[3].c_str(), name_list.alphaL[3].c_str(), value);
  KpipiG_alphaL.setConstant();
  value = MCws2->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar KpipiG_alphaR(name_list.alphaR[3].c_str(), name_list.alphaR[3].c_str(), value);
  KpipiG_alphaR.setConstant();
  RooDoubleGaussExp KpipiG_pdf(name_list.comppdf[3].c_str(), name_list.comppdf[3].c_str(), B_M, KpipiG_mean, KpipiG_width, KpipiG_alphaL, KpipiG_alphaR);
  RooRealVar KpipiG_f(name_list.fcomp[3].c_str(), name_list.fcomp[3].c_str(), 4000., 0., double(entries));

  RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(KpiG_pdf, Kpipi0X_pdf, comb_pdf, KpipiG_pdf), RooArgList(KpiG_f, Kpipi0X_f, comb_f, KpipiG_f));
  RooDataSet data(name_list.dataset.c_str(), name_list.dataset.c_str(), chain, B_M);
  //Perform the fit
  model.fitTo(data /*, RooFit::ExternalConstraints(gc_list)*/);
  //Import to workspace
  ws->import(model);
  ws->import(data);

  //Save yields in a file
  SaveRooVars(ws, "output/" + variablename + "_RooYields_Wrong.txt");
  return ws;
}

RooWorkspace *FitLb2NstG_pK_Wrong(string variablename, TTree *chain, string opts)
{
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);
  //Initialize fit workspace
  RooWorkspace *ws = new RooWorkspace(name_list.workspace.c_str());
  //Deactivate all unused variables (speeds up the fit)
  Activate(chain, 0, 0);
  chain->SetBranchStatus(variablename.c_str(), 1);
  //Get Entries.
  int entries = chain->GetEntries();
  //Initialize mass varibale
  RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

  //Initialize array of predefined fit functions
  FitFunction *fitf = FitFunction_init();
  //For this fit we have several components:
  //CBExp (Crystal-ball with exponential tail on the other tail) for the pKG (signal). Tails fixed from MC
  //Exponential for combinatorial. Directly obtained from data

  /** Therefore, we can start with the fits to MC **/
  //Fit CBExp for pKG
  stringstream ss;
  TFile *MCprefile = TFile::Open("Tuples/Wrong2_temp1.root");
  TTree *MCpretree = (TTree *)MCprefile->Get("DecayTree");
  AddTreeBranch(MCpretree, "Event_PIDCalibEff_ppibar_global_weight", "Event_PIDCalibEff_ppibar*Global_weight", "Tuples/temp1_global_ppibar.root");
  TFile *MCfile = TFile::Open("Tuples/temp1_global_ppibar.root");
  TTree *MCtree = (TTree *)MCfile->Get("DecayTree");
  cout << endl
       << "Starting MC fit number 1"
       << endl;
  cout << "------------------------" << endl
       << endl;
  //Fit and plot
  RooWorkspace *MCws1 = fitf[DoubleCB](variablename, MCtree, "Event_PIDCalibEff_ppibar_global_weight", 0, 0, opts + "_MC");
  GoodPlot(MCws1, variablename, "", "", opts + "_MC", "_MC1_Wrong");
  SaveRooVars(MCws1, "output/" + variablename + "_RooYields_MC1_Wrong.txt");
  //Close file
  MCfile->Close();
  MCprefile->Close();
  /*************************************************/

  //Once this is done, we can define all the components of the fit
  double value;
  //Signal (pKG)
  //Initialize CBExp for signal
  RooRealVar pKG_mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);       //From fit
  RooRealVar pKG_width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max); //From fit
  value = MCws1->var(name_list.alphaL[0].c_str())->getValV();
  RooRealVar pKG_alphaL(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), value);
  pKG_alphaL.setConstant();
  value = MCws1->var(name_list.alphaR[0].c_str())->getValV();
  RooRealVar pKG_alphaR(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), value);
  pKG_alphaR.setConstant();
  value = MCws1->var(name_list.nL[0].c_str())->getValV();
  RooRealVar pKG_nL(name_list.nL[0].c_str(), name_list.nL[0].c_str(), value);
  pKG_nL.setConstant();
  value = MCws1->var(name_list.nR[0].c_str())->getValV();
  RooRealVar pKG_nR(name_list.nR[0].c_str(), name_list.nR[0].c_str(), value);
  pKG_nR.setConstant();
  BifurcatedCB pKG_pdf(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, pKG_mean, pKG_width, pKG_alphaL, pKG_nL, pKG_alphaR, pKG_nR);
  RooRealVar pKG_f(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), 2700., 0., double(entries));
  //Constantize automatically if we set equal limits
  pKG_mean.setConstant(!(const_list.mean_min - const_list.mean_max));
  pKG_width.setConstant(!(const_list.width_min - const_list.width_max));

  //Initialize Exponential for combinatorial
  RooRealVar comb_tau(name_list.exp_par[1].c_str(), name_list.exp_par[1].c_str(), -0.0019, -1., -0.0001);
  RooExponential comb_pdf(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, comb_tau);
  RooRealVar comb_f(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), 3900., 0., double(entries));

  RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(pKG_pdf, comb_pdf), RooArgList(pKG_f, comb_f));
  RooDataSet data(name_list.dataset.c_str(), name_list.dataset.c_str(), chain, B_M);
  //Perform the fit
  model.fitTo(data /*, RooFit::ExternalConstraints(gc_list)*/);
  //Import to workspace
  ws->import(model);
  ws->import(data);

  //Save yields in a file
  SaveRooVars(ws, "output/" + variablename + "_RooYields_Wrong.txt");
  return ws;
}
