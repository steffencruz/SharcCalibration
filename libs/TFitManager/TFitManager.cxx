

#include "TFitManager.h"

#ifndef PI
#define PI = 3.141593
#endif


ClassImp(TFitManager);

const char *TFitManager::fFitOpts  = "QR+";
const char *TFitManager::fDispOpts = "";

TFitManager::TFitManager() { }

TFitManager::~TFitManager() { }

void TFitManager::Print(Option_t *opt) {};
void TFitManager::Clear(Option_t *opt) {};


TFitInfo *TFitManager::FitHist(void *fcn, TH1D *h, Double_t *parms, UInt_t Nparms, Double_t xlow, Double_t xhigh, const char *fname){
  // example void *fcn = TGSIFunctions::LanGaus
  //
  TF1 *func = new TF1(fname,fcn,xlow,xhigh,Nparms);
  func->SetParameters(parms);

  h->Fit(func,fFitOpts,fDispOpts,xlow,xhigh);//done.

  TSpectrum *spec = PeakSearch(h,1,100,0.5);
  TFitInfo *tfi = new TFitInfo(func,spec->GetPositionX(),spec->GetPositionY(),spec->GetNPeaks(),true);

  if(true)
    tfi->Print();

  return tfi;
}


TFitInfo *TFitManager::FitGraph(void *fcn, TGraph *g, Double_t *parms, UInt_t Nparms, Double_t xlow, Double_t xhigh, const char *fname){

  TF1 *func = new TF1(fname,fcn,xlow,xhigh,Nparms);
  func->SetParameters(parms);

  g->Fit(func,fFitOpts,fDispOpts,xlow,xhigh);//done.

}

// return TSpecInfo?
TSpectrum *TFitManager::PeakSearch(TH1D *h, UInt_t npeaks, Double_t resolution, Double_t threshold){ 

  TSpectrum *s = new TSpectrum(10);
  s->SetName("TSpec");
  s->Search(h,resolution,"",threshold); //resolution is peak width in bins

  if(s->GetNPeaks()!=npeaks){
    printf("\nERROR: Peak search failed! change axis range or detector resolution\n");
    s->Clear(); // return enmpty spectrum
//    h->GetListOfFunctions()->FindObject("TSpec")->Clear();// don't leave markers on plot
  }

  Float_t *xpos, *ypos;
  xpos = s->GetPositionX();
  ypos = s->GetPositionY();

  // only works for 2 peaks
  double xtemp,ytemp;
  if(xpos[0]>xpos[1]){
    xtemp = xpos[0];
    ytemp = ypos[0];
    xpos[0] = xpos[1];
    ypos[0] = ypos[1];
    xpos[1] = xtemp;
    ypos[1] = ytemp;
  }

  return s;
}


