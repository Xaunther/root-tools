/********************************************************************
 *                                                                  *
 * \[T]/  P R A I S E  T H E  R O O C L A S S  F A C T O R Y  \[T]/ *
 *                                                                  *
 *******************************************************************/
#include "Riostream.h"


#include "Functions/Roo2Exp.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"
#include "RooMath.h"

//ClassImp(Roo2Exp)

 Roo2Exp::Roo2Exp(const char *name, const char *title,
		  RooAbsReal& _m,
		  RooAbsReal& _m0,
		  RooAbsReal& _tau1,
		  RooAbsReal& _tau2) :
  RooAbsPdf(name,title),
  m("m","m",this,_m),
  m0("m0","m0",this,_m0),
  tau1("tau1","tau1",this,_tau1),
  tau2("tau2","tau2",this,_tau2)
 {
 }


 Roo2Exp::Roo2Exp(const Roo2Exp& other, const char* name) :
   RooAbsPdf(other,name),
   m("m",this,other.m),
   m0("m0",this,other.m0),
   tau1("tau1",this,other.tau1),
   tau2("tau2",this,other.tau2)
 {
 }

 Double_t Roo2Exp::evaluate() const
 {
   double result;
   //Left side
   if(m <= m0)
     {
       result = std::exp(m*tau1);
     }
   //Right side
   else
     {
       result = std::exp(m0*tau1+tau2*(m-m0));
     }
   return result ;
 }

Int_t Roo2Exp::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
   if( matchArgs(allVars,analVars,m) ) return 1 ;
   return 0 ;
}

Double_t Roo2Exp::analyticalIntegral(Int_t code, const char* rangeName) const
{
  R__ASSERT(code==1);
  double mmin = m.min(rangeName) ;
  double mmax = m.max(rangeName) ;

  assert( code==1 ) ;
  double result;
  // Integrate depending on the range selected
  if(mmax <= m0) //All in left side
    {
      result = (-std::exp(mmin*tau1)+std::exp(mmax*tau1))/tau1;
    }
  else if(mmin >= m0) //All in right side
    {
      result = (std::exp(m0*(tau1-tau2)+mmax*tau2)-std::exp(m0*(tau1-tau2)+mmin*tau2))/tau2;
    }
  else //From left side to right side
    {
      result = (-std::exp(mmin*tau1)+std::exp(m0*tau1))/tau1; //Left side
      result +=	(std::exp(m0*(tau1-tau2)+mmax*tau2)-std::exp(m0*tau1))/tau2; //Right side
    }
  return result;
}
