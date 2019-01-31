//File to invert the cut number pos. Comes in handy for N*gam analysis
#include <string>
#include "../Functions/StringTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/Filesaving.h"
using namespace std;
void InvertCut(string filein, string fileout, int pos)
{
  //Read cuts into array
  int N = 0;
  string* cuts = ReadVariables(N, filein);
  //Substitute
  if(cuts[pos].find(">")!=string::npos)
    {
      cuts[pos].replace(cuts[pos].find(">"), 1, "<");
    }
  else if(cuts[pos].find("<")!=string::npos)
    {
      cuts[pos].replace(cuts[pos].find("<"), 1, ">");
    }
  //Write cuts into file!
  SaveVariables(cuts, N, fileout);
}
