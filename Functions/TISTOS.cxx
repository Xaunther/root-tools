#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "TMath.h"
#include "Functions/TISTOS.h"
using namespace std;

vector<string> MakeTIS(const vector<string> &v)
{
    return MakeAny(v, "_TIS");
}
vector<string> MakeTOS(const vector<string> &v)
{
    return MakeAny(v, "_TOS");
}
vector<string> MakeTISTOS(const vector<string> &v)
{
    auto v_TIS = MakeTIS(v);
    auto v_TOS = MakeTOS(v);
    vector<string> v_TISTOS;
    for (unsigned long i = 0; i < v.size(); i++)
        v_TISTOS.push_back(v_TIS[i] + " && " + v_TOS[i]);
    return v_TISTOS;
}
vector<string> MakeAny(const vector<string> &v, const string &suffix)
{
    vector<string> v_suffix;
    for (auto &_v : v)
        v_suffix.push_back(_v + suffix);
    return v_suffix;
}

string JoinCutsAND(const vector<string> &v)
{
    string result = "";
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

string JoinCutsOR(const vector<string> &v)
{
    string result = "";
    bool first = true;
    for (auto &_v : v)
    {
        if (first)
            result = result + "(" + _v + ")";
        else
            result = result + " || (" + _v + ")";
        first = false;
    }
    return result;
}

double TISTOSUncertainty(const TUncertainty &N_TIS, const TUncertainty &N_TISTOS)
{
    //Using notation from TISTOS method paper
    double d = N_TISTOS.GetValue();
    double b = N_TIS.GetValue() - d;
    double sigma_d = N_TISTOS.GetUncertainty()[0];
    double sigma_b = TMath::Sqrt(TMath::Power(N_TIS.GetUncertainty()[0], 2.) + TMath::Power(sigma_d, 2.));

    return TMath::Sqrt(TMath::Power(b * sigma_d, 2.) + TMath::Power(d * sigma_b, 2.)) / TMath::Power(b + d, 2.);
}

void PrintOutput(const string &datadir, const string &outfile, const string &TIScuts, const string &TISTOScuts, const TUncertainty &eff_trig)
{
    ofstream outf;
    outf.open(outfile);
    if (!outf)
    {
        cout << "Cannot open " << outfile << ". Exiting" << endl;
        exit(1);
    }
    outf << "Trigger efficiency on " << datadir << endl;
    outf << "----------------------------------------------------------------------" << endl;
    outf << "TIS cuts: " << TIScuts << endl;
    outf << "TISTOS cuts: " << TISTOScuts << endl;
    outf << "Efficiency: ";
    eff_trig.Print(outf);
    outf << endl;
    outf.close();
}

void RemoveTISTOS(vector<string> &v)
{
    for (auto &_v : v)
    {
        _v = _v.substr(0, _v.find("_TIS"));
        _v = _v.substr(0, _v.find("_TOS"));
    }
}