#include "TCalibrate.h"

ClassImp(TCalibrate);

TCalibrate::TCalibrate() { }

TCalibrate::~TCalibrate() { }

void TCalibrate::Print() { }

void TCalibrate::Clear() { }


void TCalibrate::DeltaCal() {
// STEPS REQUIRED FOR BOTH RUN & SRC DATA
  // Make charge matrices
  fDataManager->MakeChargeMatrices();
  // Make charge spectra
  fDataManager->MakeChargeSpectra();
  // Fit charge spectra
  fDataManager->FitChargeSpectra();
  // extract charge centroids
  fDataManager->MakeCentroidMats(); // TH2Fs are probably overkill for this purpose since there is a lot of overhead with TH2Fs [ = time + memory ]
  // calculate corresponding energies
  fDataManager->MakeEnergyMats();   // same issue as above. Use array, or simpler structure?? TH2F is nice because we can look at it easily
  // make calgraphs

// FROM THIS POINT ON WE DON'T DEAL WITH RUN & SRC DATA, JUST COMBINED DATA
  // fit calgraphs
  // extract coefficients
  // write coefficients
  // profit

 // looping over DET FS and BS happens at every step. As long as the objects have been created and exist in TObjectManager's lists, TDataManager's FillHists function will locate and fill the plots.
  
}

void TCalibrate::FinishDeltaCal() {


  TFileWriter::WriteFile(); // uses TSharcName output file convention and writes TObjectManager's master list
}

UInt_t TCalibrate::InitDeltaCal(const char *ifname) {

  h = new TH2F(chgmatname,chgmatname,48,0,48,16000,0,16000);
  fInputFile = ifname;

  fSharcInput = TSharcInput::Get();
  if(!fSharcInput->ParseInputFile(fInputFile)){
    printf("{TCalibrate} Error : Input file '%s' not read.",fInputFile);
    return 1;
  }

  fObjectManager = TObjectManager::Get();
  fDataManager = TDataManager::Get();
  CreateCalObjects(); // makes all necessary objects
  
  return 0;
}

void TCalibrate::CreateCalObjects(Int_t det_min, Int_t fs_min, Int_t bs_min, Int_t det_max, Int_t fs_max, Int_t bs_max) {

  if(det_min<0) det_min = 1 ;
  if(det_max<0) det_max = 16;
  if(fs_min<0)  fs_min  = 0 ;
  if(fs_max<0)  fs_max  = 24;
  if(bs_min<0)  bs_min  = 0 ;
  if(bs_max<0)  bs_max  = 48;

//  std::string lName;
//  TH2F *hChgMat;
  for(int DET=det_min; DET<det_max; DET++){
    for(int FS=fs_min; FS<fs_max; FS++){
/*
      lName.assign(TSharcName::GetListName(DET,FS));
      fObjectManager->CreateList(lName.c_str());
      hChgMat = fObjectManager->CreateChgMat(DET,FS);
      fObjectManager->AddObjectToList(hChgMat,lName.c_str());
*/   

// I feel as though TObjectManager should put objectsthat it creates straight into lists 
      fObjectManager->CreateList(TSharcName::GetListName(DET,FS));
      fObjectManager->CreateChgMat(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateCalGraph(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateMulGraph(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateKinMat(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateResMat(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateExcMat(TSharcName::GetChgMatName(DET,FS));

      for(int BS=bs_max; BS<bs_max; BS++){
        fObjectManager->CreateChgSpec(TSharcName::GetChgMatName(DET,FS,BS));
        fObjectManager->CreateExcSpec(TSharcName::GetChgMatName(DET,FS,BS));
      }
    }
  }

}

