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
  RooWorkspace *MCws0 = fitf[CBExp](variablename, MCtree, "Event_PIDCalibEff_pbarpi_global_weight", 0, 0, opts + "_MC");
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
  value = MCws0->var(name_list.n.c_str())->getValV();
  RooRealVar KpiG_nL(name_list.nL[0].c_str(), name_list.nL[0].c_str(), value);
  KpiG_nL.setConstant();
  RooCBExp KpiG_pdf(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, KpiG_mean, KpiG_width, KpiG_alphaL, KpiG_nL, KpiG_alphaR);
  RooRealVar KpiG_f(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), 8000., 0., double(entries));
  //Constantize automatically if we set equal limits
  KpiG_mean.setConstant(!(const_list.mean_min - const_list.mean_max));
  KpiG_width.setConstant(!(const_list.width_min - const_list.width_max));

  //Initialize Exponential for Kpipi0
  RooRealVar Kpipi0X_tau(name_list.exp_par[1].c_str(), name_list.exp_par[1].c_str(), -0.0019, -1., -0.0001);
  RooExponential Kpipi0X_pdf(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, Kpipi0X_tau);
  RooRealVar Kpipi0X_f(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), 41000., 0., double(entries));

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

  RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(KpiG_pdf, Kpipi0X_pdf, KpipiG_pdf), RooArgList(KpiG_f, Kpipi0X_f, KpipiG_f));
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
