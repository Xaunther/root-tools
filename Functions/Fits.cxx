#include "TChain.h"
#include <string>
#include <sstream>
#include <iostream>
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooFormulaVar.h"
#include "TLegend.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooCBShape.h"
#include "RooArgusBG.h"
#include "RooPolynomial.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TPad.h"
#include "TH1F.h"
#include "TColor.h"
#include "Functions/Fits.h"
#include "Functions/Filesaving.h"
#include "Functions/BifurcatedCB.h"
#include "Functions/RooCBExp.h"
#include "Functions/Roo2Exp.h"
#include "Functions/RooGaussExp.h"
#include "Functions/RooDoubleGaussExp.h"
#include "Functions/RooArgusGauss.h"
#include "Functions/TreeTools.h"
#include "Dictionaries/Constants.h"
#include "Dictionaries/Names.h"

using namespace std;

FitFunction* FitFunction_init()
{
	//I create an array of pointers to the fitting functions, so no need to change code in VarFit.C if we add more ;)
	FitFunction* fitf = new FitFunction[18];
	fitf[0] = FitGauss_Exp;
	fitf[1] = FitGauss_Exp_Line;
	fitf[2] = FitDoubleCB_Exp;
	fitf[3] = FitGauss;
	fitf[4] = FitCB;
	fitf[5] = FitDoubleCB;
	fitf[6] = FitCBExp;
	fitf[7] = FitGaussExp;
	fitf[8] = FitDoubleGaussExp;
	fitf[9] = FitDoubleGaussExp_Exp;
	fitf[10] = FitCBExp_Exp;
	fitf[11] = FitArgusGauss;
	fitf[12] = FitB2KstIsoG;
	fitf[13] = FitNothing;
	fitf[14] = FitArgus_Gauss_Exp;
	fitf[15] = FitExp;
	fitf[16] = FitExp_Line;
	fitf[17] = FitLine;
	return fitf;
}

