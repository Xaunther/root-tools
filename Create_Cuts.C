#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void Create_Cuts()
{
  ofstream fout;
  stringstream ssfile;
  int ii[4];
  int i=0;
  int n=0;
  int CutValue[4][16]=
    {
      {15, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
      {5, 1, 2, 4, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {5, 1, 2, 4, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {5, 1, 2, 4, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
		  fout << "BDT_response > 0." << CutValue[0][ii[0]+1] << endl
		       << "piminus_ProbNNpi*(1-piminus_ProbNNp)*(1-piminus_ProbNNk) > 0." << CutValue[1][ii[1]+1] << endl
		       << "piplus_ProbNNpi*(1-piplus_ProbNNp)*(1-piplus_ProbNNk) > 0." << CutValue[2][ii[2]+1] << endl
		       << "piplus0_ProbNNpi*(1-piplus0_ProbNNp)*(1-piplus0_ProbNNk) > 0." << CutValue[3][ii[3]+1] << endl;
		  n++;
		  fout.close();
		}
	    }
	}
    }
  return;
}
