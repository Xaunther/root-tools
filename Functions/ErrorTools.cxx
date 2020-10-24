#include "Functions/ErrorTools.h"
#include "TMath.h"
#include <iostream>
using namespace std;

int GetErrPrecision(double error)
{
  //Final result var declaration
  int ErrPrecision;
  double test_error;
  int counter = 0;
 
  test_error = error;
  //So far, if the error is bigger than 2 the precision will be set to zero (no decimals)
  if(error==0){cout << "WARNING: Error cannot be zero!"; return 3;}
  do
    {
      ErrPrecision = counter;
      test_error *= 10.;
      counter++;
    }while(test_error < 19.5);
  //Return
  return ErrPrecision;
}
