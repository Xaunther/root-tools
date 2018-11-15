#include <string>
#include "RooWorkspace.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "../Functions/Fits.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/Filesaving.h"
using namespace std;

//Does the same as VarFit, but making a latextable with the result instead of a plot
void LatexTable(string variablename, FitOption fitopt, string filedir, bool newfile, string cutfile = "", string w_var = "", string outfile = "", string col1 = "")
{
  FitFunction* fitf = FitFunction_init();
  RooWorkspace* ws = new RooWorkspace();

  int N_files = 0;
  string* filenames = ReadVariables(N_files, filedir);
  //Load TChain
  string cuts = GetCuts(cutfile);

  TChain* chain = GetChain(filedir);
  TTree* temptree = new TTree();
  TFile* tempfile = new TFile();

  //Cut chain into new TChain in a temp root file
  tempfile = new TFile("Tuples/temp.root", "recreate");
  temptree = (TTree*)chain->CopyTree(cuts.c_str());
  tempfile->Write();

  cout << "Will fit " << temptree->GetEntries() << " events" << endl << endl;

  //Do fit depending on request
  ws = fitf[fitopt](variablename, temptree, w_var, 0, 0);
  SaveLatex(ws, fitopt, newfile, col1, outfile);
}
