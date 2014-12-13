#include "TCalibrate.h"

#include <map>
#include <utility>

#include <TH1.h>
#include <TH2.h>

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
/*
  std::vector<std::string> v;
  printf(DYELLOW"\n\tCal flags :-"RESET_COLOR"\n");
  for(std::map<std::string,bool>::iterator it = fCalFlags.begin(); it != fCalFlags.end(); ++it) {
    printf("\t%24s status : %s\n",it->first.c_str(),it->second?"true":"false");
  }
*/
  printf(DYELLOW"\n\tCal flags :-"RESET_COLOR"\n");
  printf("\tfSharcInputFlag     status : %s\n",GetFlag("SharcInput")           ? "set" : "not set");
  printf("\tfRunCalibrationFlag status : %s\n",GetFlag("DeltaCalStyle","Run")  ? "enabled" : "disabled");
  printf("\tfSrcCalibrationFlag status : %s\n",GetFlag("DeltaCalStyle","Src")  ? "enabled" : "disabled");
  
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
  
  SetFlag("SharcInput",false);
  SetFlag("CalibrationFlag",false,"Run"); 
  SetFlag("CalibrationFlag",false,"Src"); 
  
  SetFlag("ChgMats"  ,false,"Run");
  SetFlag("ChgSpecs" ,false,"Run");
  SetFlag("ChgFits"  ,false,"Run");
  std::vector<std::string> runions = TSharcInput::Get()->GetIons("Run");
  SetFlag("CentMats" ,false,"Run");
  SetFlag("CalcMats" ,false,"Run");
  SetFlag("CalGraphs",false,"Run");
  SetFlag("MulGraphs",false,"Run");
  
  SetFlag("ChgMats"  ,false,"Src");
  SetFlag("ChgSpecs" ,false,"Src");
  SetFlag("ChgFits"  ,false,"Src");
  std::vector<std::string> srcions = TSharcInput::Get()->GetIons("Src");
  SetFlag("CentMats" ,false,"Src");
  SetFlag("CalcMats" ,false,"Src");
  SetFlag("CalGraphs",false,"Src");
  SetFlag("MulGraphs",false,"Src");
 
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

  SetFlag("SharcInput",true);
  SetFlag("DeltaCalStyle",TSharcInput::Get()->GetRunDeltaCalFlag(),"Run"); 
  SetFlag("DeltaCalStyle",TSharcInput::Get()->GetSrcDeltaCalFlag(),"Src"); 
  
  TSharcFormat *sf = TSharcFormat::Get();
  
  SetFlag("ChgMats"  ,ExistInFile(  sf->GetChgMatName("Run")),"Run");
  SetFlag("ChgSpecs" ,ExistInFile( sf->GetChgSpecName("Run")),"Run");
  SetFlag("ChgFits"  ,ExistInFile( sf->GetFitInfoName("Run")),"Run");
  std::vector<std::string> runions = TSharcInput::Get()->GetIons("Run");
  SetFlag("CentMats" ,ExistInFile( sf->GetCentMatName(runions.at(0).c_str())),"Run");
  SetFlag("CalcMats" ,ExistInFile( sf->GetCalcMatName(runions.at(0).c_str())),"Run");
  SetFlag("CalGraphs",ExistInFile(sf->GetCalGraphName(runions.at(0).c_str())),"Run");
  SetFlag("MulGraphs",ExistInFile(sf->GetMulGraphName(runions.at(0).c_str())),"Run");
  
  SetFlag("ChgMats"  ,ExistInFile(  sf->GetChgMatName("Src")),"Src");
  SetFlag("ChgSpecs" ,ExistInFile( sf->GetChgSpecName("Src")),"Src");
  SetFlag("ChgFits"  ,ExistInFile( sf->GetFitInfoName("Src")),"Src");
  std::vector<std::string> srcions = TSharcInput::Get()->GetIons("Src");
  SetFlag("CentMats" ,ExistInFile( sf->GetCentMatName(srcions.at(0).c_str())),"Src");
  SetFlag("CalcMats" ,ExistInFile( sf->GetCalcMatName(srcions.at(0).c_str())),"Src");
  SetFlag("CalGraphs",ExistInFile(sf->GetCalGraphName(srcions.at(0).c_str())),"Src");
  SetFlag("MulGraphs",ExistInFile(sf->GetMulGraphName(srcions.at(0).c_str())),"Src");

}

