//Another script.... COMEON
//So, this script simply takes a bunch of files containing yields, one more file containing the total yields I want to have, and rescales the list of yields so the new sum of yields now evaluates to the total yield I want
//The list of yields at the end it saved in a different file, which name is the original filename + the suffix. EZY

#include <string>
#include <iostream>
#include <fstream>
#include "../Functions/StringTools.h"
#include "../Functions/Filereading.h"
using namespace std;

void ScaleYields(string yieldlist, string totyieldfile, string suffix = "_Rescaled", string separator = " ");
void ScaleYields(string yieldlist, string totyieldfile, string suffix, string separator)
{
  //Get list of original yield files
  int N_files = 0;
  string* yieldfname = SplitString(N_files, yieldlist, separator);
  //Prepare sum to rescale
  double sum = 0;
  int nn;
  //Prepare array of yields
  double* yield = new double[N_files];
  for(int i=0;i<N_files;i++)
    {
      //Read each file, get the number
      yield[i] = stod(ReadVariablesWord(nn, yieldfname[i], 1)[0]);
      sum += yield[i];
    }
  //Read the new total sum
  double newsum = stod(ReadVariablesWord(nn, totyieldfile, 0)[0]);
  //Rescale and save
  ofstream outf;
  for (int i=0;i<N_files;i++)
    {
      yield[i] *= newsum/sum;
      outf.open((yieldfname[i]+suffix).c_str());
      outf << yield[i] << endl;
      outf.close();
    }

    //Delete heap
    delete[] yieldfname;
    delete[] yield;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 2:
      ScaleYields(*(new string(argv[1])), *(new string(argv[2])));
      break;
    case 3:
      ScaleYields(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    case 4:
      ScaleYields(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for ScaleYields" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
