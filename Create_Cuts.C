#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#define Nvars 6

void Create_Cuts()
{
  ofstream fout;
  stringstream ssfile;
  int ii[Nvars];
  int i=0;
  int n=0;
  int CutValue[Nvars][6]=
    {
      {5, 832, 837, 842, 847, 852},
      {5, 927, 932, 937, 942, 947},
      {5, 3000, 3400, 3700, 4000, 4400},
      {5, 486, 488, 490, 492, 494},
      {5, 501, 503, 505, 507, 509},
      {3, 5, 6, 7}
    };
  for(ii[0]=0;ii[0]<CutValue[0][0];ii[0]++)
    {
      for(ii[1]=0;ii[1]<CutValue[1][0];ii[1]++)
	{
	  for(ii[2]=0;ii[2]<CutValue[2][0];ii[2]++)
	    {
	      for(ii[3]=0;ii[3]<CutValue[3][0];ii[3]++)
		{
		  for(ii[4]=0;ii[4]<CutValue[4][0];ii[4]++)
		    {
		      for(ii[5]=0;ii[5]<CutValue[5][0];ii[5]++)
			{
			  ssfile.str("");
			  ssfile << "Cuts/PIDCuts" << n << ".txt";
			  fout.open(ssfile.str().c_str());
			  fout << "piplus_TRACK_Type == 3" << endl
			       << "piminus_TRACK_Type == 3" << endl
			       << "B_M > 4300 && B_M < 6300" << endl
			       << "Kst_892_plus_M > " << CutValue[0][ii[0]+1] <<" && Kst_892_plus_M < " << CutValue[1][ii[1]+1] << endl
			       << "B_PT > " << CutValue[2][ii[2]+1] << endl
			       << "KS0_M > " << CutValue[3][ii[3]+1] << " && KS0_M < " << CutValue[4][ii[4]+1] << endl
			       << "B_DIRA_OWNPV > 0.999" << CutValue[5][ii[5]+1] << endl;
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
