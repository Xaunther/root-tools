#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Functions/TUncertainty.h"

//Overloaded operators
//SUM
TUncertainty operator+(const double& u1, const TUncertainty& u2)
{
	TUncertainty _u1(u1);
	return _u1 + u2;
}
TUncertainty operator+(const TUncertainty& u1, const double& u2)
{
	TUncertainty _u2(u2);
	return u1 + _u2;
}
TUncertainty operator+(const TUncertainty& u1, const TUncertainty& u2)
{
	TUncertainty result;
	result.value = u1.value + u2.value; //Add values
	for (unsigned int i = 0; i < std::max(u1.uncertainty.size(), u2.uncertainty.size()); i++) //Add errors in quadrature. Careful with sizes
		if (i < std::min(u1.uncertainty.size(), u2.uncertainty.size()))
		{
			result.uncertainty.push_back(sqrt(u1.uncertainty[i]*u1.uncertainty[i] + u2.uncertainty[i]*u2.uncertainty[i]));
		}
		else if (i < u2.uncertainty.size())
		{
			result.uncertainty.push_back(u2.uncertainty[i]);
		}
		else
		{
			result.uncertainty.push_back(u1.uncertainty[i]);
		}
	return result;
}
//SUBSTRACTION
TUncertainty operator-(const double& u1, const TUncertainty& u2)
{
	TUncertainty _u1(u1);
	return _u1 - u2;
}
TUncertainty operator-(const TUncertainty& u1, const double& u2)
{
	TUncertainty _u2(u2);
	return u1 - _u2;
}
TUncertainty operator-(const TUncertainty& u1, const TUncertainty& u2)
{
	TUncertainty result;
	result.value = u1.value - u2.value; //Add values
	for (unsigned int i = 0; i < std::max(u1.uncertainty.size(), u2.uncertainty.size()); i++) //Add errors in quadrature. Careful with sizes
		if (i < std::min(u1.uncertainty.size(), u2.uncertainty.size()))
		{
			result.uncertainty.push_back(sqrt(u1.uncertainty[i]*u1.uncertainty[i] + u2.uncertainty[i]*u2.uncertainty[i]));
		}
		else if (i < u2.uncertainty.size())
		{
			result.uncertainty.push_back(u2.uncertainty[i]);
		}
		else
		{
			result.uncertainty.push_back(u1.uncertainty[i]);
		}
	return result;
}
//PRODUCT
TUncertainty operator*(const double& u1, const TUncertainty& u2)
{
	TUncertainty _u1(u1);
	return _u1 * u2;
}
TUncertainty operator*(const TUncertainty& u1, const double& u2)
{
	TUncertainty _u2(u2);
	return u1 * _u2;
}
TUncertainty operator*(const TUncertainty& u1, const TUncertainty& u2)
{
	TUncertainty result;
	result.value = u1.value * u2.value; //Add values
	for (unsigned int i = 0; i < std::max(u1.uncertainty.size(), u2.uncertainty.size()); i++) //Add errors in quadrature. Careful with sizes
		if (i < std::min(u1.uncertainty.size(), u2.uncertainty.size()))
		{
			result.uncertainty.push_back(result.value * sqrt(u1.uncertainty[i]*u1.uncertainty[i] / u1.value / u1.value + u2.uncertainty[i]*u2.uncertainty[i] / u2.value / u2.value));
		}
		else if (i < u2.uncertainty.size())
		{
			result.uncertainty.push_back(result.value * u2.uncertainty[i] / u2.value);
		}
		else
		{
			result.uncertainty.push_back(result.value * u1.uncertainty[i] / u1.value);
		}
	return result;
}
//DIVISION
TUncertainty operator/(const double& u1, const TUncertainty& u2)
{
	TUncertainty _u1(u1);
	return _u1 / u2;
}
TUncertainty operator/(const TUncertainty& u1, const double& u2)
{
	TUncertainty _u2(u2);
	return u1 / _u2;
}
TUncertainty operator/(const TUncertainty& u1, const TUncertainty& u2)
{
	TUncertainty result;
	if (u2.value != 0.) {result.value = u1.value / u2.value;} //Substract values}
	else {result.value = 0.; std::cout << "Dividing by zero. Setting result to zero" << std::endl;}
	for (unsigned int i = 0; i < std::max(u1.uncertainty.size(), u2.uncertainty.size()); i++) //Add errors in quadrature. Careful with sizes
		if (i < std::min(u1.uncertainty.size(), u2.uncertainty.size()))
		{
			result.uncertainty.push_back(result.value * sqrt(u1.uncertainty[i]*u1.uncertainty[i] / u1.value / u1.value + u2.uncertainty[i]*u2.uncertainty[i] / u2.value / u2.value));
		}
		else if (i < u2.uncertainty.size())
		{
			result.uncertainty.push_back(result.value * u2.uncertainty[i] / u2.value);
		}
		else
		{
			result.uncertainty.push_back(result.value * u1.uncertainty[i] / u1.value);
		}
	return result;
}

void TUncertainty::Print(Option_t *option) const
{
	Print(std::cout, option);
	return;
}

void TUncertainty::Print(std::ostream& ss, Option_t *option)const
{
	ss << this->value;
	for (unsigned int i = 0; i < this->uncertainty.size(); i++)
	{
		if (strncmp(option, "rel", 10) == 0) //Use relative uncertainties. No zero check :)
		{
			ss << " \u00B1R " << this->uncertainty[i] / this->value;
		}
		else //Use uncertainties as usual
		{
			ss << " \u00B1 " << this->uncertainty[i];
		}
	}
	ss << std::endl;
	return;
}
//Save results in file
void TUncertainty::SaveResult(std::string filename)
{
	std::ofstream fout;
	fout.open(filename.c_str());
	Print(fout);
	fout.close();
	return;
}

//The magic to show results when using ROOT terminal
namespace cling
{
std::string printValue(TUncertainty *val)
{
	std::stringstream printval;
	printval << val->GetValue();
	for (unsigned int i = 0; i < val->GetUncertainty().size(); i++)
	{
		printval << " \u00B1 " << val->GetUncertainty()[i];
	}
	return printval.str();
}
};

