#include "TMath.h"
#include "Dictionaries/Constants.h"
#include "Functions/Dictreading.h"
#include "Functions/Interpol_exp.h"
using namespace std;

double Interpol_exp(int N_LM, int N_HM)
{
  //Initialize constants
  Constants const_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  double N_bkg;
  //Interpolate from sidebands
  if(N_HM != 0 && N_LM != 0 && N_HM != N_LM)
    {
      N_bkg = double(N_LM)*(1-TMath::Power(double(N_LM)/double(N_HM),(const_list.xmin-const_list.xmax)/(const_list.xHM-const_list.xmin)))/(1-TMath::Power(double(N_LM)/double(N_HM),(const_list.xmin-const_list.xLM)/(const_list.xHM-const_list.xmin)));
    }
  else if(N_HM != 0 && N_LM != 0)
    {
      N_bkg = N_LM/double(const_list.xLM-const_list.xmin)*(const_list.xmax-const_list.xmin);
    }
  else
    {
      N_bkg = N_HM + N_LM;
    }
  
  return N_bkg;
}

