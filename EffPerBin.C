//Take a ntuple, or a file containing a list of them, and calculate the efficiency of each cut given in cutfile, starting from a previous set of cuts, precutfile
//The output is given by outfile. Event weights can be included using weight

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "TChain.h"
#include <sstream>
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/ArrayTools.h"
#include "../Functions/StringTools.h"
using namespace std;

void EffPerBin(string dirfile, string cutfile, string varname, string binfile, string outfile, string plotfile, string precutfile = "1", string weight = "1")
{
  //Get the cuts and the precuts
  string cuts = GetCuts(cutfile);
  string precuts = GetCuts(precutfile);

  //Get the tree, it might use some precuts
  TChain* chain = GetChain(dirfile);

  //Get the binning!
  int NBins=0;
  double* binning = ReadNumbers(NBins, binfile);
  //Construct the array for bin-efficiency
  double* bineff = new double[NBins-1];
  //Declare array where we'll save the final number of events for each cut, as well as the initial number
  for(int i=0;i<NBins-1;i++)
  {
    stringstream bincut;
    bincut << " * (" << varname << " > " << binning[i] << ") * (" << varname << " < " << binning[i+1] << ")";
    bineff[i] = GetMeanEntries(chain, cuts+bincut.str(), weight)/GetMeanEntries(chain, precuts+bincut.str(), weight);
  }

  //Save array of efficiencies in desired file
  ofstream fout;
  fout.open(outfile.c_str());
  for(int i=0;i<NBins-1;i++)
  {
    cout << (binning[i]+binning[i+1])/2. << "  |  " << bineff[i] << endl;
  }

  //Close files and clean memory
  fout.close();
  CloseChain(chain);
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
  {
    case 6:
    EffPerBin(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
    case 7:
    EffPerBin(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
    case 8:
    EffPerBin(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])));
    break;
    default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return(1);
    break;
  }
  return 0;
}
#endif
