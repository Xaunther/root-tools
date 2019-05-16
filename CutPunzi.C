//This script computes a Punzi f.o.m. for a set of cuts. All possible combinations
//It returns both a log file with the values, so one can peek them. And a file with such best set of cuts
//The instructions must be given in a separate file, with this format. The separators will be blank spaces, so beware...
/**********************************************************************
cut1 min1 max1 steps1
cut2 min2 max2 steps2
cut3 min3 max3 steps3
...  ...  ...  ...
cutN minN maxN stepsN
 *********************************************************************/
//And each cut must be something like (NO BLANK SPACES):
//B_M01<892||B_M01>
//B_M01>892||B_M01<
//B_M01_Subst01_Kpi2piK<892||B_M01_Subst01_Kpi2piK>
//B_M01_Subst01_Kpi2piK>892||B_M01_Subst01_Kpi2piK<
//When using "OR"s like in this case, the values put by hand must stand between max(<) and min(>). Just choose the resonance mass :)

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "TChain.h"
#include "TMath.h"
#include "../Functions/TreeTools.h"
#include "../Functions/StringTools.h"
#include "../Functions/ArrayTools.h"
#include "../Functions/Filereading.h"
using namespace std;

void CutPunzi(string sigfile, string bkgfile, string instrfile, string bkgyieldfile, string dumpname, string cutname, string precutsfile = "", string sigtree = "", string bkgtree = "", string sigw = "1", string bkgw = "1")
{
  //Define sigma of Punzi (5 is usual)
  const double sigma = 5.;
  //Read signal and background chains
  TChain* sigchain = GetChain(sigfile, sigtree);
  TChain* bkgchain = GetChain(bkgfile, bkgtree);
  //Read precuts
  string precuts = GetCuts(precutsfile);
  if(precuts == ""){precuts = "(1)";}
  //Read initial bkg yield
  int Ny = 0;
  double yield0 = stod(ReadVariables(Ny, bkgyieldfile)[0]);
  //Read instructions file, has 4 columns
  const int Ncol = 4;
  int N = 0;
  //Read line by line, then split in the different arrays
  string* cut = ReadVariables(N, instrfile);
  double* minV = new double[N];
  double* maxV = new double[N];
  int* steps = new int[N];
  int combs = 1;
  for(int i=0;i<N;i++)
    {
      //Split in temporary array
      int Ntemp = 0;
      string* tempst = SplitString(Ntemp, cut[i], " ");
      cut[i] = tempst[0];
      minV[i] = stod(tempst[1]);
      maxV[i] = stod(tempst[2]);
      steps[i] = stoi(tempst[3]);
      //Count combs so far
      combs *= steps[i];
    }
  //Create list of results, it has steps1*steps2*...*stepsN elements
  double* punzifom = new double[combs];

  // DATE START!!! (THANKS UNDERTALE)
  //Open dump file
  ofstream dumpf;
  dumpf.open(dumpname.c_str());
  dumpf << "precuts = " << precuts << endl;
  //Let'sa go
  for(int i=0;i<combs;i++)
    {
      dumpf << "(precuts)";
      //Build this set of cuts
      string thiscuts = precuts;
      int remnant = i;
      //Get this cut combination and go writing in the dumpfile
      for(int j=0;j<N;j++)
	{
	  stringstream ss;
	  ss << thiscuts << " * (" << cut[j] << minV[j] + (maxV[j]-minV[j])/(steps[j]-1)*(remnant%steps[j]) << ")";
	  dumpf << " * (" << cut[j] << minV[j] + (maxV[j]-minV[j])/(steps[j]-1)*(remnant%steps[j]) << ")";
	  thiscuts = ss.str();
	  ss.str("");
	  remnant = remnant/steps[j];
	}
      //Compute Punzi
      //Upper part
      punzifom[i] = GetMeanEntries(sigchain, thiscuts, sigw);
      //Bottom part
      punzifom[i] = punzifom[i] / (sigma/2. + TMath::Sqrt(yield0*GetMeanEntries(bkgchain, thiscuts, bkgw)));
      //Save Punzi in dumpfile
      dumpf << " | " << punzifom[i] << endl;
    }
  //Close dumpfile
  dumpf.close();

  //Get best cut and save it in the desired file
  int bestcomb = GetMaxPos(punzifom, combs);
  ofstream cutf;
  cutf.open(cutname.c_str());
  for(int j=0;j<N;j++)
    {
      cutf << cut[j] << minV[j] + (maxV[j]-minV[j])/(steps[j]-1)*(bestcomb%steps[j]) << endl;
      bestcomb = bestcomb/steps[j];
    }
  //Close cutfile
  cutf.close();
}
