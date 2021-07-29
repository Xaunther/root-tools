//Spin-off of AnalFits.h containing simultaneous fits used in NstG analysis
#include <string>
#include "TTree.h"
#include "RooWorkspace.h"
using namespace std;

RooWorkspace *FitLb2NstG_Simult(string *, TTree **, string);