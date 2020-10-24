//Implementation of Extract_Var function
#include <string>
#include "Functions/Extract_Var.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"

using namespace std;
//Pick up the parameter requested (name_0) and add it to RooWorkspace with the name provided (name_f)
void Extract_Var(RooWorkspace* ws, RooWorkspace* Param_ws, string name_0, string name_f)
{
  RooRealVar* dummy = new RooRealVar(name_f.c_str(), name_f.c_str(),
				     ws->var(name_0.c_str())->getValV(), ws->var(name_0.c_str())->getValV(), ws->var(name_0.c_str())->getValV());
  Param_ws->import(*dummy);
  return;
}
