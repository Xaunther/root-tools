/********************************************************************
 *                                                                  *
 * \[T]/  P R A I S E  T H E  R O O C L A S S  F A C T O R Y  \[T]/ *
 *                                                                  *
 *******************************************************************/
#include "Riostream.h"


#include "Functions/RooGaussExp.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"
#include "RooMath.h"

//ClassImp(RooGaussExp)

 RooGaussExp::RooGaussExp(const char *name, const char *title,
                        RooAbsReal& _m,
                        RooAbsReal& _mu,
                        RooAbsReal& _sigma,
	                RooAbsReal& _alpha) :
   RooAbsPdf(name,title),
   m("m","m",this,_m),
   mu("mu","mu",this,_mu),
   sigma("sigma","sigma",this,_sigma),
   alpha("alpha","alpha",this,_alpha)
 {
 }


 RooGaussExp::RooGaussExp(const RooGaussExp& other, const char* name) :
   RooAbsPdf(other,name),
   m("m",this,other.m),
   mu("mu",this,other.mu),
   sigma("sigma",this,other.sigma),
   alpha("alpha",this,other.alpha)
 {
 }

Double_t RooGaussExp::ApproxErf(Double_t arg) const
{
   static const double erflim = 5.0 ;
   if( arg > erflim )  return 1.0 ;
   if( arg < -erflim ) return -1.0 ;

   return RooMath::erf(arg) ;
}


 Double_t RooGaussExp::evaluate() const
 {
   double t = (m-mu)/sigma ;
   if( alpha < 0 ) t = -t;

   double absAlpha = fabs(alpha) ;
   double result = 0.0 ;

   // Left side
   if ( t <= -absAlpha )
   {
     result = std::exp(0.5*absAlpha*absAlpha)*std::exp(t*absAlpha);
   }
   // Gaussian
   else
   {
     result = std::exp(-0.5*t*t) ;
   }
   return result ;
 }

Int_t RooGaussExp::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
   if( matchArgs(allVars,analVars,m) ) return 1 ;
   return 0 ;
}

Double_t RooGaussExp::analyticalIntegral(Int_t code, const char* rangeName) const
{
  R__ASSERT(code==1);
   static const double sqrtPiOver2 = 1.2533141373 ;
   static const double sqrt2 = 1.4142135624 ;

   assert( code==1 ) ;
   double result = 0.0 ;

   double sig = fabs((Double_t) sigma) ;

   double tmin = (m.min(rangeName)-mu)/sig ;
   double tmax = (m.max(rangeName)-mu)/sig ;

   if( alpha < 0)
     {
       double tmp = tmin;
       tmin = -tmax;
       tmax = -tmp;
     }

   double absAlpha = fabs((Double_t) alpha) ;
   // Integrate depending on tmin and tmax
   if ( tmin <= -absAlpha ) // Tmin is in the left tail
   {
      double a = std::exp(0.5*absAlpha*absAlpha) ;
      if ( tmax <= -absAlpha ) // All range included in left tail
      {
 	  return (sig/absAlpha*a*(std::exp(tmax*absAlpha)-std::exp(tmin*absAlpha))) ;
      }
      else
      {
          // Range extends further than left tail, so integrate it all
	  result = sig/absAlpha*a*(std::exp(-absAlpha*absAlpha)-std::exp(tmin*absAlpha)) ;
          // And now the Gaussian part
          return (result + sig*sqrtPiOver2*(ApproxErf(tmax/sqrt2) - ApproxErf(-absAlpha/sqrt2))) ;
      }
   }
   else // Tmin is in the Gaussian part
   {
     return sig*sqrtPiOver2*(ApproxErf(tmax/sqrt2) - ApproxErf(tmin/sqrt2)) ;
   }

   return result ;
}
