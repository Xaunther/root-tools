#include <iostream>
#include <fstream>
#include <string>
#include "../Functions/Filereading.h"
using namespace std;

#define Nresults 5

void OutputSearch(string filename)
{
  double results[Nresults];
  ifstream fichero;
  string basura;
  string linesearch1 = "--- Factory                  : Method:          @B=0.01    @B=0.10    @B=0.30    ROC-integ. | ration:  cance:   ";
  string linesearch2 = "  NO.   NAME      VALUE            ERROR       STEP SIZE       VALUE   ";
  string linesearch3 = "Processing root/Vodka.C+(\"BDT\")...";
  string linesearch4 = "";
  string linesearch5 = "";

  fichero.open(filename.c_str());
  AdvanceLine(fichero, linesearch1);
  getline(fichero, basura);
  fichero >> basura; fichero >> basura; fichero >> basura; fichero >> basura; fichero >> basura; fichero >> basura; fichero >> basura; fichero >> basura;
  fichero >> results[0];
  if(linesearch2!="")
    {
      AdvanceLine(fichero, linesearch2);
      getline(fichero, basura); getline(fichero, basura); getline(fichero, basura);
      fichero >> basura; fichero >> basura;
      fichero >> results[2];
      getline(fichero, basura);
      fichero >> basura; fichero >> basura;
      fichero >> results[1];
    }
  if(linesearch3!="")
    {
      AdvanceLine(fichero, linesearch3);
      fichero >> basura; fichero >> basura; fichero >> basura;
      fichero >> results[3];
      getline(fichero, basura);
      fichero >> basura; fichero >> basura; fichero >> basura;
      fichero >> results[4];      
    }
  if(linesearch4!="")
    {
      AdvanceLine(fichero, linesearch4);
      //Stuff
    }
  if(linesearch5!="")
    {
      AdvanceLine(fichero, linesearch5);
      //Stuff
    }

  fichero.close();

  //Append to file
  ofstream outfile;
  outfile.open("output.txt", std::ios_base::app);
  for(int i=0;i<Nresults;i++)
    {
      outfile << results[i] << " | ";
    }
  outfile << endl;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc)
    {
    case 1:
      OutputSearch(*(new string(argv[1])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for OutputSearch" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
