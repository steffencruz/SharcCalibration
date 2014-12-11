#include "TFitInfo.h"

#include <cstdio>

#include <TF1.h>

#include "Globals.h"

//#include <TGRSIFunctions.h>

ClassImp(TFitInfo);

TFitInfo::TFitInfo() { }


TFitInfo::~TFitInfo() { }

TFitInfo::TFitInfo(TF1 *func,Double_t *xvals, Double_t *yvals, UInt_t npeaks, Bool_t flag, const char *finame) { 
  
  SetNPeaks(npeaks);
  
  char buffer[16];
  fpeaknames.resize(npeaks);
  
  for(int i=0; i<npeaks; i++){
     SetXY(xvals[i],yvals[i]);
     sprintf(buffer,"PEAK%i",i);
     SetPeakName(i,buffer);
  }
  SetFunction(func);
  SetStatus(flag);
  SetName(finame);

}

void TFitInfo::SetInfoName(const char *infoname){

  TNamed::SetName(infoname);
  return;
}

void TFitInfo::SetPeakNames(std::vector<std::string> ions){
  
  for(int i=0; i<ions.size();i++){
    if(i>=GetNPeaks())
       break;
    SetPeakName(i,ions.at(i).c_str());
  }  
}

Double_t TFitInfo::GetX(const char *name){
  
  for(int i=0; i<fpeaknames.size(); i++){
    if(fpeaknames.at(i).compare(name)==0)
       return GetX(i);
  }
  printf("{TFitInfo} Warning :  Peak name '%s' not recognised. X was not returned. \n",name);
  return 0.0;
}

Double_t TFitInfo::GetY(const char *name){
  
  for(int i=0; i<fpeaknames.size(); i++){
    if(fpeaknames.at(i).compare(name)==0)
       return GetY(i);
  }
  printf("{TFitInfo} Warning :  Peak name '%s' not recognised. Y was not returned. \n",name);
  return 0.0;
}

void TFitInfo::Print(Option_t *opt) {
  
  printf(DCYAN);
  printf("Name: %s\n",TNamed::GetName());
  ffunction.Print();
  printf(RESET_COLOR);
  
  return;
}  

void TFitInfo::Clear(Option_t *opt) {}

