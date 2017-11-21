#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "RooStats/SPlot.h"
#include "Functions/misc.h"
#include "Dictionaries/Constants.h"
using namespace std;

void sPlot(string wVarname, string pVarname, string tupledir)
{
  //Read the data
  int N_files = 0;
  string* filenames = ReadVariables(N_files, tupledir);
  //Load TChain
  string treename = GetTreeName(tupledir); 
  int N_part = GetNPart(tupledir);
  int N_part_plot = GetNPartPlot(pVarname);
  if(N_part_plot==0)
    {
      N_part_plot = N_part;
    }
  TChain* chain = new TChain(treename.c_str());
  //Add to chain and get N of entries
  for(int i=0;i<N_files;i++)
    {
      chain->Add(filenames[i].c_str());
      cout << i+1 << " file(s) chained" << endl;
    }

  //Cut chain into new TChain in a temp root file
  tempfile = new TFile("Tuples/temp.root", "recreate");
  temptree = (TTree*)chain->CopyTree("");

  // ---------------RooFit Start!---------------
  //Define the pdf (linear + gaussian)
  int entries = temptree->GetEntries();
  RooRealVar wVar(wVarname.c_str(), wVarname.c_str(), Constants::xmin, Constants::xmax);
  RooRealVar pVar(pVarname.c_str(), pVarname.c_str(), Constants::xmin2, Constants::xmax2);
  RooRealVar fsig("fsig","fsig", entries/10, 0, entries);
  RooRealVar fbkg("fbkg","fbkg", entries/10, 0, entries);

  //Linear coefficients
  RooRealVar slope("slope", "slope", Constants::slope_0, Constants::slope_min, Constants::slope_max);
  RooPolynomial bkg("bkg", "bkg", wVar, RooArgList(slope));
  //Gaussian coefficients
  RooRealVar mean("mean", "mean", Constants::mean_0, Constants::xmin, Constants::xmax);
  RooRealVar width("width","width", Constants::width_0, Constants::width_min, Constants::width_max);
  RooGaussian signal("signal","signal", wVar, mean, width);
  
  //Combine PDFs
  RooAddPdf model("model", "model", RooArgList(signal,bkg), RooArgList(fsig,fbkg));
  
  //Define DataSet
  RooDataSet data("data", "data", temptree, RooArgSet(wVar, pVar));

  // fit the model to the data.
  model.fitTo(data);
  
  // The sPlot technique requires that we fix the parameters
  // of the model that are not yields after doing the fit.
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
  
  //Plot check
  RooPlot* xframe = pVar.frame();
  data.plotOn(xframe);
  TCanvas* c = new TCanvas();
  xframe->Draw();
  
  RooDataSet* dataw = new RooDataSet(data.GetName(), data.GetTitle(), &data, *data.get(), 0, "fsig_sw");

  RooPlot* xframew = pVar.frame();
  dataw->plotOn(xframew);
  TCanvas* c2 = new TCanvas();
  xframew->Draw();
}
