//This functions is designed to, from a pre-configured keyconfig given, read a bunch of files containing PID efficiency and solve the equation:
//Y = M·x
//Where Y is an array of yields, for a certain cut. This is usually obtained from fits to data (normalization)
//Where x is the array of the contamination we expect on our signal (What we want to know).
//M is a matrix where the efficiencies are encoded.
//For NstG analysis, M_ij = F_ij + R_ij
//where F_ij is eff for cut i in channel j divided by eff for signal cut in channel j
//Where R_ij is eff for cut i in the reflection channel of j, divided by eff for signal cut in channel j
//The efficiency is of all the selections involved since the divergence of the signal and its reflection counterpart (since the tuple was made)
#include <iostream>
#include <string>
#include "TMatrixT.h"
#include "TVectorT.h"
#include "../Functions/Filereading.h"
#include "../Functions/Dictreading.h"
using namespace std;

//List of functions
void PIDMatrix(string keyconfig);
void PIDMatrix_NstG();
void PIDMatrix_NstG_simple();
void PIDMatrix_NstG_simplest();
vector<string> PIDMatrix_namelist();
//////////////////////////////////////////////////
//General function
void PIDMatrix(string keyconfig)
{
	if (keyconfig == "NstG") {PIDMatrix_NstG();}
	else if (keyconfig == "NstG_simple") {PIDMatrix_NstG_simple();}
	else if (keyconfig == "NstG_simplest") {PIDMatrix_NstG_simplest();}
	else {cout << "Could not match " << keyconfig << " to any keyconfig." << endl;}
}
//////////////////////////////////////////////////
//For NstG analysis
void PIDMatrix_NstG()
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
	TMatrixT<double> pid_eff(3, 14);
	pid_eff(0, 0) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_2hG.txt"));
	pid_eff(0, 1) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_2hG.txt"));
	pid_eff(0, 2) = stod(GetValueFor("Global", "output/CutEff_PID_K2stG_2hG.txt"));
	pid_eff(0, 3) = stod(GetValueFor("Global", "output/CutEff_PID_L1520G_2hG.txt"));
	pid_eff(0, 4) = stod(GetValueFor("Global", "output/CutEff_PID_L1670G_2hG.txt"));
	pid_eff(0, 5) = stod(GetValueFor("Global", "output/CutEff_PID_L1820G_2hG.txt"));
	pid_eff(0, 6) = stod(GetValueFor("Global", "output/CutEff_PID_L1830G_2hG.txt"));
	//pid_eff(0, 7) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_Ref_2hG.txt"));
	pid_eff(0, 7) = 0.;
	pid_eff(0, 8) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_Ref_2hG.txt"));
	pid_eff(0, 9) = stod(GetValueFor("Global", "output/CutEff_PID_K2stG_Ref_2hG.txt"));
	pid_eff(0, 10) = stod(GetValueFor("Global", "output/CutEff_PID_L1520G_Ref_2hG.txt"));
	pid_eff(0, 11) = stod(GetValueFor("Global", "output/CutEff_PID_L1670G_Ref_2hG.txt"));
	pid_eff(0, 12) = stod(GetValueFor("Global", "output/CutEff_PID_L1820G_Ref_2hG.txt"));
	pid_eff(0, 13) = stod(GetValueFor("Global", "output/CutEff_PID_L1830G_Ref_2hG.txt"));
	//pbarpi
	pid_eff(1, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_2hG.txt"));
	pid_eff(1, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_2hG.txt"));
	pid_eff(1, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_K2stG_2hG.txt"));
	pid_eff(1, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1520G_2hG.txt"));
	pid_eff(1, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1670G_2hG.txt"));
	pid_eff(1, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1820G_2hG.txt"));
	pid_eff(1, 6) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1830G_2hG.txt"));
	//pid_eff(1, 7) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_Ref_2hG.txt"));
	pid_eff(1, 7) = 0.;
	pid_eff(1, 8) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_Ref_2hG.txt"));
	pid_eff(1, 9) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_K2stG_Ref_2hG.txt"));
	pid_eff(1, 10) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1520G_Ref_2hG.txt"));
	pid_eff(1, 11) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1670G_Ref_2hG.txt"));
	pid_eff(1, 12) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1820G_Ref_2hG.txt"));
	pid_eff(1, 13) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1830G_Ref_2hG.txt"));
	//ppibar
	pid_eff(2, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_2hG.txt"));
	pid_eff(2, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_2hG.txt"));
	pid_eff(2, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_K2stG_2hG.txt"));
	pid_eff(2, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1520G_2hG.txt"));
	pid_eff(2, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1670G_2hG.txt"));
	pid_eff(2, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1820G_2hG.txt"));
	pid_eff(2, 6) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1830G_2hG.txt"));
	//pid_eff(2, 7) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_Ref_2hG.txt"));
	pid_eff(2, 7) = 0.;
	pid_eff(2, 8) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_Ref_2hG.txt"));
	pid_eff(2, 9) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_K2stG_Ref_2hG.txt"));
	pid_eff(2, 10) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1520G_Ref_2hG.txt"));
	pid_eff(2, 11) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1670G_Ref_2hG.txt"));
	pid_eff(2, 12) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1820G_Ref_2hG.txt"));
	pid_eff(2, 13) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1830G_Ref_2hG.txt"));
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
		x(i) = 0;
		for (int j = 0; j < M.GetNcols(); j++)
		{
			x(i) += M_inv(i, j) * Y(j);
		}
	}

	TVectorT<double> x_final_ppi(14);
	//Contributions for ppi PID cuts
	//Main
	//ppiG
	x_final_ppi(0) = ch_weight(0) * x(0) * F(0, 0);
	//KpiG
	x_final_ppi(1) = ch_weight(1) * x(1) * F(0, 1);
	x_final_ppi(2) = ch_weight(2) * x(1) * F(0, 2);
	//pKG
	x_final_ppi(3) = ch_weight(3) * x(2) * F(0, 3);
	x_final_ppi(4) = ch_weight(4) * x(2) * F(0, 4);
	x_final_ppi(5) = ch_weight(5) * x(2) * F(0, 5);
	x_final_ppi(6) = ch_weight(6) * x(2) * F(0, 6);
	//Reflections
	//ppiG
	x_final_ppi(7) = ch_weight(0) * x(0) * R(0, 0);
	//KpiG
	x_final_ppi(8) = ch_weight(1) * x(1) * R(0, 1);
	x_final_ppi(9) = ch_weight(2) * x(1) * R(0, 2);
	//pKG
	x_final_ppi(10) = ch_weight(3) * x(2) * R(0, 3);
	x_final_ppi(11) = ch_weight(4) * x(2) * R(0, 4);
	x_final_ppi(12) = ch_weight(5) * x(2) * R(0, 5);
	x_final_ppi(13) = ch_weight(6) * x(2) * R(0, 6);

	TVectorT<double> x_final_pbarpi(14);
	TVectorT<double> x_final_ppibar(14);
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		x_final_pbarpi(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(1, i);
		x_final_ppibar(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(2, i);
	}

	vector<string> namelist = PIDMatrix_namelist();
	cout << "===== RESULT =====" << endl;
	cout << "ppi cuts" << endl;
	cout << "--------" << endl;
	for (int i = 0; i < x_final_ppi.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_ppi(i) << endl;
	}
	cout << endl;
	cout << "pbarpi cuts" << endl;
	cout << "-----------" << endl;
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_pbarpi(i) << endl;
	}
	cout << endl;
	cout << "ppibarcuts" << endl;
	cout << "----------" << endl;
	for (int i = 0; i < x_final_ppibar.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_ppibar(i) << endl;
	}
	cout << "==================" << endl;
}

//No Reflections
void PIDMatrix_NstG_simple()
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
	TMatrixT<double> pid_eff(3, 7);
	pid_eff(0, 0) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_2hG.txt"));
	pid_eff(0, 1) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_2hG.txt"));
	pid_eff(0, 2) = stod(GetValueFor("Global", "output/CutEff_PID_K2stG_2hG.txt"));
	pid_eff(0, 3) = stod(GetValueFor("Global", "output/CutEff_PID_L1520G_2hG.txt"));
	pid_eff(0, 4) = stod(GetValueFor("Global", "output/CutEff_PID_L1670G_2hG.txt"));
	pid_eff(0, 5) = stod(GetValueFor("Global", "output/CutEff_PID_L1820G_2hG.txt"));
	pid_eff(0, 6) = stod(GetValueFor("Global", "output/CutEff_PID_L1830G_2hG.txt"));
	//pbarpi
	pid_eff(1, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_2hG.txt"));
	pid_eff(1, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_2hG.txt"));
	pid_eff(1, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_K2stG_2hG.txt"));
	pid_eff(1, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1520G_2hG.txt"));
	pid_eff(1, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1670G_2hG.txt"));
	pid_eff(1, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1820G_2hG.txt"));
	pid_eff(1, 6) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1830G_2hG.txt"));
	//ppibar
	pid_eff(2, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_2hG.txt"));
	pid_eff(2, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_2hG.txt"));
	pid_eff(2, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_K2stG_2hG.txt"));
	pid_eff(2, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1520G_2hG.txt"));
	pid_eff(2, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1670G_2hG.txt"));
	pid_eff(2, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1820G_2hG.txt"));
	pid_eff(2, 6) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1830G_2hG.txt"));

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
		for (int j = 0; j < F.GetNcols(); j++)
		{
			_M(i, j) = F(i, j);
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
		x(i) = 0;
		for (int j = 0; j < M.GetNcols(); j++)
		{
			x(i) += M_inv(i, j) * Y(j);
		}
	}

	TVectorT<double> x_final_ppi(7);
	//Contributions for ppi PID cuts
	//Main
	//ppiG
	x_final_ppi(0) = ch_weight(0) * x(0) * F(0, 0);
	//KpiG
	x_final_ppi(1) = ch_weight(1) * x(1) * F(0, 1);
	x_final_ppi(2) = ch_weight(2) * x(1) * F(0, 2);
	//pKG
	x_final_ppi(3) = ch_weight(3) * x(2) * F(0, 3);
	x_final_ppi(4) = ch_weight(4) * x(2) * F(0, 4);
	x_final_ppi(5) = ch_weight(5) * x(2) * F(0, 5);
	x_final_ppi(6) = ch_weight(6) * x(2) * F(0, 6);

	TVectorT<double> x_final_pbarpi(7);
	TVectorT<double> x_final_ppibar(7);
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		x_final_pbarpi(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(1, i);
		x_final_ppibar(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(2, i);
	}

	vector<string> namelist = PIDMatrix_namelist();
	cout << "===== RESULT =====" << endl;
	cout << "ppi cuts" << endl;
	cout << "--------" << endl;
	for (int i = 0; i < x_final_ppi.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_ppi(i) << endl;
	}
	cout << endl;
	cout << "pbarpi cuts" << endl;
	cout << "-----------" << endl;
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_pbarpi(i) << endl;
	}
	cout << endl;
	cout << "ppibarcuts" << endl;
	cout << "----------" << endl;
	for (int i = 0; i < x_final_ppibar.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_ppibar(i) << endl;
	}
	cout << "==================" << endl;
}

//No reflections, no cross-contamination
void PIDMatrix_NstG_simplest()
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
	TMatrixT<double> pid_eff(3, 7);
	pid_eff(0, 0) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_2hG.txt"));
	pid_eff(0, 1) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_2hG.txt"));
	pid_eff(0, 2) = stod(GetValueFor("Global", "output/CutEff_PID_K2stG_2hG.txt"));
	pid_eff(0, 3) = stod(GetValueFor("Global", "output/CutEff_PID_L1520G_2hG.txt"));
	pid_eff(0, 4) = stod(GetValueFor("Global", "output/CutEff_PID_L1670G_2hG.txt"));
	pid_eff(0, 5) = stod(GetValueFor("Global", "output/CutEff_PID_L1820G_2hG.txt"));
	pid_eff(0, 6) = stod(GetValueFor("Global", "output/CutEff_PID_L1830G_2hG.txt"));
	//pbarpi
	pid_eff(1, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_2hG.txt")) * 0;
	pid_eff(1, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_2hG.txt"));
	pid_eff(1, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_K2stG_2hG.txt"));
	pid_eff(1, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1520G_2hG.txt")) * 0;
	pid_eff(1, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1670G_2hG.txt")) * 0;
	pid_eff(1, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1820G_2hG.txt")) * 0;
	pid_eff(1, 6) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1830G_2hG.txt")) * 0;
	//ppibar
	pid_eff(2, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_2hG.txt")) * 0;
	pid_eff(2, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_2hG.txt")) * 0;
	pid_eff(2, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_K2stG_2hG.txt")) * 0;
	pid_eff(2, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1520G_2hG.txt"));
	pid_eff(2, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1670G_2hG.txt"));
	pid_eff(2, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1820G_2hG.txt"));
	pid_eff(2, 6) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1830G_2hG.txt"));

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
		for (int j = 0; j < F.GetNcols(); j++)
		{
			_M(i, j) = F(i, j);
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
		x(i) = 0;
		for (int j = 0; j < M.GetNcols(); j++)
		{
			x(i) += M_inv(i, j) * Y(j);
		}
	}

	TVectorT<double> x_final_ppi(7);
	//Contributions for ppi PID cuts
	//Main
	//ppiG
	x_final_ppi(0) = ch_weight(0) * x(0) * F(0, 0);
	//KpiG
	x_final_ppi(1) = ch_weight(1) * x(1) * F(0, 1);
	x_final_ppi(2) = ch_weight(2) * x(1) * F(0, 2);
	//pKG
	x_final_ppi(3) = ch_weight(3) * x(2) * F(0, 3);
	x_final_ppi(4) = ch_weight(4) * x(2) * F(0, 4);
	x_final_ppi(5) = ch_weight(5) * x(2) * F(0, 5);
	x_final_ppi(6) = ch_weight(6) * x(2) * F(0, 6);

	TVectorT<double> x_final_pbarpi(7);
	TVectorT<double> x_final_ppibar(7);
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		x_final_pbarpi(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(1, i);
		x_final_ppibar(i) = x_final_ppi(i) / pid_eff(0, i) * pid_eff(2, i);
	}

	vector<string> namelist = PIDMatrix_namelist();
	cout << "===== RESULT =====" << endl;
	cout << "ppi cuts" << endl;
	cout << "--------" << endl;
	for (int i = 0; i < x_final_ppi.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_ppi(i) << endl;
	}
	cout << endl;
	cout << "pbarpi cuts" << endl;
	cout << "-----------" << endl;
	for (int i = 0; i < x_final_pbarpi.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_pbarpi(i) << endl;
	}
	cout << endl;
	cout << "ppibarcuts" << endl;
	cout << "----------" << endl;
	for (int i = 0; i < x_final_ppibar.GetNrows(); i++)
	{
		cout << namelist[i] << ": " << x_final_ppibar(i) << endl;
	}
	cout << "==================" << endl;
}

vector<string> PIDMatrix_namelist()
{
	vector<string> namelist;
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
	return namelist;
}



#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
	case 1:
		PIDMatrix(*(new string(argv[1])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for PIDMatrix" << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
