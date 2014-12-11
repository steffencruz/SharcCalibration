#include "TCalibrate.h"

#include <TROOT.h>

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

  printf("\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
  printf(DCYAN"\nTCalibrate:"RESET_COLOR"");

  printf(DYELLOW"\n\tCal flags :-"RESET_COLOR"\n");
  printf("\tfSharcInputSet      status : %s\n",GetFlag("SharcInput")           ? "set" : "not set");
  printf("\tfRunCalibration     status : %s\n",GetFlag("DeltaCalStyle","Run")  ? "enabled" : "disabled");
  printf("\tfSrcCalibration     status : %s\n",GetFlag("DeltaCalStyle","Src")  ? "enabled" : "disabled");
  
  printf(DYELLOW"\n\tRun flags :-"RESET_COLOR"\n");
  printf("\tfRunChgMatsFlag     status : %s\n",GetFlag("ChgMats"  ,"Run") ? "complete!" : "unfinished..");
  printf("\tfRunChgSpecsFlag    status : %s\n",GetFlag("ChgSpecs" ,"Run") ? "complete!" : "unfinished..");
  printf("\tfRunChgFitsFlag     status : %s\n",GetFlag("ChgFits"  ,"Run") ? "complete!" : "unfinished..");
  printf("\tfRunCentMatsFlag    status : %s\n",GetFlag("CentMats" ,"Run") ? "complete!" : "unfinished..");
  printf("\tfRunCalcMatsFlag    status : %s\n",GetFlag("CalcMats" ,"Run") ? "complete!" : "unfinished..");
  printf("\tfRunCalGraphsFlag   status : %s\n",GetFlag("CalGraphs","Run") ? "complete!" : "unfinished..");
  printf("\tfRunMulGraphsFlag   status : %s\n",GetFlag("MulGraphs","Run") ? "complete!" : "unfinished..");

  printf(DYELLOW"\n\tRun flags :-"RESET_COLOR"\n");
  printf("\tfSrcChgMatsFlag     status : %s\n",GetFlag("ChgMats"  ,"Src") ? "complete!" : "unfinished..");
  printf("\tfSrcChgSpecsFlag    status : %s\n",GetFlag("ChgSpecs" ,"Src") ? "complete!" : "unfinished..");
  printf("\tfSrcChgFitsFlag     status : %s\n",GetFlag("ChgFits"  ,"Src") ? "complete!" : "unfinished..");
  printf("\tfSrcCentMatsFlag    status : %s\n",GetFlag("CentMats" ,"Src") ? "complete!" : "unfinished..");
  printf("\tfSrcCalcMatsFlag    status : %s\n",GetFlag("CalcMats" ,"Src") ? "complete!" : "unfinished..");
  printf("\tfSrcCalGraphsFlag   status : %s\n",GetFlag("CalGraphs","Src") ? "complete!" : "unfinished..");
  printf("\tfSrcMulGraphsFlag   status : %s\n",GetFlag("MulGraphs","Src") ? "complete!" : "unfinished..");

  printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n\n");
  return;
}

