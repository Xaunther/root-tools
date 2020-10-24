#include "TColor.h"
#include <iostream>
#include <string>
#include "Functions/Styles.h"

namespace MyStyles
{
  void SetPalette(palette option, double alpha)
  {
    Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
    switch ((int)option)
      {
      case (0): //kBird
	{
	  Double_t red[9]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
	  Double_t green[9] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
	  Double_t blue[9] = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
	  TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
	}
	break;
      case (1): //kRainBow
	{
	  Double_t red[9]   = {  0./255.,   5./255.,  15./255.,  35./255., 102./255., 196./255., 208./255., 199./255., 110./255.};
	  Double_t green[9] = {  0./255.,  48./255., 124./255., 192./255., 206./255., 226./255.,  97./255.,  16./255.,   0./255.};
	  Double_t blue[9]  = { 99./255., 142./255., 198./255., 201./255.,  90./255.,  22./255.,  13./255.,   8./255.,   2./255.};
	  TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
	}
	break;
      default:
	std::cout << "Error: Unkown palette" << std::endl;
	break;
      }
    return;
  }

  palette StringToPalette(std::string strpalette)
  {
    palette option = kBird;
    if(strpalette == "kBird")
      {
	option = kBird;
      }
    else if(strpalette == "kRainBow")
      {
	option = kRainBow;
      }
    return option;
  }
}
