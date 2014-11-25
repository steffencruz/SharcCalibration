#include "TDataManager.h"

#include<vector>
#include<string>

ClassImp(TDataManager);


TSharc      *TDataManager::fSharc   = 0;
TTigress    *TDataManager::fTigress = 0;
TTriFoil    *TDataManager::fTrifoil = 0;

TSharcHit   *TDataManager::fSharcHit   = 0;
TTigressHit *TDataManager::fTigressHit = 0;
TTriFoilHit *TDataManager::fTrifoilHit = 0;

TDataManager::TDataManager() { 

  ApplySharcInput();
}

TDataManager::~TDataManager() { }

void TDataManager::Print(Option_t *opt){ }

void TDataManager::Clear(Option_t *opt){ }


void TDataManager::ApplySharcInput(Option_t *opt){
  
   // Get sharc input [only loop over run data for now]
   TSharcInput *shinput = TSharcInput::Get();

   // make chain
   std::vector<std::string> datafiles = shinput->GetDataFiles(opt);
   for(int i=0; i<datafiles.size(); i++)
      AddToChain(datafiles.at(i).c_str());

   // set chain start and end point
//    fNbegin = shinput->GetDataNbegin();
//    fNend   = shinput->GetDataNend();

    // take care of user settings for charge cuts
    if(shinput->GetFrontChargeMin()>=0 && shinput->GetFrontChargeMax()>=0){
      fFCsel = true;
      fFrontCharge_min = shinput->GetFrontChargeMin();
      fFrontCharge_max = shinput->GetFrontChargeMax();
    }

    if(shinput->GetBackChargeMin()>=0 && shinput->GetBackChargeMax()>=0){
      fFBsel = true;
      fBackCharge_min = shinput->GetBackChargeMin();
      fBackCharge_max = shinput->GetBackChargeMax();
    }
  
    if(shinput->GetPadChargeMin()>=0 && shinput->GetPadChargeMax()>=0){
      fPCsel = true;
      fPadCharge_min = shinput->GetPadChargeMin();
      fPadCharge_max = shinput->GetPadChargeMax();
    }

}
  
void TDataManager::AddToChain(const char *tree){
  if(!fChain)
     fChain = new TChain("AnalysisTree");

  fChain->Add(tree);
}

void TDataManager::ProcessData(Option_t *opt){

   if(!fChain){
      printf("{TDataManager} Warning :  chain not set.\n"); 
      return;
    }

//FillOptions maybe contains things like index of first and last event to loop over. 
  TStopwatch w;
  w.Begin();

  UInt_t err;
  for(int i=0; i<fChain->GetEntries(); i++){
	  
    err = fChain->GetEntry(i);
    if(err)
       continue;

    if(i%50000==0){
      printf(DYELLOW"\tProcessing event %i/%i [%.2f %%] in time %i minutes and %.1f seconds\r",i,nbegin-nend,(double)100*i/(nbegin-nend),floor(w.RealTime()/60),fmod(w.RealTime(),60));
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

   if(fSharc->GetMultiplicity()>1)
      return true;

   fSharcHit = fSharc->GetHit(0);
   
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
	TList *fList = TObjectManager::GetObject(TSharcName::GetListName(fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
	if(!fList){
		printf("List not found\n");
		return;
	}
	TIter iter(fList);

//  TChannel *chan    = TChannel::GetChannel(fSharcHit->GetFrontAddress());
//  Double_t charge   = chan->CalibrateENG(fSharcHit->GetFrontCharge());

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
