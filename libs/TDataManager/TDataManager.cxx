#include "TDataManager.h"

#include<vector>
#include<string>

#include<TH1.h>
#include<TH2.h>
#include<TStopwatch.h>

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

//FillOptions maybe contains things like index of first and last event to loop over. 
  TStopwatch w;

  UInt_t err, nentries = fChain->GetEntries();
  for(int i=0; i<nentries; i++){
	  
    err = fChain->GetEntry(i);
    if(!err)
       continue;
    printf("i = %i\n",i);
    if(i%50000==0){
      printf(DYELLOW"\tProcessing event %i/%i [%.2f %%] in time %i minutes and %.1f seconds"RESET_COLOR"\r",i,nentries,(double)100*i/nentries,floor(w.RealTime()/60),fmod(w.RealTime(),60));
      fflush(stdout);
      w.Continue();
    }

	  if(ThrowEvent()) // ThrowEvent makes decisions about what is 'good data'
		  continue;

	  FillHists();
  }
   
  return;
}


Bool_t TDataManager::ThrowEvent(Option_t *opt){

    printf("HERE!\t fSharcHit = %p\tfSharc->GetMultiplicity() = %i\n",fSharc,fSharc->GetMultiplicity());
    fflush(stdout);
   if(fSharc->GetMultiplicity()>1)
      return true;

   fSharcHit = fSharc->GetHit(0);
   printf("channel = 0x%08x\t charge = %f\n",fSharcHit->GetFrontAddress(),fSharcHit->GetFrontCharge());
   if(fFCsel){
      TChannel *chan = TChannel::GetChannel(fSharcHit->GetFrontAddress());
      fFrontCharge   = chan->CalibrateENG(fSharcHit->GetFrontCharge());
      if (fFrontCharge<fFrontCharge_min || fFrontCharge>fFrontCharge_max)
         return true;
   }

   if(fBCsel){
      TChannel *chan = TChannel::GetChannel(fSharcHit->GetBackAddress());
      fBackCharge    = chan->CalibrateENG(fSharcHit->GetBackCharge());
      if (fBackCharge<fBackCharge_min || fBackCharge>fBackCharge_max)
         return true;
   }
   if(fPCsel){
      TChannel *chan = TChannel::GetChannel(fSharcHit->GetPadAddress());
      fPadCharge     = chan->CalibrateENG(fSharcHit->GetPadCharge());
      if (fPadCharge<fPadCharge_min || fPadCharge>fPadCharge_max)
         return true;
   }
// can also add tigress and trifoil cuts etc in this standardised way

   //apply other options

   return false;
}


void TDataManager::FillHists(Option_t *opt){

//	TList *fList = mList->FindObject(TSharcName::GetListName(DET,FS));
	TList *fList = (TList*) TObjectManager::Get()->GetObject(TSharcName::GetListName(fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
	if(!fList){
		printf("List not found\n");
		return;
	}
	TIter iter(fList);
  TH2F *h;

	while(TObject *obj = iter.Next()){
		if(!obj->InheritsFrom("TH2F")) 
			continue;

		h = (TH2F*) obj;

		if(fChgMat) // only makes charge matrix
			h->Fill(fSharcHit->GetBackStrip(),fFrontCharge);

		// fill charge matrices
  }
  
	return;
}
