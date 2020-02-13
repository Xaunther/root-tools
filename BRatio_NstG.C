//Function to calculate branching fraction.
//Used for Lb->n*gamma analysis

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "../Functions/TUncertainty.h"
#include "../Functions/Dictreading.h"
using namespace std;

//Compute BRatio for NstG analysis. The formula is as follows
//BR = N_S/N_B * (sum BF(B)eff_B)/eff_S
/*The list of uncertainties is:
	1: Statistical
	2: Systematic (sample size)
	3: Theorical (L* BF)
	4: Systematic (alternative reweight)
*/
void BRatio_NstG(string outfile)
{
	//Branching Fractions of each Lambda channel into proton-Kaon
	//This is our "theory" error. Values from the PDG
	std::vector<TUncertainty> BF_pKG;
	BF_pKG.push_back(TUncertainty(0.45 * 2 / 3, {0, 0, 0.01 * 2 / 3}));
	BF_pKG.push_back(TUncertainty(0.25 * 2 / 3, {0, 0, 0.05 * 2 / 3}));
	BF_pKG.push_back(TUncertainty(0.6 * 2 / 3 , {0, 0, 0.05 * 2 / 3}));
	BF_pKG.push_back(TUncertainty(0.07 * 2 / 3, {0, 0, 0.035 * 2 / 3}));


	//Dummy variables we will use when loading efficiencies
	TUncertainty eff1, eff2;
	//Efficiencies for each step, and channel. 0-th component is NstG
	std::vector<TUncertainty> eff_Gen, eff_Full, eff_DST, eff_MCM, eff_Pre, eff_sel, eff_PID;
	//Total efficiency for signal and bkg
	TUncertainty eff_ppiG;
	std::vector<TUncertainty> eff_pKG;

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
	//Generator
	eff1 = TUncertainty(stod(GetValueFor("NstG_2012_Down_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Down_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("NstG_2012_Up_eff", "Systematics/Generation/GenLevelCut_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Up_err", "Systematics/Generation/GenLevelCut_Eff.txt"))
	});

	eff_Gen.push_back((eff1 + eff2) / 2.);

	//Full event cut. Pick difference from alternative reweighting
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_Gauss_NstG_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_Gauss_NstG_2hG.txt"))
	});
	eff2 = TUncertainty(0, {0, 0, 0,
	                        abs(stod(GetValueFor("Global", "Systematics/Reweight/CutEff_Gauss_NstG_2hG_alt.txt")) - eff1.GetValue())
	                       });

	eff_Full.push_back(eff1 + eff2);

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("NstG_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("NstG_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("NstG_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_DST.push_back((eff1 + eff2) / 2.);

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_NstG_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_NstG_2hG.txt"))
	});

	eff_MCM.push_back(eff1);

	//Offline selection. Pick difference from alternative reweighting
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_NstG_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_NstG_2hG.txt"))
	});
	eff2 = TUncertainty(0, {0, 0, 0,
	                        abs(stod(GetValueFor("Global", "Systematics/Reweight/PreCutEff_NstG_2hG_alt.txt")) - eff1.GetValue())
	                       });

	eff_Pre.push_back(eff1 + eff2);

	eff1 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_NstG_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_NstG_2hG.txt"))
	});
	eff2 = TUncertainty(0, {0, 0, 0,
	                        abs(stod(GetValueFor("Global", "Systematics/Reweight/CutEff_NstG_2hG_alt.txt")) - eff1.GetValue())
	                       });

	eff_sel.push_back(eff1 + eff2);

	//PID
	eff1 = TUncertainty(stod(GetValueFor("Mean", "Systematics/PID/NstG_PIDEff.txt")), {
		0,
		stod(GetValueFor("Error", "Systematics/PID/NstG_PIDEff.txt"))
	});
	eff2 = TUncertainty(0, {0, 0, 0,
	                        abs(stod(GetValueFor("Mean", "Systematics/Reweight/NstG_alt_PIDEff.txt")) - eff1.GetValue())
	                       });

	eff_PID.push_back(eff1 + eff2);

	/**********************************************************/
	//Now we must retrieve the efficiency for each Lambda channel (4)
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

	eff_Gen.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("L1520G_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1520G_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_Full.push_back((eff1 + eff2) / 2.);

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("L1520G_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1520G_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1520G_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_DST.push_back((eff1 + eff2) / 2.);

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_L1520G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_L1520G_2hG.txt"))
	});

	eff_MCM.push_back(eff1);

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_L1520G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_L1520G_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_L1520G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_L1520G_2hG.txt"))
	});

	eff_Pre.push_back(eff1);
	eff_sel.push_back(eff2);

	//PID
	eff1 = TUncertainty(stod(GetValueFor("Mean", "Systematics/PID/L1520G_PIDInv2Eff.txt")), {
		0,
		stod(GetValueFor("Error", "Systematics/PID/L1520G_PIDInv2Eff.txt"))
	});

	eff_PID.push_back(eff1);

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

	eff_Gen.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("L1670G_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1670G_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_Full.push_back((eff1 + eff2) / 2.);

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("L1670G_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1670G_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1670G_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_DST.push_back((eff1 + eff2) / 2.);

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_L1670G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_L1670G_2hG.txt"))
	});

	eff_MCM.push_back(eff1);

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_L1670G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_L1670G_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_L1670G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_L1670G_2hG.txt"))
	});

	eff_Pre.push_back(eff1);
	eff_sel.push_back(eff2);

	//PID
	eff1 = TUncertainty(stod(GetValueFor("Mean", "Systematics/PID/L1670G_PIDInv2Eff.txt")), {
		0,
		stod(GetValueFor("Error", "Systematics/PID/L1670G_PIDInv2Eff.txt"))
	});

	eff_PID.push_back(eff1);

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

	eff_Gen.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("L1820G_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1820G_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_Full.push_back((eff1 + eff2) / 2.);

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("L1820G_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1820G_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1820G_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_DST.push_back((eff1 + eff2) / 2.);

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_L1820G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_L1820G_2hG.txt"))
	});

	eff_MCM.push_back(eff1);

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_L1820G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_L1820G_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_L1820G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_L1820G_2hG.txt"))
	});

	eff_Pre.push_back(eff1);
	eff_sel.push_back(eff2);

	//PID
	eff1 = TUncertainty(stod(GetValueFor("Mean", "Systematics/PID/L1820G_PIDInv2Eff.txt")), {
		0,
		stod(GetValueFor("Error", "Systematics/PID/L1820G_PIDInv2Eff.txt"))
	});

	eff_PID.push_back(eff1);

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

	eff_Gen.push_back((eff1 + eff2) / 2.);

	//Full event cut
	eff1 = TUncertainty(stod(GetValueFor("L1830G_2012_Down_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Down_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1830G_2012_Up_eff", "Systematics/Generation/FullEventCut_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Up_err", "Systematics/Generation/FullEventCut_Eff.txt"))
	});

	eff_Full.push_back((eff1 + eff2) / 2.);

	//Stripping
	eff1 = TUncertainty(stod(GetValueFor("L1830G_2012_Down_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Down_err", "Systematics/DST/Global_Eff.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("L1830G_2012_Up_eff", "Systematics/DST/Global_Eff.txt")), {
		0,
		stod(GetValueFor("L1830G_2012_Up_err", "Systematics/DST/Global_Eff.txt"))
	});

	eff_DST.push_back((eff1 + eff2) / 2.);

	//MCMatch
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_MCMatch_L1830G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_MCMatch_L1830G_2hG.txt"))
	});

	eff_MCM.push_back(eff1);

	//Offline selection
	eff1 = TUncertainty(stod(GetValueFor("Global", "output/PreCutEff_L1830G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/PreCutEff_L1830G_2hG.txt"))
	});
	eff2 = TUncertainty(stod(GetValueFor("Global", "output/CutEff_L1830G_2hG.txt")), {
		0,
		stod(GetValueFor("Error", "output/CutEff_L1830G_2hG.txt"))
	});

	eff_Pre.push_back(eff1);
	eff_sel.push_back(eff2);

	//PID
	eff1 = TUncertainty(stod(GetValueFor("Mean", "Systematics/PID/L1830G_PIDInv2Eff.txt")), {
		0,
		stod(GetValueFor("Error", "Systematics/PID/L1830G_PIDInv2Eff.txt"))
	});

	eff_PID.push_back(eff1);



	//Compute the efficiencies for each channel
	eff_ppiG = eff_Gen[0] * eff_Full[0] * eff_DST[0] * eff_MCM[0] * eff_Pre[0] * eff_sel[0] * eff_PID[0];
	for (unsigned int i = 0; i < eff_Gen.size() - 1; i++)
	{
		eff_pKG.push_back(eff_Gen[i + 1]*eff_Full[i + 1]*eff_DST[i + 1]*eff_MCM[i + 1]*eff_Pre[i + 1]*eff_sel[i + 1]*eff_PID[i + 1]);
	}


	//Part to dump into file the results, let's define our vector of components
	std::vector<string> decname;
	decname.push_back("NstG"); decname.push_back("L1520G"); decname.push_back("L1670G"); decname.push_back("L1820G"); decname.push_back("L1830G");
	//Open output file, we will print different results here
	ofstream outf;
	outf.open(outfile.c_str());
	//First, efficiency for each channel, then we will collapse the 4 bkg channels
	for (unsigned int i = 0; i < decname.size(); i++)
	{
		outf << "Generator Level " << decname[i] << endl;
		outf << "----------------------" << endl;
		eff_Gen[i].Print(outf, "rel"); outf << endl;
		outf << "Full Event Cut " << decname[i] << endl;
		outf << "---------------------" << endl;
		eff_Full[i].Print(outf, "rel"); outf << endl;
		outf << "DST2Tuple " << decname[i] << endl;
		outf << "----------------" << endl;
		(eff_DST[i]*eff_MCM[i]).Print(outf, "rel"); outf << endl;
		outf << "Selection " << decname[i] << endl;
		outf << "----------------" << endl;
		(eff_Pre[i]*eff_sel[i]).Print(outf, "rel"); outf << endl;
		outf << "PID " << decname[i] << endl;
		outf << "----------" << endl;
		eff_PID[i].Print(outf, "rel"); outf << endl;
	}
	//Do the sum, to show the error after summing. Save all results
	TUncertainty sum_Gen = 0, sum_Full = 0, sum_DST = 0, sum_sel = 0, sum_PID = 0;
	for (unsigned int i = 0; i < BF_pKG.size(); i++)
	{
		sum_Gen = sum_Gen + (BF_pKG[i] * eff_Gen[i + 1]);
		sum_Full = sum_Full + (BF_pKG[i] * eff_Full[i + 1]);
		sum_DST = sum_DST + (BF_pKG[i] * eff_DST[i + 1] * eff_MCM[i + 1]);
		sum_sel = sum_sel + (BF_pKG[i] * eff_Pre[i + 1] * eff_sel[i + 1]);
		sum_PID = sum_PID + (BF_pKG[i] * eff_PID[i + 1]);
	}
	sum_Gen = sum_Gen / eff_Gen[0];
	sum_Full = sum_Full / eff_Full[0];
	sum_DST = sum_DST / eff_DST[0] / eff_MCM[0];
	sum_sel = sum_sel / eff_Pre[0] / eff_sel[0];
	sum_PID = sum_PID / eff_PID[0];
	outf << "Generator Level LambdaG" << endl;
	outf << "-----------------------" << endl;
	sum_Gen.Print(outf, "rel"); outf << endl;
	outf << "Full Event Cut LambdaG" << endl;
	outf << "----------------------" << endl;
	sum_Full.Print(outf, "rel"); outf << endl;
	outf << "DST2Tuple LambdaG" << endl;
	outf << "-----------------" << endl;
	sum_DST.Print(outf, "rel"); outf << endl;
	outf << "Selection LambdaG" << endl;
	outf << "-----------------" << endl;
	sum_sel.Print(outf, "rel"); outf << endl;
	outf << "PID LambdaG" << endl;
	outf << "-----------" << endl;
	sum_PID.Print(outf, "rel"); outf << endl;

	//Compute BRatio
	TUncertainty B_Ratio = 0;
	for (unsigned int i = 0; i < BF_pKG.size(); i++)
	{
		B_Ratio = B_Ratio + (BF_pKG[i] * eff_pKG[i]);
	}
	B_Ratio = B_Ratio * N_ppiG / N_pKG / eff_ppiG;
	//Print BF result
	outf << endl;
	outf << "Branching Ratio" << endl;
	outf << "---------------" << endl;
	B_Ratio.Print(outf); B_Ratio.Print(outf, "rel"); B_Ratio.Print();
	outf.close();
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
	case 1:
		BRatio_NstG(*(new string(argv[1])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
