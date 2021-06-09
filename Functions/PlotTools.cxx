#include <string>
#include <sstream>
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooAddPdf.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooPlot.h"
#include "RooHist.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLine.h"
#include "TAxis.h"
#include "TMath.h"
#include "TIterator.h"
#include "Functions/Dictreading.h"
#include "Functions/PlotTools.h"
#include "Functions/StringTools.h"
#include "Dictionaries/Names.h"
#include "Dictionaries/Constants.h"
using namespace std;

//Returns formatted title
string SetPlotTitle(Constants *const_list, double bin_size, string title, string Xtitle)
{
  //Get bin size as string, no decimal places
  stringstream ss;
  ss << fixed;
  ss.precision(0);
  ss << bin_size;
  if (title == "")
  {
    title = const_list->plot_title + " ; ";
  }
  else //Override title from Dictionary
  {
    title = title + " ; ";
  }
  if (Xtitle == "")
  {
    title = title + const_list->plot_Xtitle + "(" + const_list->plot_units + ") ; Candidates / (" + ss.str() + " " + const_list->plot_units + ")";
  }
  else //Override Xtitle from Dictionary
  {
    title = title + Xtitle + "(" + const_list->plot_units + ") ; Candidates / (" + ss.str() + " " + const_list->plot_units + ")";
  }
  return title;
}

//Modifies legend to set up its position and components
TLegend *SetupLegend(Constants *const_list, const int N_comps, RooPlot *xframe, RooWorkspace *ws)
{
  TLegend *leg = 0;
  //Align legend
  if (const_list->legend_L && (const_list->Legend_y0 != const_list->Legend_yf)) //Left aligned, given height
  {
    leg = new TLegend(const_list->Legend_x0_L, const_list->Legend_y0, const_list->Legend_xf_L, const_list->Legend_yf);
  }
  else if (const_list->Legend_y0 != const_list->Legend_yf) //Right aligned, given height
  {
    leg = new TLegend(const_list->Legend_x0_R, const_list->Legend_y0, const_list->Legend_xf_R, const_list->Legend_yf);
  }
  else //Auto height, top is fixed and goes down as much as needed
  {
    double height_per_item = 0.065; //Height per item is legend
    int N_items = 1;
    if (N_comps == 1)
    {
      N_items = 2;
    }
    else if (N_comps > 1)
    {
      N_items = N_comps + 2;
    }
    leg = new TLegend(const_list->Legend_x0_R, const_list->Legend_yf - (height_per_item * N_items), const_list->Legend_xf_R, const_list->Legend_yf);
  }
  //Data points
  RooAbsData *data = GetDataFromWS(ws);
  leg->AddEntry(xframe->getObject(0), data->GetName(), "lpe");
  //Whole fit only if we have 2 or more components
  RooArgSet pdflist = ws->allPdfs();
  TIterator *pdfiter = pdflist.createIterator();
  string pdfname = pdfiter->Next()->GetName();
  if (N_comps > 1)
  {
    leg->AddEntry(xframe->getObject(1), pdfname.c_str(), "lf");
    //PDF components
    for (int i = 0; i < N_comps; i++)
    {
      pdfname = pdfiter->Next()->GetName();
      leg->AddEntry(xframe->getObject(2 + i), pdfname.c_str(), "lf");
    }
  }
  else if (N_comps == 1)
  {
    leg->AddEntry(xframe->getObject(1), pdfname.c_str(), "lf");
  }
  leg->SetTextSize(0.04);
  return leg;
}

//Get the data sample, it takes the first one it finds.
RooAbsData *GetDataFromWS(RooWorkspace *ws)
{
  std::list<RooAbsData *> datalist = ws->allData();
  return datalist.front();
}

