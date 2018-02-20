void rootinit()
{
  //First of all come the personal constants
  gROOT->ProcessLine(".L ./Dictionaries/Constants.cxx+");
  gROOT->ProcessLine(".L ./Dictionaries/Names.cxx+");
  //Utilities from independent sources
  gROOT->ProcessLine(".L ./Functions/BifurcatedCB.cxx+");
  //Personal tools not depending on anything external
  gROOT->ProcessLine(".L ./Functions/TISTOS.cxx+");
  //Miscellaneous functions, must be loaded in top of all personal functions
  gROOT->ProcessLine(".L ./Functions/misc.cxx+");  
  //Personal codes
  gROOT->ProcessLine(".L ./Functions/Fits.cxx+");

  //Finally I can load the user-run scripts, which depend on the libraries already loaded
  //This away when running ROOT interactively I have some help on how to use them (not compiled!)
  gROOT->ProcessLine(".L ./root/AddBranch.C");
  gROOT->ProcessLine(".L ./root/BDTApply.C");
  gROOT->ProcessLine(".L ./root/BDTCut.C");
  gROOT->ProcessLine(".L ./root/BDTMerit.C");
  gROOT->ProcessLine(".L ./root/BDTTrain.C");
  gROOT->ProcessLine(".L ./root/CompareVar.C");
  gROOT->ProcessLine(".L ./root/CutEff.C");
  gROOT->ProcessLine(".L ./root/CutSig.C");
  gROOT->ProcessLine(".L ./root/CutTree.C");
  gROOT->ProcessLine(".L ./root/MakeVarList.C");
  gROOT->ProcessLine(".L ./root/Merit.C");
  gROOT->ProcessLine(".L ./root/OutputSearch.C");
  gROOT->ProcessLine(".L ./root/PhaseSpace.C");
  gROOT->ProcessLine(".L ./root/PIDTable.C");
  gROOT->ProcessLine(".L ./root/PlotCorrVar.C");
  gROOT->ProcessLine(".L ./root/PlotUsedVars.C");
  gROOT->ProcessLine(".L ./root/PlotVar.C");
  gROOT->ProcessLine(".L ./root/RenameBranch.C");
  gROOT->ProcessLine(".L ./root/ROC_Area.C");
  gROOT->ProcessLine(".L ./root/sPlot.C");
  gROOT->ProcessLine(".L ./root/TISTOS.C");
  gROOT->ProcessLine(".L ./root/VarFit.C");
  gROOT->ProcessLine(".L ./root/Vodka.C+");
}
