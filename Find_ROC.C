#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void Find_ROC()
{
  ofstream fout;
  stringstream ss;
  ifstream fin;
  string basura = "";
  double ROC_area;

  fout.open("Roc_area.txt");
  for(int i=0;i<15*5**2*8;i++)
    {
      if(i%100==0)
	{
	  cout << "Reading file number " << i << endl;
	}
      ss.str("");
      ss << "log_I_" << i << ".txt";
      fin.open(ss.str().c_str());
      if(!fin)
	{
	  continue;
	}
      while(basura != "  NO.   NAME      VALUE            ERROR       STEP SIZE       VALUE   " && !fin.eof())
	{
	  getline(fin,basura);
	}
      if(!fin.eof())
	{
	  getline(fin, basura);
	  getline(fin, basura);
	  getline(fin, basura);
	  fin >> basura;
	  fin >> basura;
	  fin >> ROC_area;
	  fout << i << " | " << ROC_area;
	  getline(fin, basura);
	  fin >> basura;
	  fin >> basura;
	  fin >> ROC_area;
	  fout << " | " << ROC_area << endl;
	}
      else
	{
	  cout << "Error reading file number " << i << endl;
	}
      fin.close();
    }
  fout.close();
  return;
}
