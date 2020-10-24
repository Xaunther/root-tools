#include <string>
#include "Functions/TISTOS.h"
using namespace std;

string MakeTIS(string* list, int N)
{
  string line = "";
  for (int i = 0; i < N - 1; i++)
  {
    line += list[i] + "_TIS || ";
  }
  line += list[N - 1] + "_TIS";
  return line;
}

string MakeTISTOS(string* list, int N)
{
  string line = "";
  for (int i = 0; i < N - 1; i++)
  {
    line += "(" + list[i] + "_TIS && " + list[i] + "_TOS) || ";
  }
  line += "(" + list[N - 1] + "_TIS && " + list[N - 1] + "_TOS)";
  return line;
}

string MakeTOS(string* list, int N)
{
  string line = "";
  for (int i = 0; i < N - 1; i++)
  {
    line += list[i] + "_TOS || ";
  }
  line += list[N - 1] + "_TOS";
  return line;
}

string MakeTISorTOS(string* list, int N)
{
  string line = "";
  for (int i = 0; i < N - 1; i++)
  {
    line += "(" + list[i] + "_TIS || " + list[i] + "_TOS) || ";
  }
  line += "(" + list[N - 1] + "_TIS || " + list[N - 1] + "_TOS)";
  return line;
}

//Remove _TIS, _TOS and _Dec suffixes
string* CleanTISTOS(string* list, int N)
{
  for (int i = 0; i < N; i++)
  {
    while (list[i].find("_TIS") != string::npos)
    {
      list[i].replace(list[i].find("_TIS"), string::npos, "");
    }
    while (list[i].find("_TOS") != string::npos)
    {
      list[i].replace(list[i].find("_TOS"), string::npos, "");
    }
    while (list[i].find("_Dec") != string::npos)
    {
      list[i].replace(list[i].find("_Dec"), string::npos, "");
    }
  }
  return list;
}
