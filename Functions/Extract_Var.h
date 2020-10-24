#ifndef Extract_Var_h
#define Extract_Var_h

//Function to extract variable from one workspace and add it to another, with maybe a different name
#include <string>
#include "RooWorkspace.h"
using namespace std;
void Extract_Var(RooWorkspace*, RooWorkspace*, string, string);

#endif
