void rootinit()
{
  //Utilities from independent sources
  gROOT->ProcessLine(".L ./Functions/BifurcatedCB.cxx+");
  //Miscellaneous functions, must be loaded in top of all personal functions
  gROOT->ProcessLine(".L ./Functions/misc.cxx+");  
  //Personal codes
  gROOT->ProcessLine(".L ./Functions/Fits.cxx+");
}
