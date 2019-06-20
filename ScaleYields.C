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
      nn = 0;
      yield[i] = stod(ReadVariables(nn, yieldfname[i])[0]);
      sum += yield[i];
    }
  //Read the new total sum
  double newsum = stod(ReadVariables(nn, totyieldfile)[0]);
  //Rescale and save
  ofstream outf;
  for (int i=0;i<N_files;i++)
    {
      yield[i] *= newsum/sum;
      outf.open((yieldfname[i]+suffix).c_str());
      outf << yield[i];
      outf.close();
    }
}
