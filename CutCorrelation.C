//Takes a ntuple, or a file containing a list of them, and calculates the correlation among pairs of cuts.
//That is, a matrix which elements M_{ij} are P(C_j|C_i), with C_i the cut i
//One cut is defined as 1 line in the input file
//The output is a matrix in Markdown format, which is easy to read raw and is cute in gitlab
//It also accepts a given set of precuts for flexibility, a variable that acts as weight, and the name of the TTree (if not given, it will search in Dictionaries/treenames.dic for a match)

#include <string>
#include <iostream>
#include "TChain.h"
#include "TMatrixD.h"
#include "TCut.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/ArrayTools.h"

//Modification of TMatrix Print to add index names
void PrintMatrix(TMatrixD matrix, std::string *cuts, std::string outfile)
{
    // Print the matrix as a table of elements (zeros are printed as dots).

    if (!matrix.IsValid())
    {
        Error("Print", "matrix not initialized");
        return;
    }

    //Open output file
    FILE *fout = fopen(outfile.c_str(), "w");

    // Get the longest size from the index array (cuts)
    auto longest_str = GetMaxLength(cuts, matrix.GetNcols());

    Int_t cols_per_sheet = 5;
    Int_t sheet_counter;

    for (sheet_counter = 1; sheet_counter <= matrix.GetNcols(); sheet_counter += cols_per_sheet)
    {
        fprintf(fout, "| %s |", std::string(longest_str, ' ').c_str());
        Int_t i, j;
        for (j = sheet_counter; j < sheet_counter + cols_per_sheet && j <= matrix.GetNcols(); j++)
        {
            fprintf(fout, " %s", std::string(longest_str - cuts[j - 1].size(), ' ').c_str());
            fprintf(fout, "%s |", cuts[j - 1].c_str());
        }
        //Horizontal line
        fprintf(fout, "\n|-%s:|", std::string(longest_str, '-').c_str());
        for (j = sheet_counter; j < sheet_counter + cols_per_sheet && j <= matrix.GetNcols(); j++)
        {
            fprintf(fout, ":%s:|", std::string(longest_str, '-').c_str());
        }
        fprintf(fout, "\n");
        for (i = 1; i <= matrix.GetNrows(); i++)
        {
            fprintf(fout, "| %s", std::string(longest_str - cuts[i - 1].size(), ' ').c_str());
            fprintf(fout, "%s |", cuts[i - 1].c_str());
            for (j = sheet_counter; j < sheet_counter + cols_per_sheet && j <= matrix.GetNcols(); j++)
                fprintf(fout, (" %" + std::to_string(longest_str) + ".4g |").c_str(), matrix[i - 1][j - 1]);
            fprintf(fout, "\n");
        }
    }
    fprintf(fout, "\n");
    fclose(fout);
}

void CutCorrelation(std::string dirfile, std::string cutfile, std::string outfile, std::string treename = "", std::string precutfile = "", std::string weight = "")
{
    //Read the file
    auto chain = GetChain(dirfile, treename);
    //Read the preselection
    auto precuts = GetCuts(precutfile);
    //Read the selection, but save it in an array
    int N_cuts = 0;
    auto cuts = ReadVariables(N_cuts, cutfile);

    //Now we must do a double loop to make all cut pairs and compute P(C_j|C_i)
    TMatrixD correlations(N_cuts, N_cuts);
    for (int i = 0; i < N_cuts; i++)
    {
        for (int j = 0; j < N_cuts; j++)
        {
            TCut C_i(cuts[i].c_str());
            TCut C_j(cuts[j].c_str());
            correlations[i][j] = double(GetMeanEntries(chain, (C_i + C_j).GetTitle(), weight)) / double(GetMeanEntries(chain, C_i.GetTitle(), weight));
        }
    }

    //Print for now
    PrintMatrix(correlations, cuts, outfile);

    //Print good ending
    std::cout << "Correlation matrix saved at " << outfile << std::endl;
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
    switch (argc - 1)
    {
    case 3:
        CutCorrelation(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])));
        break;
    case 4:
        CutCorrelation(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])));
        break;
    case 5:
        CutCorrelation(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])));
        break;
    case 6:
        CutCorrelation(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])), *(new std::string(argv[6])));
        break;
    default:
        std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
        return (1);
        break;
    }
    return 0;
}
#endif