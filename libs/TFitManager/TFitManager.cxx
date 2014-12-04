

#include "TFitManager.h"

#include <TROOT.h>
#include <TMath.h>

#include "TGRSIFunctions.h"

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


TFitInfo *TFitManager::FitHist(const char *fcn, TH1D *h, Double_t *parms, UInt_t Nparms, Double_t xlow, Double_t xhigh){
  // example void *fcn = TGSIFunctions::LanGaus
  //
  TF1 *func=0;
  std::string funstr = fcn;
  // to keep things tidy make a private member function that does these string comparisons
  if(funstr.find("MultiGausWithBG")!=std::string::npos)
    func = new TF1(fcn,TGRSIFunctions::MultiGausWithBG,xlow,xhigh,Nparms);
  else if(funstr.find("MultiSkewedGausWithBG2")!=std::string::npos)
    func = new TF1(fcn,TGRSIFunctions::MultiSkewedGausWithBG2,xlow,xhigh,Nparms);
  else if(funstr.find("LanGaus")!=std::string::npos)
    func = new TF1(fcn,TGRSIFunctions::LanGaus,xlow,xhigh,Nparms);
  else {
    printf("{TFitManager} Warning :  Function '%s' not recognized.\n",fcn);
    return 0;
  }

  UInt_t npeaks = abs(parms[0]);
  func->FixParameter(0,npeaks); // Fix number of peaks
  func->SetParameters(parms);

  std::vector<std::string> parnames = GetParNames(fcn,npeaks);
  for(int i=0; i<parnames.size(); i++)
    func->SetParName(i,parnames.at(i).c_str());

  func->SetNpx(1000);

  TFitResultPtr p = h->Fit(func,fFitOpts,fDispOpts,xlow,xhigh);//done.
  Bool_t status = p;

  Double_t xpos[npeaks],ypos[npeaks];
  for(int i=0;i<npeaks;i++){
    xpos[i] = func->GetParameter(Form("PEAK%i_MEAN",i));
    ypos[i] = func->GetParameter(Form("PEAK%i_HEIGHT",i));
  }

  TFitInfo *tfi = new TFitInfo(func,xpos,ypos,npeaks,status);
  
  if(true)
    tfi->Print();

  return tfi;
}


TFitInfo *TFitManager::FitGraph(const char *fcn, TGraph *g, Double_t xlow, Double_t xhigh, const char *fname){

  TF1 *func = new TF1(fname,fcn,xlow,xhigh);

  g->Fit(func,fFitOpts,fDispOpts,xlow,xhigh);//done.

}

// return TSpecInfo?
TSpectrum *TFitManager::PeakSearch(TH1D *h, UInt_t npeaks, Double_t resolution, Double_t threshold){ 

  TSpectrum *s = new TSpectrum(10);
  s->SetName("TSpec");
  s->Search(h,resolution,"",threshold); //resolution is peak width in bins
  
  UInt_t npf = s->GetNPeaks();
  if(npf==0){//!=npeaks){
    printf("\n{TFitManager} ERROR :  Peak search for '%s' found %i peaks.. change axis range or something\n",h->GetName(),npf);
//    s->Clear(); // return enmpty spectrum
    return 0;
//    h->GetListOfFunctions()->FindObject("TSpec")->Clear();// don't leave markers on plot
  }

  Float_t *xpos, *ypos;
  xpos = s->GetPositionX();
  ypos = s->GetPositionY();
/*
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
*/

  UInt_t indx[npf];
  Float_t xx[npf], yy[npf];
  TMath::Sort(npf,xpos,indx,true);
  for(int i=0; i<npf; i++){
    xx[indx[i]] = xpos[i];
    yy[indx[i]] = ypos[i];
  }
  // now actually change the values at each array address to be sorted
  for(int i=0; i<npf; i++) {
     xpos[i] = xx[i]; 
     ypos[i] = yy[i];
  }

  return s;
}

