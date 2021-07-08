#ifndef TISTOS_h
#define TISTOS_h

#include <string>
#include <vector>

using namespace std;

//Functions index
//Make TIS/TOS/TISTOS are intended to just add a suffix to every member of the vector (_TIS, _TOS or _TIS and then _TOS). Also joins the members with an AND requirement
std::string MakeTIS(const std::vector<std::string> &);
std::string MakeTOS(const std::vector<std::string> &);
std::string MakeTISTOS(const std::vector<std::string> &);
std::string MakeAny(const std::vector<std::string> &, const std::string &);
std::string JoinCutsAND(const std::vector<std::string> &);

#endif
