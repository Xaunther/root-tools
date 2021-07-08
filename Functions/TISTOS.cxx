#include <string>
#include <vector>
#include "Functions/TISTOS.h"
using namespace std;

std::string MakeTIS(const std::vector<std::string> &v)
{
    return MakeAny(v, "_TIS");
}
std::string MakeTOS(const std::vector<std::string> &v)
{
    return MakeAny(v, "_TOS");
}
std::string MakeTISTOS(const std::vector<std::string> &v)
{
    return MakeTIS(v) + " && " + MakeTOS(v);
}
std::string MakeAny(const std::vector<std::string> &v, const std::string &suffix)
{
    std::vector<std::string> v_suffix;
    for (auto &_v : v)
        v_suffix.push_back(_v + suffix);
    return JoinCutsAND(v_suffix);
}

std::string JoinCutsAND(const std::vector<std::string> &v)
{
    std::string result = "";
    bool first = true;
    for (auto &_v : v)
    {
        if (first)
            result = result + "(" + _v + ")";
        else
            result = result + " && (" + _v + ")";
        first = false;
    }
    return result;
}