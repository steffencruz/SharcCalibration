
#include<string>

#include "TSharcName.h"

ClassImp(TSharcName)

std::string TSharcName::fChgMatName   = "ChargeMatrix";       // [TH2F] Charge vs BackStrip    [p+d+c],[a]
std::string TSharcName::fChgSpecName  = "ChargeSpectrum";     // [TH1D] Charge                 [p+d+c],[a]
std::string TSharcName::fCalGraphName = "CalibrationGraph";   // [TGE]  Energy vs Charge       [p+d+a]
std::string TSharcName::fMulGraphName = "MultiGraph";         // [TGE]  Energy vs Charge       [p+d+a]
std::string TSharcName::fKinMatName   = "KinematicsMatrix";   // [TH2F] Energy vs ThetaLab     [p],[d]
std::string TSharcName::fResMatName   = "KinematicsResidual"; // [TH2F] Energy-Kin vs ThetaLab [p],[d]
std::string TSharcName::fExcMatName   = "ExcitationMatrix";   // [TH2F] Excitation vs ThetaLab [p],[d]
std::string TSharcName::fExcSpecName  = "ExcitationSpectrum";   // [TH1D] Excitation             [p],[d]

TSharcName::TSharcName() {}

TSharcName::~TSharcName() {}

void TSharcName::Print(Option_t *opt) {}

void TSharcName::Clear(Option_t *opt) {}

// Uncalibrated data objects
const char *TSharcName::GetChgMatName(UInt_t det, UInt_t fs){
  std::string name = GetListName(det,fs);
  name.append(fChgMatName.c_str());
  return name.c_str();
}

const char *TSharcName::GetChgSpecName(UInt_t det, UInt_t fs, UInt_t bs){
  std::string name = GetListName(det,fs);
  name.append(fChgSpecName.c_str());
  return name.c_str();
}

const char *TSharcName::GetCalGraphName(UInt_t det, UInt_t fs){
  std::string name = GetListName(det,fs);
  name.append(fCalGraphName.c_str());
  return name.c_str();
}

const char *TSharcName::GetMulGraphName(UInt_t det, UInt_t fs){
  std::string name = GetListName(det,fs);
  name.append(fMulGraphName.c_str());
  return name.c_str();
}

// Calibrated data objects 
const char *TSharcName::GetKinMatName(Int_t det, Int_t fs){
  std::string name = GetListName(det,fs);
  name.append(fKinMatName.c_str());
  return name.c_str();
}

const char *TSharcName::GetResMatName(Int_t det, Int_t fs){
  std::string name = GetListName(det,fs);
  name.append(fResMatName.c_str());
  return name.c_str();
}

const char *TSharcName::GetExcMatName(Int_t det, Int_t fs){
  std::string name = GetListName(det,fs);
  name.append(fExcMatName.c_str());
  return name.c_str();
}

const char *TSharcName::GetExcSpecName(Int_t det, Int_t fs, Int_t bs){
  std::string name = GetListName(det,fs);
  name.append(fExcSpecName.c_str());
  return name.c_str();
}

const char *TSharcName::GetListName(Int_t det, Int_t fs, Int_t bs){

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
