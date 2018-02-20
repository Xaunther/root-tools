#include <iostream>
#include <fstream>
#include <string>
#include "TMath.h"
using namespace std;

void PhaseSpace(double M, double m1, double m2)
{
  double p = TMath::Sqrt(((M*M-(m1*m1+m2*m2))*(M*M-(m1*m1+m2*m2))-4*m1*m1*m2*m2)/(M*M))/double(2);
  double E1 = TMath::Sqrt(m1*m1+p*p);
  double E2 = TMath::Sqrt(m2*m2+p*p);
  
  cout << "CM momentum: " << p << endl;
  cout << "CM 4-momentum of 1: (" << E1 << ", " << p << ")" << endl;
  cout << "CM 4--momentum of 2: (" << E2 << ", " << p << ")" << endl;
  cout << "Gammas: (" << E1/m1 << ", " << E2/m2 << ")" << endl;
  cout << "Betas: (" << p/E1 << ", " << p/E2 << ")" << endl;
  cout << "E%: (" << E1/M*100 << ", " << E2/M*100 << ")" << endl;
}
