#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Functions/misc.h"
using namespace std;

void sPlot(string wVar, string pVar, string tupledir)
{
  //____________________________________
  //  void DoSPlot(RooWorkspace* ws){
  std::cout << "Calculate sWeights" << std::endl;
  
  // get what we need out of the workspace to do the fit
  RooAbsPdf* model = ws->pdf("model");
  RooRealVar* zYield = ws->var("zYield");
  RooRealVar* qcdYield = ws->var("qcdYield");
  RooDataSet* data = (RooDataSet*) ws->data("data");
  
  // fit the model to the data.
  model->fitTo(*data, Extended() );
  
  // The sPlot technique requires that we fix the parameters
  // of the model that are not yields after doing the fit.
  RooRealVar* sigmaZ = ws->var("sigmaZ");
  RooRealVar* qcdMassDecayConst = ws->var("qcdMassDecayConst");
  sigmaZ->setConstant();
  qcdMassDecayConst->setConstant();
  
  
  //RooMsgService::instance().setSilentMode(true);
  
  
  // Now we use the SPlot class to add SWeights to our data set
  // based on our model and our yield variables
  RooStats::SPlot* sData = new RooStats::SPlot("sData","An SPlot",
					       *data, model, RooArgList(*zYield,*qcdYield) );
  
  
  // Check that our weights have the desired properties
  
  std::cout << "Check SWeights:" << std::endl;
  
  
  std::cout << std::endl <<  "Yield of Z is "
	    << zYield->getVal() << ".  From sWeights it is "
	    << sData->GetYieldFromSWeight("zYield") << std::endl;
  
  
  std::cout << "Yield of QCD is "
	    << qcdYield->getVal() << ".  From sWeights it is "
	    << sData->GetYieldFromSWeight("qcdYield") << std::endl
	    << std::endl;
  
  for(Int_t i=0; i < 10; i++)
    {
      std::cout << "z Weight   " << sData->GetSWeight(i,"zYield")
		<< "   qcd Weight   " << sData->GetSWeight(i,"qcdYield")
		<< "  Total Weight   " << sData->GetSumOfEventSWeight(i)
		<< std::endl;
    }
  
  std::cout << std::endl;
  
  // import this new dataset with sWeights
  std::cout << "import new dataset with sWeights" << std::endl;
  ws->import(*data, Rename("dataWithSWeights"));
}