RooWorkspace* FitGauss_Exp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	double entries = chain->GetEntries();
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	//Initialize all the stuff needed for the fit
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooGaussian signal(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width);

	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential bkg(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, bkgpar);

	RooRealVar fsig(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), entries * const_list.fsig_0, 0, entries);
	RooRealVar fbkg(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), entries * const_list.fbkg_0, 0, entries);

	RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(signal, bkg), RooArgList(fsig, fbkg));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitGauss_Exp_Line(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	double entries = chain->GetEntries();
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	//Initialize all the stuff needed for the fit
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooGaussian signal(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width);

	//Exponential
	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), 4.*const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential bkg(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, bkgpar);

	//Line
	RooRealVar slope(name_list.slope.c_str(), name_list.slope.c_str(), const_list.slope_0, const_list.slope_min, const_list.slope_max);
	slope.setConstant(!(const_list.slope_min - const_list.slope_max));
	RooPolynomial bkg2(name_list.comppdf[2].c_str(), name_list.comppdf[2].c_str(), B_M, RooArgList(slope));

	RooRealVar fsig(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), entries * const_list.fsig_0, 0, entries);
	RooRealVar fbkg(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), entries * const_list.fbkg_0, 0, entries);
	RooRealVar fbkg2(name_list.fcomp[2].c_str(), name_list.fcomp[2].c_str(), entries * const_list.fbkg_0, 0, entries);

	RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(signal, bkg, bkg2), RooArgList(fsig, fbkg, fbkg2));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitDoubleCB_Exp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	double entries = chain->GetEntries();
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	//Initialize all the stuff needed for the fit
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alphaL(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), const_list.alpha1_0, const_list.alpha1_min, const_list.alpha1_max);
	alphaL.setConstant(!(const_list.alpha1_min - const_list.alpha1_max));
	RooRealVar alphaR(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), const_list.alpha2_0, const_list.alpha2_min, const_list.alpha2_max);
	alphaR.setConstant(!(const_list.alpha2_min - const_list.alpha2_max));
	RooRealVar nL(name_list.nL[0].c_str(), name_list.nL[0].c_str(), const_list.n1_0, const_list.n1_min, const_list.n1_max);
	nL.setConstant(!(const_list.n1_min - const_list.n1_max));
	RooRealVar nR(name_list.nR[0].c_str(), name_list.nR[0].c_str(), const_list.n2_0, const_list.n2_min, const_list.n2_max);
	nR.setConstant(!(const_list.n2_min - const_list.n2_max));
	BifurcatedCB signal(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width, alphaL, nL, alphaR, nR);

	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential bkg(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, bkgpar);

	RooRealVar fsig(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), entries * const_list.fsig_0, 0, entries);
	RooRealVar fbkg(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), entries * const_list.fbkg_0, 0, entries);

	RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(signal, bkg), RooArgList(fsig, fbkg));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitGauss(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);

	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	//Initialize all the stuff needed for the fit
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooGaussian model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, mean, width);

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitCB(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);

	//Define plot basic parameters which are use repeteadly
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	//Initialize all the stuff needed for the fit
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alpha(name_list.alpha.c_str(), name_list.alpha.c_str(), const_list.alpha_0, const_list.alpha_min, const_list.alpha_max);
	alpha.setConstant(!(const_list.alpha_min - const_list.alpha_max));
	RooRealVar n(name_list.n.c_str(), name_list.n.c_str(), const_list.n_0, const_list.n_min, const_list.n_max);
	n.setConstant(!(const_list.n_min - const_list.n_max));
	RooCBShape model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, mean, width, alpha, n);

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitDoubleCB(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);

	//Initialize all the stuff needed for the fit
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alpha1(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), const_list.alpha1_0, const_list.alpha1_min, const_list.alpha1_max);
	alpha1.setConstant(!(const_list.alpha1_min - const_list.alpha1_max));
	RooRealVar alpha2(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), const_list.alpha2_0, const_list.alpha2_min, const_list.alpha2_max);
	alpha2.setConstant(!(const_list.alpha2_min - const_list.alpha2_max));
	RooRealVar n1(name_list.nL[0].c_str(), name_list.nL[0].c_str(), const_list.n1_0, const_list.n1_min, const_list.n1_max);
	n1.setConstant(!(const_list.n1_min - const_list.n1_max));
	RooRealVar n2(name_list.nR[0].c_str(), name_list.nR[0].c_str(), const_list.n2_0, const_list.n2_min, const_list.n2_max);
	n2.setConstant(!(const_list.n2_min - const_list.n2_max));
	BifurcatedCB model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, mean, width, alpha1, n1, alpha2, n2);

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}
RooWorkspace* FitCBExp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);

	//Initialize all the stuff needed for the fit
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alpha1(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), const_list.alpha1_0, const_list.alpha1_min, const_list.alpha1_max);
	alpha1.setConstant(!(const_list.alpha1_min - const_list.alpha1_max));
	RooRealVar alpha2(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), const_list.alpha2_0, const_list.alpha2_min, const_list.alpha2_max);
	alpha2.setConstant(!(const_list.alpha2_min - const_list.alpha2_max));
	RooRealVar n(name_list.n.c_str(), name_list.n.c_str(), const_list.n_0, const_list.n_min, const_list.n_max);
	n.setConstant(!(const_list.n_min - const_list.n_max));
	RooCBExp model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, mean, width, alpha1, n, alpha2);

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}
RooWorkspace* FitGaussExp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);

	//Initialize all the stuff needed for the fit
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alpha(name_list.alpha.c_str(), name_list.alpha.c_str(), const_list.alpha_0, const_list.alpha_min, const_list.alpha_max);
	alpha.setConstant(!(const_list.alpha_min - const_list.alpha_max));
	RooGaussExp model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, mean, width, alpha);

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitDoubleGaussExp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);

	//Initialize all the stuff needed for the fit
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alpha1(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), const_list.alpha1_0, const_list.alpha1_min, const_list.alpha1_max);
	alpha1.setConstant(!(const_list.alpha1_min - const_list.alpha1_max));
	RooRealVar alpha2(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), const_list.alpha2_0, const_list.alpha2_min, const_list.alpha2_max);
	alpha2.setConstant(!(const_list.alpha2_min - const_list.alpha2_max));
	RooDoubleGaussExp model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, mean, width, alpha1, alpha2);

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitDoubleGaussExp_Exp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	double entries = chain->GetEntries();

	//Initialize all the stuff needed for the fit
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alpha1(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), const_list.alpha1_0, const_list.alpha1_min, const_list.alpha1_max);
	alpha1.setConstant(!(const_list.alpha1_min - const_list.alpha1_max));
	RooRealVar alpha2(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), const_list.alpha2_0, const_list.alpha2_min, const_list.alpha2_max);
	alpha2.setConstant(!(const_list.alpha2_min - const_list.alpha2_max));
	RooDoubleGaussExp signal(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width, alpha1, alpha2);

	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential bkg(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, bkgpar);

	RooRealVar fsig(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), entries * const_list.fsig_0, 0, entries);
	RooRealVar fbkg(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), entries * const_list.fbkg_0, 0, entries);
	RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(signal, bkg), RooArgList(fsig, fbkg));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitCBExp_Exp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	double entries = chain->GetEntries();

	//Initialize all the stuff needed for the fit
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alpha1(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), const_list.alpha1_0, const_list.alpha1_min, const_list.alpha1_max);
	alpha1.setConstant(!(const_list.alpha1_min - const_list.alpha1_max));
	RooRealVar alpha2(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), const_list.alpha2_0, const_list.alpha2_min, const_list.alpha2_max);
	alpha2.setConstant(!(const_list.alpha2_min - const_list.alpha2_max));
	RooRealVar n(name_list.n.c_str(), name_list.n.c_str(), const_list.n_0, const_list.n_min, const_list.n_max);
	n.setConstant(!(const_list.n_min - const_list.n_max));
	RooCBExp signal(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width, alpha1, n, alpha2);

	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential bkg(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, bkgpar);

	RooRealVar fsig(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), entries * const_list.fsig_0, 0, entries);
	RooRealVar fbkg(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), entries * const_list.fbkg_0, 0, entries);
	RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(signal, bkg), RooArgList(fsig, fbkg));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitArgusGauss(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);

	//Define plot basic parameters which are use repeteadly
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	//Initialize all the stuff needed for the fit
	RooRealVar m0(name_list.m0_Argus.c_str(), name_list.m0_Argus.c_str(), const_list.m0_Argus_0, const_list.m0_Argus_min, const_list.m0_Argus_max);
	m0.setConstant(!(const_list.m0_Argus_min - const_list.m0_Argus_max));
	RooRealVar width(name_list.width_Argus.c_str(), name_list.width_Argus.c_str(), const_list.width_Argus_0, const_list.width_Argus_min, const_list.width_Argus_max);
	width.setConstant(!(const_list.width_Argus_min - const_list.width_Argus_max));
	RooRealVar c(name_list.c_Argus.c_str(), name_list.c_Argus.c_str(), const_list.c_Argus_0, const_list.c_Argus_min, const_list.c_Argus_max);
	c.setConstant(!(const_list.c_Argus_min - const_list.c_Argus_max));
	RooRealVar p(name_list.p_Argus.c_str(), name_list.p_Argus.c_str(), const_list.p_Argus_0, const_list.p_Argus_min, const_list.p_Argus_max);
	p.setConstant(!(const_list.p_Argus_min - const_list.p_Argus_max));
	RooArgusGauss model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, m0, c, p, width);

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitB2KstIsoG(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	double entries = chain->GetEntries();
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	//Initialize all the stuff needed for the fit
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooRealVar alpha1(name_list.alphaL[0].c_str(), name_list.alphaL[0].c_str(), const_list.alpha1_0, const_list.alpha1_min, const_list.alpha1_max);
	alpha1.setConstant(!(const_list.alpha1_min - const_list.alpha1_max));
	RooRealVar alpha2(name_list.alphaR[0].c_str(), name_list.alphaR[0].c_str(), const_list.alpha2_0, const_list.alpha2_min, const_list.alpha2_max);
	alpha2.setConstant(!(const_list.alpha2_min - const_list.alpha2_max));
	RooRealVar n1(name_list.nL[0].c_str(), name_list.nL[0].c_str(), const_list.n1_0, const_list.n1_min, const_list.n1_max);
	n1.setConstant(!(const_list.n1_min - const_list.n1_max));
	RooRealVar n2(name_list.nR[0].c_str(), name_list.nR[0].c_str(), const_list.n2_0, const_list.n2_min, const_list.n2_max);
	n2.setConstant(!(const_list.n2_min - const_list.n2_max));
	BifurcatedCB signal(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width, alpha1, n1, alpha2, n2);

	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential bkg(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, bkgpar);

	RooRealVar fsig(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), entries * const_list.fsig_0, 0, entries);
	RooRealVar fbkg(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), entries * const_list.fbkg_0, 0, entries);

	RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(signal, bkg), RooArgList(fsig, fbkg));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitNothing(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	//Workspace creation
	RooWorkspace* ws = new RooWorkspace(name_list.workspace.c_str());

	double xmin = const_list.xmin;
	double xmax = const_list.xmax;
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), xmin, xmax);
	//Initialize all the stuff needed for the fit
	RooArgSet* args;
	RooRealVar* wvar;
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
	RooDataSet data("data", "data", chain, *args, 0, w_var.c_str());
	RooRealVar N_comps(name_list.N_comps.c_str(), name_list.N_comps.c_str(), 0., 0., 0.);
	ws->import(data);
	ws->import(N_comps);
	SaveRooVars(ws, "output/" + variablename + "_RooYields_VarFit.txt");
	return ws;
}

