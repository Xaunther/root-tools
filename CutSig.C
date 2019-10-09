//This script is used to compute a Punzi figure of merit, but it is designed so it can be tweaked and used to use different foms
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

//The algorithm needs an ntuple that will be the signal, and its efficiency will be used
//Also need to provide a list (blank space separated) of bkg MC files. For each MC sample provided a corresponding yield file must be given as another list. This list might be empty
//Then, also need to provide data. No corresponding yield file must be provided, but a certain mass variable and options must be given to determine the mass window through the dictionary
//Precuts are passed so that the provided ntuples with the precuts match the yields provided
//Tree names might be explicitly provided (as a list in the case of bkg MC, the list can be shorter than the list of samples, only the first n provided will be given the explicit name and the rest will get the one determined by treenames.dic)
//Signal weight variable can be specified
//A list of weight variables for MC bkg can be specified. If the list is shorter, the last ones will not get a weight (just 1)
//Sigma for the Punzi fom can be provided (default is 5)
//Power for the B can be provided (default is 1/2)

/**********************************************************************
General Formula: e/(sigma/2+(y1+y2+y3+... + D)^p)
Now, the general formula does not make much sense, one has to be clever to use the known figure of merits. Typical examples are here:
1) Punzi FOM where B is obtained from MC (e/(sigma/2+sqrt(B1+B2+B3...))): Do not provide datafile, provide list of bkgs, set whatever sigma you fancy and power = 1/2
2) Punzi FOM where B is obtained from data (e/(sigma/2+sqrt(B))): Do not provide list of bkgs, provide data, set whatever sigma you fancy and power = 1/2. This one is biased since the data actually provides S+B instead of just B, use only when S << B)
3) Significance where S+B is obtained from data (e/sqrt(S+B)): Do not provide list of bkgs, provide data, set sigma = 0 and power = 1/2
4) Significance where B is obtained from MC (e/sqrt(S+B1+B2+B3...)): Do not provide datafile, provide as list of bkgs the signal AND the bkgs, set sigma = 0 and power = 1/2
5) Significance 2, where B is obtained from MC (e/sqrt(B1+B2+B3...)): Do not provide datafile, provide list of bkgs and set sigma = 0, power = 1/2
6) Signal purity, where B is obtained from MC (e/(B1+B2+B3+...)): Do not provide datafile, provide list of bkgs, set sigma = 0 and power = 1.
... And any more combinations you can think of
 **********************************************************************/
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