void GoodPlot(RooWorkspace *ws, string variablename, string title, string Xtitle, string opts, string namesuf, int Nbins)
{
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);

  //Compute bin size

  if (Nbins == 0)
  {
    Nbins = const_list.bins[0];
  }
  double bin_size = (const_list.xmax - const_list.xmin) / Nbins;

  //Array of colours for the components
  EColor comp_colours[] = {kRed, kBlue, kYellow, kGreen, kMagenta, kCyan, kBlack, kOrange};
  //Retrieve stuff from workspace
  //Take fit variable from its name
  RooRealVar *B_M = ws->var(variablename.c_str());
  //Take dataset
  RooAbsData *data = GetDataFromWS(ws);
  //Count number of components, must be contained in the same workspace
  int N_comps;
  if (ws->allPdfs().size()) //We either have a RooAddPdf with multiple or a single component
  {
    //If there are PDFs, then it goes like this:
    //1 pdf: 1 entry in list
    //2 pdfs: 3 entries in list (rooaddpdf + each component)
    //n pdfs: n+1 entries in list
    N_comps = ws->allPdfs().size();
    if (N_comps > 2)
    {
      N_comps--;
    }
  }
  else
  {
    N_comps = 0;
  } //No fit was done
  //Set plot title
  title = SetPlotTitle(&const_list, bin_size, title, Xtitle);
  //Define data frame
  RooPlot *xframe = B_M->frame(RooFit::Title(title.c_str()), RooFit::Bins(Nbins));
  //Define pull plots frame
  RooPlot *resframe = new RooPlot();
  //Tune the axes
  xframe->GetYaxis()->SetTitleOffset(1.3);
  //Define data points style. Plot them first
  data->plotOn(xframe, RooFit::MarkerColor(kBlack), RooFit::MarkerStyle(kFullDotMedium), RooFit::LineColor(kBlack), RooFit::DataError((RooAbsData::ErrorType)const_list.SumW2Error));
  //Define legend
  TLegend *leg = 0;
  if (N_comps > 0) //We have a fit to plot!
  {
    //Extract the list of pdfs and iterate on it
    RooArgSet pdflist = ws->allPdfs();
    TIterator *pdfiter = pdflist.createIterator();
    //One of these is the RooAddPdf (or the only function), we have to find it
    string totalpdf = pdfiter->Next()->GetName();
    //Plot whole PDF, default colour is the second of the array, as it usually coincides with combinatorial bkg
    ws->pdf(totalpdf.c_str())->plotOn(xframe, RooFit::LineColor(comp_colours[1]), RooFit::Name(totalpdf.c_str()));
    //pdfiter->Reset();
    //If we have more than 1 component, plot each of them
    if (N_comps > 1)
    {
      for (int i = 0; i < N_comps; i++)
      {
        ws->pdf(totalpdf.c_str())->plotOn(xframe, RooFit::LineColor(comp_colours[i]), RooFit::Components(pdfiter->Next()->GetName()), RooFit::LineStyle(kDashed));
      }
    }
    //Set up the legend
    leg = SetupLegend(&const_list, N_comps, xframe, ws);
    // S h o w   p u l l   d i s t r i b u t i o n
    // -------------------------------------------
    // Construct a histogram with the residuals of the data w.r.t. the curve
    RooHist *hresid = xframe->residHist(("h_" + string(data->GetName())).c_str(), totalpdf.c_str(), true);

    // Create a new frame to draw the residual distribution and add the distribution to the frame
    resframe = B_M->frame(RooFit::Title("Residual plot"));
    hresid->SetFillColor(39);
    hresid->SetMarkerSize(0.01);
    resframe->addPlotable(hresid, "E3");
  }

  //Define canvas, pads and lines
  TCanvas *c = new TCanvas();
  TPad *padHisto = new TPad();
  TPad *padResid = new TPad();
  TLine *uppLine = new TLine();
  TLine *midLine = new TLine();
  TLine *lowLine = new TLine();
  if (N_comps >= 1)
  {
    double res_ratio = 0.2;
    double label_size = 0.03; //0.04 is default
    c->Divide(1, 2);
    c->cd(0)->SetTicks();
    c->cd(1);
    xframe->Draw();
    if (const_list.Legend)
    {
      leg->Draw();
    }
    c->cd(2);
    resframe->Draw();
    //Retrieve canvas parts and configure them
    padHisto = (TPad *)c->GetListOfPrimitives()->At(0);
    padResid = (TPad *)c->GetListOfPrimitives()->At(1);
    //Configure histo pad
    padHisto->SetPad(0., res_ratio, 1.0, 1.0);
    padHisto->SetLogy(const_list.logY);
    //Configure resid pad
    padResid->SetPad(0.0, 0.0, 1.0, res_ratio);
    padResid->SetBottomMargin(0.2);
    //Configure histogram axes
    xframe->GetXaxis()->SetLabelSize(0); //Don't plot labels
    xframe->GetXaxis()->CenterTitle();
    xframe->GetXaxis()->SetTitleSize(label_size / (1 - res_ratio));
    xframe->GetYaxis()->SetLabelSize(label_size / (1 - res_ratio));
    xframe->GetYaxis()->SetTitleOffset(1.5);
    //Configure residue title
    resframe->SetTitleSize(0.);
    resframe->SetTitle("");
    //Configure residue axes
    resframe->GetXaxis()->SetTickLength(5 * resframe->GetXaxis()->GetTickLength());
    resframe->GetXaxis()->SetTitleSize(0.);
    resframe->GetXaxis()->SetLabelSize(label_size / res_ratio);
    resframe->GetYaxis()->SetLabelSize(label_size / res_ratio); //Because 0.04 is default size
    resframe->GetYaxis()->SetNdivisions(504);
    //Configure Y range
    resframe->SetMinimum(-5);
    resframe->SetMaximum(5);
    //Plot lines
    // Create lines
    uppLine = new TLine(resframe->GetXaxis()->GetXmin(), 2.0, resframe->GetXaxis()->GetXmax(), 2.0);
    midLine = new TLine(resframe->GetXaxis()->GetXmin(), 0.0, resframe->GetXaxis()->GetXmax(), 0.0);
    lowLine = new TLine(resframe->GetXaxis()->GetXmin(), -2.0, resframe->GetXaxis()->GetXmax(), -2.0);
    uppLine->SetLineColor(kRed);
    lowLine->SetLineColor(kRed);
    uppLine->Draw("same");
    midLine->Draw("same");
    lowLine->Draw("same");
  }
  else
  {
    c->cd(0)->SetTicks(1, 1);
    c->SetLogy(const_list.logY);
    xframe->Draw();
  }
  string pdfname = "plots/" + variablename + "Fit" + namesuf + ".pdf";
  c->SaveAs(pdfname.c_str());
  delete resframe;
  delete padHisto;
  delete padResid;
  delete uppLine;
  delete midLine;
  delete lowLine;
  delete c;
  return;
}
void GoodPlot_Simult(RooWorkspace *ws, string *varnames, int N_vars, string title, string Xtitle, string opts, string namesuf)
{
  //The idea of this function is to extract each component of the simultaneous fit and do a "GoodPlot" to it
  //Initialize constants
  Constants const_list(opts);
  Names name_list(opts);

  //Split titles in their different components
  int Ntitle = 0, NXtitle = 0;
  string *_splittitle = SplitString(Ntitle, title, "; ");
  string *_splitXtitle = SplitString(NXtitle, Xtitle, "; ");
  //Now, define titles for each component, we do this so the array always have proper size even if no titles are passed, or fewer.
  string *splittitle = new string[N_vars];
  string *splitXtitle = new string[N_vars];
  for (int i = 0; i < N_vars; i++)
  {
    if (i < Ntitle)
      splittitle[i] = _splittitle[i];
    else
      splittitle[i] = "";
    if (i < NXtitle)
      splitXtitle[i] = _splitXtitle[i];
    else
      splitXtitle[i] = "";
  }
  //What we do is to loop over the different variables, and split the big workspace in sub-workspaces, which we then plot as usual :D
  RooWorkspace **subws = new RooWorkspace *[N_vars];
  for (int i = 0; i < N_vars; i++)
  {
    //Create the subws
    subws[i] = new RooWorkspace((name_list.workspace + std::to_string(i)).c_str(), (name_list.workspace + std::to_string(i)).c_str());
    //Import the pdf
    subws[i]->import(*(RooAddPdf *)ws->pdf(name_list.pdfmodel[i].c_str()));
    //Import the corresponding data (2 steps)
    RooAbsData *tempws = ws->data(name_list.dataset.c_str())->reduce(RooFit::Cut(("cats==cats::" + varnames[i]).c_str()), RooFit::Name(name_list.dataset.c_str()), RooFit::Title(name_list.dataset.c_str()));
    subws[i]->import(*tempws);
    //Plot this part
    GoodPlot(subws[i], varnames[i], splittitle[i], splitXtitle[i], opts, namesuf, const_list.bins[i]);
  }
  return;
}