void MultiPlot(string variablename, int N_trees, TTree** chain, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	double xmin = const_list.xmin;
	double xmax = const_list.xmax;
	double bin_size = (xmax - xmin) / const_list.bins[0];

	stringstream ss;
	ss << fixed;
	ss.precision(0);
	ss << bin_size;

	//Automatize titles
	string title;
	title = const_list.plot_title + " ; " + const_list.plot_Xtitle + "(" + const_list.plot_units + ") ; Candidates / (" + ss.str() + " " + const_list.plot_units + ")";

	//Options used
	string hist_opts = "HISTO";

	//Color roulette
	int colours[] = {kRed, kBlue, kGreen, kMagenta, kCyan, kYellow, kBlack, kMagenta + 2, kCyan + 2, kYellow + 2};

	//Create mah canvas
	TCanvas* c = new TCanvas();
	c->cd(0)->SetTicks(1, 1);
	c->SetTitle(title.c_str());

	//Plot on the canvas to retrieve histograms, Then add format
	for (int i = 0; i < N_trees; i++)
	{
		//Plot
		if (i == 0)
		{
			chain[i]->Draw(variablename.c_str(), "", hist_opts.c_str());
		}
		else
		{
			chain[i]->Draw(variablename.c_str(), "", (hist_opts + " SAME").c_str());
		}
		//Retrieve histo
		TH1* hist1 = chain[i]->GetHistogram();
		//Set format
		hist1->SetFillColorAlpha(colours[i], 0.5);
		hist1->SetLineColor(colours[i]);
		hist1->SetFillStyle(1001);

	}

	//Axis titles
	TH1F* h1 = (TH1F*)gPad->GetPrimitive("h1");
	h1->SetTitle(title.c_str());
	h1->SetStats(0);


	//If logarithm is requested
	if (const_list.logY)
	{
		c->SetLogy();
	}
	if (const_list.logX)
	{
		c->SetLogx();
	}
	/*
	//Define legend
	TLegend* leg = new TLegend();

	if(const_list.legend_L)//Left aligned
	  {
	    leg = new TLegend(const_list.Legend_x0_L, const_list.Legend_y0, const_list.Legend_xf_L, const_list.Legend_yf);
	  }
	else //Right aligned
	  {
	    leg = new TLegend(const_list.Legend_x0_R, const_list.Legend_y0, const_list.Legend_xf_R, const_list.Legend_yf);
	  }

	ss.str("");
	ss << chain1->GetEntries();
	leg->AddEntry(chain1->GetHistogram(),(const_list.legend_data1+" ("+ss.str()+" evts)").c_str(),"l");
	ss.str("");
	ss << chain2->GetEntries();
	leg->AddEntry(chain2->GetHistogram(),(const_list.legend_data2+" ("+ss.str()+" evts)").c_str(),"l");
	leg->Draw();

	c->Update();
	*/
	string pdfname = "plots/" + variablename + "_Compare.pdf";
	c->SaveAs(pdfname.c_str());

}