std::vector<double> TFitManager::GetParameters(const char *fname, TSpectrum *s){

  std::string name = fname;
  std::vector<double> pars;

  UInt_t npeaks = s->GetNPeaks();
  Float_t *xpos, *ypos;
  xpos = s->GetPositionX();
  ypos = s->GetPositionY();

  pars.push_back((double)npeaks); // first parameter is number of peaks
  
/*
  pars.push_back(0.0); // constant background
  pars.push_back(0.0); // linear background
  if(name.compare("TGRSIFunctions::LanGaus")==0)
     pars.push_back(0.0); // convolution resolution (don't set it here)
  for(int i=0; i<npeaks; i++){
     pars.push_back(ypos[i]); // peak height
     pars.push_back(xpos[i]); // peak centroid
     pars.push_back(0.0);     // peak width (don't set here)
     if(name.compare("TGRSIFunctions::MultiSkewedGausWithBG2")==0)
      pars.push_back(0.0);     // peak width (don't set here)
  }
*/
  if(name.compare("TGRSIFunctions::LanGaus")==0){
    pars.push_back(0.0); // constant background
    pars.push_back(0.0); // linear background
    pars.push_back(0.0); // convolution resolution (don't set it here)
    for(int i=0; i<npeaks; i++){
       pars.push_back(ypos[i]); // peak height
       pars.push_back(xpos[i]); // peak centroid
       pars.push_back(0.0);     // peak width (don't set here)
    }
  }
  else if(name.compare("TGRSIFunctions::MultiSkewedGausWithBG2")==0){
    pars.push_back(0.0); // constant background
    pars.push_back(0.0); // linear background
    for(int i=0; i<npeaks; i++){
       pars.push_back(ypos[i]); // peak height
       pars.push_back(xpos[i]); // peak centroid
       pars.push_back(100.0);     // gaus peak width (don't set here)
       pars.push_back(100.0);     // skew peak width (don't set here)
    }
  }
  else if(name.compare("TGRSIFunctions::MultiGausWithBG")==0){
    pars.push_back(0.0); // constant background
    pars.push_back(0.0); // linear background
    for(int i=0; i<npeaks; i++){
       pars.push_back(ypos[i]); // peak height
       pars.push_back(xpos[i]); // peak centroid
       pars.push_back(0.0);     // peak width (don't set here)
    }
  } else {
    printf("{TFitManager} Warning :  Function '%s' not recognised.\n");
  }

  return pars;
}


std::vector<std::string> TFitManager::GetParNames(const char *fname, UInt_t npeaks){

  std::string name = fname;
  std::vector<std::string> parnames;
  parnames.push_back("NPEAKS");
  char buffer[32];

  if(name.compare("TGRSIFunctions::LanGaus")==0){
    parnames.push_back("BG_CONSTANT");
    parnames.push_back("BG_LINEAR");
    parnames.push_back("RESOLUTION");
    for(int i=0; i<npeaks; i++){
       sprintf(buffer,"PEAK%i_HEIGHT",i);
       parnames.push_back(buffer); 
       sprintf(buffer,"PEAK%i_MEAN",i);
       parnames.push_back(buffer); 
       sprintf(buffer,"PEAK%i_WIDTH",i);
       parnames.push_back(buffer);     
    }
  }
  else if(name.compare("TGRSIFunctions::MultiSkewedGausWithBG2")==0){
    parnames.push_back("BG_CONSTANT");
    parnames.push_back("BG_LINEAR"); 
    for(int i=0; i<npeaks; i++){
       sprintf(buffer,"PEAK%i_HEIGHT",i);
       parnames.push_back(buffer); 
       sprintf(buffer,"PEAK%i_MEAN",i);
       parnames.push_back(buffer); 
       sprintf(buffer,"PEAK%i_GAUSWIDTH",i);
       parnames.push_back(buffer);     
       sprintf(buffer,"PEAK%i_SKEWWIDTH",i);
       parnames.push_back(buffer);     
    }
  }
  else if(name.compare("TGRSIFunctions::MultiGausWithBG")==0){
    parnames.push_back("BG_CONSTANT");
    parnames.push_back("BG_LINEAR");
    for(int i=0; i<npeaks; i++){
       sprintf(buffer,"PEAK%i_HEIGHT",i);
       parnames.push_back(buffer); 
       sprintf(buffer,"PEAK%i_MEAN",i);
       parnames.push_back(buffer); 
       sprintf(buffer,"PEAK%i_WIDTH",i);
       parnames.push_back(buffer); 
    }
  } else {
    printf("{TFitManager} Warning :  Function '%s' not recognised.\n");
  }

  return parnames;
}
