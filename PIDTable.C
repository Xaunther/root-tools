#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
using namespace std;

void PIDTable(string filedir, string resultsfile = "PIDEff.txt", bool abspath = false, string varname = "Event_PIDCalibEff")
{
  //Read filedir
  int NFiles = 0;
  string* tuple_names = ReadVariables(NFiles, filedir);


  //Get absolute path (Maybe make a function of this?)
  int last_pos = 0;
  while(filedir.find('/', last_pos+1)!=string::npos)
    {
      last_pos = filedir.find('/', last_pos+1);
    }
  string filepath = filedir.substr(0, last_pos+1);
  if(abspath)
    {
      filepath = "";
    }
  ofstream outfile;
  outfile.open(resultsfile.c_str());
  //Loop over all files in filedir
  for(int i=0;i<NFiles;i++)
    {
      //Deal with empty files
      if(TreeExists(tuple_names[i],"CalibTool_PIDCalibTree"))
	{
	  outfile << tuple_names[i] << " | " << TMath::Abs(GetMean(tuple_names[i], varname, "CalibTool_PIDCalibTree")) << endl;
	}
      else
	{
	  outfile << tuple_names[i] << " | 0." << endl;	  
	}
      cout << "Processed " << tuple_names[i] << endl;
    }
  outfile.close();
}
