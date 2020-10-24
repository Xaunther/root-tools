//Declaration of particle info-related functions

#include <string>
#include <stdlib.h>
#include "Functions/ParticleInfo.h"
#include "Functions/Dictreading.h"

using namespace std;

int GetNPart(string filedir)
{
  int N_Part;
  string N_Part_string;
  N_Part_string = StringSearch("Dictionaries/treenames.dic", filedir, 0, 2);
  N_Part = atoi(N_Part_string.c_str());
  return N_Part;
}

int GetNPartPlot(string varname)
{
  int N_Part;
  N_Part = (varname.substr(varname.find("_")+2,varname.find("_", varname.find("_")+1)-2-varname.find("_"))).length();
  return N_Part;
}

string* GetParticleList(string variablename, int N_vars, int N_vars_plot)
{
  string* particles = new string[N_vars_plot];
  if(N_vars==3)
    {
      if(N_vars_plot==3)
	{
	  particles[2] = "#gamma";
        }
      if(variablename.find("Subst") == string::npos) //Kpi
	{
	  particles[0] = "K";
	  particles[1] = "#pi";
	}
      else if(variablename.find("Subst1_pi2K") != string::npos) //KK
	{
	  particles[0] = "K";
	  particles[1] = "K";
	}
      else if(variablename.find("Subst1_pi2p") != string::npos) //Kp
	{
	  particles[0] = "K";
	  particles[1] = "p";
	}
      else if(variablename.find("Subst01_Kpi2pK") != string::npos) //pK
	{
	  particles[0] = "p";
	  particles[1] = "K";
	}
      else if(variablename.find("Subst01_Kpi2pp~") != string::npos) //pp
	{
	  particles[0] = "p";
	  particles[1] = "p";
	}
      else if(variablename.find("Subst01_Kpi2piK") != string::npos) //piK
	{
	  particles[0] = "#pi";
	  particles[1] = "K";
	}
      else if(variablename.find("Subst01_Kpi2pip~") != string::npos) //pip
	{
	  particles[0] = "#pi";
	  particles[1] = "p";
	}
      else if(variablename.find("Subst0_K2pi") != string::npos) //pipi
	{
	  particles[0] = "#pi";
	  particles[1] = "#pi";
	}
      else if(variablename.find("Subst0_K2p") != string::npos) //ppi
	{
	  particles[0] = "p";
	  particles[1] = "#pi";
	}
    }
  else if(N_vars==4)
    {
      particles[3] = "#gamma";
      if(variablename.find("Subst") == string::npos) //Kpi
	{
	  particles[0] = "#pi";
	  particles[1] = "(#pi";
	  particles[2] = "#pi)";
	}
      else //I might need more later, but so far it's fine
	{
	  particles[0] = "";
	  particles[1] = "";
	  particles[2] = "";
	}

    }
  return particles;
}
