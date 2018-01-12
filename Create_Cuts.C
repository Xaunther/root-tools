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
  int ii[6];
  int i=0;
  int n=0;
  int CutValue[6][5]=
    {
      {832,837,842,847,852},
      {927,932,937,942,947},
      {3000,3500,4000,4500,5000},
      {486,488,490,492,494},
      {501,503,505,507,509},
      {9995,9997,9999,99992,99995}
    };
  for(ii[0]=0;ii[0]<5;ii[0]++)
    {
      for(ii[1]=0;ii[1]<5;ii[1]++)
	{
	  for(ii[2]=0;ii[2]<5;ii[2]++)
	    {
	      for(ii[3]=0;ii[3]<5;ii[3]++)
		{
		  for(ii[4]=0;ii[4]<5;ii[4]++)
		    {
		      for(ii[5]=0;ii[5]<5;ii[5]++)
			{
			  ssfile.str("");
			  ssfile << "Cuts/BDTCuts" << n << ".txt";
			  fout.open(ssfile.str().c_str());
			  fout << "Kst_892_plus_M > " << CutValue[0][ii[0]] <<" && Kst_892_plus_M < " << CutValue[1][ii[1]] << endl
			       << "B_PT > " << CutValue[2][ii[2]] << endl
			       << "KS0_M > " << CutValue[3][ii[3]] << " && KS0_M < " << CutValue[4][ii[4]] << endl
			       << "B_DIRA_OWNPV > 0." << CutValue[5][ii[5]] << endl;
			  n++;
			  fout.close();
			}
		    }
		}
	    }
	}
    }
  return;
}
