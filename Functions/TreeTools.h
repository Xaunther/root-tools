#ifndef TreeTools_h
#define TreeTools_h

#include <string>
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
using namespace std;

TChain *GetChain(string *, int, string, bool = true);
TChain *GetChain(string, string = "", bool = true);
TTree *GetTree(string, string = "", bool = true);
void AddTreeBranch(TTree *, string, string, string);
int GetEvents(TTree *, string = "");
int GetEvents(TChain *, string = "");
int GetCorrEvents(TTree *, TTree *, string = "", string = "");
int GetCorrEvents(TTree *, TChain *, string = "", string = "");
int GetCorrEvents(TChain *, TTree *, string = "", string = "");
int GetCorrEvents(TChain *, TChain *, string = "", string = "");
void Activate(TTree *, string);
void Activate(TChain *, string);
void Activate(TTree *, string *, int);
void activate(TChain *, string *, int);
double GetMean(string, string, string = "", string = "1", string = "1");
double GetMean(TChain *, string, string = "1", string = "1");
double GetMean(TTree *, string, string = "1", string = "1");
double GetMeanEntries(string, string = "", string = "1", string = "1");
double GetMeanEntries(TChain *, string = "1", string = "1");
double GetMeanEntries(TTree *, string = "1", string = "1");
bool TreeExists(string, string = "");
void CloseChain(TChain *&);
void CloseTree(TTree *&);
TH1F *GetHistogram(TTree *, string, int, double *, string, string = "");
TH1F *GetHistogram(TChain *, string, int, double *, string, string = "");
TH2F *GetHistogram2D(TTree *, string, int, double *, int, double *, string, string = "");
TH2F *GetHistogram2D(TChain *, string, int, double *, int, double *, string, string = "");

#endif
