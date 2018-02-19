#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "TH3F.h"
#include "TFile.h"
using namespace std;

void PIDTable(string filename, string particle, string PIDopt)
{
  TH3F* hist; //Histogram variable
  TFile* file = TFile::Open(filename.c_str());
  stringstream ss_i, ss_j;
  string histname;
  long entries[10][10];
  string operation = "(2+MC12TuneV2_ProbNNpi-MC12TuneV2_ProbNNK-MC12TuneV2_ProbNNp)_div_3";
  for(int i=0;i<1;i++)
    {
      for(int j=0;j<10;j++)//Loop 10x10 times
	{
	  ss_i.str("");
	  ss_j.str("");
	  ss_i << i;
	  ss_j << j;
	  histname = "PassedHist_" + particle + "_" + operation + " > 0." + ss_i.str() + " && " + operation + " > 0." + ss_j.str() + "_All__" + particle + "_P_" + particle + "_Eta_nTracks";
	  //cout << histname << endl;
	  hist = (TH3F*)file->Get(histname.c_str());
	  entries[i][j] = hist->GetEntries();
	}
    }
  
  for(int i=0;i<1;i++)
    {
      for(int j=0;j<10;j++)
	{
	  cout << setw(7) << endl;
	  cout << entries[i][j] << "  ";
	}
      cout << endl;
    }
}
