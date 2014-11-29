#include "TDataManager.h"

#include<vector>
#include<string>

#include<TH1.h>
#include<TH2.h>
#include<TStopwatch.h>

#include <TFitInfo.h>
#include <TSharcFormat.h>

#include "TKinematics.h"
#include "TSharcAnalysis.h"

ClassImp(TDataManager);

TDataManager *TDataManager::fDataManager = 0;

TSharc      *TDataManager::fSharc   = 0;
TTigress    *TDataManager::fTigress = 0;
TTriFoil    *TDataManager::fTrifoil = 0;

TSharcHit   *TDataManager::fSharcHit   = 0;
TTigressHit *TDataManager::fTigressHit = 0;

TDataManager::TDataManager() : fChain(0) { }

TDataManager::~TDataManager() { }


TDataManager *TDataManager::Get(){
  if(!fDataManager)
    fDataManager = new TDataManager(true);
  return fDataManager;
} 

TDataManager::TDataManager(Bool_t flag){   
  Clear();

  NewChain();
  ApplySharcInput();
}

void TDataManager::NewChain(){

  fChain = new TChain("AnalysisTree");

  fChain->SetBranchAddress("TSharc",&fSharc);
  fChain->SetBranchAddress("TTigress",&fTigress);
  fChain->SetBranchAddress("TTriFoil",&fTrifoil);

  return;
}
  

void TDataManager::Print(Option_t *opt){
   
  printf(DCYAN"\nTDataManager\n"RESET_COLOR);
  printf("\t Chain      : %i entries, %i trees\n",fChain->GetEntries(),fChain->GetNtrees());
  printf("\t MakeChgMat : %s \n",fChgMat ? "true" : "false");

  if(fFCsel) printf("\t-> [ %05.1f  < Front Charge  < %05.1f]\n",fFrontCharge_min,fFrontCharge_max);
  if(fBCsel) printf("\t-> [ %05.1f  <  Back Charge  < %05.1f]\n",fBackCharge_min,fBackCharge_max);
  if(fPCsel) printf("\t-> [ %05.1f  <   Pad Charge  < %05.1f]\n",fPadCharge_min,fPadCharge_max);

  return;
}

void TDataManager::Clear(Option_t *opt){ }


void TDataManager::ApplySharcInput(Option_t *opt){
  
   // Get sharc input [only loop over run data for now]
   TSharcInput *shinput = TSharcInput::Get();

   // make chain
   std::vector<std::string> datafiles = shinput->GetRunData();
   for(int i=0; i<datafiles.size(); i++)
      AddToChain(datafiles.at(i).c_str(),shinput->GetRunDataDir());

   fChgMat = true; // fill charge matrices flag

    // take care of user settings for charge cuts
   if(shinput->GetFrontChargeMin()>=0 && shinput->GetFrontChargeMax()>=0){
      fFCsel = true;
      fFrontCharge_min = shinput->GetFrontChargeMin();
      fFrontCharge_max = shinput->GetFrontChargeMax();
    }

   if(shinput->GetBackChargeMin()>=0 && shinput->GetBackChargeMax()>=0){
      fBCsel = true;
      fBackCharge_min = shinput->GetBackChargeMin();
      fBackCharge_max = shinput->GetBackChargeMax();
    }
  
   if(shinput->GetPadChargeMin()>=0 && shinput->GetPadChargeMax()>=0){
      fPCsel = true;
      fPadCharge_min = shinput->GetPadChargeMin();
      fPadCharge_max = shinput->GetPadChargeMax();
    }

}
  
void TDataManager::AddToChain(const char *tree, const char *dir){
  if(!fChain)
     fChain = new TChain("AnalysisTree");
  
  char buffer[256];
  sprintf(buffer,"%s%s",dir,tree);
  
  fChain->Add(buffer);
  return;
}

void TDataManager::ProcessData(Option_t *opt){

  if(!fChain){
     printf("{TDataManager} Warning :  chain not set.\n"); 
     return;
   }
  TStopwatch w;

  UInt_t i, err, nentries = fChain->GetEntries();
  for(i=0; i<nentries; i++){
	  
    err = fChain->GetEntry(i);
    if(!err)
       continue;

    if(i%50000==0){
      printf(DYELLOW"\tProcessing event %i/%i [%.2f %%] in time %i minutes and %.1f seconds"RESET_COLOR"\r",i,nentries,(double)100*i/nentries,floor(w.RealTime()/60),fmod(w.RealTime(),60));
      fflush(stdout);
      w.Continue();
    }

	  if(ThrowEvent()) // ThrowEvent makes decisions about what is 'good data'
		  continue;

	  FillHists();
  }
   
  printf(DGREEN"\tProcessing event %i/%i [%.2f %%] in time %i minutes and %.1f seconds"RESET_COLOR"\n",i,nentries,(double)100*i/nentries,floor(w.RealTime()/60),fmod(w.RealTime(),60));
  return;
}


