//Function to dump into a file the bin limits for some homogeneous binning (1D)
//It means that each bin will have the same content
//Weights can be included
#include <string>
#include <iostream>
#include <fstream>
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TTreeFormula.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/Ordenar.h"
using namespace std;

void HomoBins(string var, int Nbins, string filedir, string cutfile, string outfile, string treename = "", string wvar = "")
{
	//Read TChain and cuts
	TChain* chain = GetChain(filedir, treename);
	string cuts = GetCuts(cutfile);

	//Cut TChain and save it in temporary file
	TFile* file = new TFile("Tuples/temp.root", "recreate");
	TTree* tree = (TTree*)chain->CopyTree(cuts.c_str());
	file->Write();

	//We'll need the number of entries a few times, so just request it once
	int entries = tree->GetEntries();
	//Also, entries times the average weight will be used
	double wentries = tree->GetEntries() * GetMeanEntries(tree, wvar);

	//Time to create a matrix with the columns being the variable we want to use to order and the corresponding weight
	double** var_array = new double*[entries];
	//Loop over each entry and save important data in array of doubles
	TTreeFormula* formulavar = new TTreeFormula(var.c_str(), var.c_str(), tree);
	TTreeFormula* formulawvar = new TTreeFormula(wvar.c_str(), wvar.c_str(), tree);
	for (int i = 0; i < entries; i++)
	{
		var_array[i] = new double[2];
		tree->GetEntry(i);
		var_array[i][0] = formulavar->EvalInstance();
		var_array[i][1] = formulawvar->EvalInstance();
	}
	//We can now close chains and trees
	CloseChain(chain);
	CloseTree(tree);

	//We have our matrix, let's order it from lowest to highest
	var_array = Ordenar(entries, 2, 0, var_array);

	//Now it is just a matter to print out in the file the limits
	ofstream fout;
	fout.open(outfile.c_str());
	int index = 0;
	double sum = 0;
	for (int i = 0; index < Nbins; i++)
	{
		if (sum >= wentries / double(Nbins)*index)
		{
			fout << var_array[i][0] << endl;
			index++;
		}
		sum += var_array[i][1];
	}
	fout << var_array[entries-1][0] << endl;
	fout.close();

}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
	case 5:
		HomoBins(*(new string(argv[1])), stoi(*(new string(argv[2]))), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
		break;
	case 6:
		HomoBins(*(new string(argv[1])), stoi(*(new string(argv[2]))), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
		break;
	case 7:
		HomoBins(*(new string(argv[1])), stoi(*(new string(argv[2]))), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
