//This function appends all variables in file2 to file1, and saves it in outfile
//No need for the trees to be same size, so careful with what you mix...

#include <string>
#include <iostream>
#include "TChain.h"
#include "TBranch.h"
#include "TTree.h"
#include "TFile.h"
#include "TError.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Filereading.h"
#include "../Functions/StringTools.h"
using namespace std;

void AppendVars(string file1, string file2, string outfile, string cutfile1 = "", string treename1 = "", string treename2 = "")
{
	//Cuts
	string cuts1 = GetCuts(cutfile1);
	//Get 2 chains
	TChain* raw_chain1 = GetChain(file1, treename1);
	TTree* chain1 = raw_chain1->CopyTree(cuts1.c_str());
	TChain* chain2 = GetChain(file2, treename2);
	//Define outtree
	TFile* file = new TFile(Gridify(outfile).c_str(), "RECREATE");
	TTree* tree = chain1->CloneTree();
	//Print Number of events in each ntuple
	cout << "Tuple 1: " << chain1->GetEntries() << " events" << endl;
	cout << "Tuple 2: " << chain2->GetEntries() << " events" << endl;
	if (chain1->GetEntries() == 0 || chain2->GetEntries() == 0) //Just do nothing. I need this behaviour for PhiG ntuples where I might have 0 entries after selection...
	{
		exit(0);
	}
	if (chain1->GetEntries() != chain2->GetEntries())
	{
		Error("AppendVars.C", "The number of entries in each ntuple must be equal!");
		//Close correctly to avoid other root errors
		file->Close();
		CloseChain(chain2);
		CloseChain(raw_chain1);
		exit(1);
	}
	//Get list of variables from file2
	//List size
	int size = chain2->GetListOfLeaves()->GetEntries();
	//Variable types. For now, double, float int.
	int N_Double = 0;
	int N_Float = 0;
	int N_Int = 0;
	//Get how many vars of each type
	for (int i = 0; i < size; i++)
	{
		string classname = (*chain2->GetListOfLeaves())[i]->ClassName();
		if (classname == "TLeafF")
		{
			N_Float++;
		}
		else if (classname == "TLeafD")
		{
			N_Double++;
		}
		else if (classname == "TLeafI")
		{
			N_Int++;
		}
	}
	//Define arrays
	double* var_Double = new double[N_Double];
	float* var_Float = new float[N_Float];
	int* var_Int = new int[N_Int];
	//Link to chain!
	TBranch** newbranch = new TBranch*[size];
	//Counters for each type
	int i_Double = 0;
	int i_Float = 0;
	int i_Int = 0;

	for (int i = 0; i < size; i++)
	{
		string classname = (*chain2->GetListOfLeaves())[i]->ClassName();
		string varname = (*chain2->GetListOfLeaves())[i]->GetName();
		if (classname == "TLeafF")
		{
			chain2->SetBranchAddress(varname.c_str(), &var_Float[i_Float]);
			newbranch[i] = tree->Branch(varname.c_str(), &var_Float[i_Float], (varname + "/F").c_str());
			i_Float++;
		}
		else if (classname == "TLeafD")
		{
			chain2->SetBranchAddress(varname.c_str(), &var_Double[i_Double]);
			newbranch[i] = tree->Branch(varname.c_str(), &var_Double[i_Double], (varname + "/D").c_str());
			i_Double++;
		}
		else if (classname == "TLeafI")
		{
			chain2->SetBranchAddress(varname.c_str(), &var_Int[i_Int]);
			newbranch[i] = tree->Branch(varname.c_str(), &var_Int[i_Int], (varname + "/I").c_str());
			i_Int++;
		}
	}

	//Loop over chain2 and add to tree
	for (int i = 0; i < chain2->GetEntries(); i++)
	{
		chain2->GetEntry(i);
		tree->GetEntry(i);
		for (int j = 0; j < size; j++)
		{
			newbranch[j]->Fill();
		}
	}

	tree->Write();
	//tree->Print();
	//Close all files
	file->Close();
	CloseChain(chain2);
	CloseChain(raw_chain1);
	//Deallocate
	delete[] newbranch;
	delete[] var_Double; delete[] var_Float; delete[] var_Int;
	delete raw_chain1; delete chain2; delete file;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
	case 3:
		AppendVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
		break;
	case 4:
		AppendVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
		break;
	case 5:
		AppendVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
		break;
	case 6:
		AppendVars(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
