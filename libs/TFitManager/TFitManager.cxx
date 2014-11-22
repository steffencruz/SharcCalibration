

#include "TFitManager.h"

#ifndef PI
#define PI = 3.141593
#endif

ClassImp(TFitManager);

TFitManager::TFitManager() { }

TFitManager::~TFitManager() { }

void TFitManager::Print(Option_t *opt) {};
void TFitManager::Clear(Option_t *opt) {};

TFitInfo *TFitManager::FitHist(TF1 *func, TH1D *h, Double_t *parms, UInt_t Nparms, Double_t xlow, Double_t xhigh){

  func->SetParameters(parms);
  h->Fit(func);//done.

}


TFitInfo *TFitManager::FitGraph(TF1 *func, TGraph *g, Double_t *parms, UInt_t Nparms, Double_t xlow, Double_t xhigh){

  func->SetParameters(parms);
  g->Fit(func);//done.

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

