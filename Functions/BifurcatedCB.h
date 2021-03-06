/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

#ifndef BIFURCATEDCB
#define BIFURCATEDCB

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class BifurcatedCB : public RooAbsPdf {
public:
  BifurcatedCB() {} ;
  BifurcatedCB(const char *name, const char *title,
	      RooAbsReal& _m,
	      RooAbsReal& _mu,
	      RooAbsReal& _sigma,
	      RooAbsReal& _alphaL,
	      RooAbsReal& _nL,
	      RooAbsReal& _alphaR,
	      RooAbsReal& _nR);
  BifurcatedCB(const BifurcatedCB& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new BifurcatedCB(*this,newname); }
  inline virtual ~BifurcatedCB() { } 

  virtual Int_t getAnalyticalIntegral( RooArgSet& allVars,  RooArgSet& analVars, const char* rangeName=0 ) const;
  virtual Double_t analyticalIntegral( Int_t code, const char* rangeName=0 ) const;

protected:

  RooRealProxy m ;
  RooRealProxy mu ;
  RooRealProxy sigma ;
  RooRealProxy alphaL ;
  RooRealProxy nL ;
  RooRealProxy alphaR ;
  RooRealProxy nR ;

  Double_t evaluate() const ;
  Double_t ApproxErf(Double_t arg) const;

private:

  //  ClassDef(BifurcatedCB,1) // Your description goes here...
};


#endif
