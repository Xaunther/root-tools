//Aim of this function is to read an output-like of CutSig (which has evaluated the significance for a list of cuts) and perform a plot
//The cut must match the pattern to be removed, like "BDT_response>"
#include <string>
#include <iostream>
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "Functions/StringTools.h"
#include "Functions/Filereading.h"
void PlotSig(std::string filename, std::string cut, std::string outputname, std::string plotopt = "", std::string title = "")
{
    //Read the cuts in string format
    int N_x = 0, N_y = 0, N_y_err = 0;
    auto x_str = GetColData(filename, N_x, 3);
    auto y_str = GetColData(filename, N_y, 5);
    auto y_err_str = GetColData(filename, N_y_err, 7);

    //Split and get just the number. SKip the first line
    auto x = new double[N_x - 1];
    auto y = new double[N_y - 1];
    auto x_err = new double[N_x - 1];
    auto y_err = new double[N_y - 1];
    for (int i = 1; i < N_x; i++)
    {
        auto pos0 = x_str[i].find(cut);
        auto posf = x_str[i].find(")");
        x[i - 1] = std::stod(x_str[i].substr(pos0 + cut.size(), posf - pos0 - cut.size()));
        y[i - 1] = std::stod(y_str[i]);
        x_err[i - 1] = 0.;
        if (y_err_str[i].find("nan") != std::string::npos)
            y_err[i - 1] = 0;
        else
            y_err[i - 1] = std::stod(y_err_str[i]);
        ç
    }

    //Make the graph
    TGraphErrors *gr = new TGraphErrors(N_x - 1, x, y, x_err, y_err);
    gr->SetTitle(title.c_str());
    gr->SetLineColor(kBlue);
    TCanvas *c = new TCanvas();
    c->SetLeftMargin(0.15);
    gr->Draw(plotopt.c_str());
    c->SaveAs(outputname.c_str());

    //Cleanup
    delete[] x_str;
    delete[] y_str;
    delete[] x;
    delete[] y;
    delete gr;
    delete c;
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
    switch (argc - 1)
    {
    case 3:
        PlotSig(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])));
        break;
    case 4:
        PlotSig(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])));
        break;
    case 5:
        PlotSig(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])));
        break;
    default:
        std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
        return (1);
        break;
    }
    return 0;
}
#endif