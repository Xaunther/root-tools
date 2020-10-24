//Implementation of functions to do stuff with arrays
#include <string>
#include <math.h>
#include "Functions/ArrayTools.h"
using namespace std;

int GetMaxLength(string* line, int N_lines)
{
  int maxL = -1;
  for (int i = 0; i < N_lines; i++)
  {
    if (int(line[i].size()) > maxL)
    {
      maxL = int(line[i].size());
    }
  }
  return maxL;
}

int GetMaxPos(double* list, int N)
{
  int maxPos = 0;
  for (int i = 0; i < N; i++)
  {
    if (*(list + maxPos) < * (list + i))
    {
      maxPos = i;
    }
  }
  return maxPos;
}

//Merge all elements with an OR statement
string MergeArray_OR(string* array, int N)
{
  return MergeArray(array, N, "||");
}
//Merge all elements with an AND statement
string MergeArray_AND(string* array, int N)
{
  return MergeArray(array, N, "&&");
}
//Merge all elements with some statement
string MergeArray(string* array, int N, string op)
{
  string merged = "";
  for (int i = 0; i < N - 1; i++)
  {
    merged = merged + "(" + array[i] + ") " + op + " ";
  }
  merged = merged + "(" + array[N - 1] + ")";
  return merged;
}

//Forward declaration
template <typename T> double GetArrayMean(T* array, int N)
{
  double mean = 0;
  for (int i = 0; i < N; i++) {mean += array[i];}
  return mean / N;
}
template <typename T> double GetArrayStdDev(T* array, int N)
{
  double mean = GetArrayMean(array, N);
  double stddev = 0;
  for (int i = 0; i < N; i++) {stddev += (array[i] - mean) * (array[i] - mean);}
  return sqrt(stddev / (N - 1.));
}
template <typename T> double GetArraySum(T* array, int N)
{
  double sum = 0;
  for (int i = 0; i < N; i++) {sum += array[i];}
  return sum;
}
template <typename T> double GetArraySumSq(T* array, int N)
{
  double sumsq = 0;
  for (int i = 0; i < N; i++) {sumsq += array[i] * array[i];}
  return sumsq;
}

//Function to find if some string is present in an array (only returns true or false)
bool IsInArray(string searchw, string* array, int N)
{
  for (int i = 0; i < N; i++)
  {
    if (searchw == array[i])
    {
      return true;
    }
  }
  return false;
}

template double GetArrayMean(double*, int);
template double GetArrayStdDev(double*, int);
template double GetArraySum(double*, int);
template double GetArraySumSq(double*, int);
