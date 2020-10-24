/********************************************************************
 *                                                                  *
 * \[T]/  P R A I S E  T H E  R O O C L A S S  F A C T O R Y  \[T]/ *
 *                                                                  *
 *******************************************************************/
#include "Riostream.h"


#include "Functions/Roo2SumExp.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"
#include "RooMath.h"

//ClassImp(Roo2SumExp)

 Roo2SumExp::Roo2SumExp(const char *name, const char *title,
		  RooAbsReal& _m,
		  RooAbsReal& _A,
		  RooAbsReal& _tau1,
		  RooAbsReal& _tau2) :
  RooAbsPdf(name,title),
  m("m","m",this,_m),
  A("A","A",this,_A),
  tau1("tau1","tau1",this,_tau1),
  tau2("tau2","tau2",this,_tau2)
 {
 }


 Roo2SumExp::Roo2SumExp(const Roo2SumExp& other, const char* name) :
   RooAbsPdf(other,name),
   m("m",this,other.m),
   A("A",this,other.A),
   tau1("tau1",this,other.tau1),
   tau2("tau2",this,other.tau2)
 {
 }

 Double_t Roo2SumExp::evaluate() const
 {
   double result;
   result = std::exp(m*tau1)+A*std::exp(m*tau2);
   return result ;
 }

Int_t Roo2SumExp::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
   if( matchArgs(allVars,analVars,m) ) return 1 ;
   return 0 ;
}

Double_t Roo2SumExp::analyticalIntegral(Int_t code, const char* rangeName) const
{
  R__ASSERT(code==1);
  double mmin = m.min(rangeName) ;
  double mmax = m.max(rangeName) ;

  assert( code==1 ) ;
  double result;
  // Integrate
  result = (std::exp(mmax*tau1)-std::exp(mmin*tau1))/tau1+A*(std::exp(mmax*tau2)-std::exp(mmin*tau2))/tau2;
  return result;
}
