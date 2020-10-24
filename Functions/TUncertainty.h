#ifndef __TUncertainty__
#define __TUncertainty__

#include "TObject.h"
#include <string>

class TUncertainty : public TObject {
private:
	double value; //Central value
	std::vector<double> uncertainty; //List of uncertainties
public:
	//Constructor
	TUncertainty(double _value) {value = _value;}
	TUncertainty() { TUncertainty(0); }
	//Constructor with a single uncertainty
	TUncertainty(double _value, double _uncertainty) {value = _value; uncertainty = {_uncertainty};}
	//Constructor with a vector of uncertainties
	TUncertainty(double _value, std::vector<double> _uncertainty) {value = _value; uncertainty = _uncertainty;}
	//Destructor
	~TUncertainty() {}

	//Access elements
	double GetValue() {return value;}
	std::vector<double> GetUncertainty() {return uncertainty;}

	//Operation overload
	friend TUncertainty operator+(const double& u1, const TUncertainty& u2);
	friend TUncertainty operator+(const TUncertainty& u1, const double& u2);
	friend TUncertainty operator+(const TUncertainty& u1, const TUncertainty& u2);
	friend TUncertainty operator-(const double& u1, const TUncertainty& u2);
	friend TUncertainty operator-(const TUncertainty& u1, const double& u2);
	friend TUncertainty operator-(const TUncertainty& u1, const TUncertainty& u2);
	friend TUncertainty operator*(const double& u1, const TUncertainty& u2);
	friend TUncertainty operator*(const TUncertainty& u1, const double& u2);
	friend TUncertainty operator*(const TUncertainty& u1, const TUncertainty& u2);
	friend TUncertainty operator/(const double& u1, const TUncertainty& u2);
	friend TUncertainty operator/(const TUncertainty& u1, const double& u2);
	friend TUncertainty operator/(const TUncertainty& u1, const TUncertainty& u2);

	//Print info
	virtual void Print(Option_t *option = "") const;
	virtual void Print(std::ostream&, Option_t *option = "") const;

	//Save results in file
	void SaveResult(std::string);
};

#endif