void CutSig(string sigfile, string bkglist, string datafile, string instrfile, string bkgyieldlist, string dumpname, string cutname, string precutsfile = "", string sigtree = "", string bkgtreelist = "", string datatree = "", string sigw = "1", string bkgwlist = "1", string massvar = "B_M", string opts = "", const double sigma = 5., const double spb_pow = 0.5);
void CutSig(string sigfile, string bkglist, string datafile, string instrfile, string bkgyieldlist, string dumpname, string cutname, string precutsfile, string sigtree, string bkgtreelist, string datatree, string sigw, string bkgwlist, string massvar, string opts, const double sigma, const double spb_pow)
{
  int NN = 0; //DUMMY!!

  //Read signal chain
  TChain* sigchain = GetChain(sigfile, sigtree);
  /**********************************************************************/
  //Read bkg chain
  int Nbkg = 0;
  //Need to split in array for bkg
  string* bkgfile = SplitString(Nbkg, bkglist, " ");
  string* bkgtree = SplitString(NN, bkgtreelist, " ");
  TChain** bkgchain = new TChain*[Nbkg];
  for(int i=0;i<Nbkg;i++)
    {
      if(i<NN)
	{
	  bkgchain[i] = GetChain(bkgfile[i], bkgtree[i]);
	}
      else
	{
	  bkgchain[i] = GetChain(bkgfile[i]);
	}
    }
  //Will not use list of trees anymore
  delete[] bkgtree;
  /**********************************************************************/
  //Read data chain
  TChain* datachain = GetChain(datafile, datatree);
  /**********************************************************************/
  //Read precuts
  string precuts = GetCuts(precutsfile);
  if(precuts == ""){precuts = "(1)";}
  /**********************************************************************/
  //Read initial bkg yields
  NN = 0;
  string* bkgyieldfile = SplitString(NN, bkgyieldlist, " ");
  //Raise error if different
  if(NN!=Nbkg)
    {
      cout << "Error! Number of bkg files (" << Nbkg << ") and bkg yields (" << NN << ") must coincide! Stopping..." << endl;
      exit(1);
    }
  /**********************************************************************/
  //Read list of bkg weights
  NN = 0;
  string* _bkgw = SplitString(NN, bkgwlist, " ");
  string* bkgw = new string[Nbkg]; for(int i=0;i<NN;i++){bkgw[i]=_bkgw[i];}
  delete[] _bkgw;
  /**********************************************************************/
  //Now READ!
  double* bkgyield0 = new double[Nbkg];
  for(int i=0;i<Nbkg;i++)
    {
      NN=0;
      bkgyield0[i] = stod(ReadVariables(NN, bkgyieldfile[i])[0])/GetMeanEntries(bkgchain[i], precuts, bkgw[i]);
    }
  NN=0;
  /**********************************************************************/
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
  /**********************************************************************/
  //Get Mass window (used for all but greatest effect is for data, or should be
  //First, options
  if(opts=="")
    {
      opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
    }
  Constants const_list(opts);
  //Second, cuts
  stringstream ss;
  ss << precuts << " * (" << massvar << " > " << const_list.xmin << ") * (" << massvar << " < " << const_list.xmax << ")";
  precuts = ss.str();
  ss.str("");
  /**********************************************************************/
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
	  ss << thiscuts << " * (" << cut[j] << minV[j] + (maxV[j]-minV[j])/(steps[j]-1)*(remnant%steps[j]) << ")";
	  dumpf << " * (" << cut[j] << minV[j] + (maxV[j]-minV[j])/(steps[j]-1)*(remnant%steps[j]) << ")";
	  thiscuts = ss.str();
	  ss.str("");
	  remnant = remnant/steps[j];
	}
      /**********************************************************************/
      //Compute efficiency
      double eff = GetMeanEntries(sigchain, thiscuts, sigw);
      //Initialize B
      double B = 0;
      for(int j=0;j<Nbkg;j++)
	{
	  if(bkgchain[j]->GetEntries()>0)
	    {
	      B += bkgyield0[j]*GetMeanEntries(bkgchain[j], thiscuts, bkgw[j]);
	    }
	}
      //Compute B
      double D = 0;
      if(datachain->GetEntries()>0)
	{
	  D = datachain->GetEntries(thiscuts.c_str());
	}
      //Compute Punzi
      double fom = eff / (sigma/2. + TMath::Power(B+D, spb_pow));
      /**********************************************************************/
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

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc)
    {
    case 7:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
      break;
    case 8:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
		*(new string(argv[8])));
      break;
    case 9:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
		*(new string(argv[8])), *(new string(argv[9])));
      break;
    case 10:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
		*(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])));
      break;
    case 11:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), 
		*(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])));
      break;
    case 12:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), 
		*(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])));
      break;
    case 13:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), 
		*(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])));
      break;
    case 14:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), 
		*(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])), *(new string(argv[14])));
      break;
    case 15:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
		*(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])), *(new string(argv[14])),
		*(new string(argv[15])));
      break;
    case 16:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
		*(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])), *(new string(argv[14])),
		*(new string(argv[15])), stod(*(new string(argv[16]))));
      break;
    case 17:
      CutSig(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
		*(new string(argv[8])), *(new string(argv[9])), *(new string(argv[10])), *(new string(argv[11])), *(new string(argv[12])), *(new string(argv[13])), *(new string(argv[14])),
		*(new string(argv[15])), stod(*(new string(argv[16]))), stod(*(new string(argv[17]))));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for CutSig" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