Bool_t TCalibrate::GetFlag(const char *flagname, Option_t *opt){
  std::string str = Form("f%s%sFlag",opt,flagname);  
  
  //if(fCalFlags.count("fDEBUGFlag") && fCalFlags["fDEBUGFlag"]){
   if(fCalFlags.count(str)==0){
     printf("{TCalibrate} Warning : Cannot get flag %s because it doesn't exist. Returning false\n",str.c_str());
     return false;
   //} else 
   //  printf("{TCalibrate} Info : Flag '%s' status is %s\n",flagname,fCalFlags[str]);
  }
  return fCalFlags[str];
}

void TCalibrate::SetFlag(const char *flagname, Bool_t flag, Option_t *opt){
  std::string str = Form("f%s%sFlag",opt,flagname);
  
  fCalFlags[str] = flag;
  
 // if(fCalFlags.count("fDEBUGFlag") && fCalFlags["fDEBUGFlag"])
 //    printf("{TCalibrate} Info : Flag '%s' set to %s\n",flagname,fCalFlags[str]?"true":"false");



//  fCalFlags.insert(std::make_pair(str,flag));
}

void TCalibrate::SaveCal(const char *ofname) {
   // more stuff here? use flags to save only subsets of results??
  if(!ofname)
     ofname = TSharcInput::Get()->MakeOutputName();
  TFileManager::WriteFile(ofname); // uses TSharcName output file convention and writes TObjectManager's master list
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
  TObjectManager *om = TObjectManager::Get();
  TSharcInput *si = TSharcInput::Get();

  TH2F *h;
  TH1D *h1;
  Double_t xmin = si->GetChgSpecFitRangeMin(opt);
  Double_t xmax = si->GetChgSpecFitRangeMax(opt);
  const char *fname = si->GetChgSpecFitFunction(opt);
  
  for(int DET=5;DET<=8;DET++){
      if(GetFlag("Quick") && DET!=5){
         printf("Skipping DET %i [quick flag enabled]\n",DET);
         continue;
      }
     for(int FS=0;FS<24;FS++){
        
        h = (TH2F*)om->GetObject(TSharcFormat::Get()->GetChgMatName(opt,true,DET,FS),TSharcFormat::Get()->GetListName(DET,FS));
        if(!h)
          continue;
         
        for(int BS=0;BS<8;BS++){
          
          // Projection (aka Charge Spectrum)
          if(strcmp(opt,"Run")==0)
            h1 = h->ProjectionY(TSharcFormat::Get()->GetChgSpecName(opt,true,DET,FS,BS),BS,BS); // project out each back strip 
          if(strcmp(opt,"Src")==0){
            h1 = h->ProjectionY(TSharcFormat::Get()->GetChgSpecName(opt,true,DET,FS));          // project all back strips together
            BS = -1;  // this tells sharc format to ignore BS  
          }

          // Put projection into list
          h1->SetTitle(TSharcFormat::Get()->GetChgSpecName(opt,true,DET,FS,BS));
          om->GetList(TSharcFormat::Get()->GetListName(DET,FS,BS));
          om->AddObjectToList(h1,TSharcFormat::Get()->GetListName(DET,FS,BS));

          // locate peaks (Using TSharcInput settings)
          TSpectrum *s = TFitManager::PeakSearch(h1,si->GetIons(opt).size(),si->GetChgSpecResolution(opt),si->GetChgSpecThreshold(opt));
          
          if(!s){
             if(BS<0)
                break;
            continue;
           }
          
          // fit peaks (Using TSharcInput settings)
          std::vector<double> pars = TFitManager::GetParameters(fname,s,si->GetChgSpecResolution(opt));
          TFitInfo *finfo = TFitManager::FitHist(fname,h1,pars.data(),pars.size(),xmin,xmax); 
          
          // Set Some names and put TFitInfo into list
          finfo->SetInfoName(TSharcFormat::Get()->GetFitInfoName(opt,true,DET,FS,BS));
          finfo->SetPeakNames(si->GetIons(opt),opt); // calls the peaks Prot,Deut,Alpha0 or whatever
          om->AddObjectToList(finfo,TSharcFormat::Get()->GetListName(DET,FS,BS));

          //finfo->Print();
          if(strcmp(opt,"Src")==0) // we don't want multiple copies of Src chg spec, so leave loop
             break;
        }
     }
  }

  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Charge spectra have been created and fitted using charge matrices."RESET_COLOR"\n",opt);
  return true;
}


