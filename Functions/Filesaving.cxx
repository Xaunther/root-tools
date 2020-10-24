//Implementation of functions related to saving stuff in files

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "TIterator.h"
#include "Functions/Filesaving.h"
#include "Functions/Dictreading.h"
#include "Functions/ErrorTools.h"
#include "Dictionaries/Names.h"
using namespace std;

void SaveVariables(string* varnames, int N, string filename)
{
  ofstream f;
  f.open(filename.c_str());
  for (int i = 0; i < N; i++)
  {
    f << varnames[i] << endl;
  }
  cout << "Array saved in " << filename << endl;
  f.close();
  return;
}

//Partially implemented, only for fitopt = DoubleCB
void SaveLatex(RooWorkspace* ws, FitOption fitopt, bool newfile, string col1, string outfile)
{
  //Set outfile if no name is given
  if (outfile == "") {outfile = "LatexTable.tex";}
  //Open file
  ofstream f;
  //Initialize name dictionary
  Names name_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  if (newfile) {f.open(outfile.c_str());} else {f.open(outfile.c_str(), ios_base::app);}
  switch (fitopt)
  {
  case DoubleCB:
    if (newfile)
    {
      f << "\\begin{table}" << endl;
      f << "    \\begin{center}" << endl;
      f << "      \\begin{tabular}{c c c c c c c}" << endl;
      f << "        \\hline" << endl;
      f << "        Mode & $\\mu$ (\\mev) & $\\sigma$ (\\mev) & $\\alpha_L$ & $\\alpha_R$ & $n_L$ & $n_R$\\\\\\hline" << endl;
    }
    f << "        " << col1;
    f << std::fixed;
    f << setprecision(GetErrPrecision(ws->var(name_list.mean[0].c_str())->getError()));
    f << " & $" << ws->var(name_list.mean[0].c_str())->getValV() << "\\pm" << ws->var(name_list.mean[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.width[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.width[0].c_str())->getValV() << "\\pm" << ws->var(name_list.width[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.alphaL[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.alphaL[0].c_str())->getValV() << "\\pm" << ws->var(name_list.alphaL[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.alphaR[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.alphaR[0].c_str())->getValV() << "\\pm" << ws->var(name_list.alphaR[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.nL[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.nL[0].c_str())->getValV() << "\\pm" << ws->var(name_list.nL[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.nR[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.nR[0].c_str())->getValV() << "\\pm" << ws->var(name_list.nR[0].c_str())->getError() << "$\\\\" << endl;
    break;
  case CBExp:
    if (newfile)
    {
      f << "\\begin{table}" << endl;
      f << "    \\begin{center}" << endl;
      f << "      \\begin{tabular}{c c c c c c}" << endl;
      f << "        \\hline" << endl;
      f << "        Mode & $\\mu$ (\\mev) & $\\sigma$ (\\mev) & $\\alpha_L$ & $\\alpha_R$ & $n$\\\\\\hline" << endl;
    }
    f << "        " << col1;
    f << std::fixed;
    f << setprecision(GetErrPrecision(ws->var(name_list.mean[0].c_str())->getError()));
    f << " & $" << ws->var(name_list.mean[0].c_str())->getValV() << "\\pm" << ws->var(name_list.mean[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.width[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.width[0].c_str())->getValV() << "\\pm" << ws->var(name_list.width[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.alphaL[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.alphaL[0].c_str())->getValV() << "\\pm" << ws->var(name_list.alphaL[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.alphaR[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.alphaR[0].c_str())->getValV() << "\\pm" << ws->var(name_list.alphaR[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.n.c_str())->getError()));
    f << "$ & $" << ws->var(name_list.n.c_str())->getValV() << "\\pm" << ws->var(name_list.n.c_str())->getError() << "$\\\\" << endl;
    break;
  case GaussExp:
    if (newfile)
    {
      f << "\\begin{table}" << endl;
      f << "    \\begin{center}" << endl;
      f << "      \\begin{tabular}{c c c c}" << endl;
      f << "        \\hline" << endl;
      f << "        Mode & $\\mu$ (\\mev) & $\\sigma$ (\\mev) & $\\alpha$\\\\\\hline" << endl;
    }
    f << "        " << col1;
    f << std::fixed;
    f << setprecision(GetErrPrecision(ws->var(name_list.mean[0].c_str())->getError()));
    f << " & $" << ws->var(name_list.mean[0].c_str())->getValV() << "\\pm" << ws->var(name_list.mean[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.width[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.width[0].c_str())->getValV() << "\\pm" << ws->var(name_list.width[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.alpha.c_str())->getError()));
    f << "$ & $" << ws->var(name_list.alpha.c_str())->getValV() << "\\pm" << ws->var(name_list.alpha.c_str())->getError() << "$\\\\" << endl;
    break;
  case DoubleGaussExp:
    if (newfile)
    {
      f << "\\begin{table}" << endl;
      f << "    \\begin{center}" << endl;
      f << "      \\begin{tabular}{c c c c c}" << endl;
      f << "        \\hline" << endl;
      f << "        Mode & $\\mu$ (\\mev) & $\\sigma$ (\\mev) & $\\alpha_L$ & $\\alpha_R$\\\\\\hline" << endl;
    }
    f << "        " << col1;
    f << std::fixed;
    f << setprecision(GetErrPrecision(ws->var(name_list.mean[0].c_str())->getError()));
    f << " & $" << ws->var(name_list.mean[0].c_str())->getValV() << "\\pm" << ws->var(name_list.mean[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.width[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.width[0].c_str())->getValV() << "\\pm" << ws->var(name_list.width[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.alphaL[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.alphaL[0].c_str())->getValV() << "\\pm" << ws->var(name_list.alphaL[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.alphaR[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.alphaR[0].c_str())->getValV() << "\\pm" << ws->var(name_list.alphaR[0].c_str())->getError() << "$\\\\" << endl;
    break;
  case Gauss:
    if (newfile)
    {
      f << "\\begin{table}" << endl;
      f << "    \\begin{center}" << endl;
      f << "      \\begin{tabular}{c c c}" << endl;
      f << "        \\hline" << endl;
      f << "        Mode & $\\mu$ (\\mev) & $\\sigma$ (\\mev)\\\\\\hline" << endl;
    }
    f << "        " << col1;
    f << std::fixed;
    f << setprecision(GetErrPrecision(ws->var(name_list.mean[0].c_str())->getError()));
    f << " & $" << ws->var(name_list.mean[0].c_str())->getValV() << "\\pm" << ws->var(name_list.mean[0].c_str())->getError();
    f << setprecision(GetErrPrecision(ws->var(name_list.width[0].c_str())->getError()));
    f << "$ & $" << ws->var(name_list.width[0].c_str())->getValV() << "\\pm" << ws->var(name_list.width[0].c_str())->getError() << "$\\\\" << endl;
    break;
  default: //Other
    cout << "Option not implemented at SaveLatex function (../Functions/Filesaving.cxx)" << endl;
  }
  f.close();
}

//Save list of yields in a file
void SaveRooVars(RooWorkspace* ws, string outname)
{
  ofstream f;
  f.open(outname.c_str());
  RooArgSet varlist = ws->allVars();
  TIterator* it = varlist.createIterator();
  RooRealVar* varit;
  while ((varit = (RooRealVar*)it->Next()))
  {
    f << varit->GetName() << " = " << varit->getValV() << " +- " << varit->getError() << endl;
  }
  f.close();
}
