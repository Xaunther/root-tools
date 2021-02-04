#include <string>
#include <iostream>
#include "TChain.h"
#include "TObject.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "Functions/TreeTools.h"
#include "Functions/GetCoincidences.h"
#include "Functions/Dictreading.h"
#include "Functions/Filereading.h"
#include "Functions/StringTools.h"
using namespace std;

TChain *GetChain(string *filenames, int N_files, string treename, bool verbose)
{
  if (verbose)
  {
    cout << "Reading Tree " << treename << endl;
  }
  TChain *chain = new TChain(treename.c_str());

  //Add to chain and get N of entries
  for (int i = 0; i < N_files; i++)
  {
    chain->Add(Gridify(filenames[i]).c_str());
  }
  //Safecheck to avoid errors when opening trees with 0 entries
  N_files = chain->GetEntries();
  return chain;
}

TChain *GetChain(string filedir, string tuplename, bool verbose)
{
  //Data chain
  string treename;
  if (tuplename != "")
    treename = tuplename;
  else
    treename = GetTreeName(filedir);

  int N_files = 0;
  string *filenames = ReadVariables(N_files, filedir);

  return GetChain(filenames, N_files, treename, verbose);
}

TTree *GetTree(string filedir, string tuplename, bool verbose)
{
  //Data chain
  string treename;
  if (tuplename != "")
  {
    treename = tuplename;
  }
  else
  {
    treename = GetTreeName(filedir);
  }
  if (verbose)
  {
    cout << "Reading Tree " << treename << endl;
  }
  TFile *file = TFile::Open(Gridify(filedir).c_str());
  TTree *tree = (TTree *)file->Get(treename.c_str());

  return tree;
}

void AddTreeBranch(TTree *starttree, string branchname, string formula, string filename)
{
  //Add new branch
  double branchvalue; //Stick to a double output for now...

  //Define TTreeFormula
  TTreeFormula *formulavar = new TTreeFormula(formula.c_str(), formula.c_str(), starttree);

  //Add new branch
  TFile *file = TFile::Open(filename.c_str(), "RECREATE");
  TTree *tree = starttree->CloneTree(0);
  tree->Branch(branchname.c_str(), &branchvalue, (branchname + "/D").c_str());

  //Loop over all events and get value
  for (int i = 0; i < starttree->GetEntries(); i++)
  {
    starttree->GetEntry(i);
    tree->GetEntry(i);
    //Insert formula, if any
    branchvalue = formulavar->EvalInstance();
    tree->Fill();
  }
  tree->Write();
  file->Close();
}

int GetEvents(TTree *alltree, string cuts)
{
  ULong64_t evtnumber;
  TTree *tree = (TTree *)alltree->CopyTree(cuts.c_str());
  tree->SetBranchAddress("eventNumber", &evtnumber);
  ULong64_t currentevt = 0;
  int N = 0;
  for (int i = 0; i < tree->GetEntries(cuts.c_str()); i++)
  {
    tree->GetEntry(i);
    if (i == 0)
    {
      currentevt = evtnumber;
      N++;
    }
    if (currentevt != evtnumber)
    {
      currentevt = evtnumber;
      N++;
    }
  }
  return N;
}

int GetEvents(TChain *chain, string cuts)
{
  return GetEvents((TTree *)chain->CopyTree(cuts.c_str()));
}