RooWorkspace* FitArgus_Gauss_Exp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	double entries = chain->GetEntries();
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);
	//Initialize all the stuff needed for Gaussian
	RooRealVar mean(name_list.mean[0].c_str(), name_list.mean[0].c_str(), const_list.mean_0, const_list.mean_min, const_list.mean_max);
	mean.setConstant(!(const_list.mean_min - const_list.mean_max));
	RooRealVar width(name_list.width[0].c_str(), name_list.width[0].c_str(), const_list.width_0, const_list.width_min, const_list.width_max);
	width.setConstant(!(const_list.width_min - const_list.width_max));
	RooGaussian signal(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, mean, width);

	//Initialize all the stuff needed for ArgusGauss
	RooRealVar m0(name_list.m0_Argus.c_str(), name_list.m0_Argus.c_str(), const_list.m0_Argus_0, const_list.m0_Argus_min, const_list.m0_Argus_max);
	m0.setConstant(!(const_list.m0_Argus_min - const_list.m0_Argus_max));
	RooRealVar width_Argus(name_list.width_Argus.c_str(), name_list.width_Argus.c_str(), const_list.width_Argus_0, const_list.width_Argus_min, const_list.width_Argus_max);
	width_Argus.setConstant(!(const_list.width_Argus_min - const_list.width_Argus_max));
	RooRealVar c(name_list.c_Argus.c_str(), name_list.c_Argus.c_str(), const_list.c_Argus_0, const_list.c_Argus_min, const_list.c_Argus_max);
	c.setConstant(!(const_list.c_Argus_min - const_list.c_Argus_max));
	RooRealVar p(name_list.p_Argus.c_str(), name_list.p_Argus.c_str(), const_list.p_Argus_0, const_list.p_Argus_min, const_list.p_Argus_max);
	p.setConstant(!(const_list.p_Argus_min - const_list.p_Argus_max));
	RooArgusGauss pdf_Argus(name_list.comppdf[2].c_str(), name_list.comppdf[2].c_str(), B_M, m0, c, p, width);

	//Initialize exponential
	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential bkg(name_list.comppdf[1].c_str(), name_list.comppdf[1].c_str(), B_M, bkgpar);

	RooRealVar fsig(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), entries * const_list.fsig_0, 0, entries);
	RooRealVar fbkg(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), entries * const_list.fbkg_0, 0, entries);
	RooRealVar fbkg_Argus(name_list.fcomp[2].c_str(), name_list.fcomp[2].c_str(), entries * const_list.fbkg_0, 0, entries);

	RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(signal, bkg, pdf_Argus), RooArgList(fsig, fbkg, fbkg_Argus));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitExp(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

	//Initialize exponential
	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, bkgpar);

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitLine(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

	//Initialize exponential
	RooRealVar slope(name_list.slope.c_str(), name_list.slope.c_str(), const_list.slope_0, const_list.slope_min, const_list.slope_max);
	slope.setConstant(!(const_list.slope_min - const_list.slope_max));
	RooPolynomial model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), B_M, RooArgList(slope));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* FitExp_Line(string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts)
{
	//Initialize constants
	Constants const_list(opts);
	Names name_list(opts);

	Activate(chain, act_vars, N);
	chain->SetBranchStatus(variablename.c_str(), 1);
	double entries = chain->GetEntries();
	RooRealVar B_M(variablename.c_str(), variablename.c_str(), const_list.xmin, const_list.xmax);

	//Initialize exponential
	RooRealVar bkgpar(name_list.exp_par[0].c_str(), name_list.exp_par[0].c_str(), const_list.bkgpar_0, const_list.bkgpar_min, const_list.bkgpar_max);
	bkgpar.setConstant(!(const_list.bkgpar_min - const_list.bkgpar_max));
	RooExponential bkg(name_list.comppdf[0].c_str(), name_list.comppdf[0].c_str(), B_M, bkgpar);
	//Initialize line
	RooRealVar slope(name_list.slope.c_str(), name_list.slope.c_str(), const_list.slope_0, const_list.slope_min, const_list.slope_max);
	slope.setConstant(!(const_list.slope_min - const_list.slope_max));
	RooPolynomial bkg2(name_list.comppdf[0].c_str(), name_list.comppdf[1].c_str(), B_M, RooArgList(slope));

	RooRealVar fsig(name_list.fcomp[0].c_str(), name_list.fcomp[0].c_str(), entries * const_list.fsig_0, 0, entries);
	RooRealVar fbkg(name_list.fcomp[1].c_str(), name_list.fcomp[1].c_str(), entries * const_list.fbkg_0, 0, entries);

	RooAddPdf model(name_list.pdfmodel[0].c_str(), name_list.pdfmodel[0].c_str(), RooArgList(bkg, bkg2), RooArgList(fsig, fbkg));

	return DoFit(B_M, chain, model, const_list, name_list, variablename, w_var);
}