void TCalibrate::Clear(Option_t *opt) {
  
  SetFlag("SharcInput","",false);
  SetFlag("CalibrationFlag","Run",false); 
  SetFlag("CalibrationFlag","Src",false); 
  
  SetFlag("ChgMats"  ,"Run",false);
  SetFlag("ChgSpecs" ,"Run",false);
  SetFlag("ChgFits"  ,"Run",false);
  std::vector<std::string> runions = TSharcInput::Get()->GetIons("Run");
  SetFlag("CentMats" ,"Run",false);
  SetFlag("CalcMats" ,"Run",false);
  SetFlag("CalGraphs","Run",false);
  SetFlag("MulGraphs","Run",false);
  
  SetFlag("ChgMats"  ,"Src",false);
  SetFlag("ChgSpecs" ,"Src",false);
  SetFlag("ChgFits"  ,"Src",false);
  std::vector<std::string> srcions = TSharcInput::Get()->GetIons("Src");
  SetFlag("CentMats" ,"Src",false);
  SetFlag("CalcMats" ,"Src",false);
  SetFlag("CalGraphs","Src",false);
  SetFlag("MulGraphs","Src",false);
 
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

  SetFlag("SharcInput","",true);
  SetFlag("DeltaCalStyle","Run",TSharcInput::Get()->GetRunDeltaCalFlag()); 
  SetFlag("DeltaCalStyle","Src",TSharcInput::Get()->GetSrcDeltaCalFlag()); 
  
  TSharcFormat *sf = TSharcFormat::Get();
  
  SetFlag("ChgMats"  ,"Run",ExistInFile(  sf->GetChgMatName("Run")));
  SetFlag("ChgSpecs" ,"Run",ExistInFile( sf->GetChgSpecName("Run")));
  SetFlag("ChgFits"  ,"Run",ExistInFile( sf->GetFitInfoName("Run")));
  std::vector<std::string> runions = TSharcInput::Get()->GetIons("Run");
  SetFlag("CentMats" ,"Run",ExistInFile( sf->GetCentMatName(runions.at(0).c_str())));
  SetFlag("CalcMats" ,"Run",ExistInFile( sf->GetCalcMatName(runions.at(0).c_str())));
  SetFlag("CalGraphs","Run",ExistInFile(sf->GetCalGraphName(runions.at(0).c_str())));
  SetFlag("MulGraphs","Run",ExistInFile(sf->GetMulGraphName(runions.at(0).c_str())));
  
  SetFlag("ChgMats"  ,"Src",ExistInFile(  sf->GetChgMatName("Src")));
  SetFlag("ChgSpecs" ,"Src",ExistInFile( sf->GetChgSpecName("Src")));
  SetFlag("ChgFits"  ,"Src",ExistInFile( sf->GetFitInfoName("Src")));
  std::vector<std::string> srcions = TSharcInput::Get()->GetIons("Src");
  SetFlag("CentMats" ,"Src",ExistInFile( sf->GetCentMatName(srcions.at(0).c_str())));
  SetFlag("CalcMats" ,"Src",ExistInFile( sf->GetCalcMatName(srcions.at(0).c_str())));
  SetFlag("CalGraphs","Src",ExistInFile(sf->GetCalGraphName(srcions.at(0).c_str())));
  SetFlag("MulGraphs","Src",ExistInFile(sf->GetMulGraphName(srcions.at(0).c_str())));
}

Bool_t TCalibrate::GetFlag(const char *flagname, Option_t *opt){
  std::string str = Form("f%s%sFlag",opt,flagname);
  return fCalFlags[str];
}

void TCalibrate::SetFlag(const char *flagname, Option_t *opt, Bool_t flag){
  std::string str = Form("f%s%sFlag",opt,flagname);
  fCalFlags[str] = flag;
  //printf("Set %s to be %s\t LOOK -> fCalFlags[%s] = %s\n",str.c_str(),flag?"true":"false",str.c_str(),fCalFlags[str]?"true":"false");
}

void TCalibrate::SaveCal() {

  TFileManager::WriteFile(); // uses TSharcName output file convention and writes TObjectManager's master list
}

Bool_t TCalibrate::InitDeltaCal(const char *ifname) {
  
  TSharcInput *si = TSharcInput::Get();
  if(!si->InitSharcInput(ifname))
    return false;

//  TSharcAnalysis::SetTarget(si->GetTargetThickness());
//  TSharcAnalysis::SetTargetMaterial(si->GetTargetMaterial());
//  TSharcAnalysis::SetSharcOffset(si->GetPosOffs());

  return true;
}

