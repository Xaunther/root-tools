#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "RooStats/SPlot.h"
#include "Functions/Fits.h"
#include "RooWorkspace.h"
using namespace std;

void sPlot(string wVarname, string pVarname, string tupledir, FitOption fitopt)
{
  //RooWorkspace                                                                                                                                                                  
  RooWorkspace* ws = new RooWorkspace();
  //Array of fitting fitted fit functions                                                                                                                                          
  FitFunction* fitf = FitFunction_init();
  
  //Read the data
  int N_files = 0;
  string* filenames = ReadVariables(N_files, tupledir);
  //Load TChain
  string treename = GetTreeName(tupledir); 
  TChain* chain = new TChain(treename.c_str());
  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }
  //Cut chain into new TChain in a temp root file
  tempfile = new TFile("Tuples/temp.root", "recreate");
  temptree = (TTree*)chain->CopyTree(cuts.c_str());
  tempfile->Write();

  //Fit to the desired thingy
  ws = fitf[fitopt](wVarname, temptree);

  //Retrieve stuff from workspace
  //Number of backgrounds
  int N_bkgs = int(ws->var(Names::N_bkgs.c_str())->getValV());
  //Take B_M variable and its name
  RooRealVar* B_M = ws->var(wVarname.c_str());
  //Take dataset
  RooAbsData* data = ws->data(Names::dataset.c_str());
  //Initialize all possible variables
  RooRealVar** mean = new RooRealVar*[N_bkgs+1];
  RooRealVar** width = new RooRealVar*[N_bkgs+1];
  RooRealVar* alpha = new RooRealVar();
  RooRealVar** alphaL = new RooRealVar*[N_bkgs+1];
  RooRealVar** alphaR = new RooRealVar*[N_bkgs+1];
  RooRealVar* n = new RooRealVar();
  RooRealVar** nL = new RooRealVar*[N_bkgs+1];
  RooRealVar** nR = new RooRealVar*[N_bkgs+1];
  RooRealVar* tau = new RooRealVar();
  RooRealVar* fsig = new RooRealVar();
  RooRealVar** fbkg = new RooRealVar*[abs(N_bkgs)];
  //If N_bkgs is -1 it means we don't even fit! Cannot do sPlot!
  //Also, we need at least 2 components so N_bkgs must be bigger than 0

  /**************************************************
   *Future me, this piece should go in another function, since it will be huge and the only thing I'm doing is setting all the parameters except the yields to constant
   *************************************************/
  if(N_bkgs <= 0)
    {
      return;
    }
  else
    {
      //Take out parameters that are not in array
      alpha = 0;
      n = 0;
      tau = 0;
      
      //Take parameters out of ws
      mean = ws->var(Names::mean[0].c_str());
      width = ws->var(Names::width[0].c_str());
      fsig = ws->var(Names::fsig.c_str());
      for(int i=0;i<N_bkgs;i++)
	{
	  fbkg[i] = ws->var(Names::fbkg[i].c_str());
	}
    }
  
  // The sPlot technique requires that we fix the parameters
  // of the model that are not yields after doing the fit.
  mean->setConstant();
  width->setConstant();
  slope.setConstant();
  mean.setConstant();
  width.setConstant();

  //RooMsgService::instance().setSilentMode(true);
  
  
  // Now we use the SPlot class to add SWeights to our data set
  // based on our model and our yield variables
  
  RooStats::SPlot sData = RooStats::SPlot("sData","An SPlot", data, &model, RooArgList(fsig,fbkg));
  
  // Check that our weights have the desired properties
  
  std::cout << "Check SWeights:" << std::endl;
  
  
  std::cout << std::endl <<  "Yield of signal is "
	    << fsig.getVal() << ".  From sWeights it is "
	    << sData.GetYieldFromSWeight("fsig") << std::endl;
  
  
  std::cout << "Yield of background is "
	    << fbkg.getVal() << ".  From sWeights it is "
	    << sData.GetYieldFromSWeight("fbkg") << std::endl
	    << std::endl;
  //This is a loop over some events to check the sWeights
  for(int i=0;i<10;i++)
    {
      std::cout << "   Signal Weight      " << sData.GetSWeight(i,"fsig")
		<< "   Background Weight  " << sData.GetSWeight(i,"fbkg")
		<< "   Total Weight       " << sData.GetSumOfEventSWeight(i)
		<< std::endl;
    }

  
  std::cout << std::endl;
  
  // import this new dataset with sWeights
  std::cout << "import new dataset with sWeights" << std::endl;
  
  //I should plot 4 things:
  //Var 1 normal and sWeighted (2 contributions)
  //Var 2 normal and sWeighted (2 contributions)
  RooPlot* pframe = pVar.frame(RooFit::Bins(Constants::bins));
  RooPlot* pframeW = pVar.frame(RooFit::Bins(Constants::bins));
  RooPlot* wframe = wVar.frame(RooFit::Bins(Constants::bins));
  RooPlot* wframeW = wVar.frame(RooFit::Bins(Constants::bins));

  RooDataSet* sigdata = new RooDataSet(data.GetName(), data.GetTitle(), &data, *data.get(), 0, "fsig_sw");
  RooDataSet* bkgdata = new RooDataSet(data.GetName(), data.GetTitle(), &data, *data.get(), 0, "fbkg_sw");

  data.plotOn(pframe); //Plot raw pVar
  data.plotOn(wframe); //Plot raw wVar
  sigdata->plotOn(pframeW, RooFit::MarkerColor(kRed));
  bkgdata->plotOn(pframeW, RooFit::MarkerColor(kBlue)); //Plot both contributions in pVar
  sigdata->plotOn(wframeW, RooFit::MarkerColor(kRed));
  bkgdata->plotOn(wframeW, RooFit::MarkerColor(kBlue)); //Plot both contributions in wVar

  TCanvas* c1 = new TCanvas();
  pframe->Draw();
  TCanvas* c2 = new TCanvas();
  pframeW->Draw();
  TCanvas* c3 = new TCanvas();
  wframe->Draw();
  TCanvas* c4 = new TCanvas();
  wframeW->Draw();
  //Save plots
  c1->SaveAs(("plots/"+pVarname+"_orig.pdf").c_str());
  c2->SaveAs(("plots/"+pVarname+"_sWeight.pdf").c_str());
  c3->SaveAs(("plots/"+wVarname+"_orig.pdf").c_str());
  c4->SaveAs(("plots/"+wVarname+"_sWeight.pdf").c_str());
}
