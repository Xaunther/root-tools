#ifndef Fits_h
#define Fits_h

#include "TTree.h"
#include <string>
#include "RooWorkspace.h"
#include "Dictionaries/Names.h"
#include "Dictionaries/Constants.h"

using namespace std;

//Enumeration of all possible plot options
enum FitOption {Gauss_exp, Gauss_exp_line, DoubleCB_exp, Gauss, CB, DoubleCB, CBExp, GaussExp, DoubleGaussExp, DoubleGaussExp_Exp, CBExp_exp, ArgusGauss, B2KstIsoG, Nothing, Argus_Gauss_exp, Exp, Exp_Line, Line};

RooWorkspace* FitGauss_Exp(string, TTree*, string, string*, int, string);
RooWorkspace* FitGauss_Exp_Line(string, TTree*, string, string*, int, string);
RooWorkspace* FitDoubleCB_Exp(string, TTree*, string, string*, int, string);
RooWorkspace* FitGauss(string, TTree*, string, string*, int, string);
RooWorkspace* FitCB(string, TTree*, string, string*, int, string);
RooWorkspace* FitDoubleCB(string, TTree*, string, string*, int, string);
RooWorkspace* FitCBExp(string, TTree*, string, string*, int, string);
RooWorkspace* FitGaussExp(string, TTree*, string, string*, int, string);
RooWorkspace* FitDoubleGaussExp(string, TTree*, string, string*, int, string);
RooWorkspace* FitDoubleGaussExp_Exp(string, TTree*, string, string*, int, string);
RooWorkspace* FitCBExp_Exp(string, TTree*, string, string*, int, string);
RooWorkspace* FitArgusGauss(string, TTree*, string, string*, int, string);
RooWorkspace* FitB2KstIsoG(string, TTree*, string, string*, int, string);
RooWorkspace* FitNothing(string, TTree*, string, string*, int, string);
RooWorkspace* FitArgus_Gauss_Exp(string, TTree*, string, string*, int, string);
RooWorkspace* FitExp(string, TTree*, string, string*, int, string);
RooWorkspace* FitExp_Line(string, TTree*, string, string*, int, string);
RooWorkspace* FitLine(string, TTree*, string, string*, int, string);
RooWorkspace* DoFit(RooRealVar, TTree*, RooAbsPdf&, Constants&, Names&, string, string);
void MultiPlot(string, int, TTree**, string);
FitOption StringToFitOption(string);

//There is a clash here between what the pointer result is when this function is compiled or just read by the interpreter.
//As a workaround, this piece will only be read by the interpreter, so the functions needing this must #include "Functions/Fits.h"
//#if defined(__CINT__) && !defined(__MAKECINT__)
//I create an array of pointers to the fitting functions, so no need to change code in VarFit.C if we add more ;)
typedef RooWorkspace* (*FitFunction) (string variablename, TTree* chain, string w_var, string* act_vars, int N, string opts);

FitFunction* FitFunction_init(); //Init the array
//#endif

#endif

