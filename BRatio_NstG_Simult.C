//Function to calculate branching fraction.
//Used for Lb->n*gamma analysis
//Good one, it uses pKG reweighted MC instead of some Lambda* resonances

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "Functions/TUncertainty.h"
#include "Functions/Dictreading.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
using namespace std;

//Compute BRatio for NstG analysis. The formula is as follows
//BR = N_S/N_B * (sum BF(B)eff_B)/eff_S
/*The list of uncertainties is:
	1: Statistical
	2: Systematic (sample size)
	3: Theorical (L* BF) TO BE REMOVEEED!!!!!
	4: Systematic (alternative reweight)
	5: Systematic (mean shift)
	6: Trigger differences 2011-2012
*/

//Function to compute the variation between 2011 and 2012 for every used channel and take average.
double Avg_Trigger_Variation()
{
	auto chain_2011 = GetChain("../Directories/Lb2pKG_2011_2hG_tuples.dir");
	auto chain_2012 = GetChain("../Directories/Lb2pKG_2hG_tuples.dir");
	auto precuts = GetCuts("test.txt");
	double eff2011 = 1, eff2012 = 1;
	//pKG
	eff2011 = (stod(GetValueFor("pKG_2011_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")) + stod(GetValueFor("pKG_2011_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt"))) / 2.;
	eff2011 *= (stod(GetValueFor("pKG_2011_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")) + stod(GetValueFor("pKG_2011_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt"))) / 2.;
	eff2011 *= (stod(GetValueFor("pKG_2011_Down_eff", "Systematics/DST/Global_Eff.txt")) + stod(GetValueFor("pKG_2011_Up_eff", "Systematics/DST/Global_Eff.txt"))) / 2.;
	eff2011 *= GetMeanEntries(chain_2011, precuts);
	eff2011 *= stod(GetValueFor("Global", "output/CutEff_pKG_2011_2hG.txt")) * stod(GetValueFor("Mean", "Systematics/PID/pKG_2011_PIDEff.txt"));
	eff2012 = (stod(GetValueFor("pKG_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")) + stod(GetValueFor("pKG_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt"))) / 2.;
	eff2012 *= (stod(GetValueFor("pKG_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")) + stod(GetValueFor("pKG_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt"))) / 2.;
	eff2012 *= (stod(GetValueFor("pKG_2012_Down_eff", "Systematics/DST/Global_Eff.txt")) + stod(GetValueFor("pKG_2012_Up_eff", "Systematics/DST/Global_Eff.txt"))) / 2.;
	eff2012 *= GetMeanEntries(chain_2012, precuts);
	eff2012 *= stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt")) * stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDEff.txt"));
	CloseChain(chain_2011);
	CloseChain(chain_2012);
	return abs(eff2012 - eff2011) / eff2012 / 3.; //Factor 1/3 to take into account that 2011 is 1/3 of the total lumi
}

