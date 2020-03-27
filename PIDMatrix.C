//This functions is designed to, from a pre-configured keyconfig given, read a bunch of files containing PID efficiency and solve the equation:
//Y = M·x
//Where Y is an array of yields, for a certain cut. This is usually obtained from fits to data (normalization)
//Where x is the array of the contamination we expect on our signal (What we want to know).
//M is a matrix where the efficiencies are encoded.
//For NstG analysis, M_ij = F_ij + R_ij
//where F_ij is eff for cut i in channel j divided by eff for signal cut in channel j
//Where R_ij is eff for cut i in the reflection channel of j, divided by eff for signal cut in channel j
//The efficiency is of all the selections involved since the divergence of the signal and its reflection counterpart (since the tuple was made)

//This script is not compatible with ROOTCINT
#include <iostream>
#include <string>
#include <fstream>
#include "TMatrixT.h"
#include "TVectorT.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
#include "../Functions/BoostTools.h"
#include <boost/math/tools/polynomial.hpp>
using namespace std;


//List of functions
void PIDMatrix(string keyconfig, string outfile);
void PIDMatrix_NstG(string outfile);
void PIDMatrix_NstG_split(string outfile);
void PIDMatrix_NstG_simple(string outfile);
void PIDMatrix_NstG_simplest(string outfile);
vector<string> PIDMatrix_namelist(string option = "all");
//////////////////////////////////////////////////
//General function
void PIDMatrix(string keyconfig, string outfile)
{
	if (keyconfig == "NstG") {PIDMatrix_NstG(outfile);}
	else if(keyconfig == "split") {PIDMatrix_NstG_split(outfile);}
	else if (keyconfig == "NstG_simple") {PIDMatrix_NstG_simple(outfile);}
	else if (keyconfig == "NstG_simplest") {PIDMatrix_NstG_simplest(outfile);}
	else {cout << "Could not match " << keyconfig << " to any keyconfig." << endl;}
}
//////////////////////////////////////////////////
//For NstG analysis
void PIDMatrix_NstG(string outfile)
{
	//Dummy integer to use
	int N = 0;
	//Vector with total Yields. Cuts:(ppi, pbarpi, ppibar)
	//Now we want to compute the contamination, but without knowing the total yield in the ppigamma channel
	//This was done with mathematica, let's try this boost library...
	std::vector<boost::math::tools::polynomial<double>> Y;
	Y.push_back({100., 100.});//100(1+x) is the total ppiG yield
	Y.push_back({stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012.txt", 1)[0])});
	Y.push_back({stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012_Subst01_Kpi2pK.txt", 1)[0])});
	//Also read errors
	std::vector<double> Y_error;
	Y_error.push_back(sqrt(2 * 100.)); //worst case scenario something like sqrt(200)
	Y_error.push_back(stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012.txt", 3)[0]));
	Y_error.push_back(stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012_Subst01_Kpi2pK.txt", 3)[0]));
	//Our unknowns (contamination), with error
	std::vector<boost::math::tools::polynomial<double>> x;
	std::vector<double> x_error;
	//Now, we'll compose the PID matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TMatrixT<double> pid_eff(3, 8);
	pid_eff(0, 0) = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt"));
	pid_eff(0, 1) = stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt"));
	pid_eff(0, 2) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_PIDEff.txt"));
	pid_eff(0, 3) = stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
	pid_eff(0, 4) = stod(GetValueFor("Mean", "Systematics/PID/NstG_Ref_PIDEff.txt"));
	pid_eff(0, 5) = stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDEff.txt"));
	pid_eff(0, 6) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_Ref_PIDEff.txt"));
	pid_eff(0, 7) = stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDEff.txt"));
	//pbarpi
	pid_eff(1, 0) = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv1Eff.txt"));
	pid_eff(1, 1) = stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDInv1Eff.txt"));
	pid_eff(1, 2) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_PIDInv1Eff.txt"));
	pid_eff(1, 3) = stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv1Eff.txt"));
	pid_eff(1, 4) = stod(GetValueFor("Mean", "Systematics/PID/NstG_Ref_PIDInv1Eff.txt"));;
	pid_eff(1, 5) = stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDInv1Eff.txt"));
	pid_eff(1, 6) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_Ref_PIDInv1Eff.txt"));
	pid_eff(1, 7) = stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDInv1Eff.txt"));
	//ppibar
	pid_eff(2, 0) = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv2Eff.txt"));
	pid_eff(2, 1) = stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDInv2Eff.txt"));
	pid_eff(2, 2) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_PIDInv2Eff.txt"));
	pid_eff(2, 3) = stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv2Eff.txt"));
	pid_eff(2, 4) = stod(GetValueFor("Mean", "Systematics/PID/NstG_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 5) = stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 6) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 7) = stod(GetValueFor("Mean", "Systematics/PID/pKG_Ref_PIDInv2Eff.txt"));
	//Now, we'll compose the non-PID matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TVectorT<double> nopid_eff(8);
	nopid_eff(0) = stod(GetValueFor("Global", "output/CutEff_NstG_2hG.txt"))*stod(GetValueFor("Global", "output/PreCutEff_NstG_2hG.txt"));
	nopid_eff(1) = stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt"))*stod(GetValueFor("Global", "output/PreCutEff_KstG_2hG.txt"));
	nopid_eff(2) = stod(GetValueFor("Global", "output/CutEff_K2stG_2hG.txt"))*stod(GetValueFor("Global", "output/PreCutEff_K2stG_2hG.txt"));
	nopid_eff(3) = stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt"))*stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt"));
	nopid_eff(4) = stod(GetValueFor("Global", "output/CutEff_NstG_Ref_2hG.txt"))*stod(GetValueFor("Global", "output/PreCutEff_NstG_Ref_2hG.txt"));
	nopid_eff(5) = stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt"))*stod(GetValueFor("Global", "output/PreCutEff_KstG_Ref_2hG.txt"));
	nopid_eff(6) = stod(GetValueFor("Global", "output/CutEff_K2stG_Ref_2hG.txt"))*stod(GetValueFor("Global", "output/PreCutEff_K2stG_Ref_2hG.txt"));
	nopid_eff(7) = stod(GetValueFor("Global", "output/CutEff_pKG_Ref_2hG.txt"))*stod(GetValueFor("Global", "output/PreCutEff_pKG_Ref_2hG.txt"));

	//Now since we have several channels for each components, we have to weight them
	TVectorT<double> ch_weight(4);
	//ppiG
	ch_weight(0) = 1.;
	//KpiG
	ch_weight(1) = stod(ReadVariablesWord(N, "output/Yield_Exp_noPID_KstG.txt", 1)[0]);
	ch_weight(2) = stod(ReadVariablesWord(N, "output/Yield_Exp_noPID_K2stG.txt", 1)[0]);
	for (int i = 2; i >= 1; i--) {ch_weight(i) /= ch_weight(1);}
	//pKG
		ch_weight(3) = 1.;
	//Time to build up the matrices
	//M Matrix
	TMatrixT<double> _M(3, 4);
	TMatrixT<double> M(3, 3);
	//F Matrix
	TMatrixT<double> F(3, 4);
	//R Matrix
	TMatrixT<double> R(3, 4);
	//Let's fill up first each channel as an independent source
	for (int i = 0; i < F.GetNrows(); i++)
	{
		F(i, 0) = pid_eff(i, 0) / pid_eff(0, 0);
		F(i, 1) = pid_eff(i, 1) / pid_eff(0, 1);
		F(i, 2) = pid_eff(i, 2) / pid_eff(0, 1);
		F(i, 3) = pid_eff(i, 3) / pid_eff(0, 3);
		R(i, 0) = pid_eff(i, 0 + F.GetNcols()) * nopid_eff(0 + F.GetNcols()) / (pid_eff(0, 0) * nopid_eff(0));
		R(i, 1) = pid_eff(i, 1 + F.GetNcols()) * nopid_eff(1 + F.GetNcols()) / (pid_eff(0, 1) * nopid_eff(1));
		R(i, 2) = pid_eff(i, 2 + F.GetNcols()) * nopid_eff(2 + F.GetNcols()) / (pid_eff(0, 1) * nopid_eff(2));
		R(i, 3) = pid_eff(i, 3 + F.GetNcols()) * nopid_eff(3 + F.GetNcols()) / (pid_eff(0, 3) * nopid_eff(3));
		for (int j = 0; j < F.GetNcols(); j++)
		{
			_M(i, j) = F(i, j) + R(i, j);
		}
	}
	//Time to merge the channels now
	for (int i = 0; i < F.GetNrows(); i++)
	{
		//ppiG
		M(i, 0) = _M(i, 0) * ch_weight(0);
		//KpiG
		M(i, 1) = 0.;
		for (int j = 1; j <= 2; j++) {M(i, 1) += _M(i, j) * ch_weight(j);}
		//pKG
			M(i, 2) = _M(i, 3) * ch_weight(3);
	}
	//Time to invert our matrix to solve the problem: Y = M·x --> x = M^-1·Y
	TMatrixT<double> M_inv = M.Invert();
	//Result on x
	for (int i = 0; i < M.GetNrows(); i++)
	{
		x.push_back({0});
		x_error.push_back(0.);
		for (int j = 0; j < M.GetNcols(); j++)
		{
			x[i] += M_inv(i, j) * Y[j];
			x_error[i] += M_inv(i, j) * M_inv(i, j) * Y_error[j] * Y_error[j];
		}
		x_error[i] = sqrt(x_error[i]);
	}

	std::vector<boost::math::tools::polynomial<double>> x_final_ppi;
	std::vector<double> x_final_ppi_error;
	//Contributions for ppi PID cuts
	//Main
	//ppiG
	x_final_ppi.push_back({ch_weight(0) * x[0] * F(0, 0)});
	x_final_ppi_error.push_back(ch_weight(0) * x_error[0] * F(0, 0));
	//KpiG
	x_final_ppi.push_back({ch_weight(1) * x[1] * F(0, 1)});
	x_final_ppi.push_back({ch_weight(2) * x[1] * F(0, 2)});
	x_final_ppi_error.push_back(ch_weight(1) * x_error[1] * F(0, 1));
	x_final_ppi_error.push_back(ch_weight(2) * x_error[1] * F(0, 2));
	//pKG
	x_final_ppi.push_back({ch_weight(3) * x[2] * F(0, 3)});
	x_final_ppi_error.push_back(ch_weight(3) * x_error[2] * F(0, 3));
	//Reflections
	//ppiG
	x_final_ppi.push_back({ch_weight(0) * x[0] * R(0, 0)});
	x_final_ppi_error.push_back(ch_weight(0) * x_error[0] * R(0, 0));
	//KpiG
	x_final_ppi.push_back({ch_weight(1) * x[1] * R(0, 1)});
	x_final_ppi.push_back({ch_weight(2) * x[1] * R(0, 2)});
	x_final_ppi_error.push_back(ch_weight(1) * x_error[1] * R(0, 1));
	x_final_ppi_error.push_back(ch_weight(2) * x_error[1] * R(0, 2));
	//pKG
	x_final_ppi.push_back({ch_weight(3) * x[2] * R(0, 3)});
	x_final_ppi_error.push_back(ch_weight(3) * x_error[2] * R(0, 3));

	std::vector<boost::math::tools::polynomial<double>> x_final_pbarpi;
	std::vector<double> x_final_pbarpi_error;
	std::vector<boost::math::tools::polynomial<double>> x_final_ppibar;
	std::vector<double> x_final_ppibar_error;
	for (unsigned int i = 0; i < x_final_ppi.size(); i++)
	{
		x_final_pbarpi.push_back({x_final_ppi[i] / pid_eff(0, i) * pid_eff(1, i)});
		x_final_pbarpi_error.push_back({x_final_ppi_error[i] / pid_eff(0, i) * pid_eff(1, i)});
		x_final_ppibar.push_back({x_final_ppi[i] / pid_eff(0, i) * pid_eff(2, i)});
		x_final_ppibar_error.push_back({x_final_ppi_error[i] / pid_eff(0, i) * pid_eff(2, i)});
	}

	vector<string> namelist = PIDMatrix_namelist("pKG");
	ofstream fout;
	fout.open(outfile.c_str());
	fout << "===== RESULT =====" << endl;
	fout << "ppi cuts" << endl;
	fout << "--------" << endl;
	for (unsigned int i = 0; i < x_final_ppi.size(); i++)
	{
		fout << namelist[i] << "_ppi = " << formula_format(x_final_ppi[i], "\u03B5") << " \u00B1 " << max(x_final_ppi_error[i], sqrt(x_final_ppi[i](0.))) << endl;
	}
	fout << endl;
	fout << "pbarpi cuts" << endl;
	fout << "-----------" << endl;
	for (unsigned int i = 0; i < x_final_pbarpi.size(); i++)
	{
		fout << namelist[i] << "_pbarpi = " << formula_format(x_final_pbarpi[i], "\u03B5") << " \u00B1 " << max(x_final_pbarpi_error[i], sqrt(x_final_pbarpi[i](0.))) << endl;
	}
	fout << endl;
	fout << "ppibarcuts" << endl;
	fout << "----------" << endl;
	for (unsigned int i = 0; i < x_final_ppibar.size(); i++)
	{
		fout << namelist[i] << "_ppibar = " << formula_format(x_final_ppibar[i], "\u03B5") << " \u00B1 " << max(x_final_ppibar_error[i], sqrt(x_final_ppibar[i](0.))) << endl;
	}
	fout << endl;
	M.Invert();
	fout << "M matrix" << endl;
	fout << "--------" << endl;
	for (int i = 0; i < M.GetNrows(); i++)
	{
		for (int j = 0; j < M.GetNcols(); j++)
		{
			fout << M(i, j) << "  ";
		}
		fout << endl;
	}
	fout << endl;
	fout << "M^-1 matrix" << endl;
	fout << "-----------" << endl;
	for (int i = 0; i < M_inv.GetNrows(); i++)
	{
		for (int j = 0; j < M_inv.GetNcols(); j++)
		{
			fout << M_inv(i, j) << "  ";
		}
		fout << endl;
	}
	fout << "==================" << endl;

}
//For NstG analysis with Lambda* channels splitted in 4
void PIDMatrix_NstG_split(string outfile)
{
	//Dummy integer to use
	int N = 0;
	//Vector with total Yields. Cuts:(ppi, pbarpi, ppibar)
	//Now we want to compute the contamination, but without knowing the total yield in the ppigamma channel
	//This was done with mathematica, let's try this boost library...
	std::vector<boost::math::tools::polynomial<double>> Y;
	Y.push_back({100., 100.});//100(1+x) is the total ppiG yield
	Y.push_back({stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012.txt", 1)[0])});
	Y.push_back({stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012_Subst01_Kpi2pK.txt", 1)[0])});
	//Also read errors
	std::vector<double> Y_error;
	Y_error.push_back(sqrt(2 * 100.)); //worst case scenario something like sqrt(200)
	Y_error.push_back(stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012.txt", 3)[0]));
	Y_error.push_back(stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012_Subst01_Kpi2pK.txt", 3)[0]));
	//Our unknowns (contamination), with error
	std::vector<boost::math::tools::polynomial<double>> x;
	std::vector<double> x_error;
	//Now, we'll compose the PID matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TMatrixT<double> pid_eff(3, 14);
	pid_eff(0, 0) = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt"));
	pid_eff(0, 1) = stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDEff.txt"));
	pid_eff(0, 2) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_PIDEff.txt"));
	pid_eff(0, 3) = stod(GetValueFor("Mean", "Systematics/PID/L1520G_PIDEff.txt"));
	pid_eff(0, 4) = stod(GetValueFor("Mean", "Systematics/PID/L1670G_PIDEff.txt"));
	pid_eff(0, 5) = stod(GetValueFor("Mean", "Systematics/PID/L1820G_PIDEff.txt"));
	pid_eff(0, 6) = stod(GetValueFor("Mean", "Systematics/PID/L1830G_PIDEff.txt"));
	//pid_eff(0, 7) = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff_2hG.txt"));
	pid_eff(0, 7) = 0.;
	pid_eff(0, 8) = stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDEff.txt"));
	pid_eff(0, 9) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_Ref_PIDEff.txt"));
	pid_eff(0, 10) = stod(GetValueFor("Mean", "Systematics/PID/L1520G_Ref_PIDEff.txt"));
	pid_eff(0, 11) = stod(GetValueFor("Mean", "Systematics/PID/L1670G_Ref_PIDEff.txt"));
	pid_eff(0, 12) = stod(GetValueFor("Mean", "Systematics/PID/L1820G_Ref_PIDEff.txt"));
	pid_eff(0, 13) = stod(GetValueFor("Mean", "Systematics/PID/L1830G_Ref_PIDEff.txt"));
	//pbarpi
	pid_eff(1, 0) = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv1Eff.txt"));
	pid_eff(1, 1) = stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDInv1Eff.txt"));
	pid_eff(1, 2) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_PIDInv1Eff.txt"));
	pid_eff(1, 3) = stod(GetValueFor("Mean", "Systematics/PID/L1520G_PIDInv1Eff.txt"));
	pid_eff(1, 4) = stod(GetValueFor("Mean", "Systematics/PID/L1670G_PIDInv1Eff.txt"));
	pid_eff(1, 5) = stod(GetValueFor("Mean", "Systematics/PID/L1820G_PIDInv1Eff.txt"));
	pid_eff(1, 6) = stod(GetValueFor("Mean", "Systematics/PID/L1830G_PIDInv1Eff.txt"));
	//pid_eff(1, 7) = stod(GetValueFor("Mean", "Systematics/PID/NstG_Ref_PIDInv1Eff.txt"));
	pid_eff(1, 7) = 0.;
	pid_eff(1, 8) = stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDInv1Eff.txt"));
	pid_eff(1, 9) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_Ref_PIDInv1Eff.txt"));
	pid_eff(1, 10) = stod(GetValueFor("Mean", "Systematics/PID/L1520G_Ref_PIDInv1Eff.txt"));
	pid_eff(1, 11) = stod(GetValueFor("Mean", "Systematics/PID/L1670G_Ref_PIDInv1Eff.txt"));
	pid_eff(1, 12) = stod(GetValueFor("Mean", "Systematics/PID/L1820G_Ref_PIDInv1Eff.txt"));
	pid_eff(1, 13) = stod(GetValueFor("Mean", "Systematics/PID/L1830G_Ref_PIDInv1Eff.txt"));
	//ppibar
	pid_eff(2, 0) = stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDInv2Eff.txt"));
	pid_eff(2, 1) = stod(GetValueFor("Mean", "Systematics/PID/KstG_PIDInv2Eff.txt"));
	pid_eff(2, 2) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_PIDInv2Eff.txt"));
	pid_eff(2, 3) = stod(GetValueFor("Mean", "Systematics/PID/L1520G_PIDInv2Eff.txt"));
	pid_eff(2, 4) = stod(GetValueFor("Mean", "Systematics/PID/L1670G_PIDInv2Eff.txt"));
	pid_eff(2, 5) = stod(GetValueFor("Mean", "Systematics/PID/L1820G_PIDInv2Eff.txt"));
	pid_eff(2, 6) = stod(GetValueFor("Mean", "Systematics/PID/L1830G_PIDInv2Eff.txt"));
	//pid_eff(2, 7) = stod(GetValueFor("Mean", "Systematics/PID/NstG_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 7) = 0.;
	pid_eff(2, 8) = stod(GetValueFor("Mean", "Systematics/PID/KstG_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 9) = stod(GetValueFor("Mean", "Systematics/PID/K2stG_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 10) = stod(GetValueFor("Mean", "Systematics/PID/L1520G_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 11) = stod(GetValueFor("Mean", "Systematics/PID/L1670G_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 12) = stod(GetValueFor("Mean", "Systematics/PID/L1820G_Ref_PIDInv2Eff.txt"));
	pid_eff(2, 13) = stod(GetValueFor("Mean", "Systematics/PID/L1830G_Ref_PIDInv2Eff.txt"));
	//Now, we'll compose the non-PID matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TVectorT<double> nopid_eff(14);
	nopid_eff(0) = stod(GetValueFor("Global", "output/CutEff_NstG_2hG.txt"));
	nopid_eff(1) = stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt"));
	nopid_eff(2) = stod(GetValueFor("Global", "output/CutEff_K2stG_2hG.txt"));
	nopid_eff(3) = stod(GetValueFor("Global", "output/CutEff_L1520G_2hG.txt"));
	nopid_eff(4) = stod(GetValueFor("Global", "output/CutEff_L1670G_2hG.txt"));
	nopid_eff(5) = stod(GetValueFor("Global", "output/CutEff_L1820G_2hG.txt"));
	nopid_eff(6) = stod(GetValueFor("Global", "output/CutEff_L1830G_2hG.txt"));
	//nopid_eff(7) = stod(GetValueFor("Global", "output/CutEff_NstG_Ref_2hG.txt"));
	nopid_eff(7) = 0.;
	nopid_eff(8) = stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt"));
	nopid_eff(9) = stod(GetValueFor("Global", "output/CutEff_K2stG_Ref_2hG.txt"));
	nopid_eff(10) = stod(GetValueFor("Global", "output/CutEff_L1520G_Ref_2hG.txt"));
	nopid_eff(11) = stod(GetValueFor("Global", "output/CutEff_L1670G_Ref_2hG.txt"));
	nopid_eff(12) = stod(GetValueFor("Global", "output/CutEff_L1820G_Ref_2hG.txt"));
	nopid_eff(13) = stod(GetValueFor("Global", "output/CutEff_L1830G_Ref_2hG.txt"));

	//Now since we have several channels for each components, we have to weight them
	TVectorT<double> ch_weight(7);
	//ppiG
	ch_weight(0) = 1.;
	//KpiG
	ch_weight(1) = stod(ReadVariablesWord(N, "output/Yield_Exp_noPID_KstG.txt", 1)[0]);
	ch_weight(2) = stod(ReadVariablesWord(N, "output/Yield_Exp_noPID_K2stG.txt", 1)[0]);
	for (int i = 2; i >= 1; i--) {ch_weight(i) /= ch_weight(1);}
	//pKG
		ch_weight(3) = stod(ReadVariablesWord(N, "output/Yield_Exp_noPID_L1520G.txt", 1)[0]);
	ch_weight(4) = stod(ReadVariablesWord(N, "output/Yield_Exp_noPID_L1670G.txt", 1)[0]);
	ch_weight(5) = stod(ReadVariablesWord(N, "output/Yield_Exp_noPID_L1820G.txt", 1)[0]);
	ch_weight(6) = stod(ReadVariablesWord(N, "output/Yield_Exp_noPID_L1830G.txt", 1)[0]);
	for (int i = 6; i >= 3; i--) {ch_weight(i) /= ch_weight(3);}
	//Time to build up the matrices
	//M Matrix
		TMatrixT<double> _M(3, 7);
	TMatrixT<double> M(3, 3);
	//F Matrix
	TMatrixT<double> F(3, 7);
	//R Matrix
	TMatrixT<double> R(3, 7);
	//Let's fill up first each channel as an independent source
	for (int i = 0; i < F.GetNrows(); i++)
	{
		F(i, 0) = pid_eff(i, 0) / pid_eff(0, 0);
		F(i, 1) = pid_eff(i, 1) / pid_eff(0, 1);
		F(i, 2) = pid_eff(i, 2) / pid_eff(0, 1);
		F(i, 3) = pid_eff(i, 3) / pid_eff(0, 3);
		F(i, 4) = pid_eff(i, 4) / pid_eff(0, 3);
		F(i, 5) = pid_eff(i, 5) / pid_eff(0, 3);
		F(i, 6) = pid_eff(i, 6) / pid_eff(0, 3);
		R(i, 0) = pid_eff(i, 0 + F.GetNcols()) * nopid_eff(0 + F.GetNcols()) / (pid_eff(0, 0) * nopid_eff(0));
		R(i, 1) = pid_eff(i, 1 + F.GetNcols()) * nopid_eff(1 + F.GetNcols()) / (pid_eff(0, 1) * nopid_eff(1));
		R(i, 2) = pid_eff(i, 2 + F.GetNcols()) * nopid_eff(2 + F.GetNcols()) / (pid_eff(0, 1) * nopid_eff(2));
		R(i, 3) = pid_eff(i, 3 + F.GetNcols()) * nopid_eff(3 + F.GetNcols()) / (pid_eff(0, 3) * nopid_eff(3));
		R(i, 4) = pid_eff(i, 4 + F.GetNcols()) * nopid_eff(4 + F.GetNcols()) / (pid_eff(0, 3) * nopid_eff(4));
		R(i, 5) = pid_eff(i, 5 + F.GetNcols()) * nopid_eff(5 + F.GetNcols()) / (pid_eff(0, 3) * nopid_eff(5));
		R(i, 6) = pid_eff(i, 6 + F.GetNcols()) * nopid_eff(6 + F.GetNcols()) / (pid_eff(0, 3) * nopid_eff(6));
		for (int j = 0; j < F.GetNcols(); j++)
		{
			_M(i, j) = F(i, j) + R(i, j);
		}
	}
	//Time to merge the channels now
	for (int i = 0; i < F.GetNrows(); i++)
	{
		//ppiG
		M(i, 0) = _M(i, 0) * ch_weight(0);
		//KpiG
		M(i, 1) = 0.;
		for (int j = 1; j <= 2; j++) {M(i, 1) += _M(i, j) * ch_weight(j);}
		//pKG
			M(i, 2) = 0.;
		for (int j = 3; j <= 6; j++) {M(i, 2) += _M(i, j) * ch_weight(j);}
	}
	//Time to invert our matrix to solve the problem: Y = M·x --> x = M^-1·Y
TMatrixT<double> M_inv = M.Invert();
	//Result on x
for (int i = 0; i < M.GetNrows(); i++)
{
	x.push_back({0});
	x_error.push_back(0.);
	for (int j = 0; j < M.GetNcols(); j++)
	{
		x[i] += M_inv(i, j) * Y[j];
		x_error[i] += M_inv(i, j) * M_inv(i, j) * Y_error[j] * Y_error[j];
	}
	x_error[i] = sqrt(x_error[i]);
}

std::vector<boost::math::tools::polynomial<double>> x_final_ppi;
std::vector<double> x_final_ppi_error;
	//Contributions for ppi PID cuts
	//Main
	//ppiG
x_final_ppi.push_back({ch_weight(0) * x[0] * F(0, 0)});
x_final_ppi_error.push_back(ch_weight(0) * x_error[0] * F(0, 0));
	//KpiG
x_final_ppi.push_back({ch_weight(1) * x[1] * F(0, 1)});
x_final_ppi.push_back({ch_weight(2) * x[1] * F(0, 2)});
x_final_ppi_error.push_back(ch_weight(1) * x_error[1] * F(0, 1));
x_final_ppi_error.push_back(ch_weight(2) * x_error[1] * F(0, 2));
	//pKG
x_final_ppi.push_back({ch_weight(3) * x[2] * F(0, 3)});
x_final_ppi.push_back({ch_weight(4) * x[2] * F(0, 4)});
x_final_ppi.push_back({ch_weight(5) * x[2] * F(0, 5)});
x_final_ppi.push_back({ch_weight(6) * x[2] * F(0, 6)});
x_final_ppi_error.push_back(ch_weight(3) * x_error[2] * F(0, 3));
x_final_ppi_error.push_back(ch_weight(4) * x_error[2] * F(0, 4));
x_final_ppi_error.push_back(ch_weight(5) * x_error[2] * F(0, 5));
x_final_ppi_error.push_back(ch_weight(6) * x_error[2] * F(0, 6));
	//Reflections
	//ppiG
x_final_ppi.push_back({ch_weight(0) * x[0] * R(0, 0)});
x_final_ppi_error.push_back(ch_weight(0) * x_error[0] * R(0, 0));
	//KpiG
x_final_ppi.push_back({ch_weight(1) * x[1] * R(0, 1)});
x_final_ppi.push_back({ch_weight(2) * x[1] * R(0, 2)});
x_final_ppi_error.push_back(ch_weight(1) * x_error[1] * R(0, 1));
x_final_ppi_error.push_back(ch_weight(2) * x_error[1] * R(0, 2));
	//pKG
x_final_ppi.push_back({ch_weight(3) * x[2] * R(0, 3)});
x_final_ppi.push_back({ch_weight(4) * x[2] * R(0, 4)});
x_final_ppi.push_back({ch_weight(5) * x[2] * R(0, 5)});
x_final_ppi.push_back({ch_weight(6) * x[2] * R(0, 6)});
x_final_ppi_error.push_back(ch_weight(3) * x_error[2] * R(0, 3));
x_final_ppi_error.push_back(ch_weight(4) * x_error[2] * R(0, 4));
x_final_ppi_error.push_back(ch_weight(5) * x_error[2] * R(0, 5));
x_final_ppi_error.push_back(ch_weight(6) * x_error[2] * R(0, 6));

std::vector<boost::math::tools::polynomial<double>> x_final_pbarpi;
std::vector<double> x_final_pbarpi_error;
std::vector<boost::math::tools::polynomial<double>> x_final_ppibar;
std::vector<double> x_final_ppibar_error;
for (unsigned int i = 0; i < x_final_ppi.size(); i++)
{
	x_final_pbarpi.push_back({x_final_ppi[i] / pid_eff(0, i) * pid_eff(1, i)});
	x_final_pbarpi_error.push_back({x_final_ppi_error[i] / pid_eff(0, i) * pid_eff(1, i)});
	x_final_ppibar.push_back({x_final_ppi[i] / pid_eff(0, i) * pid_eff(2, i)});
	x_final_ppibar_error.push_back({x_final_ppi_error[i] / pid_eff(0, i) * pid_eff(2, i)});
}

vector<string> namelist = PIDMatrix_namelist("all");
ofstream fout;
fout.open(outfile.c_str());
fout << "===== RESULT =====" << endl;
fout << "ppi cuts" << endl;
fout << "--------" << endl;
for (unsigned int i = 0; i < x_final_ppi.size(); i++)
{
	fout << namelist[i] << "_ppi = " << formula_format(x_final_ppi[i], "\u03B5") << " \u00B1 " << max(x_final_ppi_error[i], sqrt(x_final_ppi[i](0.))) << endl;
}
fout << endl;
fout << "pbarpi cuts" << endl;
fout << "-----------" << endl;
for (unsigned int i = 0; i < x_final_pbarpi.size(); i++)
{
	fout << namelist[i] << "_pbarpi = " << formula_format(x_final_pbarpi[i], "\u03B5") << " \u00B1 " << max(x_final_pbarpi_error[i], sqrt(x_final_pbarpi[i](0.))) << endl;
}
fout << endl;
fout << "ppibarcuts" << endl;
fout << "----------" << endl;
for (unsigned int i = 0; i < x_final_ppibar.size(); i++)
{
	fout << namelist[i] << "_ppibar = " << formula_format(x_final_ppibar[i], "\u03B5") << " \u00B1 " << max(x_final_ppibar_error[i], sqrt(x_final_ppibar[i](0.))) << endl;
}
fout << endl;
M.Invert();
fout << "M matrix" << endl;
fout << "--------" << endl;
for (int i = 0; i < M.GetNrows(); i++)
{
	for (int j = 0; j < M.GetNcols(); j++)
	{
		fout << M(i, j) << "  ";
	}
	fout << endl;
}
fout << endl;
fout << "M^-1 matrix" << endl;
fout << "-----------" << endl;
for (int i = 0; i < M_inv.GetNrows(); i++)
{
	for (int j = 0; j < M_inv.GetNcols(); j++)
	{
		fout << M_inv(i, j) << "  ";
	}
	fout << endl;
}
fout << "==================" << endl;

}

//No Reflections
void PIDMatrix_NstG_simple(string outfile)
{
	//Dummy integer to use
	int N = 0;
	//Vector with total Yields. Cuts:(ppi, pbarpi, ppibar)
	TVectorT<double> Y(3);
	Y(0) = stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012_Subst0_K2p.txt", 1)[0]);
	Y(1) = stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012.txt", 1)[0]);
	Y(2) = stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012_Subst01_Kpi2pK.txt", 1)[0]);
	//Our unknowns (contamination)
	TVectorT<double> x(3);
	//Now, we'll compose the PID matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TMatrixT<double> pid_eff(3, 6);
	pid_eff(0, 0) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_2hG.txt"));
	pid_eff(0, 1) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_2hG.txt"));
	pid_eff(0, 2) = stod(GetValueFor("Global", "output/CutEff_PID_L1520G_2hG.txt"));
	//pid_eff(0, 3) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_Ref_2hG.txt"));
	pid_eff(0, 3) = 0.;
	pid_eff(0, 4) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_Ref_2hG.txt"));
	pid_eff(0, 5) = stod(GetValueFor("Global", "output/CutEff_PID_L1520G_Ref_2hG.txt"));
	//pbarpi
	pid_eff(1, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_2hG.txt"));
	pid_eff(1, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_2hG.txt"));
	pid_eff(1, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1520G_2hG.txt"));
	//pid_eff(1, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_Ref_2hG.txt"));
	pid_eff(1, 3) = 0.;
	pid_eff(1, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_Ref_2hG.txt"));
	pid_eff(1, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1520G_Ref_2hG.txt"));
	//ppibar
	pid_eff(2, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_2hG.txt"));
	pid_eff(2, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_2hG.txt"));
	pid_eff(2, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1520G_2hG.txt"));
	//pid_eff(2, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_Ref_2hG.txt"));
	pid_eff(2, 3) = 0.;
	pid_eff(2, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_Ref_2hG.txt"));
	pid_eff(2, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1520G_Ref_2hG.txt"));
	//Now, we'll compose the non-PID matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TVectorT<double> nopid_eff(6);
	nopid_eff(0) = stod(GetValueFor("Global", "output/CutEff_NstG_2hG.txt"));
	nopid_eff(1) = stod(GetValueFor("Global", "output/CutEff_KstG_2hG.txt"));
	nopid_eff(2) = stod(GetValueFor("Global", "output/CutEff_L1520G_2hG.txt"));
	//nopid_eff(3) = stod(GetValueFor("Global", "output/CutEff_NstG_Ref_2hG.txt"));
	nopid_eff(3) = 0.;
	nopid_eff(4) = stod(GetValueFor("Global", "output/CutEff_KstG_Ref_2hG.txt"));
	nopid_eff(5) = stod(GetValueFor("Global", "output/CutEff_L1520G_Ref_2hG.txt"));

	//Time to build up the matrices
	//M Matrix
	TMatrixT<double> M(3, 3);
	//F Matrix
	TMatrixT<double> F(3, 3);
	//R Matrix
	TMatrixT<double> R(3, 3);
	//Let's fill up first each channel as an independent source
	for (int i = 0; i < F.GetNrows(); i++)
	{
		F(i, 0) = pid_eff(i, 0) / pid_eff(0, 0);
		F(i, 1) = pid_eff(i, 1) / pid_eff(0, 1);
		F(i, 2) = pid_eff(i, 2) / pid_eff(0, 2);
		R(i, 0) = pid_eff(i, 0 + F.GetNcols()) * nopid_eff(0 + F.GetNcols()) / (pid_eff(0, 0) * nopid_eff(0));
		R(i, 1) = pid_eff(i, 1 + F.GetNcols()) * nopid_eff(1 + F.GetNcols()) / (pid_eff(0, 1) * nopid_eff(1));
		R(i, 2) = pid_eff(i, 2 + F.GetNcols()) * nopid_eff(2 + F.GetNcols()) / (pid_eff(0, 2) * nopid_eff(2));
		for (int j = 0; j < F.GetNcols(); j++)
		{
			M(i, j) = F(i, j) + R(i, j);
		}
	}
	//Time to invert our matrix to solve the problem: Y = M·x --> x = M^-1·Y
	TMatrixT<double> M_inv = M.Invert();
	//Result on x
	for (int i = 0; i < M.GetNrows(); i++)
	{
		x(i) = 0;
		for (int j = 0; j < M.GetNcols(); j++)
		{
			x(i) += M_inv(i, j) * Y(j);
		}
	}

	TVectorT<double> x_final_ppi(6);
	//Contributions for ppi PID cuts
	//Main
	//ppiG
	x_final_ppi(0) = x(0) * F(0, 0);
	//KpiG
	x_final_ppi(1) = x(1) * F(0, 1);
	//pKG
	x_final_ppi(2) = x(2) * F(0, 2);
	//Reflections
	//ppiG
	x_final_ppi(3) = x(0) * R(0, 0);
	//KpiG
	x_final_ppi(4) = x(1) * R(0, 1);
	//pKG
	x_final_ppi(5) = x(2) * R(0, 2);

	TVectorT<double> x_final_pbarpi(6);
	TVectorT<double> x_final_ppibar(6);
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		x_final_pbarpi(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(1, i);
		x_final_ppibar(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(2, i);
	}

	vector<string> namelist = PIDMatrix_namelist("simple");
	ofstream fout;
	fout.open(outfile.c_str());
	fout << "===== RESULT =====" << endl;
	fout << "ppi cuts" << endl;
	fout << "--------" << endl;
	for (int i = 0; i < x_final_ppi.GetNrows(); i++)
	{
		fout << namelist[i] << "_ppi = " << x_final_ppi(i) << endl;
	}
	fout << endl;
	fout << "pbarpi cuts" << endl;
	fout << "-----------" << endl;
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		fout << namelist[i] << "_pbarpi = " << x_final_pbarpi(i) << endl;
	}
	fout << endl;
	fout << "ppibarcuts" << endl;
	fout << "----------" << endl;
	for (int i = 0; i < x_final_ppibar.GetNrows(); i++)
	{
		fout << namelist[i] << "_ppibar = " << x_final_ppibar(i) << endl;
	}
	fout << endl;
	fout << "M^-1 matrix" << endl;
	fout << "-----------" << endl;
	for (int i = 0; i < M_inv.GetNrows(); i++)
	{
		for (int j = 0; j < M_inv.GetNcols(); j++)
		{
			fout << M_inv(i, j) << "  ";
		}
		fout << endl;
	}
	fout << "==================" << endl;
}

//No reflections, no cross-contamination
void PIDMatrix_NstG_simplest(string outfile)
{
	//Dummy integer to use
	int N = 0;
	//Vector with total Yields. Cuts:(ppi, pbarpi, ppibar)
	TVectorT<double> Y(3);
	Y(0) = stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012_Subst0_K2p.txt", 1)[0]);
	Y(1) = stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012.txt", 1)[0]);
	Y(2) = stod(ReadVariablesWord(N, "output/TotalYield_Fit_B_M012_Subst01_Kpi2pK.txt", 1)[0]);
	//Our unknowns (contamination)
	TVectorT<double> x(3);
	//Now, we'll compose the PID matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TMatrixT<double> pid_eff(3, 3);
	pid_eff(0, 0) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_2hG.txt"));
	pid_eff(0, 1) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_2hG.txt"));
	pid_eff(0, 2) = stod(GetValueFor("Global", "output/CutEff_PID_L1520G_2hG.txt"));
	//pbarpi
	pid_eff(1, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_2hG.txt"));
	pid_eff(1, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_2hG.txt"));
	pid_eff(1, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1520G_2hG.txt"));
	//ppibar
	pid_eff(2, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_2hG.txt"));
	pid_eff(2, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_2hG.txt"));
	pid_eff(2, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1520G_2hG.txt"));

	//Time to build up the matrices
	//M Matrix
	TMatrixT<double> M(3, 3);
	//Let's fill up first each channel as an independent source
	for (int i = 0; i < M.GetNrows(); i++)
	{
		M(i, 0) = pid_eff(i, 0) / pid_eff(0, 0);
		M(i, 1) = pid_eff(i, 1) / pid_eff(0, 1);
		M(i, 2) = pid_eff(i, 2) / pid_eff(0, 2);
	}
	//Time to invert our matrix to solve the problem: Y = M·x --> x = M^-1·Y
	TMatrixT<double> M_inv = M.Invert();
	//Result on x
	for (int i = 0; i < M.GetNrows(); i++)
	{
		x(i) = 0;
		for (int j = 0; j < M.GetNcols(); j++)
		{
			x(i) += M_inv(i, j) * Y(j);
		}
	}

	TVectorT<double> x_final_ppi(3);
	//Contributions for ppi PID cuts
	//Main
	//ppiG
	x_final_ppi(0) = x(0);
	//KpiG
	x_final_ppi(1) = x(1);
	//pKG
	x_final_ppi(2) = x(2);

	TVectorT<double> x_final_pbarpi(3);
	TVectorT<double> x_final_ppibar(3);
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		x_final_pbarpi(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(1, i);
		x_final_ppibar(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(2, i);
	}

	vector<string> namelist = PIDMatrix_namelist("simple");
	ofstream fout;
	fout.open(outfile.c_str());
	fout << "===== RESULT =====" << endl;
	fout << "ppi cuts" << endl;
	fout << "--------" << endl;
	for (int i = 0; i < x_final_ppi.GetNrows(); i++)
	{
		fout << namelist[i] << "_ppi = " << x_final_ppi(i) << endl;
	}
	fout << endl;
	fout << "pbarpi cuts" << endl;
	fout << "-----------" << endl;
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		fout << namelist[i] << "_pbarpi = " << x_final_pbarpi(i) << endl;
	}
	fout << endl;
	fout << "ppibarcuts" << endl;
	fout << "----------" << endl;
	for (int i = 0; i < x_final_ppibar.GetNrows(); i++)
	{
		fout << namelist[i] << "_ppibar = " << x_final_ppibar(i) << endl;
	}
	fout << endl;
	fout << "M^-1 matrix" << endl;
	fout << "-----------" << endl;
	for (int i = 0; i < M_inv.GetNrows(); i++)
	{
		for (int j = 0; j < M_inv.GetNcols(); j++)
		{
			fout << M_inv(i, j) << "  ";
		}
		fout << endl;
	}
	fout << "==================" << endl;
}

vector<string> PIDMatrix_namelist(string option)
{
	vector<string> namelist;
	if (option == "all")
	{
		namelist.push_back("NstG");
		namelist.push_back("KstG");
		namelist.push_back("K2stG");
		namelist.push_back("L1520G");
		namelist.push_back("L1670G");
		namelist.push_back("L1820G");
		namelist.push_back("L1830G");
		namelist.push_back("NstG_Ref");
		namelist.push_back("KstG_Ref");
		namelist.push_back("K2stG_Ref");
		namelist.push_back("L1520G_Ref");
		namelist.push_back("L1670G_Ref");
		namelist.push_back("L1820G_Ref");
		namelist.push_back("L1830G_Ref");
	}
	else if (option == "simple")
	{
		namelist.push_back("NstG");
		namelist.push_back("KstG");
		namelist.push_back("L1520G");
		namelist.push_back("NstG_Ref");
		namelist.push_back("KstG_Ref");
		namelist.push_back("L1520G_Ref");
	}
	else if (option == "pKG")
	{
		namelist.push_back("NstG");
		namelist.push_back("KstG");
		namelist.push_back("K2stG");
		namelist.push_back("pKG");
		namelist.push_back("NstG_Ref");
		namelist.push_back("KstG_Ref");
		namelist.push_back("K2stG_Ref");
		namelist.push_back("pKG_Ref");
	}
	return namelist;
}



#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
		case 2:
		PIDMatrix(*(new string(argv[1])), *(new string(argv[2])));
		break;
		default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
