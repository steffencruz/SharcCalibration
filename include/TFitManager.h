#ifndef TFITMANAGER_H
#define TFITMANAGER_H

#include <Rtypes.h>
#include <TNamed.h>
#include <TF1.h>
#include <TSpectrum.h>

class TFitManager : public TNamed  { 
  public:
    TFitManager();  // default  constructor.
    virtual ~TFitManager();   

    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");

  private:
    struct FitInfo{
      Bool_t Flag;
      TF1 *Function;
      TSpectrum *Spec;
    }

  public:
    FitInfo *FitWithLanGaus(TH1D *h, UInt_t npeaks=2, Double_t resolution = 100.0, UInt_t groupbins = 0);
    Fitinfo *FitWithSkewedGaus(TH1D *h, UInt_t npeaks=2, Double_t resolution = 100.0, UInt_t groupbins = 0);
//    FitInfo *FitMultiGaus(TH1D *h, UInt_t npeaks, Double_t resolution = 100.0, UInt_t groupbins = 0);
    TSpectrum *PeakSearch(TH1D *h, UInt_t npeaks, Double_t resolution);

    Double_t LanGausHighRes(Double_t *x,Double_t *pars);
    Double_t LanGaus(Double_t *x,Double_t *pars);
    Double_t Landau(Double_t *x,Double_t *pars);
    Double_t MultiSkewedGausWithBG(Double_t *x,Double_t *pars);
    Double_t SkewedGaus(Double_t *x,Double_t *pars);
//    Double_t MultiGaus(Double_t *x,Double_t *pars);

  ClassDef(TFitManager,0)

};

#endif

