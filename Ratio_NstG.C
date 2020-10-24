//This script is to compute BF(Lb->ppiG)/BF(Lb->pKG)/S

#include <string>
#include <iostream>
#include "Functions/Dictreading.h"
using namespace std;

void Ratio_NstG()
{
  //Formula can be found at ()
  //Start doing B
  double B = 2095.91;

  //We have a sum over the 4 Lambda* channels, we will assign signal efficiencies to index 0
  const int N_Ch = 5;
  //We have 5 efficiency steps: Generation, fullevtcut, DST, offline, PID
  const int N_St = 5;

  //Define efficiency array for each channel
  double** effs = new double*[N_Ch];
  //Define efficiency array for each step, in each channel
  for (int i = 0; i < N_Ch; i++)
  {
    effs[i] = new double[N_St];
  }

  //We also need the importance, or weight, whatever name of each lambda channel. Sum equals to 1!
  double* R_LG = new double[N_Ch - 1];
  for (int i = 0; i < N_Ch - 1; i++)
  {
    R_LG[i] = 0.25;
  }

  //The first 3 steps have efficiencies for each magnet, treat differently
  bool* split_mag = new bool[N_St];
  split_mag[0] = true;
  split_mag[1] = true;
  split_mag[2] = true;
  split_mag[3] = false;
  split_mag[4] = false;

  //In order to make the filling a bit shorter we can define these arrays
  //Codename of each channel
  string* name_Ch = new string[N_Ch];
  name_Ch[0] = "NstG";
  name_Ch[1] = "L1520G";
  name_Ch[2] = "L1670G";
  name_Ch[3] = "L1820G";
  name_Ch[4] = "L1830G";

  //Filename that contains efficiencies of each step
  string* name_St = new string[N_St];
  name_St[0] = "Systematics/Generation/GenLevelCut_Eff.txt";
  name_St[1] = "Systematics/Generation/FullEventCut_Eff.txt";
  name_St[2] = "Systematics/DST/Global_Eff.txt";
  name_St[3] = "Systematics/Selection/Global_Eff.txt";
  name_St[4] = "Systematics/PID/PID_Eff.txt";

  //PART TO FILL UP EVERYTHING
  for (int i = 0; i < N_Ch; i++)
  {
    for (int j = 0; j < N_St; j++)
    {
      if (split_mag[j]) //Average both magnets
      {
        effs[i][j] = stod(GetValueFor(name_Ch[i] + "_2012_Down_eff", name_St[j])) + stod(GetValueFor(name_Ch[i] + "_2012_Up_eff", name_St[j]));
        effs[i][j] *= 0.5;
      }
      else
      {
        effs[i][j] = stod(GetValueFor(name_Ch[i] + "_2012_eff", name_St[j]));
      }
    }
  }

  //Computation
  double BR = 0;
  //Sum over Lambda channels
  for (int i = 1; i < N_Ch - 1; i++)
  {
    //To make the productory of effs
    double _prod = 1;
    for (int j = 0; j < N_St; j++)
    {
      _prod *= effs[i][j] / effs[0][j];
    }
    //Multiply by the importance of LG
    _prod *= R_LG[i - 1];
    //Add
    BR += _prod;
  }

  //Once this is finished, multiply by S/B
  BR *= 1 / B;

  //Output result
  cout << "Result of BF(Lb->ppiG)/BF(Lb->pKG): " << BR << " S" << endl;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 0:
    Ratio_NstG();
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
