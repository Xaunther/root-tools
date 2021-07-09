#include <string>
#include "TROOT.h"
#include "TInterpreter.h"
void rootinit(std::string path)
{
  gInterpreter->AddIncludePath(path.c_str());
  //First of all come the personal constants
  gROOT->ProcessLine((".L " + path + "Dictionaries/Constants.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Dictionaries/Names.cxx+").c_str());
  //Utilities from independent sources
  gROOT->ProcessLine((".L " + path + "Functions/BifurcatedCB.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/gaussian_quadrature_64.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Roo2Exp.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Roo2SumExp.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/RooCBExp.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/RooGaussExp.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/RooDoubleGaussExp.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/RooArgusGauss.cxx+").c_str());
  //Personal tools not depending on anything external
  gROOT->ProcessLine((".L " + path + "Functions/ErrorTools.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Extract_Var.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Probability.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/ProgressBar.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/TISTOS.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/TUncertainty.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Styles.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/StringTools.cxx+").c_str());

  //Personal tools that rely on Dictionaries
  gROOT->ProcessLine((".L " + path + "Functions/ArrayTools.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Dictreading.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Filereading.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/TreeTools.cxx+").c_str());
  //Personal tools that rely on other tools
  gROOT->ProcessLine((".L " + path + "Functions/RunInfo.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/PlotTools.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/ParticleInfo.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Interpol_exp.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Fits.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/AnalFits.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Filesaving.cxx+").c_str());
  gROOT->ProcessLine((".L " + path + "Functions/Constantize.cxx+").c_str());

  //Finally I can load the user-run scripts, which depend on the libraries already loaded
  //This away when running ROOT interactively I have some help on how to use them (not compiled!)

  gROOT->ProcessLine((".L " + path + "AddBranch.C+").c_str());
  gROOT->ProcessLine((".L " + path + "AddPIDBranch.C+").c_str());
  gROOT->ProcessLine((".L " + path + "AppendVars.C+").c_str());
  gROOT->ProcessLine((".L " + path + "AssignRefWeight.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BDTApply.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BDTApply2.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BDTCut.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BDTMerit.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BDTTrain.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BDTTrain2.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BDT_BvsS.C+").c_str());
  //gROOT->ProcessLine((".L " + path + "BinEfficiencyAfter.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BRatio_NstG.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BRatio_NstG_split.C+").c_str());
  gROOT->ProcessLine((".L " + path + "BRatio_NstG_Simult.C+").c_str());
  gROOT->ProcessLine((".L " + path + "CheckMultiplicity.C+").c_str());
  gROOT->ProcessLine((".L " + path + "CompareList.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Compute_Value.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Compare.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Create_Cuts.C+").c_str());
  gROOT->ProcessLine((".L " + path + "CutCorrelation.C+").c_str());
  gROOT->ProcessLine((".L " + path + "CutEff.C+").c_str());
  gROOT->ProcessLine((".L " + path + "CutSig.C+").c_str());
  gROOT->ProcessLine((".L " + path + "CutTree.C+").c_str());
  gROOT->ProcessLine((".L " + path + "EffPerBin.C+").c_str());
  gROOT->ProcessLine((".L " + path + "EffStats.C+").c_str());
  gROOT->ProcessLine((".L " + path + "EntriesTable.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Err_Frac.C+").c_str());
  gROOT->ProcessLine((".L " + path + "ExpYield.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Fit_NstG.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Fit_NstG_ppimumu.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Fit_NstG_pKmumu.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Fit_NstG_Simult.C+").c_str());
  gROOT->ProcessLine((".L " + path + "HomoBins.C+").c_str());
  gROOT->ProcessLine((".L " + path + "InvertCut.C+").c_str());
  gROOT->ProcessLine((".L " + path + "LatexTable.C+").c_str());
  gROOT->ProcessLine((".L " + path + "MakeVarList.C+").c_str());
  gROOT->ProcessLine((".L " + path + "MassSub.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Merge_Trigger.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Multiplicity_Extrasel.C+").c_str());
  gROOT->ProcessLine((".L " + path + "OutputSearch.C+").c_str());
  gROOT->ProcessLine((".L " + path + "PhaseSpace.C+").c_str());
  //  gROOT->ProcessLine((".L "+path+"PIDMatrix.C+").c_str());
  gROOT->ProcessLine((".L " + path + "PIDPunzi.C+").c_str());
  gROOT->ProcessLine((".L " + path + "PIDTable.C+").c_str());
  gROOT->ProcessLine((".L " + path + "plot2D.C+").c_str());
  gROOT->ProcessLine((".L " + path + "PlotCorrVar.C+").c_str());
  gROOT->ProcessLine((".L " + path + "PlotTable.C+").c_str());
  gROOT->ProcessLine((".L " + path + "PlotUsedVars.C+").c_str());
  gROOT->ProcessLine((".L " + path + "PlotVar.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Ratio_NstG.C+").c_str());
  gROOT->ProcessLine((".L " + path + "RenameBranch.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Reweight2D.C+").c_str());
  gROOT->ProcessLine((".L " + path + "ROC_Area.C+").c_str());
  gROOT->ProcessLine((".L " + path + "ScaleYields.C+").c_str());
  gROOT->ProcessLine((".L " + path + "sPlot.C+").c_str());
  gROOT->ProcessLine((".L " + path + "sWeight.C+").c_str());
  gROOT->ProcessLine((".L " + path + "TISTOS_MC.C+").c_str());
  gROOT->ProcessLine((".L " + path + "TISTOS_data.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Trigger_effs.C+").c_str());
  gROOT->ProcessLine((".L " + path + "TupleCorr.C+").c_str());
  gROOT->ProcessLine((".L " + path + "UnbinnedReweight.C+").c_str());
  gROOT->ProcessLine((".L " + path + "VarFit.C+").c_str());
  gROOT->ProcessLine((".L " + path + "VarProfile.C+").c_str());
  gROOT->ProcessLine((".L " + path + "Vodka.C+").c_str());
  gROOT->ProcessLine((".L " + path + "WriteFromDic.C+").c_str());
  gROOT->ProcessLine((".L " + path + "WriteFromWS.C+").c_str());
}
