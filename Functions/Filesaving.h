//Declaration of functions related to saving stuff in files

#ifndef Filesaving_h
#define Filesaving_h

#include <string>
#include <fstream>
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "Functions/Fits.h"
#include "TMath.h"
using namespace std;

void SaveVariables(string *, int, string);
void SaveLatex(RooWorkspace *, FitOption, bool, string = "", string = "");
void SaveRooVars(RooWorkspace *ws, string outname, RooFitResult *result = nullptr);
void SaveFitResult(ofstream &, RooFitResult *);
#endif
