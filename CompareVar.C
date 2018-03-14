#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLeaf.h"
#include "TChain.h"
#include "../Functions/misc.h"
#include "../Functions/Fits.h"
using namespace std;

void CompareVar(string variablename, string filedir1, string filedir2 = "", string filedir3 = "", string filedir4 = "", string filedir5 = "", string filedir6 = "", string filedir7 = "", string filedir8 = "", string filedir9 = "", string filedir10 = "")
{
  int N_files[] = {0};
  int N_used;
  string* filedir = new string[10];
  filedir[0] = filedir1;
  filedir[1] = filedir2;
  filedir[2] = filedir3;
  filedir[3] = filedir4;
  filedir[4] = filedir5;
  filedir[5] = filedir6;
  filedir[6] = filedir7;
  filedir[7] = filedir8;
  filedir[8] = filedir9;
  filedir[9] = filedir10;

  string** filenames = new string*[10];
  //Load TChain
  for(int i=0;i<10;i++)
    {
      N_used = i;
      if(filedir[i]!="")
	{
	  filenames[i] = ReadVariables(N_files[i], filedir[i]);
	}
      else
	{
	  break;
	}
      
    }
  //Define for exact number now
  string* treenames = new string[N_used];  
  TChain** chains = new TChain*[N_used];
  TTree** trees = new TTree*[N_used];
  
  for(int i=0;i<N_used;i++)
    {
      treenames[i] = GetTreeName(filedir[0]);
      for(int j=0;j<N_files[i];j++)
	{
	  chains[i]->Add(filenames[i][j].c_str());
	  cout << j+1 << " file(s) chained" << endl;
	}
      trees[i] = (TTree*)chains[i]->CopyTree("");
    }

  //Cut chain into new TChain in a temp root file

  //Do fit depending on request
  MultiPlot(variablename, N_used, trees);
  //Leave open
  //  tempfile1->Close();
  //  tempfile2->Close();
}
