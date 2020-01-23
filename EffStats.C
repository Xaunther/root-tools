//This script takes a bunch of files (or list of files separated by white spaces) and computes the efficiency of some cut for them, separately
//Then returns the mean efficiency and the error associated
//This is used, for example, when computing PID systematics

#include <string>
#include <iostream>
#include <fstream>
#include "../Functions/StringTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/ArrayTools.h"
using namespace std;

void EffStats(string dirfiles, string cutfile, string precutfile = "", string outfile = "EffStats_results.txt", string weight = "", string separator = " ")
{
	//Start by reading the cuts and the precuts
	string cuts = GetCuts(cutfile);
	string precuts = GetCuts(precutfile);

	//Now, get list of dirfiles in array
	int N = 0;
	string* dirfile = SplitString(N, dirfiles, separator);

	//Now we can open the chains
	TChain** chain = new TChain*[N];
	for (int i = 0; i < N; i++)
	{
		chain[i] = GetChain(dirfile[i]);
	}

	//Finally, it is time to get the efficiency in each chain, and save it in an array
	double* eff = new double[N];
	for (int i = 0; i < N; i++)
	{
		eff[i] = GetMeanEntries(chain[i], cuts, weight) / GetMeanEntries(chain[i], precuts, weight);
	}
	//Now we can make some statistics, mainly get the mean and standard deviation of the sample
	double mean_eff = GetArrayMean(eff, N);
	double stddev_eff = GetArrayStdDev(eff, N);

	//Save into requested file
	ofstream outf;
	outf.open(outfile.c_str());
	outf << "Efficiency for each sample" << endl;
	outf << "--------------------------" << endl;
	for (int i = 0; i < N; i++)
	{
		outf << i << " | " << eff[i] << endl;
	}
	outf << "Mean = " << mean_eff << endl;
	outf << "StdDev = " << stddev_eff << endl;

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