void BRatio_NstG_Simult(string outfile)
{
	//Dummy variables we will use when loading efficiencies
	TUncertainty eff1, eff2;
	//Efficiencies for each step, and channel. 0-th component is NstG
	std::vector<TUncertainty> eff_Gen, eff_Full, eff_DST, eff_MCM, eff_Pre, eff_sel, eff_PID;
	//Total efficiency for signal and bkg
	TUncertainty eff_ppiG;
	TUncertainty eff_pKG;

	//Yield of each channel
	TUncertainty N_ppiG(stod(GetValueFor("f_ppiG_mass_ppiGMC", "SimultaneousFit/NstG_MultiPlot_RooYields.txt")), {stod(GetErrorFor("f_ppiG_mass_ppiGMC", "SimultaneousFit/NstG_MultiPlot_RooYields.txt")),
																												  0, 0, 0,
																												  stod(GetValueFor("Uncertainty", "Systematics/Fit/rel_uncertainty.txt")) * stod(GetValueFor("f_ppiG_mass_ppiGMC", "SimultaneousFit/NstG_MultiPlot_RooYields.txt"))});
	TUncertainty N_pKG = 0;
	eff1 = TUncertainty(stod(GetValueFor("f_ppiG_mass_pKGMC", "SimultaneousFit/NstG_MultiPlot_RooYields.txt")),
						stod(GetErrorFor("f_ppiG_mass_pKGMC", "SimultaneousFit/NstG_MultiPlot_RooYields.txt")));
	eff2 = TUncertainty(stod(GetValueFor("R_pKG_mass_pKGMC", "SimultaneousFit/NstG_MultiPlot_RooYields.txt")),
						stod(GetErrorFor("R_pKG_mass_pKGMC", "SimultaneousFit/NstG_MultiPlot_RooYields.txt")));
	N_pKG = eff1 * eff2;
	//Generator
	eff1 = TUncertainty(stod(GetValueFor("NstG_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {0,
																												stod(GetValueFor("NstG_2012_Down_err", "Systematics/Generation/GenLevelCut_Eff.txt"))});
	eff2 = TUncertainty(stod(GetValueFor("NstG_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {0,
																											  stod(GetValueFor("NstG_2012_Up_err", "Systematics/Generation/GenLevelCut_Eff.txt"))});

	eff_Gen.push_back((eff1 + eff2) / 2.);

	//Full event cut. Pick difference from alternative reweighting
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_Gauss_NstG_2hG.txt")), {0,
																						  stod(GetValueFor("Error", "output/CutEff_Gauss_NstG_2hG.txt"))});
	eff2 = TUncertainty(0, {0, 0, 0,
							abs(stod(GetValueFor("Global", "Systematics/Reweight/CutEff_Gauss_NstG_2hG_alt.txt")) - eff1.GetValue())});

	eff_Full.push_back(eff1 + eff2);

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("NstG_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {0,
																									stod(GetValueFor("NstG_2012_Down_err", "Systematics/DST/Global_Eff.txt"))});
	eff2 = TUncertainty(stod(GetValueFor("NstG_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {0,
																								  stod(GetValueFor("NstG_2012_Up_err", "Systematics/DST/Global_Eff.txt"))});

	eff_DST.push_back((eff1 + eff2) / 2.);

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_NstG_2hG.txt")), {0,
																							   stod(GetValueFor("Error", "output/PreCutEff_MCMatch_NstG_2hG.txt"))});

	eff_MCM.push_back(eff1);

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_NstG_2hG.txt")), {0,
																					   stod(GetValueFor("Error", "output/PreCutEff_NstG_2hG.txt"))});
	eff2 = TUncertainty(0, {0, 0, 0,
							abs(stod(GetValueFor("Global", "Systematics/Reweight/PreCutEff_NstG_2hG_alt.txt")) - eff1.GetValue())});

	eff_Pre.push_back(eff1 + eff2);

	eff1 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_NstG_2hG.txt")), {0,
																					stod(GetValueFor("Error", "output/CutEff_NstG_2hG.txt"))});
	eff2 = TUncertainty(0, {0, 0, 0,
							abs(stod(GetValueFor("Global", "Systematics/Reweight/CutEff_NstG_2hG_alt.txt")) - eff1.GetValue())});

	eff_sel.push_back(eff1 + eff2);

	//PID
	eff1 = TUncertainty(stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt")), {0,
																					   stod(GetValueFor("Error", "Systematics/PID/NstG_PIDEff.txt"))});
	eff2 = TUncertainty(0, {0, 0, 0,
							abs(stod(GetValueFor("Mean", "Systematics/Reweight/NstG_alt_PIDEff.txt")) - eff1.GetValue())});

	eff_PID.push_back(eff1 + eff2);

	/**********************************************************/
	//Now we must retrieve the efficiency for pKG
	/********************************
	*             pKG            *
	********************************/
	//Generator
	eff1 = TUncertainty(stod(GetValueFor("pKG_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {0,
																											   stod(GetValueFor("pKG_2012_Down_err", "Systematics/Generation/GenLevelCut_Eff.txt"))});
	eff2 = TUncertainty(stod(GetValueFor("pKG_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {0,
																											 stod(GetValueFor("pKG_2012_Up_err", "Systematics/Generation/GenLevelCut_Eff.txt"))});

	eff_Gen.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("pKG_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {0,
																												stod(GetValueFor("pKG_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))});
	eff2 = TUncertainty(stod(GetValueFor("pKG_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {0,
																											  stod(GetValueFor("pKG_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))});

	eff_Full.push_back((eff1 + eff2) / 2.);

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("pKG_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {0,
																								   stod(GetValueFor("pKG_2012_Down_err", "Systematics/DST/Global_Eff.txt"))});
	eff2 = TUncertainty(stod(GetValueFor("pKG_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {0,
																								 stod(GetValueFor("pKG_2012_Up_err", "Systematics/DST/Global_Eff.txt"))});

	eff_DST.push_back((eff1 + eff2) / 2.);

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_pKG_2hG.txt")), {0,
																							  stod(GetValueFor("Error", "output/PreCutEff_MCMatch_pKG_2hG.txt"))});

	eff_MCM.push_back(eff1);

	//Offline selection. Pick difference from alternative reweighting. Add trigger differences between 2011 and 2012
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt")), {0,
																					  stod(GetValueFor("Error", "output/PreCutEff_pKG_2hG.txt")), 0, 0, 0, Avg_Trigger_Variation() * stod(GetValueFor("Global", "output/PreCutEff_pKG_2hG.txt"))});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_pKG_2hG.txt")), {0,
																				   stod(GetValueFor("Error", "output/CutEff_pKG_2hG.txt"))});

	eff_Pre.push_back(eff1);
	eff_sel.push_back(eff2);

	//PID
	eff1 = TUncertainty(stod(GetValueFor("Mean", "Systematics/PID/pKG_PIDInv2Eff.txt")), {0,
																						  stod(GetValueFor("Error", "Systematics/PID/pKG_PIDInv2Eff.txt"))});

	eff_PID.push_back(eff1);

	//Compute the efficiencies for each channel
	eff_ppiG = eff_Gen[0] * eff_Full[0] * eff_DST[0] * eff_MCM[0] * eff_Pre[0] * eff_sel[0] * eff_PID[0];
	eff_pKG = eff_Gen[1] * eff_Full[1] * eff_DST[1] * eff_MCM[1] * eff_Pre[1] * eff_sel[1] * eff_PID[1];

	//Part to dump into file the results, let's define our vector of components
	std::vector<string> decname;
	decname.push_back("NstG");
	decname.push_back("pKG");
	//Open output file, we will print different results here
	ofstream outf;
	outf.open(outfile.c_str());
	//First, efficiency for each channel, then we will collapse the 4 bkg channels
	for (unsigned int i = 0; i < decname.size(); i++)
	{
		outf << "Generator Level " << decname[i] << endl;
		outf << "----------------------" << endl;
		eff_Gen[i].Print(outf, "rel");
		outf << endl;
		outf << "Full Event Cut " << decname[i] << endl;
		outf << "---------------------" << endl;
		eff_Full[i].Print(outf, "rel");
		outf << endl;
		outf << "DST2Tuple " << decname[i] << endl;
		outf << "----------------" << endl;
		(eff_DST[i] * eff_MCM[i]).Print(outf, "rel");
		outf << endl;
		outf << "Selection " << decname[i] << endl;
		outf << "----------------" << endl;
		(eff_Pre[i] * eff_sel[i]).Print(outf, "rel");
		outf << endl;
		outf << "PID " << decname[i] << endl;
		outf << "----------" << endl;
		eff_PID[i].Print(outf, "rel");
		outf << endl;
	}

	//Print whole efficiencies and ratio
	TUncertainty eff_Ratio = eff_pKG / eff_ppiG;
	outf << endl;
	outf << "Signal eff" << endl;
	outf << "-----------------" << endl;
	eff_ppiG.Print(outf);
	eff_ppiG.Print(outf, "rel");
	outf << "Normalization eff" << endl;
	outf << "-----------------" << endl;
	eff_pKG.Print(outf);
	eff_pKG.Print(outf, "rel");
	outf << "Ratio eff" << endl;
	outf << "-----------------" << endl;
	eff_Ratio.Print(outf);
	eff_Ratio.Print(outf, "rel");

	//Compute BRatio
	TUncertainty B_Ratio = 0;
	B_Ratio = N_ppiG / N_pKG * eff_pKG / eff_ppiG;
	//Print BF result
	outf << endl;
	outf << "Branching Ratio" << endl;
	outf << "---------------" << endl;
	B_Ratio.Print(outf);
	B_Ratio.Print(outf, "rel");
	B_Ratio.Print();
	outf.close();
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
	switch (argc - 1)
	{
	case 1:
		BRatio_NstG_Simult(*(new string(argv[1])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
