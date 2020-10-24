#ifndef PlotTools_h
#define PlotTools_h

#include <string>
#include "RooWorkspace.h"
#include "RooAbsData.h"
#include "RooPlot.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "Dictionaries/Constants.h"
using namespace std;

string SetPlotTitle(Constants*, double, string, string);
TLegend* SetupLegend(Constants*, const int, RooPlot*, RooWorkspace*);
RooAbsData* GetDataFromWS(RooWorkspace*);
void GoodPlot(RooWorkspace*, string, string = "", string = "", string = "", string = "", int = 0);
void GoodPlot_Simult(RooWorkspace*, string*, int, string = "", string = "", string = "", string = "");


#endif
