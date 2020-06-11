//This simple script reads a saved rooworkspace and writes the value of some variable in an output file
//The name of the workspace inside the file can either be pointed out directly or taken from the dictionary

#include <string>
#include <iostream>
#include <fstream>
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "TFile.h"
#include "../Functions/Dictreading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/StringTools.h"
#include "../Dictionaries/Names.h"
using namespace std;

void WriteFromWS(string wsfilename, string varname, string outfilename, string wsname = "", string opts = "");
void WriteFromWS(string wsfilename, string varname, string outfilename, string wsname, string opts)
{
  //First, if workspace name was not provided load the dictionary
  if (wsname == "")
  {
    //If dictioanry opts was not specified, load default for the project
    if (opts == "")
    {
      opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
    }
    Names name_list(opts);
    wsname = name_list.workspace;
  }
  //Read file and its ws
  TFile* f_ws = new TFile(Gridify(wsfilename).c_str());
  cout << " caca " << endl;
  RooWorkspace* ws = (RooWorkspace*)f_ws->Get(wsname.c_str());
  //Write requested value in output file
  ofstream outf;
  outf.open(outfilename.c_str());
  outf << ws->var(varname.c_str())->getValV();
  outf.close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 3:
    WriteFromWS(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    WriteFromWS(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    WriteFromWS(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
