#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#define Nvars 4

void Create_Cuts()
{
  ofstream fout;
  stringstream ssfile;
  int ii[Nvars];
  int i=0;
  int n=0;
  int CutValue[Nvars][16]=
    {
      {15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
      {5, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {5, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {8, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0}
    };
  for(ii[0]=0;ii[0]<CutValue[0][0];ii[0]++)
    {
      for(ii[1]=0;ii[1]<CutValue[1][0];ii[1]++)
	{
	  for(ii[2]=0;ii[2]<CutValue[2][0];ii[2]++)
	    {
	      for(ii[3]=0;ii[3]<CutValue[3][0];ii[3]++)
		{
		  ssfile.str("");
		  ssfile << "Cuts/PIDCuts" << n << ".txt";
		  fout.open(ssfile.str().c_str());
		  if(CutValue[0][ii[0]+1]<10)
		    {
		      fout << "BDT_response > 0.0";
		    }
		  else
		    {
		      fout << "BDT_response > 0.";
		    }
		  fout << CutValue[0][ii[0]+1] << endl
		       << "piminus_ProbNNpi*(1-piminus_ProbNNk)*(1-piminus_ProbNNp) > 0." << CutValue[1][ii[1]+1] << endl
		       << "piplus_ProbNNpi*(1-piplus_ProbNNk)*(1-piplus_ProbNNp) > 0." << CutValue[2][ii[2]+1] << endl
		       << "piplus0_ProbNNpi*(1-piplus0_ProbNNk)*(1-piplus0_ProbNNp) > 0." << CutValue[3][ii[3]+1] << endl;
		  n++;
		  fout.close();
		}
	    }
	}
    }
  return;
}