Bool_t TCalibrate::PutCentroidsInMatrix(Option_t *opt){
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Centroids will be extracted from charge matrix fits."RESET_COLOR"\n",opt);
  /////////////////////////////////// THIS NEEDS TO BE DONE WHEN CENTROIDS ARE REPRODUCED /////////////////////////////////
  // 4.  Extract charge centroids
  TObjectManager *om = TObjectManager::Get();
  
  std::vector<std::string> ions = TSharcInput::Get()->GetIons(opt);
  if(ions.size()==0){
    printf("{TCalibrate} Warning :  There are no %s ions available for calibration. Cannot continue.\n",opt);
    return false;
  }

   TH2F *mat = 0;
   TH2F *chimat = 0;

   TFitInfo *tfi = 0;
   for(int DET=5;DET<=8;DET++){
      if(GetFlag("Quick") && DET!=5){
         printf("Skipping DET %i [quick flag enabled]\n",DET);
         continue;
      }
      for(int i=0; i<ions.size(); i++) {
         
         mat = (TH2F*)CreateCalObject(TSharcFormat::Get()->GetCentMatName(),ions.at(i).c_str(),DET); // creates the empty centroid matrices
         chimat = (TH2F*)CreateCalObject(TSharcFormat::Get()->GetCentChiMatName(),ions.at(i).c_str(),DET);//create empty chisquare matrix for fit centroids
         
         for(int FS=0;FS<24;FS++){
            if(strcmp(opt,"Run")==0) {
               for(int BS=0;BS<8;BS++){
                  tfi = (TFitInfo*) om->GetObject(TSharcFormat::Get()->GetFitInfoName(opt,true,DET,FS,BS),
                                                  TSharcFormat::Get()->GetListName(DET,FS,BS));  // haha TRUE DET FSBS
                  if(!tfi)
                     continue;
                  mat->SetBinContent(FS,BS,tfi->GetX(tfi->GetPeakNum(ions.at(i).c_str())));
                  mat->SetBinError(FS,BS,tfi->GetXE(tfi->GetPeakNum(ions.at(i).c_str())));
                   
                  chimat->SetBinContent(FS,BS,tfi->GetNormChi2(tfi->GetPeakNum(ions.at(i).c_str()))); // we have identical copies of this content
               }
            } else if(strcmp(opt,"Src")==0){
               tfi = (TFitInfo*) om->GetObject(TSharcFormat::Get()->GetFitInfoName(opt,true,DET,FS),
                                                  TSharcFormat::Get()->GetListName(DET,FS));  // haha TRUE DET FSBS
               if(!tfi)
                  continue;

               // assume alphas are in back strip 24
               mat->SetBinContent(FS,24,tfi->GetX(tfi->GetPeakNum(ions.at(i).c_str())));
               mat->SetBinError(FS,24,tfi->GetXE(tfi->GetPeakNum(ions.at(i).c_str())));
               
               chimat->SetBinContent(FS,24,tfi->GetNormChi2(tfi->GetPeakNum(ions.at(i).c_str()))); // we have identical copies of this content

             }
         }
       }
    }
  
  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Centroids have been extracted from charge matrix fits."RESET_COLOR"\n",opt);
  return true;
}


Bool_t TCalibrate::PutEnergiesInMatrix(Option_t *opt){ 
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Measured energies will be calculated from input parameters."RESET_COLOR"\n",opt);

  /////////////////////////////////// NEEDS TO BE DONE FOR EACH NEW PARAMETER SET ///////////////////////////////////
  // calculate corresponding energies
  TDataManager *dm = TDataManager::Get();
  TSharcInput *si = TSharcInput::Get();

  std::vector<std::string> ions = TSharcInput::Get()->GetIons(opt);
  if(ions.size()==0){
    printf("{TCalibrate} Warning :  There are no %s ions available for calibration. Cannot continue.\n",opt);
    return false;
  }

   std::vector<double> Emeas;
   TH2F *mat = 0;

   for(int DET=5;DET<=8;DET++){
      if(GetFlag("Quick") && DET!=5){
         printf("Skipping DET %i [quick flag enabled]\n",DET);
         continue;
      }
      for(int i=0; i<ions.size(); i++) {
         
         mat = (TH2F*)CreateCalObject(TSharcFormat::Get()->GetCalcMatName(),ions.at(i).c_str(),DET); // creates the empty calcE matrices
          
         for(int FS=0;FS<24;FS++){
            if(strcmp(opt,"Run")==0) {
               for(int BS=0;BS<8;BS++){
                  
               Emeas = si->GetEmeas(DET,FS,BS,ions.at(i).c_str(),opt);
               if(Emeas.at(1)>0) // predicts punch through
                  continue;

               mat->SetBinContent(FS,BS,Emeas.at(0));
               mat->SetBinError(FS,BS,100.0); // hard code uncertainty of error to be 100
//                  Double_t Emerr = si->GetEmerr(DET,FS,BS,ion.at(i).c_str(),opt);
//                  mat->SetBinError(FS,BS,Emerr);
               }
            } else if(strcmp(opt,"Src")==0){ // energies look the same in every front strip!!!!!!!!!!!!!!!!!!!!!!!!!
               
               Emeas = si->GetEmeas(DET,FS,24,ions.at(i).c_str(),opt);
               if(Emeas.at(1)>0) // predicts punch through
                  continue;

               mat->SetBinContent(FS,24,Emeas.at(0));
               mat->SetBinError(FS,24,100.0); // hard code uncertainty of error to be 100
          
             }
         }
       }
    }

  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Measured energies have been calculated from input parameters."RESET_COLOR"\n",opt);
  return true;
}

