#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "TMath.h"
#include "Functions/TISTOS.h"
using namespace std;

string MakeTIS(const vector<string> &v)
{
    return MakeAny(v, "_TIS");
}
string MakeTOS(const vector<string> &v)
{
    return MakeAny(v, "_TOS");
}
string MakeTISTOS(const vector<string> &v)
{
    return MakeTIS(v) + " && " + MakeTOS(v);
}
string MakeAny(const vector<string> &v, const string &suffix)
{
    vector<string> v_suffix;
    for (auto &_v : v)
        v_suffix.push_back(_v + suffix);
    return JoinCutsAND(v_suffix);
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

double TISTOSUncertainty(const TUncertainty &N_TOS, const TUncertainty &N_TIS, const TUncertainty &N_TISTOS, const TUncertainty &N_trigger)
{
    //Using notation from TISTOS method paper
    double d = N_TISTOS.GetValue();
    double b = N_TIS.GetValue() - d;
    double c = N_TOS.GetValue() - d;
    double sigma_d = N_TISTOS.GetUncertainty()[0];
    double sigma_b = TMath::Sqrt(TMath::Power(N_TIS.GetUncertainty()[0], 2.) + TMath::Power(sigma_d, 2.));
    double sigma_c = TMath::Sqrt(TMath::Power(N_TOS.GetUncertainty()[0], 2.) + TMath::Power(sigma_d, 2.));
    double sigma_N_sel = TMath::Sqrt(TMath::Power(sigma_b * (c + d) / d, 2.) + TMath::Power(sigma_c * (d * d - b * c) / (d * d), 2.) + TMath::Power(sigma_d * (b + d) / d, 2.));

    double n = N_trigger.GetValue();
    double eff = ((N_trigger / N_TIS) * (N_TISTOS / N_TOS)).GetValue();
    double m = n / eff - n;

    double sigma_n = N_trigger.GetUncertainty()[0];

    return TMath::Sqrt(TMath::Power(sigma_n * m / TMath::Power(n + m, 2.), 2.) + TMath::Power(n / TMath::Power(n + m, 2.), 2.) * (TMath::Power(sigma_N_sel, 2) - TMath::Power(sigma_n, 2)));
}

void PrintOutput(const string &datadir, const string &outfile, const string &triggercuts, const string &TIScuts, const string &TOScuts, const string &TISTOScuts, const TUncertainty &eff_trig)
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
    outf << "TOS cuts: " << TOScuts << endl;
    outf << "TISTOS cuts: " << TISTOScuts << endl;
    outf << "Trigger cuts: " << triggercuts << endl;
    outf << "Efficiency: ";
    eff_trig.Print(outf);
    outf << endl;
    outf.close();
}