void TCalibrate::DeltaCal(const char *ifname) {

  if(!GetFlag("SharcInput")){
     if(!InitDeltaCal(ifname)){
       printf("{TCalibrate} Warning :  Bad input file, '%s'... You're better than this.\n",ifname);
       return;
     }
  }
  
  if(GetFlag("DeltaCalStyle","Run")){
    const char *failedat = CalibrateSeparate("Run");
    if(failedat)
      printf(DRED"{TCalibrate} FAIL : ["DMAGENTA"Run"DRED"] Calibration aborted at "DBLUE"%s"DRED".. Fix the problems before proceeding."RESET_COLOR"\n",failedat);
    else 
      printf(DGREEN"{TCalibrate} Message : ["DCYAN"Run"DGREEN"] Calibration graphs have been successfully produced."RESET_COLOR"\n");

  }
  if(GetFlag("DeltaCalStyle","Src")){
    const char *failedat = CalibrateSeparate("Src");
    if(failedat)
      printf(DRED"{TCalibrate} FAIL : ["DMAGENTA"Src"DRED"] Calibration aborted at "DBLUE"%s"DRED".. Fix the problems before proceeding."RESET_COLOR"\n",failedat);
    else 
      printf(DGREEN"{TCalibrate} Message : ["DCYAN"Src"DGREEN"] Calibration graphs have been successfully produced."RESET_COLOR"\n");
  }

  SaveCal();
  return;
// FROM THIS POINT ON WE DON'T DEAL WITH RUN & SRC DATA, JUST COMBINED DATA
  // fit calgraphs
  // extract coefficients
  // write coefficients
  // profit
  
}

const char *TCalibrate::CalibrateSeparate(Option_t *opt){
  
  if(!GetFlag("ChgMats",opt))
    if(!MakeChargeMatrices(opt))
      return "MakeChargeMatrices";
  SetFlag("ChgMats",opt,true);
  //SaveCal();
  
  if(!GetFlag("ChgSpecs",opt) || !GetFlag("ChgFits",opt))
    if(!GetCentroidsFromData(opt))
      return "GetCentroidsFromData";
  SetFlag("ChgSpecs",opt,true);
  SetFlag("ChgFits",opt,true);
  //SaveCal(); 
  
  if(!GetFlag("CentMats",opt))
    if(!PutCentroidsInMatrix(opt))
      return "PutCentroidsInMatrix";
  SetFlag("CentMats",opt,true);
  //SaveCal(); 
  
  Print();
  if(!GetFlag("CalcMats",opt))
    if(!PutEnergiesInMatrix(opt))
      return "PutEnergiesInMatrix";
  SetFlag("CalcMat",opt,true);
  SaveCal(); 
  
  Print();
  if(!GetFlag("CalGraphs",opt))
    if(!ProduceCalGraphs(opt))
      return "ProduceCalGraphs";
  SetFlag("CalGraphs",opt,true);
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
  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
        for(int BS=0;BS<8;BS++)
         dm->MakeChargeSpectrum(DET,FS,BS,opt);
  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
        for(int BS=0;BS<8;BS++)
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

  for(int DET=5;DET<=8;DET++)
    for(int i=0; i<ions.size(); i++)
      CreateCalObject(TSharcFormat::Get()->GetCentMatName(),ions.at(i).c_str(),DET); // creates the empty centroid matrices
  
  for(int DET=5;DET<=8;DET++)
    for(int i=0; i<ions.size(); i++)
      dm->MakeCentroidMat(ions.at(i).c_str(),DET,opt); // fills the empty centroid matrices
  
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
      CreateCalObject(TSharcFormat::Get()->GetCalcMatName(),ions.at(i).c_str(),DET); // creates the empty ecalc matrics
  
  for(int DET=5;DET<=5;DET++)
    for(int i=0; i<ions.size(); i++)
      dm->MakeCalcEnergyMat(ions.at(i).c_str(),DET,opt); // fills the empty matrices

  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Measured energies have been calculated from input parameters."RESET_COLOR"\n",opt);
  return true;
  
}


