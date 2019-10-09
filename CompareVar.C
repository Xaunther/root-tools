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
#include "../Functions/Fits.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Functions/StringTools.h"
using namespace std;

void CompareVar(string variablename, string filedir1, string opts)
{
  int N_files[] = {0};
  int N_used;
  string* filedir = SplitString(N_used, filedir1);
  string** filenames = new string*[N_used];
  //Load TChain
  for(int i=0;i<N_used;i++)
    {
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
  //Options
  if(opts=="")
    {
      opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
    }
  MultiPlot(variablename, N_used, trees, opts);
  //Leave open
  //  tempfile1->Close();
  //  tempfile2->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 3:
      CompareVar(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for CompareVar" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
