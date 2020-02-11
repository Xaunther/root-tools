//Function to calculate branching fraction.
//Used for Lb->n*gamma analysis

#include <iostream>
#include <string>
#include <fstream>
#include "../Functions/TUncertainty.h"
#include "../Functions/Dictreading.h"
using namespace std;

//Compute BRatio for NstG analysis. The formula is as follows
//BR = N_S/N_B * (sum BF(B)eff_B)/eff_S
/*The list of uncertainties is:
	1: Statistical
	2: Systematic
	3: Theorical
*/
void BRatio_NstG(string outfile)
{
	//Dummy variables we will use when loading efficiencies
	TUncertainty eff1, eff2;
	//Yield of each channel
	TUncertainty N_ppiG(stod(GetValueFor("n_{S}", "output/B_M012_Subst0_K2p_RooYields.txt")),
	                    stod(GetErrorFor("n_{S}", "output/B_M012_Subst0_K2p_RooYields.txt")));
	TUncertainty N_pKG = 0;
	eff1 = TUncertainty(stod(GetColFor("L1520G_ppibar", "output/Yield_Contamination.txt", 4)),
	                    stod(GetColFor("L1520G_ppibar", "output/Yield_Contamination.txt", 6)));
	N_pKG = N_pKG + eff1;
	eff1 = TUncertainty(stod(GetColFor("L1670G_ppibar", "output/Yield_Contamination.txt", 4)),
	                    stod(GetColFor("L1670G_ppibar", "output/Yield_Contamination.txt", 6)));
	N_pKG = N_pKG + eff1;
	eff1 = TUncertainty(stod(GetColFor("L1820G_ppibar", "output/Yield_Contamination.txt", 4)),
	                    stod(GetColFor("L1820G_ppibar", "output/Yield_Contamination.txt", 6)));
	N_pKG = N_pKG + eff1;
	eff1 = TUncertainty(stod(GetColFor("L1830G_ppibar", "output/Yield_Contamination.txt", 4)),
	                    stod(GetColFor("L1830G_ppibar", "output/Yield_Contamination.txt", 6)));
	N_pKG = N_pKG + eff1;
	//Efficiency of signal. Composed by generator, stripping....
	TUncertainty eff_ppiG;
	//Generator
	eff1 = TUncertainty(stod(GetValueFor("NstG_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Down_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("NstG_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Up_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});

	eff_ppiG = (eff1 + eff2) / 2.;

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("NstG_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("NstG_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_ppiG = eff_ppiG * (eff1 + eff2) / 2.;

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("NstG_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("NstG_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_ppiG = eff_ppiG * (eff1 + eff2) / 2.;

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_NstG_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_NstG_2hG.txt"))
	});

	eff_ppiG = eff_ppiG * eff1;

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_NstG_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_NstG_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_NstG_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_NstG_2hG.txt"))
	});

	eff_ppiG = eff_ppiG * eff1 * eff2;
	/**********************************************************/
	//Now we must retrieve the efficiency for each Lambda channel (4)
	std::vector<TUncertainty> eff_pKG;
	/********************************
	*             L1520G            *
	********************************/
	//Generator
	eff1 = TUncertainty(stod(GetValueFor("L1520G_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Down_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1520G_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Up_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});

	eff_pKG.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("L1520G_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1520G_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * (eff1 + eff2) / 2.;

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("L1520G_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1520G_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * (eff1 + eff2) / 2.;

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_L1520G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_L1520G_2hG.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * eff1;

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_L1520G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_L1520G_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_L1520G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_L1520G_2hG.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * eff1 * eff2;

	/********************************
	*             L1670G            *
	********************************/
	//Generator
	eff1 = TUncertainty(stod(GetValueFor("L1670G_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Down_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1670G_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Up_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});

	eff_pKG.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("L1670G_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1670G_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * (eff1 + eff2) / 2.;

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("L1670G_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1670G_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * (eff1 + eff2) / 2.;

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_L1670G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_L1670G_2hG.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * eff1;

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_L1670G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_L1670G_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_L1670G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_L1670G_2hG.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * eff1 * eff2;

	/********************************
	*             L1820G            *
	********************************/
	//Generator
	eff1 = TUncertainty(stod(GetValueFor("L1820G_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Down_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1820G_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Up_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});

	eff_pKG.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("L1820G_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1820G_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * (eff1 + eff2) / 2.;

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("L1820G_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1820G_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * (eff1 + eff2) / 2.;

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_L1820G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_L1820G_2hG.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * eff1;

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_L1820G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_L1820G_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_L1820G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_L1820G_2hG.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * eff1 * eff2;

	/********************************
	*             L1830G            *
	********************************/
	//Generator
	eff1 = TUncertainty(stod(GetValueFor("L1830G_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Down_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1830G_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Up_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});

	eff_pKG.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("L1830G_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1830G_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * (eff1 + eff2) / 2.;

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("L1830G_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1830G_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * (eff1 + eff2) / 2.;

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_L1830G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_L1830G_2hG.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * eff1;

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_L1830G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_L1830G_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_L1830G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_L1830G_2hG.txt"))
	});

	eff_pKG[eff_pKG.size() - 1] = eff_pKG[eff_pKG.size() - 1] * eff1 * eff2;

	//Last, but not least, we need to get the Branching Fractions of each Lambda channel into proton-Kaon
	//This is our "theory" error. Values from the PDG
	std::vector<TUncertainty> BF_pKG;
	BF_pKG.push_back(TUncertainty(0.45 * 2 / 3, {0, 0, 0.01 * 2 / 3}));
	BF_pKG.push_back(TUncertainty(0.25 * 2 / 3, {0, 0, 0.05 * 2 / 3}));
	BF_pKG.push_back(TUncertainty(0.6 * 2 / 3 , {0, 0, 0.05 * 2 / 3}));
	BF_pKG.push_back(TUncertainty(0.07 * 2 / 3, {0, 0, 0.035 * 2 / 3}));

	//Compute the result and save it in a file
	TUncertainty B_Ratio = 0;
	for (unsigned int i = 0; i < BF_pKG.size(); i++)
	{
		B_Ratio = B_Ratio + (BF_pKG[i] * eff_pKG[i]);
	}
	B_Ratio = B_Ratio * N_ppiG / N_pKG / eff_ppiG;
	B_Ratio.SaveResult(outfile);
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
	case 1:
		BRatio_NstG(*(new string(argv[2])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif