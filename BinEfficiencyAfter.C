//Script that aims to compute total efficiency of a sample, given its distribution AFTER some selection
//To do so, we need two reference samples (1 before and 1 after the selection) to compute the per-bin efficiency
//With the per-bin efficiency, the efficiency for a given sample after such selection can be computed
//The selection for which we want to know the efficiency can be itself weighted! This is the case if we are dealing with sWeighted MC.
//The output file must also be specified, saving the resulting eff'/eff, the ratio of the efficiency for our sample related to the original sample efficiency

//Additionally, a file containing the bins might be specified.
// If not, the minimum and maximum values will be used as extremes and the Number of Bins from the Dictionaries will be used, equally distributed.

//This script is currently not included in the compilation, or loaded with rootinit

#include <string>
#include <iostream>
#include "../Functions/TreeTools.h"
#include "../Dictionaries/Constants.h"
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
#include "../Functions/StringTools.h"
#include "TH1F.h"
using namespace std;

void BinEfficiencyAfter(string varname, string inputfile, string beforedistfile, string afterdistfile, string outfile, string binfile = "", string weightvar = "", string opts = "")
{
	//If opts not specified, use default
	if (opts == "")
	{
		opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
	}
	//Dictionaries
	Constants const_list(opts);

	//Open files
	TChain* chain = GetChain(inputfile);
	TChain* beforechain = GetChain(beforedistfile);
	TChain* afterchain = GetChain(afterdistfile);

	//First step is to get per-bin efficiency. For that we must know the binning
	int Nbins = 0;
	double* binning;
	if (binfile != "")
	{
		binning = ReadNumbers(Nbins, binfile);
	}
	else
	{
		Nbins = const_list.bins;
		binning = 0;
	}

	//We may have the case of different varnames (whitespace separating them)
	int N_varnames = 0;
	string beforevarname, aftervarname;
	string* varnames = SplitString(N_varnames, varname, " ");
	if (N_varnames == 1)
	{
		beforevarname = varname; aftervarname = varname;
	}
	else if (N_varnames == 3)
	{
		varname = varnames[0]; beforevarname = varnames[1]; aftervarname = varnames[2];
	}
	//Time to get those histograms filled!
	TH1F* histo = GetHistogram(chain, varname, Nbins - 1, binning, "histo", weightvar);
	TH1F* beforehisto = GetHistogram(beforechain, beforevarname, Nbins - 1, binning, "beforehisto");
	TH1F* afterhisto = GetHistogram(afterchain, aftervarname, Nbins - 1, binning, "afterhisto");

	//Get totals in each sample
	double total = 0., beforetotal = 0., aftertotal = 0.;
	for (int i = 1; i <= histo->GetNbinsX(); i++)
	{
		total += histo->GetBinContent(i);
		beforetotal += beforehisto->GetBinContent(i);
		aftertotal += afterhisto->GetBinContent(i);
	}
	//Go bin after bin, and get the new efficiency
	double neweff = 0.;

	//Start doing the denominator
	for (int i = 1; i <= histo->GetNbinsX(); i++)
	{
		if (afterhisto->GetBinContent(i) == 0)
		{
			cout << "WARNING: Bin " << i << "has no entries" << endl;
		}
		neweff += histo->GetBinContent(i) * (beforehisto->GetBinContent(i) / beforetotal) / (afterhisto->GetBinContent(i) / aftertotal);
		cout << (afterhisto->GetBinContent(i) / aftertotal) / (beforehisto->GetBinContent(i) / beforetotal) << ", " << afterhisto->GetBinContent(i)/ aftertotal << ", " << beforehisto->GetBinContent(i)/ beforetotal << endl;
	}
	neweff = total / neweff;

	//Save result
	cout << neweff << endl;

	//Close everything
	CloseChain(chain);
	CloseChain(beforechain);
	CloseChain(afterchain);
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
	case 5:
		BinEfficiencyAfter(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
		break;
	case 6:
		BinEfficiencyAfter(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
		break;
	case 7:
		BinEfficiencyAfter(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
		break;
	case 8:
		BinEfficiencyAfter(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif