#include <string>
#include <sstream>
#include "Dictionaries/Constants.h"
#include "Functions/RunInfo.h"
#include "Functions/Dictreading.h"
using namespace std;

string GetRunCut(RunNumber run_number, string datatype)
{
  //Initialize constants
  Constants const_list(GetValueFor("Project_name", "Dictionaries/Project_variables.txt"));

  const unsigned long* Max_Run_Number; 
  stringstream cut;
  cut.str("");
  //Define pointer to maxrunnumber array depending on datatype requested
  if(datatype=="MC")
    {
      Max_Run_Number = const_list.Max_Run_Number_MC;
    }
  else
    {
      Max_Run_Number = const_list.Max_Run_Number;
    }
  //Get value
  if(run_number==I)
    {
      cut << "runNumber < " << Max_Run_Number[1];
    }
  else if(run_number==II)
    {
      cut << "runNumber > " << Max_Run_Number[1];
    }
  else
    {
      cut << "1";
    }
  return cut.str();
}

RunNumber StringToRunNumber(string strrun)
{
  RunNumber run_number = I;
  if(strrun == "I")
    {
      run_number = I;
    }
  else if(strrun == "II")
    {
      run_number = II;
    }
  else if(strrun == "All")
    {
      run_number = All;
    }
  return run_number;
}
