//This script takes a bunch of files (or list of files separated by white spaces) and computes the efficiency of some cut for them, separately
//Then returns the mean efficiency and the error associated
//This is used, for example, when computing PID systematics

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include "../Functions/StringTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/ArrayTools.h"
using namespace std;

void EffStats(string dirfiles, string cutfile, string precutfile = "", string treename = "", string outfile = "EffStats_results.txt", string weight = "")
{
	//Start by reading the cuts and the precuts
	string cuts = GetCuts(cutfile);
	string precuts = GetCuts(precutfile);

	//Now, get list of dirfiles in array
	int N = 0;
	string* dirfile = SplitString(N, dirfiles, " ");

	//Now we can open the chains
	TChain** chain = new TChain*[N];
	for (int i = 0; i < N; i++)
	{
		chain[i] = GetChain(dirfile[i], treename);
	}

	//We'll need the square of the weight
	string w2 = "";
	if (weight != "") {w2 = weight + "*" + weight;}

	//Let us save also the effective initial number of events, N0, and the error in each measurement
	double* N0 = new double[N];
	double* err = new double[N];

	//Finally, it is time to get the efficiency in each chain, and save it in an array. Efficiency cannot go below zero, so we'll treat any negative efficiency as zero
	double* eff = new double[N];
	for (int i = 0; i < N; i++)
	{
		N0[i] = GetMeanEntries(chain[i], precuts, weight);
		eff[i] = max(GetMeanEntries(chain[i], cuts, weight) / N0[i], 0.);
		err[i] = sqrt(eff[i] * (1 - eff[i]) / chain[i]->GetEntries()*GetMeanEntries(chain[i], precuts, w2)) / N0[i];
	}
	//Now we can make some statistics, mainly get the mean and standard deviation of the sample
	double mean_eff = GetArrayMean(eff, N);
	double stddev_eff = GetArrayStdDev(eff, N);
	double stat_error = stddev_eff / sqrt(N);
	double syst_err = sqrt(GetArraySumSq(err, N))/N;
	//Save into requested file
	ofstream outf;
	outf.open(outfile.c_str());
	outf << "Efficiency for each sample" << endl;
	outf << "--------------------------" << endl;
	for (int i = 0; i < N; i++)
	{
		outf << i << " | " << eff[i] << " \u00B1 " << err[i] << endl;
	}
	outf << "Mean = " << mean_eff << endl;
	outf << "Error = " << sqrt(stat_error*stat_error+syst_err*syst_err) << endl;

	//Close files and clean memory
	outf.close();
	for (int i = 0; i < N; i++) {CloseChain(chain[i]);}
	delete[] eff;
	delete[] chain;
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
	case 2:
		EffStats(*(new string(argv[1])), *(new string(argv[2])));
		break;
	case 3:
		EffStats(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
		break;
	case 4:
		EffStats(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
		break;
	case 5:
		EffStats(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
		break;
	case 6:
		EffStats(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for EffStats" << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
