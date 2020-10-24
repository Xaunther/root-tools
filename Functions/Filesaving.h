//Declaration of functions related to saving stuff in files

#ifndef Filesaving_h
#define Filesaving_h

#include <string>
#include "RooWorkspace.h"
#include "Functions/Fits.h"
using namespace std;

void SaveVariables(string*, int, string);
void SaveLatex(RooWorkspace*, FitOption, bool, string = "", string = "");
void SaveRooVars(RooWorkspace*, string);

#endif
