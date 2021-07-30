#include "Functions/StringTools.h"
#include <string>
#include <iostream>
using namespace std;

string *SplitString(int &N, string s, string delimiter)
{
  //Empty string has no elements, DUH
  if (s == "")
  {
    N = 0;
    return 0;
  }
  int pos = 0;
  N = CountString(s, delimiter) + 1;
  string *list = new string[N];
  for (int i = 0; i < N; i++)
  {
    list[i] = s.substr(pos, s.find(delimiter, pos) - pos);
    if (i < N - 1)
    {
      pos = s.find(delimiter, pos) + delimiter.size();
    }
  }

  return list;
}

int CountString(string s, string pattern)
{
  int N = 0;
  int pos = 0;
  while (s.find(pattern, pos) != string::npos)
  {
    N++;
    pos = s.find(pattern, pos) + pattern.size();
  }

  return N;
}

string Gridify(string filename)
{
  //Do nothing as for now, bug issue https://github.com/root-project/root/issues/8767
  return filename;
  if (filename.substr(0, 5) == "/eos/")
  {
    return "root://eoslhcb.cern.ch/" + filename;
  }
  else
  {
    return filename;
  }
}
