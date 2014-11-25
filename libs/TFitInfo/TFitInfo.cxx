#include "TFitInfo.h"

#include <cstdio>

#include <TF1.h>

//#include <TGRSIFunctions.h>



TFitInfo::TFitInfo() { }

TFitInfo::~TFitInfo() { }

void TFitInfo::Print(Option_t *opt) {}

void TFitInfo::Clear(Option_t *opt) {}

/*
TF1 *TFitInfo::MakeChgSpec(const char *fname, UInt_t npeaks, Double_t *xpos, Double_t *ypos, Double_t *sigs){

  TF1 *f = 0;

  if(!xpos || !ypos){
     printf("{TFitInfo} Error: peak centroid positions must be provided.\n"); 
     return f;
  }

  TGRSIFunctions::SetNfitPeaks(npeaks);
  std::vector<std::string> pnames = TGRSIFunctions::GetParNames(fname);
  UInt_t npars = parnames.size();
  
  f = new TF1(fname,fname,0.0,1.0,npars);

  Double_t var;
  for(int i=0; i<npars; i++){

    f->SetParName(i,pnames.at(i).c_str());

    if(pnames.a(i).find("BG")!=string::npos)
       f->SetParameter(i,0.0); // initiate background terms to zero
    else if(pnames.at(i).find("PEAK")!=string::npos){
      UInt_t peaknum = atoi(pnames.at(i).at(4).c_str());
      UInt_t peakvar = atoi(pnames.at(i).at(6).c_str());
      switch (peakvar){
        case 0:
          var = ypos[peaknum]; // first peak parameter is always height
          break;
        case 1:
          var = xpos[peaknum]; // second peak parameter is always mean
          break;
       default: 
          var = sigs[peaknum]; // all continuing parameters are widths
      }
      f->SetParameter(i,var);  
    } else{
      printf("{TFitInfo} Warning: unrecognised parameter name '%s'. Parameter not set.\n",pnames.at(i).c_str());
      f->SetParameter(i,0.0);
    }
  }

  return f;
}
*/
