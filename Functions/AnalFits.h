#ifndef AnalFits_h
#define AnalFits_h

#include <string>
#include "TTree.h"
#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "Functions/Fits.h"
#include "Dictionaries/Constants.h"
#include "Dictionaries/Names.h"
using namespace std;

//Funcions for special massfits for the analysis. ANALysis FITS :)
//This way, Fits contains more general functions and more specific fits can go in a separate file

//Modularized function to define parameter list
string* Create_Parlist_NstG(FitOption, Names*, Constants*, int, int&);
//Create corresponding pdf component
RooAbsPdf* Create_FitPDF_NstG(FitOption, string, RooRealVar*, RooAbsReal**);
//Massfit for Lb2NstG analysis
RooWorkspace* FitLb2NstG(string, TTree*, string);
RooWorkspace* FitLb2NstG_Simult(string*, TTree**, string);
RooWorkspace* FitLb2NstG_Kpi(string, TTree*, string);
RooWorkspace* FitLb2NstG_pK(string, TTree*, string);
RooWorkspace* FitLb2ppiJPsi(string, TTree*, RooWorkspace*, string, string*, int, string, FitOption*, int);
RooWorkspace* FitLb2pKJPsi(string, TTree*, string, string*, int, string);
RooWorkspace* FitLb2NstG_Kpi_Wrong(string, TTree*, string);
RooWorkspace* FitLb2NstG_pK_Wrong(string, TTree*, string);

#endif
