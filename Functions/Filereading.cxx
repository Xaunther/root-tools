//Implementation of ReadVariables functions
//Functions related to reading things from files

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Functions/Filereading.h"
using namespace std;

int GetNVariables(string filename)
{
  ifstream input;
  string basura;
  int N = 0;
  input.open(filename.c_str());
  getline(input, basura);
  while (basura != "")
  {
    N++;
    basura = "";
    getline(input, basura);
  }
  input.close();
  return N;
}
string *ReadVariables(int &N, string filename)
{
  if (filename.find(".root") != string::npos)
  {
    N = 1;
    string *array = new string[1];
    array[0] = filename;
    return array;
  }
  ifstream input;

  N = GetNVariables(filename);
  string *array = new string[N];

  input.open(filename.c_str());
  for (int i = 0; i < N; i++)
  {
    getline(input, array[i]);
  }
  input.close();

  return array;
}

vector<string> ReadVariables(string filename)
{
  int N = 0;
  auto arr = ReadVariables(N, filename);
  vector<string> v;
  for (int i = 0; i < N; i++)
    v.push_back(arr[i]);
  return v;
}

double *ReadNumbers(int &N, string filename)
{
  string *array = ReadVariables(N, filename);
  double *darray = new double[N];
  for (int i = 0; i < N; i++)
  {
    darray[i] = stod(array[i]);
  }
  return darray;
}

string *ReadVariablesWord(int &N, string filename, int pos)
{
  ifstream input;
  string basura;

  N = GetNVariables(filename);
  string *array = new string[N];

  input.open(filename.c_str());
  for (int i = 0; i < N; i++)
  {
    for (int j = 1; j < pos; j++)
    {
      input >> basura;
    }
    input >> array[i];
    getline(input, basura);
  }
  input.close();

  return array;
}

vartable *ReadVarTable(int N, string filename)
{
  ifstream input;

  vartable *array = new vartable[N];

  input.open(filename.c_str());
  for (int i = 0; i < N; i++)
  {
    input >> array[i].abs;
    for (int j = 0; j < 5; j++)
    {
      input >> array[i].variations[j];
    }
  }
  input.close();

  return array;
}

double **GetData(string filename, int &N, int Ncol)
{
  ifstream input;

  N = GetNVariables(filename);
  double **data = new double *[N];

  for (int i = 0; i < N; i++)
  {
    data[i] = new double[Ncol];
  }

  input.open(filename.c_str());
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < Ncol; j++)
    {
      input >> data[i][j];
    }
  }
  input.close();

  return data;
}

string GetCuts(string filename)
{
  ifstream input;
  string cuts = "";
  string basura;

  input.open(filename.c_str());
  if (!input)
  {
    return filename;
  }
  getline(input, basura);
  while (basura != "")
  {
    cuts += "(";
    cuts += basura;
    cuts += ")";
    basura = "";
    getline(input, basura);
    if (basura != "")
    {
      cuts += " * ";
    }
  }
  input.close();
  return cuts;
}

string GetCutsVar(string cutsfilename, string varsfilename, int N_variables, int i, int j)
{
  vartable *array = ReadVarTable(N_variables, varsfilename);
  string cuts = "";
  string basura;
  double value;
  stringstream ss;
  //Read cuts and substitute when required
  ifstream input;
  input.open(cutsfilename.c_str());
  if (!input) //In that case, try using the string as a given condition
  {
    return cutsfilename;
  }
  for (int k = 0; k < N_variables; k++)
  {
    cuts += "(";
    if (k == i) // Line modified
    {
      input >> basura;
      cuts += basura + " ";
      input >> basura;
      cuts += basura + " ";
      input >> value;
      if (array[i].abs)
      {
        value += array[i].variations[j];
      }
      else
      {
        value += value * array[i].variations[j] / double(100);
      }
      ss << value;
      cuts += ss.str();
      ss.str("");
      //Clean line
      getline(input, basura);
    }
    else
    {
      getline(input, basura);
      cuts += basura;
    }
    if (k == N_variables - 1) //Last condition
    {
      cuts += ")";
    }
    else
    {
      cuts += ") && ";
    }
  }
  input.close();
  return cuts;
}

void AdvanceLine(ifstream &fichero, string lookup)
{
  string basura;
  do
  {
    getline(fichero, basura);
  } while (basura != lookup && !fichero.eof());

  if (fichero.eof())
  {
    cout << "Reached end of file with no coincidences" << endl;
  }
  return;
}
