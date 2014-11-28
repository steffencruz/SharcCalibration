
#include<string>

#include "TSharcFormat.h"

ClassImp(TSharcFormat)

std::string TSharcFormat::fChgMatName   = "ChargeMatrix";       // [TH2F] Charge vs BackStrip    [p+d+c],[a]
std::string TSharcFormat::fChgSpecName  = "ChargeSpectrum";     // [TH1D] Charge                 [p+d+c],[a]
std::string TSharcFormat::fCalGraphName = "CalibrationGraph";   // [TGE]  Energy vs Charge       [p+d+a]
std::string TSharcFormat::fMulGraphName = "MultiGraph";         // [TGE]  Energy vs Charge       [p+d+a]
std::string TSharcFormat::fKinMatName   = "KinematicsMatrix";   // [TH2F] Energy vs ThetaLab     [p],[d]
std::string TSharcFormat::fResMatName   = "KinematicsResidual"; // [TH2F] Energy-Kin vs ThetaLab [p],[d]
std::string TSharcFormat::fExcMatName   = "ExcitationMatrix";   // [TH2F] Excitation vs ThetaLab [p],[d]
std::string TSharcFormat::fExcSpecName  = "ExcitationSpectrum";   // [TH1D] Excitation             [p],[d]

TSharcFormat::TSharcFormat() {}

TSharcFormat::~TSharcFormat() {}

void TSharcFormat::Print(Option_t *opt) {}

void TSharcFormat::Clear(Option_t *opt) {}

const char *TSharcFormat::GetObjectName(const char *objname, UInt_t det, UInt_t fs, Bool_t fullname){
  std::string name = "";
  if(fullname) name.append(GetListName(det,fs));
  name.append(objname);
  return name.c_str();
}

const char *TSharcFormat::GetListName(Int_t det, Int_t fs, Int_t bs){

  char buffer[256];
  sprintf(buffer,"/Objects/"); // might be nice to have this structure to keep main directory tidy
  if(det>0)
    sprintf(buffer,"%sDET%02i/",buffer,det);
  if(fs>0)
    sprintf(buffer,"%sFS%02i/",buffer,fs);
  if(bs>0)
    sprintf(buffer,"%sBS%02i/",buffer,bs);

  std::string name = buffer;
  return name.c_str();
}

TObject *TSharcFormat::CreateObject(const char *objtype, UInt_t DET, UInt_t FS){

  if(std::compare(calobj,fChgMatName)==0){
    TH2F *h = new TH2F(fChgMatName,fChgMatName,48,0,48,16000,0,16000);
    h->GetXaxis()->SetTitle("Back Strip");
    h->GetYaxis()->SetTitle("Charge [/Integration]"); // could get the integration and actually put it here quite easily
    h->GetYaxis()->SetTitleOffset(1.4);
    return (TObject*) h;
  } else {
     printf("{TSharcFormat} Warning :  I can't make anything except charge matrices right now..\n");
     return 0;
  }

}
