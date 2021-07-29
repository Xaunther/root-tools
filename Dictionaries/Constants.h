//File with constants declaration.
//When included in a file, you can use them directly

#ifndef Constants_h
#define Constants_h

#include <string>
using namespace std;

class Constants
{
public:
  Constants(string ananame = ""); //Default behaviour for other tasks
  void FillDefault();
  void Init(string ananame = "");
  //Binning and ranges
  double xmin;
  double xmax;
  double xmin2;
  double xmax2;
  int bins[3];
  double range;
  //Fraction yields
  double fsig_0;
  double fbkg_0;
  double yield_factor_0;
  double yield_factor_min;
  double yield_factor_max;
  //Gaussian parameters
  double mean_0;
  double mean_min;
  double mean_max;
  double width_0;
  double width_min;
  double width_max;
  //Exponential parameters
  double bkgpar_0;
  double bkgpar_min;
  double bkgpar_max;
  double m0_Exp_0;
  double m0_Exp_min;
  double m0_Exp_max;
  //CB parameters
  double alpha_0;
  double alpha_min;
  double alpha_max;
  double n_0;
  double n_min;
  double n_max;
  //Double CB parameters
  double alpha1_0;
  double alpha1_min;
  double alpha1_max;
  double alpha2_0;
  double alpha2_min;
  double alpha2_max;
  double n1_0;
  double n1_min;
  double n1_max;
  double n2_0;
  double n2_min;
  double n2_max;
  //Argus parameters
  double m0_Argus_0;
  double m0_Argus_min;
  double m0_Argus_max;
  double c_Argus_0;
  double c_Argus_min;
  double c_Argus_max;
  double p_Argus_0;
  double p_Argus_min;
  double p_Argus_max;
  double width_Argus_0;
  double width_Argus_min;
  double width_Argus_max;
  //Linear parameters
  double slope_0;
  double slope_min;
  double slope_max;
  //Part of the title for signal mass fit
  string plot_title;
  string plot_Xtitle;
  string plot_units;
  //Legend titles
  string legend_data1;
  string legend_data2;

  //Some variable shifts
  double shift1;
  double shift2;
  double widthshift1;
  double widthshift2;

  //Fit options
  bool SumW2Error;
  //Plot options
  string plotopt;
  bool logX;
  bool logY;
  bool Legend;
  bool legend_L;
  //Legend coordinates (right-aligned)
  double Legend_x0_R;
  double Legend_xf_R;
  double Legend_y0;
  double Legend_yf;
  //Legend coordinates (left-aligned)
  double Legend_x0_L;
  double Legend_xf_L;
  //Low and High Mass Sidebands
  double xLM;
  double xHM;
  //Expected events
  double N_I;
  double N_II;
  double N_I_MC;
  double N_II_MC;
  //max RunNumber per year
  unsigned long Max_Run_Number[3];
  unsigned long Max_Run_Number_MC[3];
  //BDT options
  string BDT_Prepare_options;
  string BDT_Method_options;
  //Working directory
  string workingdir;
};

#endif
