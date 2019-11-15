//Script to read some put in output the value of some variable, saved in a text file with structure:
//varname = varvalue

#include <string>
#include <iostream>
#include <fstream>
#include "../Functions/Dictreading.h"
using namespace std;

void WriteFromDic(string dicfilename, string varname, string outfilename);
void WriteFromDic(string dicfilename, string varname, string outfilename)
{
  //Write requested value in output file
  ofstream outf;
  outf.open(outfilename.c_str());
  outf << GetValueFor(varname, dicfilename) << " +- " << GetErrorFor(varname, dicfilename) << endl;
  outf.close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 3:
      WriteFromDic(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for WriteFromDic" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
