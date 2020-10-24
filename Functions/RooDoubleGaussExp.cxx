/********************************************************************
 *                                                                  *
 * \[T]/  P R A I S E  T H E  R O O C L A S S  F A C T O R Y  \[T]/ *
 *                                                                  *
 *******************************************************************/
#include "Riostream.h"


#include "Functions/RooDoubleGaussExp.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"
#include "RooMath.h"

//ClassImp(RooDoubleGaussExp)

 RooDoubleGaussExp::RooDoubleGaussExp(const char *name, const char *title,
                        RooAbsReal& _m,
                        RooAbsReal& _mu,
                        RooAbsReal& _sigma,
                        RooAbsReal& _alphaL,
	                RooAbsReal& _alphaR) :
   RooAbsPdf(name,title),
   m("m","m",this,_m),
   mu("mu","mu",this,_mu),
   sigma("sigma","sigma",this,_sigma),
   alphaL("alphaL","alphaL",this,_alphaL),
   alphaR("alphaR","alphaR",this,_alphaR)
 {
 }


 RooDoubleGaussExp::RooDoubleGaussExp(const RooDoubleGaussExp& other, const char* name) :
   RooAbsPdf(other,name),
   m("m",this,other.m),
   mu("mu",this,other.mu),
   sigma("sigma",this,other.sigma),
   alphaL("alphaL",this,other.alphaL),
   alphaR("alphaR",this,other.alphaR)
 {
 }

Double_t RooDoubleGaussExp::ApproxErf(Double_t arg) const
{
   static const double erflim = 5.0 ;
   if( arg > erflim )  return 1.0 ;
   if( arg < -erflim ) return -1.0 ;

   return RooMath::erf(arg) ;
}


 Double_t RooDoubleGaussExp::evaluate() const
 {
   double t = (m-mu)/sigma ;
   double absAlphaL = fabs(alphaL) ;
   double absAlphaR = fabs(alphaR) ;
   double result = 0.0 ;
   // Left side
   if ( t <= -absAlphaL )
   {
     result = std::exp(0.5*absAlphaL*absAlphaL)*std::exp(t*absAlphaL);
   }
   // Right side
   else if ( t >= absAlphaR )
   {
     result = std::exp(0.5*absAlphaR*absAlphaR)*std::exp(-t*absAlphaR);
   }
   // Gaussian center
   else
   {
     result = std::exp(-0.5*t*t) ;
   }
   return result ;
 }

Int_t RooDoubleGaussExp::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
   if( matchArgs(allVars,analVars,m) ) return 1 ;
   return 0 ;
}

Double_t RooDoubleGaussExp::analyticalIntegral(Int_t code, const char* rangeName) const
{
   R__ASSERT(code==1);
   static const double sqrtPiOver2 = 1.2533141373 ;
   static const double sqrt2 = 1.4142135624 ;

   assert( code==1 ) ;
   double result = 0.0 ;

   double sig = fabs((Double_t) sigma) ;

   double tmin = (m.min(rangeName)-mu)/sig ;
   double tmax = (m.max(rangeName)-mu)/sig ;

   double absAlphaL = fabs((Double_t) alphaL) ;
   double absAlphaR = fabs((Double_t) alphaR) ;
   // Integrate depending on tmin and tmax
   if ( tmin <= -absAlphaL ) // Tmin is in the left tail
   {
      double a = std::exp(0.5*absAlphaL*absAlphaL) ;
      if ( tmax <= -absAlphaL ) // All range included in left tail
      {
 	  return (sig/absAlphaL*a*(std::exp(tmax*absAlphaL)-std::exp(tmin*absAlphaL))) ;
      }
      else if ( tmax > -absAlphaL && tmax <= absAlphaR )
      {
          // Range extends further than left tail, so integrate it all
	  result = sig/absAlphaL*a*(std::exp(-absAlphaL*absAlphaL)-std::exp(tmin*absAlphaL)) ;
          // And now the Gaussian part
          return (result + sig*sqrtPiOver2*(ApproxErf(tmax/sqrt2) - ApproxErf(-absAlphaL/sqrt2))) ;
      }
      else // We need to integrate the full gaussian part
      {
          // Range extends further than left tail, so integrate it all
	  result = sig/absAlphaL*a*(std::exp(-absAlphaL*absAlphaL)-std::exp(tmin*absAlphaL)) ;
          // And now the Gaussian part
          result += sig*sqrtPiOver2*(ApproxErf(absAlphaR/sqrt2) - ApproxErf(-absAlphaL/sqrt2)) ;
          // Finally, the right tail
	  double aR = std::exp(0.5*absAlphaR*absAlphaR) ;
          return (result + sig/absAlphaR*aR*(std::exp(-absAlphaR*absAlphaR)-std::exp(-tmax*absAlphaR))) ;
      }
   }
   else if ( tmin > -absAlphaL && tmin <= absAlphaR ) // Tmin is in the Gaussian part
   {
      if ( tmax <= absAlphaR ) // Tmax also in the Gaussian part
      {
          return sig*sqrtPiOver2*(ApproxErf(tmax/sqrt2) - ApproxErf(tmin/sqrt2)) ;
      }
      else
      {
          // Tmax in the right tail, integrate the full Gaussian part
          result = sig*sqrtPiOver2*(ApproxErf(absAlphaR/sqrt2)-ApproxErf(tmin/sqrt2)) ;
          // Finally, the right tail
	  double aR = std::exp(0.5*absAlphaR*absAlphaR) ;
          return (result + sig/absAlphaR*aR*(std::exp(-absAlphaR*absAlphaR)-std::exp(-tmax*absAlphaR))) ;

      }
   }
   else // Tmin is in the right tail
   {
      double a = std::exp(0.5*absAlphaR*absAlphaR) ;
      return (sig/absAlphaR*a*(std::exp(-tmin*absAlphaR)-std::exp(-tmax*absAlphaR))) ;
   }

   return result ;
}
