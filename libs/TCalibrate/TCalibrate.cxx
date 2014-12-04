#include "TCalibrate.h"

#include <TDataManager.h>
#include <TFileWriter.h>
#include <TFitInfo.h>
#include <TFitManager.h>
#include <TObjectManager.h>
#include <TSharcInput.h>
#include <TSharcFormat.h>

#include "TSharcAnalysis.h"

ClassImp(TCalibrate)

TCalibrate *TCalibrate::fCalibrate = 0;

TCalibrate::TCalibrate() {
  fRunCal = true;
  fSrcCal = true;
}

TCalibrate::~TCalibrate() { }


TCalibrate *TCalibrate::Get(){
  if(!fCalibrate)
    fCalibrate = new TCalibrate(true);
  return fCalibrate;
}

TCalibrate::TCalibrate(Bool_t flag){
  Clear();

  fRunCal = true;
  fSrcCal = true;
  //other stuff
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
  //   int DET = 5;
  //   int FS = 12;
     /////////////////////////////////// NEEDS ONLY TO BE DONE ONCE FOR A DATA SET /////////////////////////////////
     // 1.  Make charge matrices
     for(int DET=5;DET<=8;DET++)
        for(int FS=0;FS<24;FS++)
          CreateCalObjects(TSharcFormat::Get()->GetChgMatName(),DET,DET,FS,FS); // creates the empty charge matrices
     
     dm->FillChargeMats();                            // fills the charge matrices
    FinishDeltaCal(); 
     // 2.  Make charge spectra
     for(int DET=5;DET<=8;DET++)
        for(int FS=0;FS<24;FS++)
           for(int BS=0;BS<4;BS++)
            dm->MakeChargeSpectrum(DET,FS,BS);
    FinishDeltaCal(); 
     
     // 3.  Fit charge spectra
     for(int DET=5;DET<=8;DET++)
        for(int FS=0;FS<24;FS++)
           for(int BS=0;BS<4;BS++)
            dm->FitChargeSpectrum(DET,FS,BS);
    FinishDeltaCal(); 
     
     // 4.  Extract charge centroids
     printf("I AM HERE\n\n\n\n\n");
     for(int DET=5;DET<=8;DET++)
        CreateCalObjects(TSharcFormat::Get()->GetCentMatName(),DET ,DET); // creates the empty centroid matrices
     for(int DET=5;DET<=8;DET++)
        dm->MakeCentroidMat(DET);  
    FinishDeltaCal(); 

    /////////////////////////////////// NEEDS TO BE DONE FOR EACH NEW PARAMETER SET ///////////////////////////////////
    // calculate corresponding energies
     CreateCalObjects(TSharcFormat::Get()->GetCalcMatName(),DET,DET); // creates the empty cacculated energy matrices
//     for(int DET=5;DET<=8;DET++)
       dm->MakeCalcEnergyMat(DET);   // in addition to the above comments about TH2F, we can set the bin error to be the centroid error ... noooooice 
     // make calgraphs
//     for(int DET=5;DET<=8;DET++)
//        for(int FS=0;FS<24;FS++)
          dm->MakeCalGraph(DET,FS,"pol1");
  
//     for(int DET=5;DET<=8;DET++)
//        for(int FS=0;FS<24;FS++)
          dm->CombineGraphs(DET,FS,"pd"); // combine proton and deuteron points into a single graph
  }

  FinishDeltaCal();
//  if(fRunCal && fSrcCal)  dm->CombineGraphs(); // by default merges all TGraphErrors inside the DET FS BS list.

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
  
  TSharcInput *si = TSharcInput::Get();
  if(!si->InitSharcInput(ifname)){
    printf("{TCalibrate} Error : Input file '%s' not read.\n",ifname);
    return false;
  }
  fInputFile = ifname;

  TSharcAnalysis::SetTarget(si->GetTargetThickness());
//  TSharcAnalysis::SetTargetMaterial(si->GetTargetMaterial());
//  TSharcAnalysis::SetSharcOffset(si->GetPosOffs());

  return true;
}

void TCalibrate::CreateCalObjects(const char *objname, Int_t det_min, Int_t det_max, Int_t fs_min, Int_t fs_max) {
  // the looping functionality doesn't work because th epointers go out of scope.

  if(det_min<0) det_min = 1 ;
  if(det_max<0) det_max = 16;
  if(fs_min<0)  fs_min  = 0 ;
  if(fs_max<0)  fs_max  = 24;

  TObjectManager *om = TObjectManager::Get();
  TObject *obj;
  const char *objtype =  TSharcFormat::Get()->GetChgMatName();
  for(int DET=det_min; DET<=det_max; DET++){
    for(int FS=fs_min; FS<=fs_max; FS++){
      om->CreateList(TSharcFormat::Get()->GetListName(DET,FS));
      obj = TSharcFormat::Get()->CreateObject(objname,DET,FS);
      om->AddObjectToList(obj,TSharcFormat::Get()->GetListName(DET,FS));
      // done
    }
  }

}