Bool_t TCalibrate::ProduceCalGraphs(Option_t *opt){
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Calibration graphs will be produced using centroid and ecalc mats."RESET_COLOR"\n",opt);

  /////////////////////////////////// NEEDS TO BE DONE FOR EACH NEW PARAMETER SET ///////////////////////////////////
  // calculate corresponding energies
  TDataManager *dm = TDataManager::Get();
  TObjectManager *om = TObjectManager::Get();
  TSharcInput *si = TSharcInput::Get();

  std::vector<std::string> ions = si->GetIons(opt);
  if(ions.size()==0){
    printf("{TCalibrate} Warning :  There are no %s ions available for calibration. Cannot continue.\n",opt);
    return false;
  }

  TGraphErrors *g = 0;
  TH2F *hchg = 0;
  TH2F *hchi = 0;
  TH2F *heng = 0;
  Double_t xmin = 0.0;//si->GetCalGraphFitRangeMin(opt);
  Double_t xmax = 10000.0;//si->GetCalGraphFitRangeMax(opt);
  const char *fname = "pol1";//si->GetCalGraphFitFunction(opt);

  for(int DET=5;DET<=8;DET++){
      if(GetFlag("Quick") && DET!=5){
         printf("Skipping DET %i [quick flag enabled]\n",DET);
         continue;
      }
    for(int i=0; i<ions.size(); i++) {
      
      hchg = (TH2F*) om->GetObject(TSharcFormat::Get()->GetCentMatName(ions.at(i).c_str(),true,DET),TSharcFormat::Get()->GetListName(DET));
      if(!hchg && GetFlag("DEBUG") ){
        printf("{TCalibrate} Warning :  Centroid matrix '%s' not found.\n",TSharcFormat::Get()->GetCentMatName(ions.at(i).c_str(),true,DET));
        continue;
      } 
      hchi = (TH2F*) om->GetObject(TSharcFormat::Get()->GetCentChiMatName(ions.at(i).c_str(),true,DET),TSharcFormat::Get()->GetListName(DET));
      if(!hchi && GetFlag("DEBUG") ){
        printf("{TCalibrate} Warning :  Centroid chisquared matrix '%s' not found.\n",TSharcFormat::Get()->GetCentChiMatName(ions.at(i).c_str(),true,DET));
        continue;
      } 
      heng = (TH2F*) om->GetObject(TSharcFormat::Get()->GetCalcMatName(ions.at(i).c_str(),true,DET),TSharcFormat::Get()->GetListName(DET));
      if(!heng && GetFlag("DEBUG") ){
        printf("{TCalibrate} Warning :  Calculated energy matrix '%s' not found.\n",TSharcFormat::Get()->GetCalcMatName(ions.at(i).c_str(),true,DET));
        continue;
      } 
 
     for(int FS=0;FS<24;FS++){
       if(strcmp(opt,"Run")==0) {
         g  = (TGraphErrors*)CreateCalObject(TSharcFormat::Get()->GetCalGraphName(),ions.at(i).c_str(),DET,FS); // creates the empty calibration graph
         g->SetLineColor(i+2); // graph color
         
         for(int BS=0;BS<8;BS++){
         
           if(GetFlag("DEBUG"))
             printf("hchg->GetBinContent(FS,BS) = %.2f\t heng->GetBinContent(FS,BS) = %.2f\n",hchg->GetBinContent(FS,BS),heng->GetBinContent(FS,BS));
           if(hchg->GetBinContent(FS,BS)==0 || heng->GetBinContent(FS,BS)==0)
             continue;
         
           g->SetPoint(BS,hchg->GetBinContent(FS,BS),heng->GetBinContent(FS,BS));
           g->SetPointError(BS,hchg->GetBinError(FS,BS),heng->GetBinError(FS,BS));
         }
       } else if(strcmp(opt,"Src")==0){ // don't hard code the combined alpha graph name as it is used throughout the code
         
         if(i==0){ // if the graph doesnt exist
           g  = (TGraphErrors*)CreateCalObject(TSharcFormat::Get()->GetCalGraphName(),"Alpha",DET,FS); // creates the empty calibration graph
           g->SetLineColor(1); // graph color
         }
         else 
           g = (TGraphErrors*) om->GetObject(TSharcFormat::Get()->GetCalGraphName("Alpha",true,DET,FS),TSharcFormat::Get()->GetListName(DET,FS)); // get it from list
         
         if(hchg->GetBinContent(FS,24)==0 || heng->GetBinContent(FS,24)==0)
             continue;
         
         g->SetPoint(i,hchg->GetBinContent(FS,24),heng->GetBinContent(FS,24));
         g->SetPointError(i,hchg->GetBinError(FS,24),heng->GetBinError(FS,24));

        }
       if(g->GetN()>=3)
          TFitManager::FitGraph(fname,g,xmin,xmax);
     }
   }
 }
  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Calibration graphs have been produced using centroid and ecalc mats."RESET_COLOR"\n",opt);
  return true;
}


