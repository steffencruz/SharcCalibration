

#include "TFitManager.h"

#ifndef PI
#define PI = 3.141593
#endif

ClassDef(TFitManager)


TFitManager::TFitManager();

TFitManager::~TFitManager();

void TFitManager::Print(Option_t *opt) {};
void TFitManager::Clear(Option_t *opt) {};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// define custom TF1 functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Double_t TFitManager::LanGaus(Double_t *x, Double_t *pars){
// pars = {npeaks, bg_offs, bg_slope, detector_sigma, peak1_ampl, peak1_mean, peak1_width, .... peaki_width}; 
  double dy, y, conv, spec, gaus;
  conv = 0;

  for(int i=0; i<10; i++){
    dy = 5*pars[3]/10.0; // truncate the convolution by decreasing number of evaluation points and decreasing range [2.5 sigma still covers 98.8% of gaussian]
    y = x[0]-2.5*pars[3]+dy*i;

    spec = pars[1]+pars[2]*y; // define background SHOULD THIS BE CONVOLUTED ????? *************************************
    for( int n=0; n<abs(pars[0]); n++) // the implementation of landau function should be done using the landau function
      spec +=pars[3*n+4]*TMath::Landau(-y,-pars[3*n+5],pars[3*n+6])/TMath::Landau(0,0,100); // add peaks, dividing by max height of landau

    gaus = TMath::Gaus(-x[0],-y,pars[3])/sqrt(2*PI*pars[3]*pars[3]); // gaus must be normalisd so there is no sigma weighting
    conv += gaus*spec*dy; // now convolve this [integrate the product] with a gaussian centered at x;
  }

  return conv;
}


Double_t TFitManager::LanGausHighRes(Double_t *x, Double_t *pars){ // 5x more convolution points with 1.6x larger range
  double dy, y, conv, spec, gaus;
  conv = 0;

  for(int i=0; i<50; i++){
    dy = 8*pars[3]/50.0; // 4 sigma covers 99.99% of gaussian
    y = x[0]-4*pars[3]+dy*i;

    spec = pars[1]+pars[2]*y; 
    for( int n=0; n<abs(pars[0]); n++)
      spec +=pars[3*n+4]*TMath::Landau(-y,-pars[3*n+5],pars[3*n+6])/TMath::Landau(0,0,100); 

    gaus = TMath::Gaus(-x[0],-y,pars[3])/sqrt(2*PI*pars[3]*pars[3]); 
    conv += gaus*spec*dy;
  }
  return conv;
}


Double_t TFitManager::Landau(Double_t *x, Double_t *pars){ // simple landau with tail flipped
  //This function is derived from the energy loss of charged particles in matter (Bethe-Bloch). It contains no detector physics
  //Requires the following parameters:
  //   - par[0]:  height of peak
  //   - par[1]:  centroid 
  //   - par[2]:  width

  return spec = pars[0]*TMath::Landau(-x[0],-pars[1],pars[2])/TMath::Landau(0,0,100); 
}


Double_t TFitManager::SkewedGaus(Double_t *x, Double_t *par){
  //This function is derived from the convolution of a gaussian with an exponential
  //Requires the following parameters:
  //   - par[0]:  height of photopeak
  //   - par[1]:  centroid of gaussian
  //   - par[2]:  standard deviation of gaussian 
  //   - par[3]:  "skewedness" of the skewed gaussin
  
  return par[0]*(TMath::Exp((x[0]-par[1])/par[3]))*(TMath::Erfc(((x[0]-par[1])/(TMath::Sqrt(2.0)*par[2]))+par[2]/(TMath::Sqrt(2.0)*par[3])));
}


