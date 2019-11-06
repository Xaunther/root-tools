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
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "../Dictionaries/Constants.h"
#include "../Dictionaries/Names.h"
using namespace std;

void sPlot(string wVarname, string pVarname, string tupledir, FitOption fitopt, string cutfile = "", string w_var = "", string opts = "")
{
  //Initialize constants
  if (opts == "")
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
  EColor bkg_colours[] = {kRed, kGreen, kYellow, kMagenta, kOrange, kViolet};

  //Read the data
  int N_files = 0;
  string* filenames = ReadVariables(N_files, tupledir);
  string cuts = GetCuts(cutfile);
  //Load TChain
  string treename = GetTreeName(tupledir);
  TChain* chain = new TChain(treename.c_str());
  TTree* temptree = new TTree();
  TFile* tempfile = new TFile();
  //Add to chain and get N of entries
  for (int i = 0; i < N_files; i++)
  {
    chain->Add(filenames[i].c_str());
    cout << i + 1 << " file(s) chained" << endl;
  }
  //Cut chain into new TChain in a temp root file
  tempfile = new TFile("Tuples/temp.root", "recreate");
  temptree = (TTree*)chain->CopyTree(cuts.c_str());
  tempfile->Write();

  //Fit to the desired thingy
  ws = fitf[fitopt](wVarname, temptree, w_var, new string[1] {pVarname}, 1, opts);

  //Number of backgrounds
  int N_comps = int(ws->var(name_list.N_comps.c_str())->getValV());

  //Can only do sPlot if we have a fit with 2 components, at least
  if (N_comps <= 1)
  {
    return;
  }

  //Constantize all except yields
  Constantize(ws);

  //Retrieve wVar
  RooRealVar* wVar = ws->var(wVarname.c_str());
  //Retrieve signal and bkg yields
  RooRealVar** fcomp = new RooRealVar*[N_comps];
  for (int i = 0; i < N_comps; i++)
  {
    fcomp[i] = ws->var(name_list.fcomp[i].c_str());
  }

  //RooMsgService::instance().setSilentMode(true);

  // Now we use the SPlot class to add SWeights to our data set
  // based on our model and our yield variables
  //Retrieve the dataset and add the variable to reweight (to Plot)
  RooDataSet* data = (RooDataSet*)ws->data(name_list.dataset.c_str());
  RooRealVar* pVar = new RooRealVar(pVarname.c_str(), pVarname.c_str(), const_list.xmin2, const_list.xmax2);
  RooDataSet* tempset = new RooDataSet("tempset", "tempset", temptree, RooArgSet(*pVar));
  //Important thing here. Can only merge if both sets are the same size (PASS CUTS TO AVOID!!)
  if (data->merge(tempset))
  {
    return;
  }
  //Need RooArglist of yields
  RooArgList Yield_list;
  for (int i = 0; i < N_comps; i++)
  {
    Yield_list.add(*fcomp[i]);
  }

  RooStats::SPlot sData = RooStats::SPlot("sData", "An SPlot", *data, ws->pdf(name_list.pdfmodel.c_str()), Yield_list);

  /************************************************************************************************************************************************/
  // Check that our weights have the desired properties
  std::cout << "Check SWeights:" << std::endl;
  for (int i = 0; i < N_comps; i++)
  {
    std::cout << "Yield of component is "
              << fcomp[i]->getVal() << ".  From sWeights it is "
              << sData.GetYieldFromSWeight(name_list.fcomp[i].c_str()) << std::endl
              << std::endl;
  }
  //This is a loop over some events to check the sWeights
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < N_comps; j++)
    {
      std::cout << "   Component" << i << " Weight  " << sData.GetSWeight(i, name_list.fcomp[j].c_str());
    }
    std::cout << "   Total Weight       " << sData.GetSumOfEventSWeight(i)
              << std::endl;
  }
  std::cout << std::endl;
  /************************************************************************************************************************************************/

  // import this new dataset with sWeights
  std::cout << "import new dataset with sWeights" << std::endl;

  //I should plot 4 things:
  //Var 1 normal and sWeighted (N_bkgs+1 contributions)
  //Var 2 normal and sWeighted (N_bkgs+1 contributions)
  RooPlot* pframe = pVar->frame(RooFit::Bins(const_list.bins));
  RooPlot* pframeW = pVar->frame(RooFit::Bins(const_list.bins));
  RooPlot* wframe = wVar->frame(RooFit::Bins(const_list.bins));
  RooPlot* wframeW = wVar->frame(RooFit::Bins(const_list.bins));

  RooDataSet** compdata = new RooDataSet*[N_comps];
  for (int i = 0; i < N_comps; i++)
  {
    string puto_nombre = name_list.fcomp[i] + "_sw";
    cout << puto_nombre << endl;
    compdata[i] = new RooDataSet(data->GetName(), data->GetTitle(), data, *data->get(), 0, puto_nombre.c_str());
  }

  data->plotOn(pframe); //Plot raw pVar
  data->plotOn(wframe); //Plot raw wVar
  for (int i = 0; i < N_comps; i++)
  {
    compdata[i]->plotOn(pframeW, RooFit::MarkerColor(bkg_colours[i])); //Plot bkg in pVar
    compdata[i]->plotOn(wframeW, RooFit::MarkerColor(bkg_colours[i])); //Plot bkg in wVar
  }

  TCanvas* c1 = new TCanvas();
  pframe->Draw();
  TCanvas* c2 = new TCanvas();
  pframeW->Draw();
  TCanvas* c3 = new TCanvas();
  wframe->Draw();
  TCanvas* c4 = new TCanvas();
  wframeW->Draw();
  //Save plots
  c1->SaveAs(("plots/" + pVarname + "_orig.pdf").c_str());
  c2->SaveAs(("plots/" + pVarname + "_sWeight.pdf").c_str());
  c3->SaveAs(("plots/" + wVarname + "_orig.pdf").c_str());
  c4->SaveAs(("plots/" + wVarname + "_sWeight.pdf").c_str());
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  FitOption fitopt = StringToFitOption(*(new string(argv[4])));
  switch (argc - 1)
  {
  case 4:
    sPlot(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt);
    break;
  case 5:
    sPlot(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt, *(new string(argv[5])));
    break;
  case 6:
    sPlot(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt, *(new string(argv[5])), *(new string(argv[6])));
    break;
  case 7:
    sPlot(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), fitopt, *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for sPlot" << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
