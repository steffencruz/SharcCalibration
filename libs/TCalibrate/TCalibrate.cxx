#include "TCalibrate.h"

#include <TDataManager.h>
#include <TFileWriter.h>
#include <TFitInfo.h>
#include <TFitManager.h>
#include <TObjectManager.h>
#include <TSharcInput.h>
#include <TSharcFormat.h>

ClassImp(TCalibrate)

TCalibrate::TCalibrate() {
  fRunCal = true;
  fSrcCal = true;
}

TCalibrate::~TCalibrate() { }


TCalibrate *TCalibrate::Get(){
  if(fCalibrate)
     return fCalibrate;
  else TCalibrate();
}

void TCalibrate::Print(Option_t *opt) { }
void TCalibrate::Clear(Option_t *opt) { }


void TCalibrate::DeltaCal(const char *ifname) {
// STEPS REQUIRED FOR BOTH RUN & SRC DATA
  if(!ifname || !InitDeltaCal(ifname)){
     printf("{TCalibrate} Warning :  Bad input file, '%s'... You're better than this.\n",ifname);
     return;
  }
  TDataManager *dm = TDataManager::Get();
  if(fRunCal){
     /////////////////////////////////// NEEDS ONLY TO BE DONE ONCE FOR A DATA SET /////////////////////////////////
     // 1.  Make charge matrices 
     CreateCalObjects(TSharcFormat::GetChgMatName()); // creates the empty charge matrices
     dm->MakeChargeMats();              // fills the charge matrices
     // 2.  Make charge spectra
     dm->MakeChargeSpectra();
     // 3.  Fit charge spectra
     dm->FitChargeSpectra();
     // 4.  Extract charge centroids
     CreateCalObjects(TSharcFormat::GetCentMatName()); // creates the empty centroid matrices
     for(int DET=5;DET<8;DET++)
        dm->MakeCentroidMat(DET);  

    /////////////////////////////////// NEEDS TO BE DONE FOR EACH NEW PARAMETER SET ///////////////////////////////////
    // calculate corresponding energies
     CreateCalObjects(TSharcFormat::GetCalcMatName()); // creates the empty cacculated energy matrices
     for(int DET=5;DET<8;DET++)
       dm->MakeCalcEnergyMat(DET);   // in addition to the above comments about TH2F, we can set the bin error to be the centroid error ... noooooice 
     // make calgraphs
     dm->MakeCalGraphs();
  }

  if(fRunCal && fSrcCal)  dm->CombineGraphs(); // by default merges all TGraphErrors inside the DET FS BS list.

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

Bool_t TCalibrate::InitDeltaCal(const char *ifname) {

  if(!TSharcInput::Get()->InitSharcInput(ifname)){
    printf("{TCalibrate} Error : Input file '%s' not read.",ifname);
    return false;
  }
  fInputFile = ifname;
  
  return true;
}

void TCalibrate::CreateCalObjects(const char *objtype, Int_t det_min, Int_t det_max, Int_t fs_min, Int_t fs_max) {

  if(det_min<0) det_min = 1 ;
  if(det_max<0) det_max = 16;
  if(fs_min<0)  fs_min  = 0 ;
  if(fs_max<0)  fs_max  = 24;

  TObjectManager *om = TObjectManager::Get();
  TObject *obj;
  for(int DET=det_min; DET<det_max; DET++){
    for(int FS=fs_min; FS<fs_max; FS++){

      om->CreateList(TSharcName::GetListName(DET,FS));
      obj = TSharcFormat::CreateObject(objtype,DET,FS);
      om->AddObjectToList(obj,TSharcName::GetListName(DET,FS));
      // done
    }
  }

}

