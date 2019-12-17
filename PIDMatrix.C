//This functions is designed to, from a pre-configured keyconfig given, read a bunch of files containing PID efficiency and solve the equation:
//Y = M·x
//Where Y is an array of yields, for a certain cut. This is usually obtained from fits to data (normalization)
//Where x is the array of the contamination we expect on our signal (What we want to know).
//M is a matrix where the efficiencies are encoded.
//For NstG analysis, M_ij = F_ij + R_ij
//where F_ij is eff for cut i in channel j divided by eff for signal cut in channel j
//Where R_ij is eff for cut i in the reflection channel of j, divided by eff for signal cut in channel j
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
void PIDMatrix_NstG_bad();
//////////////////////////////////////////////////
//General function
void PIDMatrix(string keyconfig)
{
	if (keyconfig == "NstG") {PIDMatrix_NstG();}
	else if(keyconfig == "NstG_bad"){PIDMatrix_NstG_bad();}
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
	//Now, we'll compose the matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TMatrixT<double> eff(3, 6);
	eff(0, 0) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_2hG.txt"));
	eff(0, 1) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_2hG.txt"));
	eff(0, 2) = stod(GetValueFor("Global", "output/CutEff_PID_L1820G_2hG.txt"));
	eff(0, 3) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_Ref_2hG.txt"));
	eff(0, 4) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_Ref_2hG.txt"));
	eff(0, 5) = stod(GetValueFor("Global", "output/CutEff_PID_L1820G_Ref_2hG.txt"));
	//pbarpi
	eff(1, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_2hG.txt"));
	eff(1, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_2hG.txt"));
	eff(1, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1820G_2hG.txt"));
	eff(1, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_Ref_2hG.txt"));
	eff(1, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_Ref_2hG.txt"));
	eff(1, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1820G_Ref_2hG.txt"));
	//ppibar
	eff(2, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_2hG.txt"));
	eff(2, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_2hG.txt"));
	eff(2, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1820G_2hG.txt"));
	eff(2, 3) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_Ref_2hG.txt"));
	eff(2, 4) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_Ref_2hG.txt"));
	eff(2, 5) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1820G_Ref_2hG.txt"));
	//Time to build up the matrices
	//M Matrix
	TMatrixT<double> M(3, 3);
	//F Matrix
	TMatrixT<double> F(3, 3);
	//R Matrix
	TMatrixT<double> R(3, 3);
	for (int i = 0; i < F.GetNrows(); i++)
	{
		for (int j = 0; j < F.GetNcols(); j++)
		{
			F(i, j) = eff(i, j) / eff(0, j);
			R(i, j) = eff(i, j + 3) / eff(0, j);
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
	cout << "===== RESULT =====" << endl;
	x.Print();
	cout << "==================" << endl;
}

void PIDMatrix_NstG_bad()
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
	//Now, we'll compose the matrix efficiency. Rows are for cuts, columns for channels (ppiG, KpiG, pKG, pipG, piKG, KpG)
	//ppi
	TMatrixT<double> eff(3, 6);
	eff(0, 0) = stod(GetValueFor("Global", "output/CutEff_PID_NstG_2hG.txt"));
	eff(0, 1) = stod(GetValueFor("Global", "output/CutEff_PID_KstG_2hG.txt"));
	eff(0, 2) = stod(GetValueFor("Global", "output/CutEff_PID_L1820G_2hG.txt"));
	eff(0, 3) = 0.;
	eff(0, 4) = 0.;
	eff(0, 5) = 0.;
	//pbarpi
	eff(1, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_NstG_2hG.txt"));
	eff(1, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_KstG_2hG.txt"));
	eff(1, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv1_L1820G_2hG.txt"));
	eff(1, 3) = 0.;
	eff(1, 4) = 0.;
	eff(1, 5) = 0.;
	//ppibar
	eff(2, 0) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_NstG_2hG.txt"));
	eff(2, 1) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_KstG_2hG.txt"));
	eff(2, 2) = stod(GetValueFor("Global", "output/CutEff_PIDInv2_L1820G_2hG.txt"));
	eff(2, 3) = 0.;
	eff(2, 4) = 0.;
	eff(2, 5) = 0.;
	//Time to build up the matrices
	//M Matrix
	TMatrixT<double> M(3, 3);
	//F Matrix
	TMatrixT<double> F(3, 3);
	//R Matrix
	TMatrixT<double> R(3, 3);
	for (int i = 0; i < F.GetNrows(); i++)
	{
		for (int j = 0; j < F.GetNcols(); j++)
		{
			F(i, j) = eff(i, j) / eff(0, j);
			R(i, j) = eff(i, j + 3) / eff(0, j);
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
	cout << "===== RESULT =====" << endl;
	x.Print();
	cout << "==================" << endl;
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
