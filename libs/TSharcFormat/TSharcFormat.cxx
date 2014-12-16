
#include<string>

#include <TH2.h>

#include "TSharcFormat.h"
#include "TGraphErrors.h"

ClassImp(TSharcFormat)

TSharcFormat *TSharcFormat::fSharcFormat = 0;

const std::string TSharcFormat::fChgMatName      = "ChargeMatrix";            // [TH2F] Charge vs BackStrip    [p+d+c],[a]
const std::string TSharcFormat::fChgSpecName     = "ChargeSpectrum";          // [TH1D] Charge                 [p+d+c],[a]
const std::string TSharcFormat::fCentMatName     = "CentroidMatrix";          // [TH2F] Centroid               [p+d+c],[a]
const std::string TSharcFormat::fCentChiMatName  = "CentroidChi2Matrix";      // [TH2F] Centroid chi squared   [p+d+c],[a]
const std::string TSharcFormat::fCalcMatName     = "EnergyCalcMatrix";        // [TH2F] Calculated energy      [p+d+c],[a]
const std::string TSharcFormat::fCalGraphName    = "CalibrationGraph";        // [TGE]  Energy vs Charge       [p+d+a]
const std::string TSharcFormat::fMulGraphName    = "MultiGraph";              // [TGE]  Energy vs Charge       [p+d+a]
const std::string TSharcFormat::fCombinedGraphName    = "CombinedCalibrationGraph";        // [TGE]  Energy vs Charge       [p+d+a]
const std::string TSharcFormat::fKinMatName      = "KinematicsMatrix";        // [TH2F] Energy vs ThetaLab     [p],[d]
const std::string TSharcFormat::fResMatName      = "KinematicsResidual";      // [TH2F] Energy-Kin vs ThetaLab [p],[d]
const std::string TSharcFormat::fExcMatName      = "ExcitationMatrix";        // [TH2F] Excitation vs ThetaLab [p],[d]
const std::string TSharcFormat::fExcSpecName     = "ExcitationSpectrum";      // [TH1D] Excitation             [p],[d]

const std::string TSharcFormat::fFitInfoName  = "FitInfo";                 // [TFitInfo] container for fit results

TSharcFormat *TSharcFormat::Get() { 
  if(!fSharcFormat)
     fSharcFormat = new TSharcFormat;
  return fSharcFormat;
}


TSharcFormat::TSharcFormat() {
}

TSharcFormat::~TSharcFormat() {}

void TSharcFormat::Print(Option_t *opt) {}

void TSharcFormat::Clear(Option_t *opt) {}

const char *TSharcFormat::GetObjectName(const char* objname, const char *ion, Bool_t fullname, Int_t det, Int_t fs, Int_t bs){
  static std::string objectname;
  objectname.clear();
  if(fullname) objectname.append(GetListName(det,fs,bs));
  objectname.append(objname);
  objectname.append(ion); // by default this is an empty string
  return objectname.c_str();
}

const char *TSharcFormat::GetListName(Int_t det, Int_t fs, Int_t bs){

  char buffer[256];
  sprintf(buffer,"Objects/"); // might be nice to have this structure to keep main directory tidy
  if(det>0)
    sprintf(buffer,"%sDET%02i/",buffer,det);
  if(fs>-1)
    sprintf(buffer,"%sFS%02i/",buffer,fs);
  if(bs>-1)
    sprintf(buffer,"%sBS%02i/",buffer,bs);

  static std::string getlistname; 
  getlistname.clear();
  getlistname.assign(buffer);
  return getlistname.c_str();
}

TObject *TSharcFormat::CreateObject(const char *objtype, Option_t *opt, UInt_t DET, Int_t FS){

  std::string name = objtype;
  //name.clear();
  //name = objtype;

  if(name.compare(GetChgMatName())==0){
    // printf("NEW HIST NAME = %s\n",GetChgMatName(true,DET,FS));
    TH2F *h = new TH2F(GetChgMatName(opt,true,DET,FS),GetChgMatName(opt,true,DET,FS),48,0,48,16000,0,16000);
    h->GetXaxis()->SetTitle("Back Strip");
    h->GetYaxis()->SetTitle("Charge [/Integration]"); // could get the integration and actually put it here quite easily
    h->GetYaxis()->SetTitleOffset(1.4);
    return (TObject*) h;
  } else if(name.compare(GetCentMatName())==0){
     
    TH2F *h = new TH2F(GetCentMatName(opt,true,DET),GetCentMatName(opt,true,DET),24,0,24,48,0,48);
    h->GetYaxis()->SetTitle("Back Strip");
    h->GetXaxis()->SetTitle("Front Strip");
    return (TObject*) h;

  } else if(name.compare(GetCentChiMatName())==0){
     
    TH2F *h = new TH2F(GetCentChiMatName(opt,true,DET),GetCentChiMatName(opt,true,DET),24,0,24,48,0,48);
    h->GetYaxis()->SetTitle("Back Strip");
    h->GetXaxis()->SetTitle("Front Strip");
    return (TObject*) h;

  } else if(name.compare(GetCalcMatName())==0){

    TH2F *h = new TH2F(GetCalcMatName(opt,true,DET),GetCalcMatName(opt,true,DET),24,0,24,48,0,48);
    h->GetYaxis()->SetTitle("Back Strip");
    h->GetXaxis()->SetTitle("Front Strip");
    return (TObject*) h;

  } else if(name.compare(GetCalGraphName())==0){

    TGraphErrors *gerrs = new TGraphErrors();
    gerrs->SetName(GetCalGraphName(opt,true,DET,FS));
    gerrs->SetTitle(GetCalGraphName(opt,true,DET,FS));
    gerrs->GetXaxis()->SetTitle("Charge [/Integration]");
    gerrs->GetYaxis()->SetTitle("Energy calculated [keV]");
    gerrs->GetYaxis()->SetTitleOffset(1.4);
    gerrs->SetEditable(false);
    return (TObject*) gerrs;
  
  } else if(name.compare(GetMulGraphName())==0){

    TGraphErrors *gerrs = new TGraphErrors();
    gerrs->SetName(GetMulGraphName(opt,true,DET,FS));
    gerrs->SetTitle(GetMulGraphName(opt,true,DET,FS));
    gerrs->GetXaxis()->SetTitle("Charge [/Integration]");
    gerrs->GetYaxis()->SetTitle("Energy calculated [keV]");
    gerrs->GetYaxis()->SetTitleOffset(1.4);
    return (TObject*) gerrs;

  } else {
     printf("{TSharcFormat} Warning :  I can't make [DET = %i, FS = %i] '%s' right now..\n",DET,FS,objtype);
     return 0;
  }

}
