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

void PIDTable(string filedir, string resultsfile = "PIDEff.txt", bool abspath = false, string varname = "Event_PIDCalibEff", string wfile = "", string weight = "1")
{
  //Read filedir
  int NFiles = 0;
  string* tuple_names = ReadVariables(NFiles, filedir);


  //Get absolute path (Maybe make a function of this?)
  int last_pos = 0;
  while (filedir.find('/', last_pos + 1) != string::npos)
  {
    last_pos = filedir.find('/', last_pos + 1);
  }
  string filepath = filedir.substr(0, last_pos + 1);
  if (abspath)
  {
    filepath = "";
  }
  ofstream outfile;
  outfile.open(resultsfile.c_str());
  //Usual procedure when no weights are needed
  if (weight == "1")
  {
    //Loop over all files in filedir
    for (int i = 0; i < NFiles; i++)
    {
      //Deal with empty files
      if (TreeExists(tuple_names[i], "CalibTool_PIDCalibTree"))
      {
        outfile << tuple_names[i] << " | " << TMath::Abs(GetMeanEntries(tuple_names[i], "CalibTool_PIDCalibTree", varname)) << endl;
      }
      else
      {
        outfile << tuple_names[i] << " | 0." << endl;
      }
      cout << "Processed " << tuple_names[i] << endl;
    }
  }
  //If weights are needed, these are stored in a different ntuple with the same number of entries. Since the pidcalib ntuples just contain the pid info...
  //So we manually compute the efficiency
  else if (TreeExists(wfile))
  {
    //Read ntuple with weights
    TChain* wchain = GetChain(wfile, "", false);
    //Loop over all files in filedir
    for (int i = 0; i < NFiles; i++)
    {
      //Deal with empty files
      if (TreeExists(tuple_names[i], "CalibTool_PIDCalibTree"))
      {
        //Read PID ntuple
        TChain* chain = GetChain(tuple_names[i], "CalibTool_PIDCalibTree", false);
        //Setup branches and sums
        float w, eff;
        wchain->SetBranchAddress(weight.c_str(), &w);
        chain ->SetBranchAddress(varname.c_str(), &eff);
        double sumf = 0.;
        double sum0 = 0.;
        //Loop over entries and make the sum
        for (int j = 0; j < chain->GetEntries(); j++)
        {
          chain ->GetEntry(j);
          wchain->GetEntry(j);
          sumf += w * eff;
          sum0 += w;
        }
        outfile << tuple_names[i] << " | " << sumf / sum0 << endl;
        delete chain;
      }
      else
      {
        outfile << tuple_names[i] << " | 0." << endl;
      }
      cout << "Processed " << tuple_names[i] << endl;
    }
    delete wchain;
  }
  outfile.close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  bool abspath = false;
  switch (argc - 1)
  {
  case 1:
    PIDTable(*(new string(argv[1])));
    break;
  case 2:
    PIDTable(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {abspath = true;}
    PIDTable(*(new string(argv[1])), *(new string(argv[2])), abspath);
    break;
  case 4:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {abspath = true;}
    PIDTable(*(new string(argv[1])), *(new string(argv[2])), abspath, *(new string(argv[4])));
    break;
  case 5:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {abspath = true;}
    PIDTable(*(new string(argv[1])), *(new string(argv[2])), abspath, *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {abspath = true;}
    PIDTable(*(new string(argv[1])), *(new string(argv[2])), abspath, *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
