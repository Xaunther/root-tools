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
  int NCuts[Nvars] = {15,7,7,7};
  int n=0;
  string CutValue[Nvars][15]=
    {
      {"06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"},
      {"05", "1", "15", "2", "3", "4", "5", "0", "0", "0", "0", "0", "0", "0", "0"},
      {"05", "1", "15", "2", "3", "4", "5", "0", "0", "0", "0", "0", "0", "0", "0"},
      {"05", "1", "15", "2", "3", "4", "5", "0", "0", "0", "0", "0", "0", "0", "0"}
    };
  for(ii[0]=0;ii[0]<NCuts[0];ii[0]++)
    {
      for(ii[1]=0;ii[1]<NCuts[1];ii[1]++)
	{
	  for(ii[2]=0;ii[2]<NCuts[2];ii[2]++)
	    {
	      for(ii[3]=0;ii[3]<NCuts[3];ii[3]++)
		{
		  ssfile.str("");
		  ssfile << "Cuts/PIDCuts" << n << ".txt";
		  fout.open(ssfile.str().c_str());
		  fout << "BDT_response > 0." << CutValue[0][ii[0]] << endl
		       << "piminus_ProbNNpi*(1-piminus_ProbNNk)*(1-piminus_ProbNNp) > 0." << CutValue[1][ii[1]] << endl
		       << "piplus_ProbNNpi*(1-piplus_ProbNNk)*(1-piplus_ProbNNp) > 0." << CutValue[2][ii[2]] << endl
		       << "piplus0_ProbNNpi*(1-piplus0_ProbNNk)*(1-piplus0_ProbNNp) > 0." << CutValue[3][ii[3]] << endl;
		  n++;
		  fout.close();
		}
	    }
	}
    }
  return;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 4:
      Create_Cuts();
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for Create_Cuts" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
