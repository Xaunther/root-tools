//This script computes a Significance f.o.m. for a set of cuts. All possible combinations.
//CutPunzi can be tweaked to also compute this f.o.m., but it does not support multiple bkg sources and this one does use data (it is designed for peaking contributions only)
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

//The algorithm uses signal ntuple (usually MC), peaking bkg ntuple (usually our bkg MC) and combinatorial (data).
//If we do not use data, the cuts tend to be rather.... tight
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "TChain.h"
#include "TMath.h"
#include "../Functions/TreeTools.h"
#include "../Functions/StringTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Functions/ProgressBar.h"
#include "../Dictionaries/Constants.h"
using namespace std;

void CutSig(string sigfile, string bkglist, string instrfile, string sigyieldfile, string bkgyieldlist, string dumpname, string cutname, string precutsfile = "", string sigtree = "", string bkgtree = "", string sigw = "1", string bkgw = "1", const double spb_pow = 0.5);
void CutSig(string sigfile, string bkglist, string instrfile, string sigyieldfile, string bkgyieldlist, string dumpname, string cutname, string precutsfile, string sigtree, string bkgtree, string sigw, string bkgw, const double spb_pow)
{
  //Read signal and background chains
  TChain* sigchain = GetChain(sigfile, sigtree);
  int Nbkg = 0;
  //Need to split in array for bkg
  string* bkgfile = SplitString(Nbkg, bkglist, " ");
  TChain** bkgchain = new TChain*[Nbkg];
  for(int i=0;i<Nbkg;i++)
    {
      bkgchain[i] = GetChain(bkgfile[i], bkgtree); //Same treename for all, might be improved if needed
    }
  //Read precuts
  string precuts = GetCuts(precutsfile);
  if(precuts == ""){precuts = "(1)";}
  //Read initial bkg yields
  int NN = 0; //DUMMY!!
  string* bkgyieldfile = SplitString(NN, bkgyieldlist, " ");
  //Raise error if different
  if(NN!=Nbkg)
    {
      cout << "Error! Number of bkg files (" << Nbkg << ") and bkg yields (" << NN << ") must coincide! Stopping..." << endl;
      exit(1);
    }
  //Now READ!
  double* bkgyield0 = new double[Nbkg];
  for(int i=0;i<Nbkg;i++)
    {
      NN=0;
      bkgyield0[i] = stod(ReadVariables(NN, bkgyieldfile[i])[0])/GetMeanEntries(bkgchain[i], precuts, bkgw);
    }
  NN=0;
  double sigyield0 = stod(ReadVariables(NN, sigyieldfile)[0])/GetMeanEntries(sigchain, precuts, sigw);
  //Read instructions file, has 4 columns
  int N = 0;
  //Read line by line, then split in the different arrays
  string* cut = ReadVariables(N, instrfile);
  double* minV = new double[N];
  double* maxV = new double[N];
  int* steps = new int[N];
  int combs = 1;
  for(int i=0;i<N;i++)
    {
      //Split in different arrays
      int Ntemp = 0;
      string* tempst = SplitString(Ntemp, cut[i], " ");
      cut[i] = tempst[0];
      minV[i] = stod(tempst[1]);
      maxV[i] = stod(tempst[2]);
      steps[i] = stoi(tempst[3]);
      //Count combs so far
      combs *= steps[i];
    }
  // DATE START!!! (THANKS UNDERTALE)
  cout << "Will perform " << combs << " combinations. Please stand by..." << endl;
  //Open dump file
  ofstream dumpf;
  int bestcomb = 0;
  double bestfom = 0;
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
      //Compute S
      double fom = GetMeanEntries(sigchain, thiscuts, sigw)*sigyield0;
      //Initialize B
      double B = 0;
      for(int j=0;j<Nbkg;j++)
	{
	  B += bkgyield0[j]*GetMeanEntries(bkgchain[j], thiscuts, bkgw);
	}
      //Bottom part
      fom = fom / TMath::Power(fom + B, spb_pow);
      //Save fom in dumpfile
      dumpf << " | " << fom << endl;
      if(fom > bestfom){bestcomb = i; bestfom = fom;}
      DrawProgress(double(i)/combs);
    }
  //Clean progress bar
  cout << endl;
  //Close dumpfile
  dumpf.close();

  //Get best cut and save it in the desired file
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
