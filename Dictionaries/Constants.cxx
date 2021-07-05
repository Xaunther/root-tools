#include <string>
#include "Dictionaries/Constants.h"

void Constants::FillDefault()
{
  //Initialize with defaults, and each option will override what's necessary
  //Binning and ranges
  xmin = 4300.0;
  xmax = 6300.0;
  xmin2 = 4000.0;
  xmax2 = 12000.0;
  bins[0] = 60;
  bins[1] = 0; //Zero value make use of bins[0] by default
  bins[2] = 0;
  range = 20000;
  //Fraction yields
  fsig_0 = 0.3;
  fbkg_0 = 1 - fsig_0;
  yield_factor_0 = 0.1;
  yield_factor_min = 0.0001;
  yield_factor_max = 1.;
  //Gaussian parameters
  mean_0 = 5620.0;
  mean_min = xmin;
  mean_max = xmax;
  width_0 = 100.0;
  width_min = 10.0;
  width_max = 200.0;
  //Exponential parameters
  bkgpar_0 = -0.00027;
  bkgpar_min = -0.04;
  bkgpar_max = 0.004;
  m0_Exp_0 = 5600.;
  m0_Exp_min = m0_Exp_0 - 200;
  m0_Exp_max = m0_Exp_0 + 200;
  //CB parameters
  alpha_0 = 2;
  alpha_min = 1;
  alpha_max = 10;
  n_0 = 3;
  n_min = 0.1;
  n_max = 200;
  //Double CB parameters
  alpha1_0 = 2;
  alpha1_min = 0.1;
  alpha1_max = 10;
  alpha2_0 = -2;
  alpha2_min = -10;
  alpha2_max = -0.1;
  n1_0 = 3;
  n1_min = 0.1;
  n1_max = 200;
  n2_0 = 3;
  n2_min = 0.1;
  n2_max = 200;
  //Argus parameters
  m0_Argus_0 = 5139.;
  m0_Argus_min = 5139.;
  m0_Argus_max = 5139.;
  c_Argus_0 = -5.2;
  c_Argus_min = -5.2;
  c_Argus_max = -5.2;
  p_Argus_0 = 0.5;
  p_Argus_min = 0.5;
  p_Argus_max = 0.5;
  width_Argus_0 = 100.;
  width_Argus_min = 100.;
  width_Argus_max = 100.;
  //Linear parameters
  slope_0 = 0;
  slope_min = -100;
  slope_max = 100;
  //Part of the title for signal mass fit
  plot_title = "Run I + 2015 + 2016 (BDT Run I)";
  plot_Xtitle = "M_{#pi#pi#pi#gamma}";
  plot_units = "MeV";
  //Legend titles
  legend_data1 = "pep";
  legend_data2 = "pop";
  //Some variable shift
  shift1 = 0.;
  shift2 = 0.;
  //Fit options
  SumW2Error = false;
  //Plot options
  plotopt = "norm";
  logX = false;
  logY = false;
  Legend = true;
  legend_L = false;
  //Legend coordinates (right-aligned)
  Legend_x0_R = 0.65;
  Legend_xf_R = 0.9;
  Legend_y0 = 0.6;
  Legend_yf = 0.9;
  //Legend coordinates (left-aligned)
  Legend_x0_L = 0.1;
  Legend_xf_L = 0.45;

  //Low and High Mass Sidebands
  xLM = 5000;
  xHM = 5600;

  //Expected events
  N_I = 483.103294757;
  N_II = 975.863316563;
  N_I_MC = 6546.0;
  N_II_MC = 1740.0;

  //max RunNumber per year
  Max_Run_Number[0] = 110E3;
  Max_Run_Number[1] = 133E3;
  Max_Run_Number[2] = 166E3;
  Max_Run_Number_MC[0] = 4000E3;
  Max_Run_Number_MC[1] = 4000E3;
  Max_Run_Number_MC[2] = 6600E3;

  //BDT options
  BDT_Prepare_options = "SplitMode=Random:SplitSeed=100"; //Seed 100 is default
  BDT_Method_options = "NTrees=400:MaxDepth=2:CreateMVAPdfs=True";

  //Working directory. Default will be current one
  workingdir = "./";
}
Constants::Constants(std::string ananame)
{
  this->FillDefault();
  this->Init(ananame);
}
void Constants::Init(std::string ananame)
{
  if (ananame == "NstGamma")
  {
    //Binning and ranges
    xmin = 4600.0;
    xmax = 6600.0;
    xmin2 = 1900.0;
    xmax2 = 2900.0;
    bins[0] = 40;
    fsig_0 = 100.;
    fbkg_0 = 2.53538E3;
    //Fixed bkg contributions
    yield_factor_0 = 1.;
    yield_factor_min = 1.;
    yield_factor_max = 1.;
    //Gaussian parameters
    width_0 = 100.0;
    width_min = 20.0;
    width_max = 200.0;
    //CB parameters
    alpha_0 = 2;
    alpha_min = 0.01;
    alpha_max = 20;
    //Double CB parameters
    alpha1_0 = 2.14;
    alpha1_min = 0.01;
    alpha1_max = 20;
    alpha2_0 = 1.5;
    alpha2_min = 0.01;
    alpha2_max = 20;
    n1_0 = 1.6;
    n2_0 = 1.6;
    //Argus parameters
    m0_Argus_0 = 5374.;
    m0_Argus_min = m0_Argus_0 - 100;
    m0_Argus_max = m0_Argus_0 + 100;
    c_Argus_0 = -11.1;
    c_Argus_min = -20.2;
    c_Argus_max = -1.2;
    p_Argus_0 = 0.5;
    p_Argus_min = 0.5;
    p_Argus_max = 0.5;
    width_Argus_0 = 134.;
    width_Argus_min = 134.;
    width_Argus_max = 134.;
    //Exponential parameters
    bkgpar_0 = -1.50585E-03;
    //Linear parameters
    slope_0 = 0.;
    slope_min = 0.;
    slope_max = 0.;
    //Part of the title for signal mass fit
    plot_title = "p #pi^{-} #gamma combined mass 2011+12";
    //plot_title = "K #pi #gamma combined mass B^{0}->#rho^{0}#gamma MC";
    plot_Xtitle = "m_{p#pi^{-}#gamma}";
    //Fit options
    SumW2Error = true;
    //Plot options
    plotopt = "norm";
    logX = false;
    logY = false;
    Legend = true;
    legend_L = false;
    //Legend coordinates (right-aligned)
    Legend_x0_R = 0.75;
    Legend_xf_R = 0.9;
    Legend_y0 = 0.9;
    Legend_yf = 0.6;
    //Legend coordinates (left-aligned)
    Legend_x0_L = 0.1;
    Legend_xf_L = 0.3;
  }
  else if (ananame == "NstGamma_Simult")
  {
    this->Init("NstGamma");
    //Argus parameters
    m0_Argus_0 = 5034.;
    m0_Argus_min = m0_Argus_0 - 100;
    m0_Argus_max = m0_Argus_0 + 100;
    c_Argus_0 = -11.1;
    c_Argus_min = -20.2;
    c_Argus_max = -1.2;
    p_Argus_0 = 0.5;
    p_Argus_min = 0.5;
    p_Argus_max = 0.5;
    width_Argus_0 = 90.;
    width_Argus_min = 90.;
    width_Argus_max = 90.;
    //Linear parameters
    slope_0 = 3e-02;
    slope_min = slope_0 - 0.1;
    slope_max = slope_0 + 0.1;
    //Bins
    bins[0] = 25;
    bins[1] = 60;
    bins[2] = 50;
    //Legend coordinates (right-aligned)
    Legend_x0_R = 0.7;
    Legend_xf_R = 0.95;
    Legend_y0 = 0.95; //This means auto height (0.065 per legend item)
    Legend_yf = 0.95;
    workingdir = "SimultaneousFit/";
  }
  else if (ananame == "NstGamma_Simult_log")
  {
    this->Init("NstGamma_Simult");
    logY = true;
  }
  else if (ananame == "NstGamma_Simult_MC")
  {
    this->Init("NstGamma_Simult");
    Legend = false;
  }
  else if (ananame == "NstGamma_Simult_ppiG_KpiGRef")
  {
    this->Init("NstGamma_Simult");
    //Gaussian parameters
    mean_0 = 5560.0;
    mean_min = xmin;
    mean_max = xmax;
    width_0 = 150.0;
    width_min = 20.0;
    width_max = 200.0;
    //CB parameters
    alpha_0 = 2;
    alpha_min = 0.01;
    alpha_max = 20;
    n_0 = 1;
    n_min = 0.1;
    n_max = 200;
    //Double CB parameters
    alpha1_0 = 2.6;
    alpha1_min = 0.01;
    alpha1_max = 20;
    alpha2_0 = 1.0;
    alpha2_min = 0.01;
    alpha2_max = 20;
    n1_0 = 0.8;
    n1_min = 0.1;
    n1_max = 200;
    n2_0 = 50.0;
    n2_min = 0.1;
    n2_max = 100;
  }
  else if (ananame == "NstGamma_Simult_ppiG_pKGRef")
  {
    this->Init("NstGamma_Simult");
    //Gaussian parameters
    mean_0 = 5560.0;
    mean_min = xmin;
    mean_max = xmax;
    width_0 = 150.0;
    width_min = 20.0;
    width_max = 200.0;
    //CB parameters
    alpha_0 = 1;
    alpha_min = 0.01;
    alpha_max = 20;
    n_0 = 4;
    n_min = 0.1;
    n_max = 200;
    //Double CB parameters
    alpha1_0 = 1.5;
    alpha1_min = 0.01;
    alpha1_max = 20;
    alpha2_0 = 1.0;
    alpha2_min = 0.01;
    alpha2_max = 20;
  }
  else if (ananame == "NstGamma_Simult_KpiG_pKG")
  {
    this->Init("NstGamma_Simult");
    //Gaussian parameters
    mean_0 = 5300.0;
    mean_min = xmin;
    mean_max = xmax;
    width_0 = 115.0;
    width_min = 50.0;
    width_max = 200.0;
    //CB parameters
    alpha_0 = 0.7;
    alpha_min = 0.01;
    alpha_max = 20;
    //Double CB parameters
    alpha1_0 = 0.7;
    alpha1_min = 0.01;
    alpha1_max = 20;
    alpha2_0 = 1.4;
    alpha2_min = 0.01;
    alpha2_max = 20;
  }
  else if (ananame == "NstGamma_minmu_ppiG_KpiGRef")
  {
    this->Init("NstGamma_Simult_ppiG_KpiGRef");
  }
  else if (ananame == "NstGamma_minmu_ppiG_pKGRef")
  {
    this->Init("NstGamma_Simult_ppiG_KpiGRef");
  }
  else if (ananame == "NstGamma_minmu_KpiG_pKG")
  {
    this->Init("NstGamma_Simult_KpiG_pKG");
  }
  else if (ananame == "NstGamma_maxmu_ppiG_KpiGRef")
  {
    this->Init("NstGamma_Simult_ppiG_KpiGRef");
  }
  else if (ananame == "NstGamma_maxmu_ppiG_pKGRef")
  {
    this->Init("NstGamma_Simult_ppiG_KpiGRef");
  }
  else if (ananame == "NstGamma_maxmu_KpiG_pKG")
  {
    this->Init("NstGamma_Simult_KpiG_pKG");
  }
  else if (ananame == "KstplusGamma")
  {
    //Double CB parameters
    alpha1_0 = 2.24;
    alpha1_min = alpha1_0;
    alpha1_max = alpha1_0;
    alpha2_0 = -2.03;
    alpha2_min = alpha2_0;
    alpha2_max = alpha2_0;
    n1_0 = 0.71;
    n1_min = n1_0;
    n1_max = n1_0;
    n2_0 = 3.1;
    n2_min = n2_0;
    n2_max = n2_0;
  }
  else if (ananame == "Upgrade_trigger")
  {
    //Part of the title for signal mass fit
    plot_title = "HHgamma";
    //Low and High Mass Sidebands
    xLM = 0;
    xHM = 0;
  }
  else if (ananame == "NstG_KpiG")
  {
    this->Init("NstGamma");
    bins[0] = 60;
    //Argus parameters
    m0_Argus_0 = 5034.;
    m0_Argus_min = m0_Argus_0 - 100;
    m0_Argus_max = m0_Argus_0 + 100;
    c_Argus_0 = -11.1;
    c_Argus_min = -20.2;
    c_Argus_max = -1.2;
    p_Argus_0 = 0.5;
    p_Argus_min = 0.5;
    p_Argus_max = 0.5;
    width_Argus_0 = 90.;
    width_Argus_min = 90.;
    width_Argus_max = 90.;
    //Exponential parameters
    bkgpar_0 = -2.88678e-03;
    bkgpar_min = -0.04;
    bkgpar_max = 0.004;
    //Linear parameters
    slope_0 = 3e-02;
    slope_min = slope_0 - 0.1;
    slope_max = slope_0 + 0.1;
    //Yield factor
    yield_factor_0 = 0.1;
    yield_factor_min = 0.1;
    yield_factor_max = 0.1;
    mean_0 = 5228.0;
    mean_min = mean_0 - 600.;
    mean_max = mean_0 + 600.;
    width_0 = 113.0;
    width_min = width_0 - 50.;
    width_max = width_0 + 800.;
    plot_title = "K^{+} #pi^{-} #gamma combined mass 2011+12";
    plot_Xtitle = "m_{K^{+}#pi^{-}#gamma}";
  }
  else if (ananame == "NstG_KpiG_Wrong")
  {
    this->Init("NstG_KpiG");
    mean_0 = 5450.0;
    mean_min = mean_0 - 600.;
    mean_max = mean_0 + 600.;
    fsig_0 = 0.3;
    fbkg_0 = 0.7;
    //Double CB parameters
    alpha1_0 = 0.5;
    alpha1_min = 0.01;
    alpha1_max = 20;
    alpha2_0 = 0.5;
    alpha2_min = 0.01;
    alpha2_max = 20;
    plot_title = "p #pi^{-} #gamma combined mass 2011+12";
    plot_Xtitle = "m_{p#pi^{-}#gamma}";
  }
  else if (ananame == "NstG_KpiG_Wrong_log")
  {
    this->Init("NstG_KpiG_Wrong");
    logY = true;
  }
  else if (ananame == "NstG_KpiG_Wrong_MC")
  {
    this->Init("NstG_KpiG_Wrong");
    SumW2Error = true;
    bins[0] = 100;
  }
  else if (ananame == "NstG_pKG")
  {
    this->Init("NstGamma");
    bins[0] = 60;
    plot_title = "p K^{-} #gamma combined mass 2011+12";
    plot_Xtitle = "m_{pK^{-}#gamma}";
    //Yield factor
    yield_factor_0 = 0.1;
    yield_factor_min = 0.0001;
    yield_factor_max = 1.;
    //Exponential parameters
    bkgpar_0 = -2.88678e-03;
    bkgpar_min = -0.04;
    bkgpar_max = 0.004;
    //CB parameters
    alpha1_0 = 2.14;
    alpha1_min = 0.01;
    alpha1_max = 20;
    alpha2_0 = 2.5;
    alpha2_min = 0.01;
    alpha2_max = 20;
    //Argus parameters
    m0_Argus_0 = 5480.;
    m0_Argus_min = 5480.;
    m0_Argus_max = 5480.;
    c_Argus_0 = -9.41665;
    c_Argus_min = -9.41665;
    c_Argus_max = -9.41665;
    //Gaussian parameters
    mean_0 = 5620.0;
    mean_min = mean_0 - 200.;
    mean_max = mean_0 + 200.;
    width_0 = 100.0;
    width_min = width_0 - 50.;
    width_max = width_0 + 100.;
    n_0 = 1;
    n_min = 0.1;
    n_max = 20;
    //Linear parameters
    slope_0 = 3e-02;
    slope_min = slope_0 - 0.1;
    slope_max = slope_0 + 0.1;
  }
  else if (ananame == "NstG_pKG_Wrong")
  {
    this->Init("NstG_pKG");
    mean_0 = 5450.0;
    fsig_0 = 0.3;
    fbkg_0 = 0.7;
    plot_title = "p #pi^{-} #gamma combined mass 2011+12";
    plot_Xtitle = "m_{p#pi^{-}#gamma}";
  }
  else if (ananame == "NstG_pKG_Wrong_MC")
  {
    this->Init("NstG_pKG_Wrong");
    SumW2Error = true;
    bins[0] = 100;
  }
  else if (ananame == "NstG_pKG_Wrong_log")
  {
    this->Init("NstG_pKG_Wrong");
    logY = true;
  }
  else if (ananame == "NstG_pipiG")
  {
    this->Init("NstGamma");
    fsig_0 = 1. / 50.;
    fbkg_0 = 1 - fsig_0;
    mean_0 = 5180.0;
  }
  else if (ananame == "NstG_ppiG_log")
  {
    this->Init("NstGamma");
    logY = true;
  }
  else if (ananame == "NstG_KpiG_log")
  {
    this->Init("NstG_KpiG");
    logY = true;
  }
  else if (ananame == "NstG_pKG_log")
  {
    this->Init("NstG_pKG");
    logY = true;
  }
  else if (ananame == "NstGamma_MC") //ppiG for MC (if any settings should be changed
  {
    this->Init("NstGamma");
    SumW2Error = true;
    bins[0] = 100;
  }
  else if (ananame == "NstG_KpiG_MC") //KpiG for MC (if any settings should be changed
  {
    this->Init("NstG_KpiG");
    SumW2Error = true;
    bins[0] = 100;
  }
  else if (ananame == "NstG_KpiG_MC2") //KpiG for MC part reco (if any settings should be changed
  {
    this->Init("NstG_KpiG_MC");
  }
  else if (ananame == "NstG_pKG_MC") //pKG for MC (if any settings should be changed
  {
    this->Init("NstG_pKG");
    SumW2Error = false;
    bins[0] = 100;
  }
  else if (ananame == "NstGamma_minmu")
  {
    this->Init("NstGamma_Simult");
    shift1 = -1.;
    shift2 = -1.;
    workingdir = "Systematics/Fit/minmu/";
  }
  else if (ananame == "NstGamma_maxmu")
  {
    this->Init("NstGamma_Simult");
    shift1 = -shift1;
    shift2 = -shift2;
    workingdir = "Systematics/Fit/maxmu/";
  }
  else if (ananame == "NstGamma_minmu_MC")
  {
    this->Init("NstGamma_minmu");
  }
  else if (ananame == "NstGamma_maxmu_MC")
  {
    this->Init("NstGamma_maxmu");
  }
  else if (ananame == "NstGamma_minmu_log")
  {
    this->Init("NstGamma_minmu");
    logY = true;
  }
  else if (ananame == "NstGamma_maxmu_log")
  {
    this->Init("NstGamma_maxmu");
    logY = true;
  }
  else if (ananame == "NstG_ppimumu") //From Lb2ppimumu analysis
  {
    //Initialize with defaults, and each option will override what's necessary
    //Binning and ranges
    xmin = 5350.0;
    xmax = 6000.0;
    xmin2 = 1120.0;
    xmax2 = 2600.0;
    bins[0] = 50;
    //Fraction yields
    fsig_0 = 1. / 2.;
    fbkg_0 = 1 - fsig_0;
    yield_factor_0 = 0.1;
    yield_factor_min = 0.0001;
    yield_factor_max = 1.;
    //Gaussian parameters
    mean_0 = 5620.0;
    mean_min = xmin;
    mean_max = xmax;
    width_0 = 40.0;
    width_min = 1.0;
    width_max = 100.0;
    //Part of the title for signal mass fit
    plot_title = "Run I p#pi^{-}#mu^{+}#mu^{-}";
    plot_Xtitle = "M_{p#pi#mu#mu}";
  }
  else if (ananame == "NstG_ppimumu_log")
  {
    this->Init("NstG_ppimumu");
    logY = true;
    Legend_y0 = 0.1;
    Legend_yf = 0.4;
  }
  else if (ananame == "NstG_ppimumu_sPlot")
  {
    this->Init("NstG_ppimumu");
    bins[0] = 30;
    //Double CB parameters
    alpha1_0 = 1.75;
    alpha1_min = alpha1_0;
    alpha1_max = alpha1_0;
    alpha2_0 = -1.93;
    alpha2_min = alpha2_0;
    alpha2_max = alpha2_0;
    n1_0 = 4.63;
    n1_min = n1_0;
    n1_max = n1_0;
    n2_0 = 5.65433;
    n2_min = n2_0;
    n2_max = n2_0;
  }
  else if (ananame == "NstG_pKmumu") //From Lb2pKmumu analysis
  {
    //Initialize with defaults, and each option will override what's necessary
    //Binning and ranges
    xmin = 5520.0;
    xmax = 5750.0;
    xmin2 = 1.43;
    xmax2 = 2.6;
    bins[0] = 100;
    //Fraction yields
    fsig_0 = 0.8;
    fbkg_0 = 1 - fsig_0;
    yield_factor_0 = 0.1;
    yield_factor_min = 0.0001;
    yield_factor_max = 1.;
    //Gaussian parameters
    mean_0 = 5620.0;
    mean_min = xmin;
    mean_max = xmax;
    width_0 = 10.0;
    width_min = 1.0;
    width_max = 100.0;
    //Crystal-Ball parameters
    alpha1_0 = 1.;
    alpha1_min = 0.1;
    alpha1_max = 10.;
    alpha2_0 = 1.;
    alpha2_min = 0.1;
    alpha2_max = 10.;
    //Part of the title for signal mass fit
    plot_title = "Run I pK^{-}#mu^{+}#mu^{-}";
    plot_Xtitle = "M_{pK#mu#mu}";
  }
  else if (ananame == "NstG_pKmumu_log")
  {
    this->Init("NstG_pKmumu");
    logY = true;
    Legend_y0 = 0.1;
    Legend_yf = 0.4;
  }
  else if (ananame == "NstG_pKmumu_sPlot")
  {
    this->Init("NstG_pKmumu");
    bins[0] = 100;
  }
  else if (ananame == "KstG_Maths")
  {
    //BDT options
    BDT_Prepare_options = "SplitMode=Random:SplitSeed=100"; //Seed 100 is default
    BDT_Method_options = "NTrees=400:MaxDepth=2:CreateMVAPdfs=True";
  }
}
