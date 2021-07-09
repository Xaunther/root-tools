#ifndef TISTOS_h
#define TISTOS_h

#include <string>
#include <vector>

#include "TUncertainty.h"

using namespace std;

//Functions index
//Make TIS/TOS/TISTOS are intended to just add a suffix to every member of the vector (_TIS, _TOS or _TIS and then _TOS). Also joins the members with an AND requirement
 string MakeTIS(const  vector< string> &);
 string MakeTOS(const  vector< string> &);
 string MakeTISTOS(const  vector< string> &);
 string MakeAny(const  vector< string> &, const  string &);
 string JoinCutsAND(const  vector< string> &);
double TISTOSUncertainty(const TUncertainty &N_TOS, const TUncertainty &N_TIS, const TUncertainty &N_TISTOS, const TUncertainty &N_trigger);
void PrintOutput(const string &datadir, const string &outfile, const string &triggercuts, const string &TIScuts, const string &TOScuts, const string &TISTOScuts, const TUncertainty &eff_trig);


#endif
