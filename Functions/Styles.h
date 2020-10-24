//Header file for styles namespace
//Place to add functions that add styles or whatever (example, add kBird)

#ifndef Styles_h
#define Styles_h

#include <string>
//Define a namespace so there's no confusion around.
namespace MyStyles
{
  enum palette{kBird = 0, kRainBow = 1};

  void SetPalette(palette, double alpha = 1);
  palette StringToPalette(std::string);
}

#endif