Bool_t TCalibrate::ProduceCombinedGraphs(){
  
  printf(DYELLOW"{TCalibrate} Message : ["DCYAN"%s"DYELLOW"] Combined graphs will be produced using individual calibration graphs [and range specs]."RESET_COLOR"\n",opt);

  /////////////////////////////////// NEEDS TO BE DONE FOR EACH NEW PARAMETER SET ///////////////////////////////////
  // calculate corresponding energies
  TDataManager *dm = TDataManager::Get();
  TObjectManager *om = TObjectManager::Get();
  TSharcInput *si = TSharcInput::Get();
  
  std::vector<std::string> ions = si->GetIons(opt);
  if(ions.size()==0){
    printf("{TCalibrate} Warning :  There are no %s ions available for calibration. Cannot continue.\n",opt);
    return false;
  }
  ions.push_back("Alpha"); // ugly and hard coded

  TGraphErrors *gmaster = 0;
  TGraphErrors *g = 0;
  Double_t x,y,xe,ye;
  UInt_t N;
  Double_t xmin = 0.0;//si->GetCalGraphFitRangeMin(opt);
  Double_t xmax = 10000.0;//si->GetCalGraphFitRangeMax(opt);
  const char *fname = "pol1";//si->GetCalGraphFitFunction(opt);

  for(int DET=5;DET<=8;DET++){
    if(GetFlag("Quick") && DET!=5){
       printf("Skipping DET %i [quick flag enabled]\n",DET);
       continue;
    }
    for(int FS=0;FS<24;FS++){
      gmaster  = (TGraphErrors*)CreateCalObject(TSharcFormat::Get()->GetCombinedGraphName(),"",DET,FS); // creates the empty combined graph
      
      for(int i=0;i<ions.size();i++){
       
        g = (TGraphErrors*) om->GetObject(TSharcFormat::Get()->GetCalGraphName(ions.at(i).c_str(),true,DET,FS),TSharcFormat::Get()->GetListName(DET,FS)); 
        if(!g)
           continue;

        for(int p=0; p<g->GetN(); p++){   
           g->GetPoint(p,x,y);
           //if(dm->ThrowPoint(x,y,ions.at(i).c_str())) // applies sharc input settings to keep or discard points in graphs
           // continue;
           N = gmaster->GetN();
           gmaster->SetPoint(N,x,y);
           gmaster->SetPointError(N,xe,ye);
        }
      }
      if(gmaster->GetN()>3)
         TFitManager::FitGraph(fname,gmaster,xmin,xmax);
    }
  }

  printf(DGREEN"{TCalibrate} Message : ["DCYAN"%s"DGREEN"] Combined graphs have been produced using individual calibration graphs [and range specs]."RESET_COLOR"\n");
  return true;
}




//Bool_t TCalibrate::ProduceResults(){}

TObject *TCalibrate::CreateCalObject(const char *objname, Option_t *opt, Int_t DET, Int_t FS) {

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
  
  if(GetFlag("DEBUG"))
    printf("-> I Just made \t%s\t :]\n",obj->GetName());
  
  return obj;
}

Bool_t TCalibrate::ExistInFile(const char *name) {

  TObjectManager *om = TObjectManager::Get();
  UInt_t n = TObjectManager::Get()->CountObjectsOfType(name);

  printf("{TCalibrate} Message :  Counted %04i objects of type '%24s'\t [flag => %s].\n",n,name,n>0 ? "true":"false");
  return n>0;
}
