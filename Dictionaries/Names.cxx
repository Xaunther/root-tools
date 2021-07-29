#include <string>
#include <iostream>
#include <vector>
#include "Dictionaries/Names.h"

void Names::FillDefault()
{
  //Workspace components
  workspace = "Fit WS";
  dataset = "Data";
  pdfmodel[0] = "Fit";
  for (int i = 1; i < TAM; i++)
  {
    pdfmodel[i] = "Fit" + std::to_string(i);
  }
  //PDFs
  comppdf[0] = "Signal";
  for (int i = 1; i < TAM; i++)
  {
    comppdf[i] = "bkg" + std::to_string(i);
  }
  //Gaussian parameters
  for (int i = 0; i < TAM; i++)
  {
    mean[i] = "#mu_{" + std::to_string(i + 1) + "}";
  }
  for (int i = 0; i < TAM; i++)
  {
    width[i] = "#sigma_{" + std::to_string(i + 1) + "}";
  }
  //CB parameters
  alpha = "#alpha";
  for (int i = 0; i < TAM; i++)
  {
    alphaL[i] = "#alpha_{L}^{" + std::to_string(i + 1) + "}";
  }
  for (int i = 0; i < TAM; i++)
  {
    alphaR[i] = "#alpha_{R}^{" + std::to_string(i + 1) + "}";
  }
  n = "#n";
  for (int i = 0; i < TAM; i++)
  {
    nL[i] = "#n_{L}^{" + std::to_string(i + 1) + "}";
  }
  for (int i = 0; i < TAM; i++)
  {
    nR[i] = "#n_{R}^{" + std::to_string(i + 1) + "}";
  }
  //Linear parameters
  slope = "slope";
  //Argus parameters
  m0_Argus = "m_{0}";
  c_Argus = "c";
  p_Argus = "p";
  width_Argus = "#sigma_{Argus}";
  //Exponential parameters
  exp_par[0] = "#tau";
  for (int i = 1; i < TAM; i++)
  {
    exp_par[i] = "#tau_{" + std::to_string(i) + "}";
  }
  //Components fractions
  fcomp[0] = "n_{S}";
  for (int i = 1; i < TAM; i++)
  {
    fcomp[i] = "n_{B}^{" + std::to_string(i) + "}";
  }
  //Number of bkgs
  N_comps = "N_comps";
  //Mean shifts
  shift[0] = "shift_{comb}";
  shift[1] = "shift_{K*#gamma}";
  shift[2] = "shift_{#rho#gamma}";
  shift[3] = "shift_{#Lambda(1520)#gamma}";
  shift[4] = "shift_{#Lambda(1670)#gamma}";
  shift[5] = "shift_{#Lambda(1820)#gamma}";
  shift[6] = "shift_{#Lambda(1830)#gamma}";
  for (int i = 7; i < TAM; i++)
  {
    exp_par[i] = "shift_{B}";
  }
  //Yields
  for (int i = 0; i < TAM; i++)
  {
    yield[i] = "yield_{" + std::to_string(i + 1) + "}";
  }
  yield_factor = "yield0";
  //RooYield Interfix
  RooYield_suffix = "";
  //RooCategory name
  catname = "cats";
}
Names::Names(std::string ananame)
{
  this->FillDefault();
  this->Init(ananame);
}
void Names::Init(std::string ananame)
{
  if (ananame == "NstGamma")
  {
    //Name of each component
    comppdf[0] = "Signal";
    comppdf[1] = "Combinatorial";
    comppdf[2] = "K#pi#gamma";
    comppdf[3] = "pK#gamma";
    //Signal
    mean[0] = "#mu_{S}";
    width[0] = "#sigma_{S}";
    fcomp[0] = "n_{S}";
    //Combinatorial (Exponential)
    exp_par[1] = "#tau_{comb}";
    fcomp[1] = "n_{comb}";
    //KpiG
    mean[2] = "#mu_{K#pi#gamma}";
    width[2] = "#sigma_{K#pi#gamma}";
    alphaL[2] = "#alpha_{L}^{K#pi#gamma}";
    alphaR[2] = "#alpha_{R}^{K#pi#gamma}";
    nL[2] = "n_{L}^{K#pi#gamma}";
    fcomp[2] = "n_{K#pi#gamma}";
    //pKG
    mean[3] = "#mu_{pK#gamma}";
    width[3] = "#sigma_{pK#gamma}";
    alphaL[3] = "#alpha_{L}^{pK#gamma}";
    alphaR[3] = "#alpha_{R}^{pK#gamma}";
    nL[3] = "n_{L}^{pK#gamma}";
    fcomp[3] = "n_{pK#gamma}";
  }
  else if (ananame == "NstGamma_Simult")
  {
    std::vector<std::string> masses = {"ppiG", "KpiG", "pKG"};
    std::vector<std::string> components = {"KpiGMC", "pKGMC", "ppiGMC", "KpiGRefMC", "pKGRefMC", "KpipiGMC", "Line", "exp"};
    std::vector<std::string> latex_masses = {"p#pi#gamma", "K#pi#gamma", "pK#gamma"};
    std::vector<std::string> latex_components = {"K#pi#gamma", "pK#gamma", "p#pi#gamma", "K#pi#gamma Ref", "pK#gamma Ref", "K#pi#pi#gamma", "K#pi#pi^{0}X", "Combinatorial"};
    pdfmodel[0] = "Fit m_{p#pi^{-}#gamma}";
    pdfmodel[1] = "Fit m_{K^{+}#pi^{-}#gamma}";
    pdfmodel[2] = "Fit m_{pK^{-}#gamma}";
    pdfmodel[3] = "Simultaneous Fit";
    for (unsigned int i = 0; i < masses.size(); i++)
    {
      for (unsigned int j = 0; j < components.size(); j++)
      {
        comppdf[i * components.size() + j] = latex_components[j] + "(m_{" + latex_masses[i] + "})";
        fcomp[i * components.size() + j] = "f_" + masses[i] + "_mass_" + components[j];
        mean[i * components.size() + j] = "mean_" + masses[i] + "_mass_" + components[j];
        width[i * components.size() + j] = "width_" + masses[i] + "_mass_" + components[j];
        alphaL[i * components.size() + j] = "alphaL_" + masses[i] + "_mass_" + components[j];
        alphaR[i * components.size() + j] = "alphaR_" + masses[i] + "_mass_" + components[j];
        nL[i * components.size() + j] = "nL_" + masses[i] + "_mass_" + components[j];
        nR[i * components.size() + j] = "nR_" + masses[i] + "_mass_" + components[j];
        exp_par[i * components.size() + j] = "tau_" + masses[i] + "_mass_" + components[j];
      }
    }
  }
  else if (ananame == "NstGamma_Simult_log")
  {
    this->Init("NstGamma_Simult");
  }
  else if (ananame == "NstGamma_Simult_MC")
  {
    this->Init("NstGamma_Simult");
  }
  else if (ananame == "NstGamma_Simult_ppiG_KpiGRef")
  {
    this->Init("NstGamma_Simult");
  }
  else if (ananame == "NstGamma_Simult_ppiG_pKGRef")
  {
    this->Init("NstGamma_Simult");
  }
  else if (ananame == "NstGamma_Simult_KpiG_pKG")
  {
    this->Init("NstGamma_Simult");
  }
  else if (ananame == "NstGamma_minmu_ppiG_KpiGRef")
  {
    this->Init("NstGamma_minmu");
  }
  else if (ananame == "NstGamma_minmu_ppiG_pKGRef")
  {
    this->Init("NstGamma_minmu");
  }
  else if (ananame == "NstGamma_minmu_KpiG_pKG")
  {
    this->Init("NstGamma_minmu");
  }
  else if (ananame == "NstGamma_maxmu_ppiG_KpiGRef")
  {
    this->Init("NstGamma_maxmu");
  }
  else if (ananame == "NstGamma_maxmu_ppiG_pKGRef")
  {
    this->Init("NstGamma_maxmu");
  }
  else if (ananame == "NstGamma_maxmu_KpiG_pKG")
  {
    this->Init("NstGamma_maxmu");
  }
  else if (ananame == "KstplusGamma")
  {
  }
  else if (ananame == "Upgrade_trigger")
  {
  }
  else if (ananame == "NstG_KpiG")
  {
    //Name of each component
    comppdf[0] = "K#pi#gamma";
    comppdf[1] = "K#pi#pi^{0}X";
    comppdf[2] = "Combinatorial";
    comppdf[3] = "K#pi#pi#gamma";
    //Double CB signal (K#pi#gamma)
    mean[0] = "#mu_{K#pi#gamma}";
    width[0] = "#sigma_{K#pi#gamma}";
    alphaL[0] = "#alpha_{L}^{K#pi#gamma}";
    alphaR[0] = "#alpha_{R}^{K#pi#gamma}";
    nL[0] = "n_{L}^{K#pi#gamma}";
    nR[0] = "n_{R}^{K#pi#gamma}";
    fcomp[0] = "n_{K#pi#gamma}";
    //K#pi#pi^{0}X
    exp_par[1] = "#tau_{K#pi#pi^{0}X}";
    fcomp[1] = "n_{K#pi#pi^{0}X}";
    //Combinatorial
    slope = "slope_{comb}";
    fcomp[2] = "n_{comb}";
    //K#pi#pi#gamma
    mean[3] = "#mu_{K#pi#pi#gamma}";
    width[3] = "#sigma_{K#pi#pi#gamma}";
    alphaL[3] = "#alpha_{L}^{K#pi#pi#gamma}";
    nL[3] = "n_{L}^{K#pi#pi#gamma}";
    alphaR[3] = "#alpha_{R}^{K#pi#pi#gamma}";
    nR[3] = "n_{R}^{K#pi#pi#gamma}";
    fcomp[3] = "n_{K#pi#pi#gamma}";
  }
  else if (ananame == "NstG_KpiG_BDT")
  {
    this->Init("NstG_KpiG");
  }
  else if (ananame == "NstG_KpiG_Wrong")
  {
    this->Init("NstG_KpiG");
  }
  else if (ananame == "NstG_KpiG_Wrong_log")
  {
    this->Init("NstG_KpiG_Wrong");
  }
  else if (ananame == "NstG_KpiG_Wrong_MC")
  {
    this->Init("NstG_KpiG");
  }
  else if (ananame == "NstG_pKG")
  {
    //Name of each component
    comppdf[0] = "pK#gamma";
    comppdf[1] = "Combinatorial";

    //CBExp for signal
    mean[0] = "#mu_{pK#gamma}";
    width[0] = "#sigma_{pK#gamma}";
    alphaL[0] = "#alpha_{L}^{pK#gamma}";
    alphaR[0] = "#alpha_{R}^{pK#gamma}";
    nL[0] = "n_{L}^{pK#gamma}";
    fcomp[0] = "n_{pK#gamma}";
    //Combinatorial
    exp_par[1] = "#tau_{comb}";
    fcomp[1] = "n_{comb}";
  }
  else if (ananame == "NstG_pKG_BDT")
  {
    this->Init("NstG_pKG");
  }
  else if (ananame == "NstG_pKG_Wrong")
  {
    this->Init("NstG_pKG");
  }
  else if (ananame == "NstG_pKG_Wrong_log")
  {
    this->Init("NstG_pKG_Wrong");
  }
  else if (ananame == "NstG_pKG_Wrong_MC")
  {
    this->Init("NstG_pKG");
  }
  else if (ananame == "NstG_ppiG_log")
  {
    this->Init("NstGamma");
  }
  else if (ananame == "NstG_KpiG_log")
  {
    this->Init("NstG_KpiG");
  }
  else if (ananame == "NstG_pKG_log")
  {
    this->Init("NstG_pKG");
  }
  //MC variants. If any names need to be changed...
  else if (ananame == "NstGamma_MC")
  {
    this->Init("NstGamma");
  }
  else if (ananame == "NstG_KpiG_MC")
  {
    this->Init("NstG_KpiG");
  }
  else if (ananame == "NstG_KpiG_MC2")
  {
    this->Init("NstG_KpiG");
    comppdf[0] = "K#pi#pi#gamma";
  }
  else if (ananame == "NstG_pKG_MC")
  {
    this->Init("NstG_pKG");
  }
  else if (ananame == "NstGamma_minmu")
  {
    this->Init("NstGamma_Simult");
    RooYield_suffix = "_minmu";
  }
  else if (ananame == "NstGamma_minmu_MC")
  {
    this->Init("NstGamma_minmu");
  }
  else if (ananame == "NstGamma_maxmu")
  {
    this->Init("NstGamma_Simult");
    RooYield_suffix = "_maxmu";
  }
  else if (ananame == "NstGamma_maxmu_MC")
  {
    this->Init("NstGamma_maxmu");
  }
  else if (ananame == "NstGamma_minmu_log")
  {
    this->Init("NstGamma_minmu");
  }
  else if (ananame == "NstGamma_maxmu_log")
  {
    this->Init("NstGamma_maxmu");
  }
  else if (ananame == "NstGamma_minsigma")
  {
    this->Init("NstGamma_Simult");
    RooYield_suffix = "_minsigma";
  }
  else if (ananame == "NstGamma_minsigma_MC")
  {
    this->Init("NstGamma_minsigma");
  }
  else if (ananame == "NstGamma_maxsigma")
  {
    this->Init("NstGamma_Simult");
    RooYield_suffix = "_maxsigma";
  }
  else if (ananame == "NstGamma_maxsigma_MC")
  {
    this->Init("NstGamma_maxsigma");
  }
  else if (ananame == "NstGamma_minsigma_log")
  {
    this->Init("NstGamma_minsigma");
  }
  else if (ananame == "NstGamma_maxsigma_log")
  {
    this->Init("NstGamma_maxsigma");
  }
  else if (ananame == "NstG_ppimumu")
  {
    comppdf[1] = "Combinatorial";
  }
  else if (ananame == "NstG_ppimumu_log")
  {
    this->Init("NstG_ppimumu");
  }
  else if (ananame == "NstG_ppimumu_sPlot")
  {
    this->Init("NstG_ppimumu");
  }
  else if (ananame == "NstG_pKmumu")
  {
    comppdf[1] = "Combinatorial";
  }
  else if (ananame == "NstG_pKmumu_log")
  {
    this->Init("NstG_pKmumu");
  }
  else if (ananame == "NstG_pKmumu_sPlot")
  {
    this->Init("NstG_pKmumu");
  }
  else if (ananame == "KstG_Maths")
  {
  }
  else
  {
    std::cout << "WARNING: " << ananame << " config not found in Names.cxx. Loading default values" << std::endl;
  }
}
