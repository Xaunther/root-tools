//This function appends all variables in file2 to file1, and saves it in outfile
//No need for the trees to be same size, so careful with what you mix...

#include <string>
#include <iostream>
#include "TChain.h"
#include "TBranch.h"
#include "TTree.h"
#include "TFile.h"
#include "TError.h"
using namespace std;

void AppendVars(string filename1,
				string filename2,
				string outfilename,
				string varname)
{
	//Get 2 input files
	TFile *file1 = TFile::Open(filename1.c_str());
	TTree *tree1 = (TTree *)file1->Get("DecayTree");
	TFile *file2 = TFile::Open(filename2.c_str());
	TTree *tree2 = (TTree *)file2->Get("DecayTree");
	//Define outtree
	TFile *outfile = TFile::Open(outfilename.c_str(), "RECREATE");
	TTree *outtree = tree1->CloneTree();

	//Define branches
	double weight;

	tree2->SetBranchAddress(varname.c_str(), &weight);
	TBranch *newbranch = outtree->Branch(varname.c_str(), &weight, (varname + "/D").c_str());

	//Loop over chain2 and add to tree
	for (int i = 0; i < tree2->GetEntries(); i++)
	{
		tree2->GetEntry(i);
		outtree->GetEntry(i);
		newbranch->Fill();
	}

	outtree->Write();
	outfile->Close();
	file1->Close();
	file2->Close();
}
