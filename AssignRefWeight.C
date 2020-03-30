//This script aims to assign the weights from a MC sample to its reflected counterpart
//Each sample contains the two components produces from the mass hypothesis: main contribution and reflection
//The weight from the main event is assigned to the reflected event if it matches its evt ID
//If a main event has been lost (reweighting range), its reflection is also stripped out
//This way, one can use the main sample to apply a reweighting, and assign the CORRESPONDING weight to the reflection sample
//Assume that the reflection sample is in order! Nothing has been done to it!

#include <string>
#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "../Functions/TreeTools.h"
#include "../Functions/StringTools.h"
#include "../Functions/Filereading.h"
using namespace std;

void AssignRefWeight(string mainfile, string reffile, string outfilename, string wvar, string idvar)
{
	//Open main chain and reflection chain
	TChain* mainchain = GetChain(mainfile);
	TChain* refchain = GetChain(reffile);

	//Open output (for the reflection)
	TFile* outfile = new TFile(Gridify(outfilename).c_str(), "RECREATE");
	TTree* outtree = refchain->CloneTree(0);

 	//Define TTreeFormula for values of the weight and IDs
	TTreeFormula* mainwvalue = new TTreeFormula(wvar.c_str(), wvar.c_str(), mainchain);
	TTreeFormula* mainidvalue = new TTreeFormula(idvar.c_str(), idvar.c_str(), mainchain);
	//Add new branch to reflection tree
	long refwvalue;
	outtree->Branch(wvar.c_str(), &refwvalue, (wvar + "/D").c_str());

	//Time to loop over all entries in mainfile
	for(long i = 0;i<mainchain->GetEntries();i++)
	{
		//Get entry of main chain and output tree
		mainchain->GetEntry(i);
		outtree->GetEntry(i);
		//Get entry corresponding to that ID in reflection chain
		refchain->GetEntry(mainidvalue->EvalInstance());
		//Assign weight
		refwvalue = mainwvalue->EvalInstance();
		outtree->Fill();
		if (i % (mainchain->GetEntries() / 10 + 1) == 0)
		{
			cout << "Processing event: " << i << " / " << mainchain->GetEntries() << endl;
		}
	}

	//Save tree
	outtree->Write();
	//Close all TFiles
	outfile->Close();
	CloseChain(mainchain);
	CloseChain(refchain);
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
		case 5:
		AssignRefWeight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
		break;
		default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
