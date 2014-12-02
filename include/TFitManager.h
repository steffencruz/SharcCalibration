#ifndef TFITMANAGER_H
#define TFITMANAGER_H

#include <Rtypes.h>
#include <TNamed.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TSpectrum.h>


//#include "TGRSIFunctions.h"

#include "TFitInfo.h"

class TFitManager : public TNamed  { 
  public:
    TFitManager();  // default  constructor.
    virtual ~TFitManager();   

    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");

  public:

    static TFitInfo *FitHist(void *fcn, TH1D *h, Double_t *parms=0, UInt_t Nparms=10, Double_t xlow=-1, Double_t xhigh=-1, const char *fname = "Function");
    static TFitInfo *FitGraph(void *fcn, TGraph *h, Double_t xlow=-1, Double_t xhigh=-1, const char *fname = "Function");
//    static TFitInfo *FitGraph(void *fcn, TGraph *h, Double_t *parms=0, UInt_t Nparms=10, Double_t xlow=-1, Double_t xhigh=-1, const char *fname = "Function");
    
//    static TFitInfo *FitHist(TF1 *func, TH1D *h, Double_t *parms=0, UInt_t Nparms=10, Double_t xlow=-1, Double_t xhigh=-1);
//    static TFitInfo *FitGraph(TF1 *func, TGraph *h, Double_t *parms=0, UInt_t Nparms=10, Double_t xlow=-1, Double_t xhigh=-1);
    static TSpectrum *PeakSearch(TH1D *h, UInt_t npeaks=2, Double_t resolution=100.0, Double_t threshold=0.25); 
    static std::vector<double> GetParameters(const char *fname, TSpectrum *s); 

  private:
    static const char *fFitOpts;
    static const char *fDispOpts;
 ClassDef(TFitManager,0)
};

#endif

