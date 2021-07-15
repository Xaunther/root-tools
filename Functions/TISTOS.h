#ifndef TISTOS_h
#define TISTOS_h

#include <string>
#include <vector>

#include "TUncertainty.h"

using namespace std;

//Functions index
//Make TIS/TOS/TISTOS are intended to just add a suffix to every member of the vector (_TIS, _TOS or _TIS and then _TOS). Also joins the members with an AND requirement
vector<string> MakeTIS(const vector<string> &);
vector<string> MakeTOS(const vector<string> &);
vector<string> MakeTISTOS(const vector<string> &);
vector<string> MakeAny(const vector<string> &, const string &);
string JoinCutsAND(const vector<string> &);
string JoinCutsOR(const vector<string> &);
double TISTOSUncertainty(const TUncertainty &N_TIS, const TUncertainty &N_TISTOS);
void PrintOutput(const string &datadir, const string &outfile, const string &TIScuts, const string &TISTOScuts, const TUncertainty &eff_trig);
void RemoveTISTOS(vector<string> &v);

#endif
