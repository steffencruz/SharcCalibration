#include "TFitInfo.h"

#include <cstdio>

#include <TF1.h>

#include "Globals.h"

//#include <TGRSIFunctions.h>

ClassImp(TFitInfo);

TFitInfo::TFitInfo() { }


TFitInfo::~TFitInfo() { }

TFitInfo::TFitInfo(TF1 *func,Double_t *xvals, Double_t *yvals, UInt_t npeaks,Int_t *indices, Bool_t flag, const char *finame) { 
  
  SetNPeaks(npeaks);
  
  char buffer[16];
  fpeaknames.resize(npeaks);
  //func->Print();
  for(int i=0; i<npeaks; i++){
     int xnum = func->GetParNumber((Form("PEAK%i_MEAN",indices[i])));
     int ynum = func->GetParNumber((Form("PEAK%i_HEIGHT",indices[i])));
     
     SetXY(func->GetParameter(xnum),func->GetParameter(ynum));
     SetXYE(func->GetParError(xnum),func->GetParError(ynum));
     SetChi2(func->GetChisquare());
     SetNormChi2(func->GetChisquare()/func->GetNDF());
     sprintf(buffer,"PEAK%i",indices[i]);
     SetPeakName(indices[i],buffer);
  }
  SetFunction(func);
  SetStatus(flag);
  SetName(finame);

}

void TFitInfo::SetInfoName(const char *infoname){

  TNamed::SetName(infoname);
  return;
}

void TFitInfo::SetPeakNames(std::vector<std::string> ions,const char *opt){
 
  if(strcmp(opt,"Run")==0) {
     for(int i=0; i<ions.size();i++){
       if(i>=GetNPeaks())
          break;
      SetPeakName(i,ions.at(i).c_str());
     }
  } else if(strcmp(opt,"Src")==0) {
     for(int i=0; i<ions.size();i++){
       if(2*i>=GetNPeaks())
          break;
      SetPeakName(i*2+1,ions.at(i).c_str());
     }
  } 
}

UInt_t TFitInfo::GetPeakNum(const char *name){
  
  for(UInt_t i=0; i<fpeaknames.size(); i++){
    if(fpeaknames.at(i).compare(name)==0)
       return i;
  }
  printf("{TFitInfo} Warning :  Peak name '%s' not recognised. Peak number was not returned. \n",name);
  return 0;
}

Double_t TFitInfo::GetParm(int, const char *parmname, const char *ion) {


}

void TFitInfo::Print(Option_t *opt) {
  
  printf(DCYAN);
  printf("Name: %s\n",TNamed::GetName());
  ffunction.Print();
  printf(RESET_COLOR);

 // printf("fpeaknames.size() = %i\t fxvalues.size() = %i\t fyvalues.size() = %i\n",fpeaknames.size(),fxvalues.size(),fyvalues.size());

  for(int i=0;i<fnpeaks;i++) {
     printf(DYELLOW);
   //  printf(" [%i] %s x = %.02f  y = %.02f \n",i,fpeaknames[i].c_str(),fxvalues[i], fyvalues[i]);
     printf(RESET_COLOR);
  }
  return;
}  

void TFitInfo::Clear(Option_t *opt) {
   
   

}