int GetCorrEvents(TTree *alltree1, TTree *alltree2, string cuts1, string cuts2)
{
  ULong64_t evtnumber1, evtnumber2;
  TTree *tree1 = (TTree *)alltree1->CopyTree(cuts1.c_str());
  TTree *tree2 = (TTree *)alltree2->CopyTree(cuts2.c_str());
  tree1->SetBranchAddress("eventNumber", &evtnumber1);
  tree2->SetBranchAddress("eventNumber", &evtnumber2);
  ULong64_t currentevt = 0;
  ULong64_t *id_evtnumber1 = new ULong64_t[tree1->GetEntries()];
  ULong64_t *id_evtnumber2 = new ULong64_t[tree2->GetEntries()];
  int N1 = 0;
  int N2 = 0;
  for (int i = 0; i < tree1->GetEntries(); i++)
  {
    tree1->GetEntry(i);
    if (i == 0)
    {
      currentevt = evtnumber1;
      id_evtnumber1[N1] = evtnumber1;
      N1++;
    }
    if (currentevt != evtnumber1)
    {
      currentevt = evtnumber1;
      id_evtnumber1[N1] = evtnumber1;
      N1++;
    }
  }
  for (int i = 0; i < tree2->GetEntries(); i++)
  {
    tree2->GetEntry(i);
    if (i == 0)
    {
      currentevt = evtnumber2;
      id_evtnumber2[N2] = evtnumber2;
      N2++;
    }
    if (currentevt != evtnumber2)
    {
      currentevt = evtnumber2;
      id_evtnumber2[N2] = evtnumber2;
      N2++;
    }
  }
  return GetCoincidences(id_evtnumber1, N1, id_evtnumber2, N2);
}
int GetCorrEvents(TTree *tree1, TChain *tree2, string cuts1, string cuts2)
{
  return GetCorrEvents(tree1, (TTree *)tree2->CopyTree(cuts2.c_str()), cuts1, cuts2);
}
int GetCorrEvents(TChain *tree1, TTree *tree2, string cuts1, string cuts2)
{
  return GetCorrEvents((TTree *)tree1->CopyTree(cuts1.c_str()), tree2, cuts1, cuts2);
}
int GetCorrEvents(TChain *tree1, TChain *tree2, string cuts1, string cuts2)
{
  return GetCorrEvents((TTree *)tree1->CopyTree(cuts1.c_str()), (TTree *)tree2->CopyTree(cuts2.c_str()), cuts1, cuts2);
}

void Activate(TTree *tree, string varname)
{
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus(varname.c_str(), 1);
  return;
}
void Activate(TChain *tree, string varname)
{
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus(varname.c_str(), 1);
  return;
}
void Activate(TTree *tree, string *vars, int N)
{
  tree->SetBranchStatus("*", 0);
  for (int i = 0; i < N; i++)
  {
    tree->SetBranchStatus(vars[i].c_str(), 1);
  }
  return;
}
void activate(TChain *tree, string *vars, int N)
{
  tree->SetBranchStatus("*", 0);
  for (int i = 0; i < N; i++)
  {
    tree->SetBranchStatus(vars[i].c_str(), 1);
  }
  return;
}
//Get mean of a certain variable out of a certain chain
double GetMean(string filedir, string varname, string treename, string cuts, string weight)
{
  if (cuts == "")
  {
    cuts = "1";
  }
  if (weight == "")
  {
    weight = "1";
  }
  if (TreeExists(filedir, treename))
  {
    TChain *chain = GetChain(filedir, treename, false);
    double result = GetMean(chain, varname, cuts, weight);
    delete chain;
    return result;
  }
  else
  {
    return 0.;
  }
}
//Intermediate function to get mean weighted number of entries. It uses the GetMean function with the cuts and weight as varname.
//In short, it returns the number of entries(weighted with weight) that passes a certain selection (cuts), divided by the number of entries of the ntuple (with no selection)
//Therefore, this function returns kind of the efficiency of the cuts+weight. To get the number of entries (weighted) one should multiply by the number of entries of the ntuple
double GetMeanEntries(string filedir, string treename, string cuts, string weight)
{
  if (cuts == "")
  {
    cuts = "1";
  }
  if (weight == "")
  {
    weight = "1";
  }
  return GetMean(filedir, "(" + cuts + ")*" + weight, treename);
}
double GetMean(TChain *chain, string varname, string cuts, string weight)
{
  if (!chain->GetEntries())
  {
    return 0;
  }
  chain->Draw(varname.c_str(), ("(" + cuts + ")*" + weight).c_str(), "goff"); //Get number of entries that pass the cut. And get a histogram
  return chain->GetHistogram()->GetMean();
}
double GetMeanEntries(TChain *chain, string cuts, string weight)
{
  if (cuts == "")
  {
    cuts = "1";
  }
  if (weight == "")
  {
    weight = "1";
  }
  return GetMean(chain, "(" + cuts + ")*" + weight);
}
double GetMean(TTree *chain, string varname, string cuts, string weight)
{
  if (!chain->GetEntries())
  {
    return 0;
  }
  chain->Draw(varname.c_str(), ("(" + cuts + ")*" + weight).c_str(), "goff"); //Get number of entries that pass the cut. And get a histogram
  return chain->GetHistogram()->GetMean();
}
double GetMeanEntries(TTree *chain, string cuts, string weight)
{
  if (cuts == "")
  {
    cuts = "1";
  }
  if (weight == "")
  {
    weight = "1";
  }
  return GetMean(chain, "(" + cuts + ")*" + weight);
}

