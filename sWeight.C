#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "RooStats/SPlot.h"
#include "../Functions/Fits.h"
#include "../Functions/Dictreading.h"
#include "../Functions/Filereading.h"
#include "../Functions/Constantize.h"
#include "../Functions/TreeTools.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "../Dictionaries/Constants.h"
#include "../Dictionaries/Names.h"
using namespace std;

void sWeight(string varname, string tupledir, string outfilename, FitOption fitopt, string cutfile = "", string w_var = "", string opts = "", string sw_suffix = "")
{
  //Initialize constants
  if(opts=="")
    {
      opts = GetValueFor("Project_name", "Dictionaries/Project_variables.txt");
    }
  Constants const_list(opts);
  Names name_list(opts);

  //RooWorkspace                                                                                                                                                                  
  RooWorkspace* ws = new RooWorkspace();
  //Array of fitting fitted fit functions                                                                                                                                          
  FitFunction* fitf = FitFunction_init();

  //Array of colours for the background. Add more if required
  EColor bkg_colours[] = {kGreen, kYellow, kMagenta, kOrange, kViolet};
  
  //Read the data
  TChain* chain = GetChain(tupledir);
  string cuts = GetCuts(cutfile);
  TTree* temptree = new TTree();
  TFile* tempfile = new TFile();
  //Cut chain into new TChain in a temp root file
  tempfile = new TFile("Tuples/temp.root", "recreate");
  temptree = (TTree*)chain->CopyTree(cuts.c_str());
  tempfile->Write();

  //Fit to the desired thingy
  ws = fitf[fitopt](varname, temptree, w_var, 0, 0, opts);

  //Number of backgrounds
  int N_bkgs = int(ws->var(name_list.N_bkgs.c_str())->getValV());

  //Can only do sPlot if we have a fit with 2 components, at least
  if(N_bkgs <= 0)
    {
      return;
    }  

  //Constantize all except yields
  Constantize(ws);

  //Retrieve wVar
  RooRealVar* wVar = ws->var(varname.c_str());
  //Retrieve signal and bkg yields
  RooRealVar* fsig = new RooRealVar();
  RooRealVar** fbkg = new RooRealVar*[N_bkgs];
  fsig = ws->var(name_list.fsig.c_str());
  for(int i=0;i<N_bkgs;i++)
    {
      fbkg[i] = ws->var(name_list.fbkg[i].c_str());
    }
  
  // Now we use the SPlot class to add SWeights to our data set
  // based on our model and our yield variables
  //Retrieve the dataset and add the variable to reweight (to Plot)
  RooDataSet* data = (RooDataSet*)ws->data(name_list.dataset.c_str());
  //Need RooArglist of yields
  RooArgList Yield_list(*fsig);
  for(int i=0;i<N_bkgs;i++)
    {
      Yield_list.add(*fbkg[i]);
    }
  
  RooStats::SPlot sData = RooStats::SPlot("sData","An SPlot", *data, ws->pdf(name_list.pdfmodel.c_str()), Yield_list);
  
  /************************************************************************************************************************************************/
  // Check that our weights have the desired properties  
  std::cout << "Check SWeights:" << std::endl;  
  std::cout << std::endl <<  "Yield of signal is "
	    << fsig->getVal() << ".  From sWeights it is "
	    << sData.GetYieldFromSWeight(name_list.fsig.c_str()) << std::endl;  
  for(int i=0;i<N_bkgs;i++)
    {
      std::cout << "Yield of background is "
		<< fbkg[i]->getVal() << ".  From sWeights it is "
		<< sData.GetYieldFromSWeight(name_list.fbkg[i].c_str()) << std::endl
		<< std::endl;
    }
  //This is a loop over some events to check the sWeights
  for(int i=0;i<10;i++)
    {
      std::cout << "   Signal Weight      " << sData.GetSWeight(i,name_list.fsig.c_str());
      for(int j=0;j<N_bkgs;j++)
	{
	  std::cout << "   Background" << i << " Weight  " << sData.GetSWeight(i,name_list.fbkg[j].c_str());
	}
      std::cout << "   Total Weight       " << sData.GetSumOfEventSWeight(i)
		<< std::endl;
    }
  std::cout << std::endl;
  /************************************************************************************************************************************************/

  //Time to save the sWeights
  temptree->SetBranchStatus("*", 1);
  double branchvalue;
  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  TTree* outtree = temptree->CloneTree();
  TBranch* newbranch = outtree->Branch(("sWeight"+sw_suffix).c_str(), &branchvalue, ("sWeight"+sw_suffix+"/D").c_str());
  for(int i=0;i<outtree->GetEntries();i++)
    {
      outtree->GetEntry(i);
      branchvalue = sData.GetSWeight(i,name_list.fsig.c_str());
      newbranch->Fill();
      if(i%(outtree->GetEntries()/10)==0)
	{
	  cout << "Processing event: " << i << " / " << outtree->GetEntries() << endl;
	}
    }
  //Write into file
  outtree->Write();
  outfile->Close();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  FitOption fitopt = StringToFitOption(*(new string(argv[4])));
  switch(argc)
    {
    case 4:
      sWeight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt);
      break;
    case 5:
      sWeight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt, *(new string(argv[5])));
      break;
    case 6:
      sWeight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt, *(new string(argv[5])), *(new string(argv[6])));
      break;
    case 7:
      sWeight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt, *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
      break;
    case 8:
      sWeight(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt, *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])), *(new string(argv[8])));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for sWeight" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
