//Script to read the efficiencies created from a series of PIDTable scripts.
//Then read the efficiencies for each channel for the previous cuts (kinematic, vetoes...) Produced by CutEff
//Will allow for the user to provide a string with the initiali expected yield (space separated)
//Will then combine initial yields with all the efficiencies to get the final yields for each
//Compute Punzi figure of merit (with sigma=5 as standard) and first channel will be treated as signal (so N-2 initial yields need to be provided)
//Finally, this will print the column and row results in a file. Very simple way. It will be later picked on by a python script which reads the PID config used and will create the corresponding BDT cuts.
//AMAZING!!!
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "TMath.h"
#include "../Functions/StringTools.h"
#include "../Functions/Filereading.h"
using namespace std;

int* Get2Indices(string word);
void PIDPunzi(string tables, string yields, string outfilename, const double sigma = 5.);


int* Get2Indices(string word)
{
  //In order to avoid repetition of string functions, I'll use soem vars to store that info
  //Different way to search depending on the file type (.root or .txt)
  int* ind =  new int[2];
  if(word.find(".txt")!=string::npos)
    {
      string find_str = "_PIDCuts_";
      string find_str2 = ".txt";
      int find_str_size = find_str.size();
      int find_str_pos = word.find(find_str);
      int find_str_end1 = word.find("_", find_str_pos+find_str_size);
      string find_substr1 = word.substr(find_str_pos+find_str_size,
					find_str_end1-find_str_pos-find_str_size);
      int find_str_end2 = word.find(find_str2);
      string find_substr2 = word.substr(find_str_end1+1,
					find_str_end2-find_str_end1-1);
      ind[0] = stoi(find_substr1);
      ind[1] = stoi(find_substr2);
    }
  else
    {
      string find_str = ".root";
      int find_str_size = find_str.size();
      int find_str_pos = word.find(find_str);
      int find_str_end1 = word.find("_", find_str_pos);
      string find_substr1 = word.substr(find_str_pos+find_str_size,
					find_str_end1-find_str_pos-find_str_size);
      int find_str_end2 = word.find(find_str, find_str_pos+1);
      string find_substr2 = word.substr(find_str_end1+1,
					find_str_end2-find_str_end1-1);
      ind[0] = stoi(find_substr1);
      ind[1] = stoi(find_substr2);
    }
  return ind;

}
void PIDPunzi(string tables, string yields, string outfilename, const double sigma)
{
  //Get array with the tables
  int N_tables = 0;
  int table_size = 0;
  string* tablename = SplitString(N_tables, tables, " ");
  int NN = 0;
  //Read each table
  int* N_cutcombs = new int[N_tables];
  string** rootname = new string*[N_tables];
  double*** effs = new double**[N_tables];
  double* mag_w = new double[N_tables];
  double** Punzi;
  //Init Punzi (compiler warning)
  N_cutcombs[0] = 0;
  rootname[0] = ReadVariables(N_cutcombs[0], tablename[0]);
      
  for(int i=0;i<N_tables;i++)
    {
      //Check first file to see if magnet separation is applied
      N_cutcombs[i] = 0;
      rootname[i] = ReadVariables(N_cutcombs[i], tablename[i]);
      //If MagDown and MagUp are separated, then weight by a factor 0.5. Else 1.
      if(rootname[i][0].find("_PID_Mag") != string::npos)
	{
	  mag_w[i] = 0.5;
	}
      else
	{
	  mag_w[i] = 1.;
	}

      //Size of the 2-D PID table, depending on the magnet config
      table_size = int(TMath::Sqrt(N_cutcombs[i]*mag_w[i]));
      effs[i] = new double*[table_size];
      for(int k=0;k<table_size;k++)
	{
	  effs[i][k] = new double[table_size];
	  for(int l=0;l<table_size;l++)
	    {
	      effs[i][k][l] = 0;
	    }
	}

      for(int j=0;j<N_cutcombs[i];j++)
	{
	  //First word is file, second is the separator "|" and third is the efficiency
	  NN = 0;
	  string* splitname = SplitString(NN, rootname[i][j], " ");
	  double eff_val = 0;
	  //Deal with nan's
	  if(splitname[NN-1].find("nan")==string::npos)
	    {
	      eff_val = stod(splitname[NN-1]);
	    }
	  //Retrieve indices (2 of them)
	  int* ind = Get2Indices(splitname[0]);
	  effs[i][ind[0]][ind[1]] += eff_val*mag_w[i];
	}
    }

  //Initialize Punzi fom
  table_size = int(TMath::Sqrt(N_cutcombs[0]*mag_w[0]));
  Punzi = new double*[table_size];
  for(int k=0;k<table_size;k++)
    {
      Punzi[k] = new double[table_size];
    }

  //Read the yields, N-1 must be provided. (All but signal)
  NN = 0;
  string* yields_str = SplitString(NN, yields, " ");
  if(yields==""){NN = 0;}
  if(NN != N_tables-1)
    {
      cout << "Number of yields(" << NN << ") must be number of tables - 2(" << N_tables-1 << ")! " << endl;
      exit(0);
    }
  //Yields to double
  double* yields_num = new double[N_tables-1];
  for(int i=0;i<N_tables-1;i++)
    {
      int _N = 0;
      yields_num[i] = stod(ReadVariables(_N, yields_str[i])[0]);
    }
  //Prepare output file
  ofstream outf;
  outf.open(outfilename.c_str());
  //Loop over all the table, compute Punzi for each cut
    for(int i=0;i<table_size;i++)
    {
      for(int j=0;j<table_size;j++)
	{
	  //Sum bkg expected yields
	  double B = 0;
	  for(int k=0;k<N_tables-1;k++)
	    {
	      B += yields_num[k]*effs[k+1][i][j];
	    }
	  //Compute Punzi fom
	  Punzi[i][j] = effs[0][i][j]/(sigma/2.+TMath::Sqrt(B));
	  //Print to file
	  outf << Punzi[i][j] << endl;
	}
    }
    outf.close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 3:
      PIDPunzi(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
      break;
    case 4:
      PIDPunzi(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), stod(*(new string(argv[4]))));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for PIDPunzi" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
