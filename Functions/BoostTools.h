//Some functions copied from Boost package examples, which for some reason are not included in the library...
//https://www.boost.org/
//This script is not compatible with ROOTCINT
#ifndef BoostTools_h
#define BoostTools_h
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/math/tools/polynomial.hpp>

//Ask about signs
template <typename T>
std::string sign_str(T const &x)
{
	return x < 0 ? "-" : "+";
}

//Add coefficient
template <typename T>
std::string inner_coefficient(T const &x)
{
	std::string result(" " + sign_str(x) + " ");
	if (abs(x) != T(1))
		result += boost::lexical_cast<std::string>(abs(x));
	return result;
}

//Add this function to print polynomials
template <typename T>
std::string formula_format(boost::math::tools::polynomial<T> const &a, std::string var = "x")
{
	std::string result;
	if (a.size() == 0)
		result += boost::lexical_cast<std::string>(T(0));
	else
	{
		// First one is a special case as it may need unary negate.
		unsigned i = a.size() - 1;
		if (a[i] < 0)
			result += "-";
		if (abs(a[i]) != T(1))
			result += boost::lexical_cast<std::string>(abs(a[i]));

		if (i > 0)
		{
			result += var;
			if (i > 1)
			{
				result += "^" + boost::lexical_cast<std::string>(i);
				i--;
				for (; i != 1; i--)
					if (a[i])
						result += inner_coefficient(a[i]) + var + "^" + boost::lexical_cast<std::string>(i);

				if (a[i])
					result += inner_coefficient(a[i]) + var;
			}
			i--;

			if (a[i])
				result += " " + sign_str(a[i]) + " " + boost::lexical_cast<std::string>(abs(a[i]));
		}
	}
	return result;
}

#endif
