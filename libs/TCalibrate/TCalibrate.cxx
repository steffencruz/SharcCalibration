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
  printf("{TCalibrate} Don't use this instantiation function. Use TCalibrate::Get() instead.\n");
}

TCalibrate::~TCalibrate() { }


TCalibrate *TCalibrate::Get(){
  if(!fCalibrate)
    fCalibrate = new TCalibrate(true);
  return fCalibrate;
}

TCalibrate::TCalibrate(Bool_t flag){

  Clear();
  //other stuff .. ?
}

void TCalibrate::Print(Option_t *opt) { 

  printf(DCYAN"\nTCalibrate:"RESET_COLOR"\n");

  printf(DYELLOW"\n\tCal flags :-"RESET_COLOR"\n");
  printf("\tfSharcInputSet      status : %s\n",fSharcInputFlag        ? "set" : "not set");
  printf("\tfRunCalibration     status : %s\n",fRunCalibrationFlag    ? "enabled" : "disabled");
  printf("\tfSrcCalibration     status : %s\n",fSrcCalibrationFlag    ? "enabled" : "disabled");
  
  printf(DYELLOW"\n\tRun flags :-"RESET_COLOR"\n");
  printf("\tfRunChgMatsFlag     status : %s\n",fRunChgMatsFlag    ? "complete!" : "unfinished..");
  printf("\tfRunChgSpecsFlag    status : %s\n",fRunChgSpecsFlag   ? "complete!" : "unfinished..");
  printf("\tfRunChgFitsFlag     status : %s\n",fRunChgFitsFlag    ? "complete!" : "unfinished..");
  printf("\tfRunCentMatsFlag    status : %s\n",fRunCentMatsFlag   ? "complete!" : "unfinished..");
  printf("\tfRunCalcMatsFlag    status : %s\n",fRunCalcMatsFlag   ? "complete!" : "unfinished..");
  printf("\tfRunCalGraphsFlag   status : %s\n",fRunCalGraphsFlag  ? "complete!" : "unfinished..");
  printf("\tfRunMulGraphsFlag   status : %s\n",fRunMulGraphsFlag  ? "complete!" : "unfinished..");

  printf(DYELLOW"\n\tRun flags :-"RESET_COLOR"\n");
  printf("\tfSrcChgMatsFlag     status : %s\n",fSrcChgMatsFlag    ? "complete!" : "unfinished..");
  printf("\tfSrcChgSpecsFlag    status : %s\n",fSrcChgSpecsFlag   ? "complete!" : "unfinished..");
  printf("\tfSrcChgFitsFlag     status : %s\n",fSrcChgFitsFlag    ? "complete!" : "unfinished..");
  printf("\tfSrcCentMatsFlag    status : %s\n",fSrcCentMatsFlag   ? "complete!" : "unfinished..");
  printf("\tfSrcCalcMatsFlag    status : %s\n",fSrcCalcMatsFlag   ? "complete!" : "unfinished..");
  printf("\tfSrcCalGraphsFlag   status : %s\n",fSrcCalGraphsFlag  ? "complete!" : "unfinished..");
  printf("\tfSrcMulGraphsFlag   status : %s\n",fSrcMulGraphsFlag  ? "complete!" : "unfinished..");

  return;
}