Bool_t TCalibrate::ProduceCalGraphs(Option_t *opt){
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Calibration graphs will be produced using centroid and ecalc mats."RESET_COLOR"\n",opt);
  if(strcmp(opt,"Src")==0)
     return ProduceSrcCalGraphs(opt); // different procedure as we want to combine the alphas

  std::vector<std::string> ions = TSharcInput::Get()->GetIons(opt);
  
  TDataManager *dm = TDataManager::Get();
  for(int DET=5;DET<=8;DET++)
    for(int FS=0;FS<24;FS++)
      for(int i=0; i<ions.size(); i++)
        CreateCalObject(TSharcFormat::Get()->GetCalGraphName(),ions.at(i).c_str(),DET,FS); // creates the empty centroid matrices

  // make calgraphs
  for(int DET=5;DET<=8;DET++)
    for(int FS=0;FS<24;FS++)
      for(int i=0; i<ions.size(); i++)
        dm->MakeCalGraph(ions.at(i).c_str(),DET,FS,opt); 

  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Calibration graphs have been produced using centroid and ecalc mats."RESET_COLOR"\n",opt);
  return true;
}

Bool_t TCalibrate::ProduceSrcCalGraphs(Option_t *opt){ 
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Alpha centroids will be merged into a combined channel."RESET_COLOR"\n",opt);

  /////////////////////////////////// NEEDS TO BE DONE FOR EACH NEW PARAMETER SET ///////////////////////////////////
  // calculate corresponding energies
  TDataManager *dm = TDataManager::Get();

  std::vector<std::string> ions = TSharcInput::Get()->GetIons("Src");

  for(int DET=5;DET<=5;DET++)
    for(int FS=0;FS<24;FS++)
      for(int i=0; i<ions.size(); i++)
        CreateCalObject(TSharcFormat::Get()->GetCalGraphName(),"Alphas",DET,FS); // creates the empty alpha graphs 
  
  for(int DET=5;DET<=5;DET++)
    for(int FS=0;FS<24;FS++)
      for(int i=0; i<ions.size(); i++)
        dm->MakeCalGraph("Alphas",DET,FS,opt);

  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Alpha centroids have been merged into a combined channel."RESET_COLOR"\n",opt);
  return true; 
}

/*
Bool_t TCalibrate::ProduceMulGraphs(Option_t *opt){
  
  TDataManager *dm = TDataManager::Get();
  for(int DET=5;DET<=8;DET++)
    for(int FS=0;FS<24;FS++)
      CreateCalObject(TSharcFormat::Get()->GetMulGraphName(),"All",DET,FS); // creates the empty centroid matrices

  // make mulgraphs
  for(int DET=5;DET<=8;DET++)
    for(int FS=0;FS<24;FS++)
      dm->MakeCalGraph(TSharcFormat::Get()->GetCalcMatName(ions.at(i).c_str()),DET,FS); // "fit pol1"

  return true;
}
*/
Bool_t TCalibrate::ProduceCombinedCalGraphs(Option_t *opt){
  // this currently selects all ions available for combining
  std::string ions;
  for(int i=0; i<TSharcInput::Get()->GetIons("Run").size(); i++)
    ions += TSharcInput::Get()->GetIons("Run").at(i) + " ";
  for(int i=0; i<TSharcInput::Get()->GetIons("Src").size(); i++)
    ions += TSharcInput::Get()->GetIons("Src").at(i) + " ";
  
  TDataManager *dm = TDataManager::Get();
  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       CreateCalObject(TSharcFormat::Get()->GetMulGraphName(),opt,DET,FS);
  
  for(int DET=5;DET<=8;DET++)
     for(int FS=0;FS<24;FS++)
       dm->CombineGraphs(DET,FS,opt); // combine proton and deuteron points into a single graph
}
//Bool_t TCalibrate::ProduceResults(){}

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

Bool_t TCalibrate::ExistInFile(const char *name) {

  TObjectManager *om = TObjectManager::Get();
  UInt_t n = TObjectManager::Get()->CountObjectsOfType(name);

  printf("{TCalibrate} Message :  Counted %04i objects of type '%24s'\t [flag => %s].\n",n,name,n>0 ? "true":"false");
  return n>0;
}