Bool_t TDataManager::ThrowEvent(Option_t *opt){

//  printf("fSharc->GetMultiplicity() = %i",fSharc->GetMultiplicity());
   if(fSharc->GetMultiplicity()!=1)
      return true;

  fSharcHit = fSharc->GetHit(0);
//  printf("\nsharc hit -> f_charge = %.2f\tback_charge = %.2f\tpad_charge = %.2f\n",fSharc->GetMultiplicity(),fSharcHit->GetFrontCharge()/25.0,fSharcHit->GetBackCharge()/25.0,fSharcHit->GetPadCharge()/125.0);

  if(fFCsel)
      if(fSharcHit->GetFrontCharge()/25.0<fFrontCharge_min || fSharcHit->GetFrontCharge()/25.0>fFrontCharge_max)
         return true;

   if(fBCsel)
      if (fSharcHit->GetBackCharge()/25.0<fBackCharge_min || fSharcHit->GetBackCharge()/25.0>fBackCharge_max)
         return true;
  
   if(fPCsel)
      if (fSharcHit->GetPadCharge()/125.0<fPadCharge_min || fSharcHit->GetPadCharge()/125.0>fPadCharge_max)
         return true;
   
// can also add tigress and trifoil cuts etc in this standardised way

   //apply other options

   return false;
}


void TDataManager::FillHists(Option_t *opt){

//	TList *fList = mList->FindObject(TSharcName::GetListName(DET,FS));
    if(fSharcHit->GetDetectorNumber()!=5 || fSharcHit->GetFrontStrip()!=12)
       return;
//	 How efficient is this? Will it be optimised by the compiler or should TDataManager just have a pointer to the master list? the time wasted making the DET_FS list will ikely be saved by the fact that object can be found way quicker within the smaller list
  TList *fList = (TList*) TObjectManager::Get()->GetObject(TSharcName::GetListName(fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
	if(!fList){
		printf("{TDataManager} Warning :  List '%s' not found\n",TSharcName::GetListName(fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
		return;
	}
  TH2F *h;
	if(fChgMat){ // go look for the appropiate charge matrix
    h = (TH2F*) fList->FindObject(TSharcName::GetChgMatName(fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
		if(h){
      h->Fill(fSharcHit->GetBackStrip(),fSharcHit->GetFrontCharge()/25.0);
//      printf("HERRO I JUSPUT CHG[%.3f]_BS[%02i] IN '%s' !!!\n",fSharcHit->GetFrontCharge()/25.0,fSharcHit->GetBackStrip(),TSharcName::GetChgMatName(fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
    }
  }
	
  return;
}



void TDataManager::MakeChargeMats(Option_t *opt){
  fChgMat = true;
  ProcessData();
}

void TDataManager::MakeChargeSpectra(Option_t *opt){
}

void TDataManager::FitChargeSpectra(Option_t *opt){

}

void TDataManager::MakeCentroidMat(UInt_t DET, Option_t *opt){
  
  TObjectManager *om = TObjectManager::Get();
  TFitInfo *tfi;
  TH2F *h = (TH2F*)om->GetObject(TSharcFormat::GetCentMatName(true,DET),TSharcFormat::GetListName(DET));  // haha TRUE DET

  for(int FS=0; FS<24; FS++){
    for(int BS=0; BS<48; BS++){
     
      tfi = (TFitInfo*) om->GetObject(TSharcFormat::GetFitInfoName(true,DET,FS,BS),TSharcFormat::GetListName(DET,FS,BS));  // haha TRUE DET FSBS
      if(tfi && tfi->Status()) // check that the spec
        h->Fill(BS,FS,tfi->GetX(0));
    }
  }
  return; 
}
 
void TDataManager::MakeCalcEnergyMat(UInt_t DET, Option_t *opt){
  
  TObjectManager *om = TObjectManager::Get();
  TSharcInput *si = TSharcInput::Get();
  TH2F *h = (TH2F*)om->GetObject(TSharcFormat::GetCentMatName(true,DET),TSharcFormat::GetListName(DET));  // haha TRUE DET
  TVector3 position;
  Double_t ekin;
  std::vector<double> emeas;

  for(int FS=0; FS<24; FS++){
    for(int BS=0; BS<48; BS++){
      
      position = TSharc::GetPosition(DET,FS,BS,si->GetPosOffs().X(),si->GetPosOffs().Y(),si->GetPosOffs().Z());
      ekin = ((TKinematics*) si->GetKinematics("p"))->ELab(position.Theta(),2);
      emeas = TSharcAnalysis::GetMeasuredEnergy(DET,position,ekin,'p');
      
      h->Fill(BS,FS,emeas.at(0));
      emeas.clear();
    }
  }

  return; 

}
  
void TDataManager::MakeCalGraphs(Option_t *opt){

}

void TDataManager::CombineGraphs(Option_t *opt){

}
