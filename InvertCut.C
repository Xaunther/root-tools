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
  if (cuts[pos].find(">") != string::npos)
  {
    cuts[pos].replace(cuts[pos].find(">"), 1, "<");
  }
  else if (cuts[pos].find("<") != string::npos)
  {
    cuts[pos].replace(cuts[pos].find("<"), 1, ">");
  }
  //Write cuts into file!
  SaveVariables(cuts, N, fileout);
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 3:
    InvertCut(*(new string(argv[1])), *(new string(argv[2])), stoi(*(new string(argv[3]))));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
