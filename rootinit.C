#include <string>
void rootinit(std::string path)
{
  //Path to mother folder
  //First of all come the personal constants
  gROOT->ProcessLine((".L "+path+"Dictionaries/Constants.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Dictionaries/Names.cxx+").c_str());
  //Utilities from independent sources
  gROOT->ProcessLine((".L "+path+"Functions/BifurcatedCB.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/RooCBExp.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/RooGaussExp.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/RooDoubleGaussExp.cxx+").c_str());
  //Personal tools not depending on anything external
  gROOT->ProcessLine((".L "+path+"Functions/ErrorTools.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/TISTOS.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/Styles.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/StringTools.cxx+").c_str());

  //Personal tools that rely on Dictionaries
  gROOT->ProcessLine((".L "+path+"Functions/ArrayTools.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/Dictreading.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/Filereading.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/TreeTools.cxx+").c_str());
  //Personal tools that rely on other tools
  gROOT->ProcessLine((".L "+path+"Functions/RunInfo.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/PlotTools.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/ParticleInfo.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/Interpol_exp.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/Fits.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/Filesaving.cxx+").c_str());
  gROOT->ProcessLine((".L "+path+"Functions/Constantize.cxx+").c_str());

  //Finally I can load the user-run scripts, which depend on the libraries already loaded
  //This away when running ROOT interactively I have some help on how to use them (not compiled!)

  gROOT->ProcessLine((".L "+path+"root/AddBranch.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/AddETA.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/AppendVars.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/BDTApply.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/BDTCut.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/BDTMerit.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/BDTTrain.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/BDTTrain2.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/BDT_BvsS.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/CheckMultiplicity.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/CompareVar.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/Compute_Value.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/Compare.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/Create_Cuts.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/CutEff.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/CutSig.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/CutTree.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/EntriesTable.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/Err_Frac.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/Fit_NstG.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/LatexTable.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/MakeVarList.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/MassSub.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/OutputSearch.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/PhaseSpace.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/PIDTable.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/plot2D.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/PlotCorrVar.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/PlotTable.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/PlotUsedVars.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/PlotVar.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/RenameBranch.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/ROC_Area.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/sPlot.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/TISTOS.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/Trigger_effs.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/TupleCorr.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/VarFit.C+").c_str());
  gROOT->ProcessLine((".L "+path+"root/Vodka.C+").c_str());
}
