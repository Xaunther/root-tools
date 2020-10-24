#include <iostream>
#include "Functions/ProgressBar.h"
using namespace std;

//Small script to draw a progress bar :)
void DrawProgress(double progress)
{
  const int barWidth = 70;
  cout << "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos) cout << "=";
    else if (i == pos) cout << ">";
    else cout << " ";
  }
  cout << "] " << int(progress * 100.0) << " %\r";
  cout.flush();
}
