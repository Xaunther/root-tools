#include <string>
#include <iostream>
#include "TChain.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
using namespace std;

#define N_configs 4
#define N_samples 4
#define N_lines 4
//Primer argumento: modo (TISTOS o TOS)
void Trigger_effs(string mode)
{
  //Leer variables del archivo
  string line[N_lines]; line[0] = "HHgamma"; line[1] = "HHgammaEE"; line[2] = "HHHgamma"; line[3] = "HHHgammaEE";
  string samples[N_samples]; samples[0] = "KstGam"; samples[1] = "PhiGam"; samples[2] = "K1Gam"; samples[3] = "MinBias";
  string configs[N_configs]; configs[0] = "VLoose"; configs[1] = "Loose"; configs[2] = "Tight"; configs[3] = "VTight";
  //Matriz de eficiencias (N1 x N2 x N3)
  double effs[N_lines][N_samples][N_configs];
  
  for(int i=0;i<N_lines;i++)
    {
      for(int j=0;j<N_samples;j++)
	{
	  TChain* chain = GetChain("../Directories/Trigger_HLT1_"+samples[j]+".dir", "CandidatesFromLine"+line[i]+"/DecayTree", false);
	  for(int k=0;k<N_configs;k++)
	    {
	      string cuts = GetCuts("Variables/HLT1Cut_"+mode+"_"+configs[k]+".txt");
	      effs[i][j][k] = GetEvents(chain, cuts);
	    }
	}
    }

  for(int i=0;i<N_lines;i++)
    {
      cout << line[i] << " Line" << endl;
      cout << "---------------" << endl;
      cout << "         ";
      for(int l=0;l<N_configs;l++){cout << configs[l] << "  ";}
      cout << endl;
      for(int j=0;j<N_samples;j++)
	{
	  cout << samples[j] << "  ";
	  for(int k=0;k<N_configs;k++)
	    {
	      //Escribo en pantalla
	      cout << effs[i][j][k] << "  ";	      
	    }
	  cout << endl;
	}
      cout << endl << endl;
    }
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc)
    {
    case 1:
      Trigger_effs(*(new string(argv[1])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for Trigger_effs" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