RooWorkspace* DoFit(RooRealVar B_M, TTree* chain, RooAbsPdf& model, Constants& const_list, Names& name_list, string variablename, string w_var)
{
	RooArgSet* args;
	RooWorkspace* ws = new RooWorkspace(name_list.workspace.c_str());
	RooRealVar* wvar;
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
	model.fitTo(data, RooFit::SumW2Error(const_list.SumW2Error));

	ws->import(model);
	ws->import(data);
	SaveRooVars(ws, "output/" + variablename + "_RooYields_VarFit.txt");
	return ws;
}

FitOption StringToFitOption(string fitoptstring)
{
	FitOption fitopt;
	if (fitoptstring == "Gauss_exp")
	{
		fitopt = Gauss_exp;
	}
	else if (fitoptstring == "Gauss_exp_line")
	{
		fitopt = Gauss_exp_line;
	}
	else if (fitoptstring == "DoubleCB_exp")
	{
		fitopt = DoubleCB_exp;
	}
	else if (fitoptstring == "Gauss")
	{
		fitopt = Gauss;
	}
	else if (fitoptstring == "CB")
	{
		fitopt = CB;
	}
	else if (fitoptstring == "DoubleCB")
	{
		fitopt = DoubleCB;
	}
	else if (fitoptstring == "CBExp")
	{
		fitopt = CBExp;
	}
	else if (fitoptstring == "GaussExp")
	{
		fitopt = GaussExp;
	}
	else if (fitoptstring == "DoubleGaussExp")
	{
		fitopt = DoubleGaussExp;
	}
	else if (fitoptstring == "DoubleGaussExp_Exp")
	{
		fitopt = DoubleGaussExp_Exp;
	}
	else if (fitoptstring == "CBExp_exp")
	{
		fitopt = CBExp_exp;
	}
	else if (fitoptstring == "ArgusGauss")
	{
		fitopt = ArgusGauss;
	}
	else if (fitoptstring == "B2KstIsoG")
	{
		fitopt = B2KstIsoG;
	}
	else if (fitoptstring == "Nothing")
	{
		fitopt = Nothing;
	}
	else if (fitoptstring == "Argus_Gauss_exp")
	{
		fitopt = Argus_Gauss_exp;
	}
	else if (fitoptstring == "Exp")
	{
		fitopt = Exp;
	}
	else if (fitoptstring == "Exp_Line")
	{
		fitopt = Exp_Line;
	}
	else if (fitoptstring == "Line")
	{
		fitopt = Line;
	}
	else
	{
		cout << "Fit option not found. Exiting..." << endl;
		exit(1);
	}
	return fitopt;
}
