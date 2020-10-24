/********************************************************************
 *                                                                  *
 * \[T]/  P R A I S E  T H E  R O O C L A S S  F A C T O R Y  \[T]/ *
 *                                                                  *
 *******************************************************************/
#include "Riostream.h"


#include "Functions/RooCBExp.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"
#include "RooMath.h"

//ClassImp(RooCBExp)

 RooCBExp::RooCBExp(const char *name, const char *title,
                        RooAbsReal& _m,
                        RooAbsReal& _mu,
                        RooAbsReal& _sigma,
                        RooAbsReal& _alphaL,
                        RooAbsReal& _n,
	                RooAbsReal& _alphaR) :
   RooAbsPdf(name,title),
   m("m","m",this,_m),
   mu("mu","mu",this,_mu),
   sigma("sigma","sigma",this,_sigma),
   alphaL("alphaL","alphaL",this,_alphaL),
   n("n","n",this,_n),
   alphaR("alphaR","alphaR",this,_alphaR)
 {
 }


 RooCBExp::RooCBExp(const RooCBExp& other, const char* name) :
   RooAbsPdf(other,name),
   m("m",this,other.m),
   mu("mu",this,other.mu),
   sigma("sigma",this,other.sigma),
   alphaL("alphaL",this,other.alphaL),
   n("n",this,other.n),
   alphaR("alphaR",this,other.alphaR)
 {
 }

Double_t RooCBExp::ApproxErf(Double_t arg) const
{
   static const double erflim = 5.0 ;
   if( arg > erflim )  return 1.0 ;
   if( arg < -erflim ) return -1.0 ;

   return RooMath::erf(arg) ;
}


 Double_t RooCBExp::evaluate() const
 {
   double t = (m-mu)/sigma ;
   if( alphaL < 0 && alphaR < 0 ) t = -t;
   double absAlphaL = fabs(alphaL) ;
   double absAlphaR = fabs(alphaR) ;
   double result = 0.0 ;
   // Left side
   if ( t <= -absAlphaL )
   {
      double a = std::pow(n/absAlphaL, n) * std::exp(-0.5*absAlphaL*absAlphaL) ;
      double b = n/absAlphaL - absAlphaL ;
      result = a/std::pow(b-t, n) ;
   }
   // Right side
   else if ( t >= fabs(alphaR) )
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

Int_t RooCBExp::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
   if( matchArgs(allVars,analVars,m) ) return 1 ;
   return 0 ;
}

Double_t RooCBExp::analyticalIntegral(Int_t code, const char* rangeName) const
{
   R__ASSERT(code==1);
   static const double sqrtPiOver2 = 1.2533141373 ;
   static const double sqrt2 = 1.4142135624 ;

   assert( code==1 ) ;
   double result = 0.0 ;
   bool useLogL = false ;


   if( fabs(n-1.0) < 1.0e-05 ) useLogL = true;


   double sig = fabs((Double_t) sigma) ;

   double tmin = (m.min(rangeName)-mu)/sig ;
   double tmax = (m.max(rangeName)-mu)/sig ;

   if( alphaL < 0 && alphaR < 0 )
     {
       double tmp = tmin;
       tmin = -tmax;
       tmax = -tmp;
     }

   double absAlphaL = fabs((Double_t) alphaL) ;
   double absAlphaR = fabs((Double_t) alphaR) ;
   // Integrate depending on tmin and tmax
   if ( tmin <= -absAlphaL ) // Tmin is in the left tail
   {
      double a = std::pow(n/absAlphaL, n)*std::exp(-0.5*absAlphaL*absAlphaL) ;
      double b = n/absAlphaL - absAlphaL ;
      if ( tmax <= -absAlphaL ) // All range included in left tail
      {
          if (useLogL) return a * sig * ( std::log(b-tmin) - std::log(b-tmax) ) ;
          else         return a*sig/(1.0-n) * (1.0/std::pow(b-tmin, n-1.0) - 1.0/std::pow(b-tmax, n-1.0)) ;
      }
      else if ( tmax > -absAlphaL && tmax <= absAlphaR )
      {
          // Range extends further than left tail, so integrate it all
          if (useLogL) result = a * sig * ( std::log(b-tmin) - std::log(n/absAlphaL) ) ;
          else         result = a*sig/(1.0-n) * (1.0/std::pow(b-tmin, n-1.0) - 1.0/std::pow(n/absAlphaL, n-1.0)) ;
          // And now the Gaussian part
          return (result + sig*sqrtPiOver2*(ApproxErf(tmax/sqrt2) - ApproxErf(-absAlphaL/sqrt2))) ;
      }
      else // We need to integrate the full gaussian part
      {
          // Range extends further than left tail, so integrate it all
          if (useLogL) result = a * sig * ( std::log(b-tmin) - std::log(n/absAlphaL) ) ;
          else         result = a*sig/(1.0-n) * (1.0/std::pow(b-tmin, n-1.0) - 1.0/std::pow(n/absAlphaL, n-1.0)) ;
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
