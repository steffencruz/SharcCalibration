#include "TCalibrate.h"

#include <TDataManager.h>
#include <TFileManager.h>
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
//  flag = true; ///////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  if(flag)
//     printf("{TCalibrate} MAKE CHARGE MATRIX = %s !\n",flag ? "true" : "false");

  fChgMats = flag;
  fRunCal = false;
  fSrcCal = true;
  //other stuff
}

void TCalibrate::Print(Option_t *opt) { }
void TCalibrate::Clear(Option_t *opt) { }


void TCalibrate::DeltaCal(const char *ifname) {
// STEPS REQUIRED FOR BOTH RUN & SRC DATA

  if(fChgMats)
     printf("{TCalibrate} A file will be read in, so charge matrix will not be reproduced.\n");
  else if(!ifname || !InitDeltaCal(ifname)){
     printf("{TCalibrate} Warning :  Bad input file, '%s'... You're better than this.\n",ifname);
     return;
  }

  if(fRunCal){
     Bool_t flag;
    if(fChgMats) 
       flag = OpenCalibration(ifname);
    //TList *list = TObjectManager::Get()->GetMasterList();
    //list->Print();
    //list->ls();
    if(!flag)
      return;
    GetCentroidsFromData("run");
    GetEnergiesFromInput("run");
 //   ProduceCalGraphs("run");
  }

  if(fSrcCal){
    GetCentroidsFromData("src");
    GetEnergiesFromInput("src");
 //   ProduceCalGraphs("src");
  }
//  if(fRunCal && fSrcCal)  dm->CombineGraphs(); // by default merges all TGraphErrors inside the DET FS BS list.

// FROM THIS POINT ON WE DON'T DEAL WITH RUN & SRC DATA, JUST COMBINED DATA
  // fit calgraphs
  // extract coefficients
  // write coefficients
  // profit

 // looping over DET FS and BS happens at every step. As long as the objects have been created and exist in TObjectManager's lists, TDataManager's FillHists function will locate and fill the plots.
  
}

void TCalibrate::GetCentroidsFromData(Option_t *opt){

  TDataManager *dm = TDataManager::Get();
  /////////////////////////////////// NEEDS ONLY TO BE DONE ONCE FOR A DATA SET /////////////////////////////////
  // 1.  Make charge matrices
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       CreateCalObject(TSharcFormat::Get()->GetChgMatName(),DET,FS); // creates the empty charge matrices
  dm->FillChargeMats();                            // fills the charge matrices

  FinishDeltaCal(); 
  
  // 2.  Make charge spectra
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
        for(int BS=0;BS<48;BS++)
         dm->MakeChargeSpectrum(DET,FS,BS,opt);
  
  FinishDeltaCal(); 
  
  // 3.  Fit charge spectra
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
        for(int BS=0;BS<48;BS++)
         dm->FitChargeSpectrum(DET,FS,BS,opt);
  
  FinishDeltaCal(); 
  
  // 4.  Extract charge centroids
  for(int DET=5;DET<=8;DET++)
     CreateCalObject(TSharcFormat::Get()->GetCentMatName(),DET); // creates the empty centroid matrices
  
  for(int DET=5;DET<=8;DET++)
     dm->MakeCentroidMat(DET);  
  
  FinishDeltaCal(); 

  return;
}

void TCalibrate::GetEnergiesFromInput(Option_t *opt){ 

  TDataManager *dm = TDataManager::Get();
   /////////////////////////////////// NEEDS TO BE DONE FOR EACH NEW PARAMETER SET ///////////////////////////////////
   // calculate corresponding energies
   for(int DET=5;DET<=8;DET++)
     CreateCalObject(TSharcFormat::Get()->GetCalcMatName(),DET); // creates the empty calculated energy matrices
   
   for(int DET=5;DET<=8;DET++)
     dm->MakeCalcEnergyMat(DET,opt);   // in addition to the above comments about TH2F, we can set the bin error to be the centroid error ... noooooice 

  FinishDeltaCal(); 
  return;
  
}

void TCalibrate::ProduceCalGraphs(Option_t *opt){
  
printf("\n\nHERE 1\n");  
  TDataManager *dm = TDataManager::Get();
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       CreateCalObject(TSharcFormat::Get()->GetCalGraphName(),DET,FS);
printf("\n\nHERE 2\n");  
  // make calgraphs
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       dm->MakeCalGraph(DET,FS,"pol1");


printf("\n\nHERE 3\n");  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       CreateCalObject(TSharcFormat::Get()->GetMulGraphName(),DET,FS);
  
printf("\n\nHERE 4\n");  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       dm->CombineGraphs(DET,FS,"pd"); // combine proton and deuteron points into a single graph

  FinishDeltaCal(); 
  return;
}

Bool_t TCalibrate::OpenCalibration(const char* cfname){

  TObjectManager *om = TObjectManager::Get();

  if(om->GetMasterList()->GetEntries()>0){
     printf("{TCalibrate} Warning :  There are already calibration objects. Calibration data may be loaded only when there are none.\n");
     return false;
  }

  TSharcInput *si = TSharcInput::Get();
  std::string siname = si->GetName();
  if(siname.length()){
    printf("{TCalibrate} Warning :  There is already a TSharcInput instance, '%s'. Another calibration cannot be loaded right now.\n",si->GetName());
    return false;
  }
  
  return TFileManager::ReadFile(cfname);
}


void TCalibrate::FinishDeltaCal() {

  TFileManager::WriteFile(); // uses TSharcName output file convention and writes TObjectManager's master list
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

void TCalibrate::CreateCalObject(const char *objname, Int_t DET, Int_t FS) {
  // the looping functionality doesn't work because th epointers go out of scope.

  TObjectManager *om = TObjectManager::Get();
  TSharcFormat *sf = TSharcFormat::Get();
  TObject *obj;
  if(DET>=0 && FS>=0){
    if(!om->GetObject(sf->GetListName(DET,FS)))
       om->CreateList(sf->GetListName(DET,FS));
    obj = sf->CreateObject(objname,DET,FS);
    om->AddObjectToList(obj,sf->GetListName(DET,FS));
  } else if(DET>=0){
    if(!om->GetObject(sf->GetListName(DET)))
      om->CreateList(sf->GetListName(DET));
    obj = sf->CreateObject(objname,DET);
    om->AddObjectToList(obj,sf->GetListName(DET));
  }

  printf("-> I Just made \t%s\t :]\n",obj->GetName());
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