Double_t TFitManager::MultiSkewedGausWithBG(Double_t *x, Double_t *par) {
  // Limits need to be impossed or error states may occour.
  double result = par[1] + x[0]*par[2]; // background.
  for(int i=0;i<abs(par[0]);i++) {
    Double_t tmp_par[4];
    tmp_par[0]   = par[4*i+3]; //height of photopeak
    tmp_par[1]   = par[4*i+4]; //Peak Centroid of non skew gaus
    tmp_par[2]   = par[4*i+5]; //standard deviation  of gaussian
    tmp_par[3]   = par[4*i+6]; //"skewedness" of the skewed gaussian
    result += SkewedGaus(x,tmp_par);
  }
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// define initialisation and fitting routines
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FitInfo *TFitManager::FitWithSkewedGaus(TH1D *h, UInt_t npeaks, Double_t resolution, UInt_t groupbins){

  FitInfo *info;
  double xmin = 520.0, xmax = 4000.0;
  h->Rebin(TMath::Power(2,groupbins));

  TSpectrum *s = PeakSearch(h,npeaks,resolution/TMath::Power(2,groupbins));
  if(s->GetNPeaks()!=npeaks){
     info->Flag     = 1;
     info->TSpec    = 0;
     info->Function = 0;
  }

  Float_t *xpos, *ypos;
  xpos = s->GetPositionX();
  ypos = s->GetPositionY();

  const UInt_t npars = npeaks*4+3;
  TF1 *f = new TF1("SkewedGaus",MultiSkewedGausWithBG,xmin,xmax,npars);
  f->SetNpx(1000);
  f->SetLineColor(4);

  h->GetXaxis()->SetRangeUser(xmin,xmax);

  double A[2], B[2], C[2];
  A[0] = ypos[0], A[1] = ypos[1];
  B[0] = xpos[0], B[1] = xpos[1];
  C[0] = 100.0  , C[1] = 100.0  ; // maybe make this less hardcoded
  
  std::vector<std::string> ions;
  ions.push_back("Prot");
  ions.push_back("Deut");
  ions.push_back("Carb");
  
  Double_t pars[npars];
  pars[0] = npeaks; // set number of peaks to fit
  f->SetParName(0,"NPeaks");
  pars[1] = 0.0; // set constant background
  f->SetParName(1,"Const_BG");
  pars[2] = 0.0; // set linear background
  f->SetParName(2,"Slope_BG");
  
  for(int i=0;i<npeaks; i++){
    pars[i*4+3] = A[i];
    f->SetParName(i*4+3,Form("Amplitude_%s",ions.at(i).c_str()));
    pars[i*4+4] = B[i];
    f->SetParName(i*4+4,Form("Centroid_%s",ions.at(i).c_str()));
    pars[i*4+5] = C[i];
    f->SetParName(i*4+5,Form("Sigma_%s",ions.at(i).c_str()));
    pars[i*4+6] = C[i];
    f->SetParName(i*4+6,Form("Decay_%s",ions.at(i).c_str()));
  }
  
  f->SetParameters(pars);
  f->FixParameter(0,npeaks);

  TFitResultPtr p = h->Fit(f,"Q","",xmin,xmax);
  Int_t status = p;

  info->Flag     = status;
  info->Function = f;
  info->TSpec    = s;

  return info;
}




FitInfo *TFitManager::FitWithiLanGaus(TH1D *h, UInt_t npeaks, Double_t resolution, UInt_t groupbins){

  FitInfo *info;
  double xmin = 520.0, xmax = 4000.0;
  h->Rebin(TMath::Power(2,groupbins));

  TSpectrum *s = PeakSearch(h,npeaks,resolution/TMath::Power(2,groupbins));
  if(s->GetNPeaks()!=npeaks){
     info->Flag     = 1;
     info->TSpec    = 0;
     info->Function = 0; // no tf1 means tspec failed
  }

  Float_t *xpos, *ypos;
  xpos = s->GetPositionX();
  ypos = s->GetPositionY();

  const UInt_t npars = npeaks*3+4;
  TF1 *f = new TF1("LanGaus",LanGaus,xmin,xmax,npars);
  f->SetNpx(1000);
  f->SetLineColor(4);

  double A[2], B[2], C[2];
  A[0] = ypos[0], A[1] = ypos[1];
  B[0] = xpos[0], B[1] = xpos[1];
  C[0] = 100.0  , C[1] = 100.0  ; // maybe make this less hardcoded
  
  std::vector<std::string> ions;
  ions.push_back("Prot");
  ions.push_back("Deut");
  ions.push_back("Carb");
  
  Double_t pars[npars];
  pars[0] = npeaks; // set number of peaks to fit
  f->SetParName(0,"NPeaks");
  pars[1] = 0.0; // set constant background
  f->SetParName(1,"Const_BG");
  pars[2] = 0.0; // set linear background
  f->SetParName(2,"Slope_BG");
  pars[3] = resolution; // set resolution
  f->SetParName(3,"Detector_Sigma");

  for(int i=0;i<npeaks; i++){
    pars[i*3+4] = A[i];
    f->SetParName(i*3+4,Form("Amplitude_%s",ions.at(i).c_str()));
    pars[i*3+5] = B[i];
    f->SetParName(i*3+5,Form("Centroid_%s",ions.at(i).c_str()));
    pars[i*3+6] = C[i];
    f->SetParName(i*3+6,Form("Sigma_%s",ions.at(i).c_str()));
  }
  
  f->SetParameters(pars);
  f->FixParameter(0,npeaks);

  TFitResultPtr p = h->Fit(f,"Q","",xmin,xmax);
  Int_t status = p;

  info->Flag     = status;
  info->Function = f;
  info->TSpec    = s;

  return info;
}

// return TSpecInfo?
TSpectrum *TFitManager::PeakSearch(TH1D *h, UInt_t npeaks, Double_t resolution){ 

  TSpectrum *s = new TSpectrum(10);
  s->SetName("TSpec");
  s->Search(h,resolution,"",0.25); //resolution is peak width in bins

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

