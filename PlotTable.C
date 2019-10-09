//Function to plot some table (complementary to EntriesTable)
#include <string>
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Dictionaries/Constants.h"
using namespace std;

void PlotTable(string tablesdir, string plot_opts = "APL", double x_factor = 1., double y_factor = 1.)
{
  //Get table files
  int N_tables = 0;
  string* table = ReadVariables(N_tables, tablesdir);

  Constants const_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  //Colours that will be used
  EColor bkg_colours[] = {kBlack, kBlue, kCyan, kGreen, kViolet, kMagenta, kRed, kOrange, kYellow};
  //Read the data into matrices, then into graphs and multigraphs! HOORAY!
  TMultiGraph* mg = new TMultiGraph();
  TGraph** g = new TGraph*[N_tables];
  double*** data = new double**[N_tables];
  int* N_points = new int[N_tables];
  for(int i=0;i<N_tables;i++)
    {
      N_points[i] = 0;
      data[i] = GetData(table[i], N_points[i], 2);
      double* x = new double[N_points[i]];
      double* y = new double[N_points[i]];      
      for(int j=0;j<N_points[i];j++)
	{
	  x[j] = data[i][j][0]*x_factor;
	  y[j] = data[i][j][1]*y_factor;
	}
      g[i] = new TGraph(N_points[i], x, y);
      g[i]->SetLineColor(bkg_colours[i]);
      g[i]->SetLineWidth(3);
      g[i]->SetTitle(table[i].c_str());
      mg->Add(g[i], plot_opts.c_str());
    }

  //Canvas
  TCanvas* c1 = new TCanvas();
  c1->SetLogy(const_list.logY);
  mg->Draw(plot_opts.c_str());

  //Title
  mg->SetTitle(const_list.plot_title.c_str());

  //X Axis
  c1->cd();
  mg->GetXaxis()->SetLimits(const_list.xmin, const_list.xmax);
  //Y Axis
  mg->SetMinimum(const_list.xmin2);
  mg->SetMaximum(const_list.xmax2);

  mg->Draw(plot_opts.c_str());
  c1->BuildLegend();
  string pdfname = "plots/TablePlot.pdf";
  c1->SaveAs(pdfname.c_str());
  return;
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch(argc-1)
    {
    case 1:
      PlotTable(*(new string(argv[1])));
      break;
    case 2:
      PlotTable(*(new string(argv[1])), *(new string(argv[2])));
      break;
    case 3:
      PlotTable(*(new string(argv[1])), *(new string(argv[2])), stod(*(new string(argv[3]))));
      break;
    case 4:
      PlotTable(*(new string(argv[1])), *(new string(argv[2])), stod(*(new string(argv[3]))), stod(*(new string(argv[4]))));
      break;
    default:
      cout << "Wrong number of arguments (" << argc << ") for PlotTable" << endl;
      return(1);
      break;
    }
  return 0;
}
#endif
