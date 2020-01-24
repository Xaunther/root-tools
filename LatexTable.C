#include <string>
#include "RooWorkspace.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "../Functions/Fits.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/Filesaving.h"
#include "../Functions/Dictreading.h"
using namespace std;

//Does the same as VarFit, but making a latextable with the result instead of a plot
void LatexTable(string variablename, FitOption fitopt, string filedir, bool newfile, string cutfile = "", string w_var = "", string outfile = "", string col1 = "", string opts = "")
{
  FitFunction* fitf = FitFunction_init();
  RooWorkspace* ws = new RooWorkspace();

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
  if (opts == "")
  {
    opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
  }
  ws = fitf[fitopt](variablename, temptree, w_var, 0, 0, opts);
  SaveLatex(ws, fitopt, newfile, col1, outfile);
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  bool newfile = false;
  FitOption fitopt = StringToFitOption(*(new string(argv[2])));
  switch (argc - 1)
  {
  case 4:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {newfile = true;}
    LatexTable(*(new string(argv[1])), fitopt, *(new string(argv[3])), newfile);
    break;
  case 5:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {newfile = true;}
    LatexTable(*(new string(argv[1])), fitopt, *(new string(argv[3])), newfile, *(new string(argv[5])));
    break;
  case 6:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {newfile = true;}
    LatexTable(*(new string(argv[1])), fitopt, *(new string(argv[3])), newfile, *(new string(argv[5])), *(new string(argv[6])));
    break;
  case 7:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {newfile = true;}
    LatexTable(*(new string(argv[1])), fitopt, *(new string(argv[3])), newfile, *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
  case 8:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {newfile = true;}
    LatexTable(*(new string(argv[1])), fitopt, *(new string(argv[3])), newfile, *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
               *(new string(argv[8])));
    break;
  case 9:
    if (*(new string(argv[4])) == "true" || *(new string(argv[4])) == "1") {newfile = true;}
    LatexTable(*(new string(argv[1])), fitopt, *(new string(argv[3])), newfile, *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
               *(new string(argv[8])), *(new string(argv[9])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
