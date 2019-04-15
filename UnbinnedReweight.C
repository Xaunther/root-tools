//Function to perform unbinned reweighting of a sample, to match the distribution of a second sample
//What it does is to assign weights to each event of the first sample
#include <iostream>
#include <string>
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "../Functions/Filereading.h"
#include "../Functions/TreeTools.h"
#include "../Functions/Ordenar.h"
using namespace std;

//Sample: Sample to reweight
//Model: Sample with the distribution we want
//varname_sample: Variable we want to reweight in the sample
//varname_model: Variable we want to look at in the model sample (names might differ)
//cutfile_sample: Cuts needed for sample
//cutfile_model: Cuts needed for model
//wVar: If the variable in the model sample has some weight, say it!
//suffix: the weights will be saved with name weight[_suffix]
void UnbinnedReweight(string sample, string model, string varname_sample, string varname_model, string outfilename, string cutfile_sample = "", string cutfile_model = "", string wVar = "", string suffix = "")
{
  //Retrieve the two samples
  TChain* chain_sample = GetChain(sample);
  TChain* chain_model = GetChain(model);
  //Get Cuts
  string cuts_sample = GetCuts(cutfile_sample);
  string cuts_model = GetCuts(cutfile_model);

  //Cut sample tree
  TFile* file_sample = new TFile("Tuples/temp_sample.root", "recreate");
  TTree* tree_sample = (TTree*)chain_sample->CopyTree(cuts_sample.c_str());
  file_sample->Write();
  //Cut model tree
  TFile* file_model = new TFile("Tuples/temp_model.root", "recreate");
  TTree* tree_model = (TTree*)chain_model->CopyTree(cuts_model.c_str());
  file_model->Write();

  //Get entries in each sample
  int entries_sample = tree_sample->GetEntries();
  int entries_model = tree_model->GetEntries();
  
  //Array with the entries, it will be ordered. It is a table with necessary stuff
  double** var_sample = new double*[entries_sample];
  double** var_model = new double*[entries_model];
  //Loop over each tree and save important data in array of doubles
  double basura;
  double basura2 = 1.;
  tree_sample->SetBranchAddress((varname_sample).c_str(), &basura);
  for(int i=0;i<entries_sample;i++)
    {
      var_sample[i] = new double[3];
      tree_sample->GetEntry(i);
      var_sample[i][0] = basura;
      var_sample[i][1] = i;
      var_sample[i][2] = 0.;
    }
  tree_model->SetBranchAddress((varname_model).c_str(), &basura);
  if(wVar!="")
    {
      tree_model->SetBranchAddress((wVar).c_str(), &basura2);
    }
  for(int i=0;i<entries_model;i++)
    {
      var_model[i] = new double[2];
      tree_model->GetEntry(i);
      var_model[i][0] = basura;
      var_model[i][1] = basura2;
    }
  //Order the sample. From lower to higher
  var_sample = Ordenar(entries_sample, 2, 0, var_sample);
  var_model = Ordenar(entries_model, 2, 0, var_model);
  //Loop over the modelling sample
  int j=0;
  for(int i=0;i<entries_model;i++)
    {
      int N_i = 0;
      //For each one of them, find when to stop in sample
      for(j=j;var_model[i][0]>var_sample[j][0];j++){N_i++;}
      for(int k=j-N_i;k<j;k++)
	{
	  var_sample[k][2] = var_model[i][1]/double(N_i); //Weights overwrite original mass array
	}
    }

  //Order back to original
  var_sample = Ordenar(entries_sample, 3, 1, var_sample);
  //Apply the weights and save in a new tree
  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  TTree* outtree = (TTree*)chain_sample->CopyTree(cuts_sample.c_str());
  TBranch* wBranch = outtree->Branch(("weight"+suffix).c_str(), &basura, ("weight"+suffix+"/D").c_str());
  for(int i=0;i<entries_sample;i++)
    {
      if(i%(entries_sample/10)==0)
	{
	  cout << "Processing event: " << i << " / " << entries_sample << endl;
	}
      outtree->GetEntry(i);
      basura = var_sample[i][2];
      wBranch->Fill();
    }
  outtree->Write();
  outfile->Close();
}
