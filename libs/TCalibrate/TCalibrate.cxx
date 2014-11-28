#include "TCalibrate.h"

ClassImp(TCalibrate);

TCalibrate::TCalibrate() { }

TCalibrate::~TCalibrate() { }

void TCalibrate::Print() { }

void TCalibrate::Clear() { }


void TCalibrate::DeltaCal() {
// STEPS REQUIRED FOR BOTH RUN & SRC DATA
  // Make charge matrices 
  CreateCalObjects(TSharcFormat::GetChgMatName()); // creates the empty charge matrices
  fDataManager->MakeChargeMatrices();              // fills the charge matrices
  // Make charge spectra
  fDataManager->MakeChargeSpectra();
  // Fit charge spectra
  fDataManager->FitChargeSpectra();
  // extract charge centroids
  fDataManager->MakeCentroidMats(); // TH2Fs are probably overkill for this purpose since there is a lot of overhead with TH2Fs [ = time + memory ]
  // calculate corresponding energies
  fDataManager->MakeEnergyMats();   // in addition to the above comments about TH2F, we can set the bin error to be the centroid error ... noooooice 
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
  
  return 0;
}

void TCalibrate::SetUpCalObjects(const char *objtype, Int_t det_min, Int_t det_max, Int_t fs_min, Int_t fs_max) {

  if(det_min<0) det_min = 1 ;
  if(det_max<0) det_max = 16;
  if(fs_min<0)  fs_min  = 0 ;
  if(fs_max<0)  fs_max  = 24;
  if(bs_min<0)  bs_min  = 0 ;
  if(bs_max<0)  bs_max  = 48;

  TObject *obj;
  for(int DET=det_min; DET<det_max; DET++){
    for(int FS=fs_min; FS<fs_max; FS++){

      fObjectManager->CreateList(TSharcName::GetListName(DET,FS));
      obj = fSharcFormat->CreateObject(objtype,DET,FS);
      fObjectManager->AddObjectToList(obj,TSharcName::GetListName(DET,FS));
      // done

/*
// I feel as though TObjectManager should put objectsthat it creates straight into lists 
      fObjectManager->CreateCalGraph(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateMulGraph(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateKinMat(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateResMat(TSharcName::GetChgMatName(DET,FS));
      fObjectManager->CreateExcMat(TSharcName::GetChgMatName(DET,FS));
*/
/*
      for(int BS=bs_max; BS<bs_max; BS++){
        fObjectManager->CreateChgSpec(TSharcName::GetChgMatName(DET,FS,BS));
        fObjectManager->CreateExcSpec(TSharcName::GetChgMatName(DET,FS,BS));
      }
*/
  }
  }

}