void TCalibrate::Clear(Option_t *opt) {
 
  fSharcInputFlag    = false ;
  fRunCalibrationFlag= false ;
  fSrcCalibrationFlag= false ;

  fRunChgMatsFlag    = false ;
  fRunChgSpecsFlag   = false ;
  fRunChgFitsFlag    = false ;
  fRunCentMatsFlag   = false ;
  fRunCalcMatsFlag   = false ;
  fRunCalGraphsFlag  = false ;
  fRunMulGraphsFlag  = false ;
  fSrcChgMatsFlag    = false ;
  fSrcChgSpecsFlag   = false ;
  fSrcChgFitsFlag    = false ;
  fSrcCentMatsFlag   = false ;
  fSrcCalcMatsFlag   = false ;
  fSrcCalGraphsFlag  = false ;
  fSrcMulGraphsFlag  = false ;
     
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

void TCalibrate::LoadCal(const char *name){

  if(!OpenCalibration(name))
     return;

  fSharcInputFlag       = true;
  fRunCalibrationFlag   = TSharcInput::Get()->GetRunDeltaCalFlag(); 
  fSrcCalibrationFlag   = TSharcInput::Get()->GetSrcDeltaCalFlag(); 
  
  TSharcFormat *sf = TSharcFormat::Get();
  
  fRunChgMatsFlag      = CountCalObjects(  sf->GetChgMatName("Run"));
  fRunChgSpecsFlag     = CountCalObjects( sf->GetChgSpecName("Run"));
  fRunChgFitsFlag      = CountCalObjects( sf->GetFitInfoName("Run"));
  fRunCentMatsFlag     = CountCalObjects( sf->GetCentMatName("Run"));
  fRunCalcMatsFlag     = CountCalObjects( sf->GetCalcMatName("Run"));
  fRunCalGraphsFlag    = CountCalObjects(sf->GetCalGraphName("Run"));
  fRunMulGraphsFlag    = CountCalObjects(sf->GetMulGraphName("Run"));

  fSrcChgMatsFlag      = CountCalObjects(  sf->GetChgMatName("Src"));
  fSrcChgSpecsFlag     = CountCalObjects( sf->GetChgSpecName("Src"));
  fSrcChgFitsFlag      = CountCalObjects( sf->GetFitInfoName("Src"));
  fSrcCentMatsFlag     = CountCalObjects( sf->GetCentMatName("Src"));
  fSrcCalcMatsFlag     = CountCalObjects( sf->GetCalcMatName("Src"));
  fSrcCalGraphsFlag    = CountCalObjects(sf->GetCalGraphName("Src"));
  fSrcMulGraphsFlag    = CountCalObjects(sf->GetMulGraphName("Src"));
}

void TCalibrate::SaveCal() {

  TFileManager::WriteFile(); // uses TSharcName output file convention and writes TObjectManager's master list
}

Bool_t TCalibrate::InitDeltaCal(const char *ifname) {
  
  TSharcInput *si = TSharcInput::Get();
  if(!si->InitSharcInput(ifname))
    return false;

  TSharcAnalysis::SetTarget(si->GetTargetThickness());
//  TSharcAnalysis::SetTargetMaterial(si->GetTargetMaterial());
//  TSharcAnalysis::SetSharcOffset(si->GetPosOffs());

  return true;
}

void TCalibrate::DeltaCal(const char *ifname) {

  if(!fSharcInputFlag){
     if(!InitDeltaCal(ifname)){
       printf("{TCalibrate} Warning :  Bad input file, '%s'... You're better than this.\n",ifname);
       return;
     }
  }
  
  if(fRunCalibrationFlag){
    const char *failedat = Calibrate("Run");
    if(failedat)
      printf(DRED"{TCalibrate} FAIL : ["DMAGENTA"Run"DRED"] Calibration aborted at "DBLUE"%s"DRED".. Fix the problems before proceeding."RESET_COLOR"\n",failedat);
    else 
      printf(DGREEN"{TCalibrate} Message : ["DCYAN"Run"DGREEN"] Calibration graphs have been successfully produced."RESET_COLOR"\n");

  }
  if(fRunCalibrationFlag){
    const char *failedat = Calibrate("Src");
    if(failedat)
      printf(DRED"{TCalibrate} FAIL : ["DMAGENTA"Src"DRED"] Calibration aborted at "DBLUE"%s"DRED".. Fix the problems before proceeding."RESET_COLOR"\n",failedat);
    else 
      printf(DGREEN"{TCalibrate} Message : ["DCYAN"Src"DGREEN"] Calibration graphs have been successfully produced."RESET_COLOR"\n");
  }

// FROM THIS POINT ON WE DON'T DEAL WITH RUN & SRC DATA, JUST COMBINED DATA
  // fit calgraphs
  // extract coefficients
  // write coefficients
  // profit
  
}

const char *TCalibrate::Calibrate(Option_t *opt){
  
  Print();
  if(!fRunChgMatsFlag)
    if(!MakeChargeMatrices(opt))
      return "MakeChargeMatrices";
  SaveCal();
  
  Print();
  if(!fRunChgSpecsFlag || !fRunChgFitsFlag)
    if(!GetCentroidsFromData(opt))
      return "GetCentroidsFromData";
  SaveCal(); 
  
  Print();
  if(!fRunCentMatsFlag)
    if(!PutCentroidsInMatrix(opt))
      return "PutCentroidsInMatrix";
  SaveCal(); 
  
  Print();
  if(!fRunCentMatsFlag)
    if(!PutEnergiesInMatrix(opt))
      return "PutEnergiesInMatrix";
  SaveCal(); 

  return 0;
}

Bool_t TCalibrate::MakeChargeMatrices(Option_t *opt){

  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Charge matrices will be produced using input trees."RESET_COLOR"\n",opt);
  /////////////////////////////////// NEEDS ONLY TO BE DONE ONCE FOR A DATA SET /////////////////////////////////
  // 1.   Make charge matrices

  if(!TDataManager::Get()->SetChain(opt)){
    printf("{TCalibrate} Warning : Charge matrices have not been made.\n");
    return false;
  }
  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       CreateCalObject(TSharcFormat::Get()->GetChgMatName(),opt,DET,FS); // creates the empty charge matrices
 
  TDataManager::Get()->FillChargeMats();
 
  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Charge matrices have been produced using input trees."RESET_COLOR"\n",opt);
  return true;
}

Bool_t TCalibrate::GetCentroidsFromData(Option_t *opt){
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Charge spectra will be created and fitted using charge matrices."RESET_COLOR"\n",opt);
  /////////////////////////////////// THIS NEEDS TO BE DONE UNTIL CENTROIDS ARE SATISFACTORY /////////////////////////////////
  // 2.   Make charge spectra
  // 3.   Fit charge spectra
  TDataManager *dm = TDataManager::Get();
  
  for(int DET=5;DET<=5;DET++)
     for(int FS=10;FS<14;FS++)
        for(int BS=0;BS<6;BS++)
         dm->MakeChargeSpectrum(DET,FS,BS,opt);
  
  for(int DET=5;DET<=5;DET++)
     for(int FS=10;FS<14;FS++)
        for(int BS=0;BS<6;BS++)
         dm->FitChargeSpectrum(DET,FS,BS,opt);

  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Charge spectra have been created and fitted using charge matrices."RESET_COLOR"\n",opt);
  return true;
}


Bool_t TCalibrate::PutCentroidsInMatrix(Option_t *opt){
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Centroids will be extracted from charge matrix fits."RESET_COLOR"\n",opt);
  /////////////////////////////////// THIS NEEDS TO BE DONE WHEN CENTROIDS ARE REPRODUCED /////////////////////////////////
  // 4.  Extract charge centroids
  TDataManager *dm = TDataManager::Get();
  
  std::vector<std::string> ions = TSharcInput::Get()->GetIons(opt);
  if(ions.size()==0){
    printf("{TCalibrate} Warning :  There are no %s ions available for calibration. Cannot continue.\n",opt);
    return false;
  }

  for(int DET=5;DET<=5;DET++)
    for(int i=0; i<ions.size(); i++)
      CreateCalObject(TSharcFormat::Get()->GetCentMatName(),ions.at(i).c_str(),DET); // creates the empty centroid matrices
  
  for(int DET=5;DET<=5;DET++)
    for(int i=0; i<ions.size(); i++)
      dm->MakeCentroidMat(ions.at(i).c_str(),DET,opt); // fills the empty centroid matrices
  
  SaveCal(); 
  
  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Centroids have been extracted from charge matrix fits."RESET_COLOR"\n",opt);
  return true;
}

Bool_t TCalibrate::PutEnergiesInMatrix(Option_t *opt){ 
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Measured energies will be calculated from input parameters."RESET_COLOR"\n",opt);

  /////////////////////////////////// NEEDS TO BE DONE FOR EACH NEW PARAMETER SET ///////////////////////////////////
  // calculate corresponding energies
  TDataManager *dm = TDataManager::Get();

  std::vector<std::string> ions = TSharcInput::Get()->GetIons(opt);
  if(ions.size()==0){
    printf("{TCalibrate} Warning :  There are no %s ions available for calibration. Cannot continue.\n",opt);
    return false;
  }

  for(int DET=5;DET<=5;DET++)
    for(int i=0; i<ions.size(); i++)
      CreateCalObject(TSharcFormat::Get()->GetCalcMatName(),ions.at(i).c_str(),DET); // creates the empty centroid matrices
  
  for(int DET=5;DET<=5;DET++)
    for(int i=0; i<ions.size(); i++)
      dm->MakeCalcEnergyMat(ions.at(i).c_str(),DET,opt); // fills the empty centroid matrices

  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Measured energies have been calculated from input parameters."RESET_COLOR"\n",opt);
  return true;
  
}

Bool_t TCalibrate::ProduceCalGraphs(Option_t *opt){
  
  std::vector<std::string> ions = TSharcInput::Get()->GetIons(opt);
  if(ions.size()==0){
    printf("{TCalibrate} Warning :  There are no %s ions available for calibration. Cannot continue.\n",opt);
    return false;
  }
  
printf("\n\nHERE 1\n");  
  TDataManager *dm = TDataManager::Get();
  for(int DET=5;DET<=8;DET++)
    for(int FS=0;FS<24;FS++)
      for(int i=0; i<ions.size(); i++)
        CreateCalObject(TSharcFormat::Get()->GetCalGraphName(),ions.at(i).c_str(),DET,FS); // creates the empty centroid matrices

printf("\n\nHERE 2\n");  
  // make calgraphs
  for(int DET=5;DET<=8;DET++)
    for(int FS=0;FS<24;FS++)
      for(int i=0; i<ions.size(); i++)
        dm->MakeCalGraph(TSharcFormat::Get()->GetCalcMatName(ions.at(i).c_str()),DET,FS); // "fit pol1"

/*
printf("\n\nHERE 3\n");  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       CreateCalObject(TSharcFormat::Get()->GetMulGraphName(),DET,FS);
  
printf("\n\nHERE 4\n");  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       dm->CombineGraphs(DET,FS,"pd"); // combine proton and deuteron points into a single graph
*/
  SaveCal(); 
  return true;
}

void TCalibrate::CreateCalObject(const char *objname, Option_t *opt, Int_t DET, Int_t FS) {

  TObjectManager *om = TObjectManager::Get();
  TSharcFormat *sf = TSharcFormat::Get();
  TObject *obj;
  if(DET>=0 && FS>=0){
    om->GetList(sf->GetListName(DET,FS));
    obj = sf->CreateObject(objname,opt,DET,FS);
    om->AddObjectToList(obj,sf->GetListName(DET,FS));
  } else if(DET>=0){
    om->GetList(sf->GetListName(DET));
    obj = sf->CreateObject(objname,opt,DET);
    om->AddObjectToList(obj,sf->GetListName(DET));
  }

//  printf("-> I Just made \t%s\t :]\n",obj->GetName());
}

Bool_t TCalibrate::CountCalObjects(const char *name) {

  TObjectManager *om = TObjectManager::Get();
  UInt_t n = TObjectManager::Get()->CountObjectsOfType(name);

  printf("{TCalibrate} Message :  Counted %i objects of type '%32s'\t [flag => %s].\n",n,name,n>0 ? "true":"false");
  return n>0;
}
/*
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
*/
