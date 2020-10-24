#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Functions/Dictreading.h"
using namespace std;

string GetValueFor(string varname, string filename)
{
  return GetColFor(varname, filename, 2);
}

string GetErrorFor(string varname, string filename)
{
  return GetColFor(varname, filename, 4);
}

//Return word in column col, row is decided by the seached word, varname, in the first column (index zero)
string GetColFor(string varname, string filename, int col)
{
  //Structure of the file must be varname = varvalue +- varerror
  ifstream infile;
  string varread = "";
  string varvalue;
  infile.open(filename.c_str());
  if(!infile){return "";}
  while(!infile.eof())
    {
      infile >> varread;
      if(varread==varname)
	{
	  varvalue=varread;
	  for(int i=0;i<col;i++){infile >> varvalue;}
	  infile.close();
	  return varvalue;
	}
      else //Go to next line
	{
	  getline(infile, varvalue);
	}
    }
  cout << "WARNING: " << varname << " not found. Returning empty string" << endl;
  infile.close();
  return "";
}
string GetTreeName(string filedir, string defaultname)
{
	string treename;
	treename = StringSearch("Dictionaries/treenames.dic", filedir, 0, 1);
	if(treename == "")
	{
		treename = defaultname;
	}
	return treename;
}

string StringSearch(string filename, string searched, int searchcol, int returncol)
{
	ifstream infile;
	string result = "";
	int Ncol;

	infile.open(filename.c_str());
	if(!infile){return result;}
	infile >> Ncol;
	if(Ncol==0){return result;}
	string* row = new string[Ncol];  
	while(result=="" && infile.eof() == false)
	{
		for(int i=0;i<Ncol;i++)
		{
			infile >> row[i];
		}
		if(row[searchcol]==searched)
		{
			result = row[returncol];
		}
	}
	infile.close();
	return result;
}