//Function to check if tree with name tuplename exists within file filename.
//If tuplename is not provided, it is looked up at the Dictionaries/treenames.dic file
bool TreeExists(string filedir, string tuplename)
{
  //Get treename
  string treename;
  if (tuplename != "")
  {
    treename = tuplename;
  }
  else
  {
    treename = GetTreeName(filedir);
  }
  //I need just the treename, not the directory... *sigh*
  treename = treename.substr(treename.find_last_of("/") + 1);
  //Get list of ntuples
  int N_files = 0;
  string *filenames = ReadVariables(N_files, filedir);
  //Search for required object
  for (int i = 0; i < N_files; i++)
  {
    TFile *file = new TFile(filenames[i].c_str());
    TObject *tree = file->FindObjectAny(treename.c_str());
    if (tree != nullptr)
    {
      file->Close();
      return true;
    }
  }
  return false;
}

//Small utility to perfectly close a file associated to a TChain
void CloseChain(TChain *&chain)
{
  TFile *file = chain->GetCurrentFile();
  if (!file)
    return;
  chain->SetDirectory(0);
  file->Close();
}

//Small utility to perfectly close a file associated to a TTree
void CloseTree(TTree *&chain)
{
  TFile *file = chain->GetCurrentFile();
  if (!file)
    return;
  chain->SetDirectory(0);
  file->Close();
}

//Functions to get histograms from ttree/tchain. Must specify binning
TH1F *GetHistogram(TTree *chain, string varname, int N, double *binning, string hname, string wvar)
{
  TH1F *histo = new TH1F(hname.c_str(), hname.c_str(), N, binning);
  chain->Draw((varname + ">>+" + hname).c_str(), wvar.c_str(), "goff");
  return histo;
}
TH1F *GetHistogram(TChain *chain, string varname, int N, double *binning, string hname, string wvar)
{
  TH1F *histo = new TH1F(hname.c_str(), hname.c_str(), N, binning);
  chain->Draw((varname + ">>+" + hname).c_str(), wvar.c_str(), "goff");
  return histo;
}
//Functions to get 2D-histograms from ttree/tchain. Must specify binning
TH2F *GetHistogram2D(TTree *chain, string varname, int NX, double *binX, int NY, double *binY, string hname, string wvar)
{
  TH2F *histo = new TH2F(hname.c_str(), hname.c_str(), NX, binX, NY, binY);
  chain->Draw((varname + ">>+" + hname).c_str(), wvar.c_str(), "goff");
  return histo;
}
TH2F *GetHistogram2D(TChain *chain, string varname, int NX, double *binX, int NY, double *binY, string hname, string wvar)
{
  TH2F *histo = new TH2F(hname.c_str(), hname.c_str(), NX, binX, NY, binY);
  chain->Draw((varname + ">>+" + hname).c_str(), wvar.c_str(), "goff");
  return histo;
}
