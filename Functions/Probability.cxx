#include "TMath.h"
#include "Functions/Probability.h"
#include <iostream>
using namespace std;

//This functions takes as arguments array (and size) of probabilities of N independent events, and computes P(A1uA2uA3...An)
double JointProbIndependent(double* p_array, int N, int depth)
{
  if(N == 0){return 0;}
  //Define result variables
  double result = 0.;
  double prod = 1.;
  //Compute Probability of intersection of the N elements
  for(int i=0;i<N;i++)
    {
      prod *= p_array[i];
    }
  //Compute (-1)^(n-1) factor and take into account repetitions
  result += prod*TMath::Power(-1, N+1)/TMath::Factorial(depth);
  //Compute sum of the N-1 elements
  for(int i=0;i<N;i++)
    {
      double* p1_array = new double[N-1];
      //Copy array but then drop i-th element
      for(int j=0;j<N-1;j++)
	{
	  p1_array[j] = p_array[j];
	}
      if(i<N-1)
	{
	  p1_array[i] = p_array[N-1];
	}
      result += JointProbIndependent(p1_array, N-1, depth+1);
    }
  return result;
}
