//File with constants declaration.
//When included in a file, you can use them directly

#ifndef Names_h
#define Names_h

#include <string>
using namespace std;

#define TAM 24
class Names
{
 public:
  Names(string ananame=""); //Default behaviour for other tasks
  void FillDefault();
  void Init(string ananame="");
  //Workspace components
  string workspace;
  string dataset;
  string pdfmodel[TAM];
  //PDFs
  string comppdf[TAM];
  //Gaussian parameters
  string mean[TAM];
  string width[TAM];
  //CB parameters
  string alpha;
  string alphaL[TAM];
  string alphaR[TAM];
  string n;
  string nL[TAM];
  string nR[TAM];
  //Argus parameters
  string m0_Argus;
  string c_Argus;
  string p_Argus;
  string width_Argus;
  //Linear parameters
  string slope;
  //Exponential parameters
  string exp_par[TAM];
  //Components fractions
  string fcomp[TAM];
  string yield[TAM];
  string yield_factor;
  //Number of bkgs
  string N_comps;
  //Mean shifts
  string shift[TAM];
  //RooYield Suffix
  string RooYield_suffix;
  //RooCategory name
  string catname;
};

#endif
