#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TFile.h"
#include "../Functions/Filereading.h"
using namespace std;

void PIDTable(string filedir, string resultsfile = "PIDEff.txt")
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
  
  ofstream outfile;
  outfile.open(resultsfile.c_str());
  //Loop over all files in filedir
  for(int i=0;i<NFiles;i++)
    {
      TFile* tuples_file = TFile::Open((filepath+tuple_names[i]).c_str());
      TTree* tree = (TTree*)tuples_file->Get("CalibTool_PIDCalibTree");
      int entries = tree->GetEntries();
      double sum = 0;
      float entry;
      tree->SetBranchAddress("Event_PIDCalibEff", &entry);
      for(int j=0;j<entries;j++)
	{
	  tree->GetEntry(j);
	  sum+=entry;
	}
      outfile << tuple_names[i] << " | " << sum/double(entries) << endl;
      cout << "Processed " << tuple_names[i] << endl;
      tuples_file->Close();
    }
  outfile.close();
}
