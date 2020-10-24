#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooAddPdf.h"
#include "Functions/Constantize.h"
#include "Dictionaries/Names.h"
#include "Functions/Dictreading.h"
using namespace std;

void Constantize(RooWorkspace* ws)
{
  //Initialize constants
  Names name_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  //Retrieve stuff from workspace
  //Number of backgrounds
  int N_bkgs = ((RooAddPdf*)ws->pdf(name_list.pdfmodel[0].c_str()))->pdfList().getSize();
  //I just need a RooRealVar to go through the parameters
  RooRealVar* param = new RooRealVar;
  //Parameters that are not part of array
  param = ws->var(name_list.alpha.c_str());
  if(param!=0){param->setConstant();} //Constantize them
  param = ws->var(name_list.n.c_str());
  if(param!=0){param->setConstant();} //Constantize them
  //Variables that have names that are in array
  for(int i=0;i<N_bkgs+1;i++)
    {
      param = ws->var(name_list.mean[i].c_str());
      if(param!=0){param->setConstant();} //Constantize them
      param = ws->var(name_list.exp_par[i].c_str());
      if(param!=0){param->setConstant();} //Constantize them
      param = ws->var(name_list.width[i].c_str());
      if(param!=0){param->setConstant();} //Constantize them
      param = ws->var(name_list.alphaL[i].c_str());
      if(param!=0){param->setConstant();} //Constantize them
      param = ws->var(name_list.alphaR[i].c_str());
      if(param!=0){param->setConstant();} //Constantize them
      param = ws->var(name_list.nL[i].c_str());
      if(param!=0){param->setConstant();} //Constantize them
      param = ws->var(name_list.nR[i].c_str());
      if(param!=0){param->setConstant();} //Constantize them
    }
}
