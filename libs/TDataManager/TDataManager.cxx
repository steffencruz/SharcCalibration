#include "TDataManager.h"

#include<vector>
#include<string>

#include<TCanvas.h>
#include<TApplication.h>
#include<TH1.h>
#include<TH2.h>
#include<TStopwatch.h>
#include<TSpectrum.h>

#include <TFitManager.h>
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

void TDataManager::NewChain(Option_t *opt){

  fChain = new TChain("AnalysisTree");
  fChain->SetTitle(opt);

  fChain->SetBranchAddress("TSharc",&fSharc);
  fChain->SetBranchAddress("TTigress",&fTigress);
  fChain->SetBranchAddress("TTriFoil",&fTrifoil);

  return;
}
  
Bool_t TDataManager::SetChain(Option_t *opt){

  NewChain(opt);
  TSharcInput *si = TSharcInput::Get();
  std::string dir = si->GetDataDir(opt);
  std::vector<std::string> files = si->GetData(opt);
  if(files.size()==0 || dir.length()==0){
    printf("{TDataManager} Warning :  Chain data not found. Found %i files in directory '%s'.\n",files.size(),dir.c_str());
    return false;
  }
  for(int i=0; i<files.size(); i++)
    AddToChain(files.at(i).c_str(),dir.c_str());

  printf(DCYAN"{TDataManager} Message :  Chain has been set (%i trees,%i entries)."RESET_COLOR"\n",fChain->GetNtrees(),fChain->GetEntries());
  return true;
}

  
void TDataManager::AddToChain(const char *tree, const char *dir){
  if(!fChain){
    printf("{TDataManager} Warning : Chain does not exist.\n");
    return;
  }

  char buffer[256];
  sprintf(buffer,"%s%s",dir,tree);
  fChain->Add(buffer);
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
      printf(DYELLOW"\tProcessing event %i/%i [%.2f %%] in time %i minutes and %.1f seconds"RESET_COLOR"\r",i,nentries,(double)100*i/nentries,(int)floor(w.RealTime()/60.0),fmod(w.RealTime(),60.0));
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

   if(fSharc->GetMultiplicity()!=1)
      return true;

  fSharcHit = fSharc->GetHit(0);
//  printf("\nsharc hit -> f_charge = %.2f\tback_charge = %.2f\tpad_charge = %.2f\n",fSharc->GetMultiplicity(),fSharcHit->GetFrontCharge()/25.0,fSharcHit->GetBackCharge()/25.0,fSharcHit->GetPadCharge()/125.0);
  if(fSharcHit->GetDetectorNumber()<5 || fSharcHit->GetDetectorNumber()>8)
    return true;

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

//	 How efficient is this? Will it be optimised by the compiler or should TDataManager just have a pointer to the master list? the time wasted making the DET_FS list will ikely be saved by the fact that object can be found way quicker within the smaller list
  TList *fList = (TList*) TObjectManager::Get()->GetObject(TSharcFormat::Get()->GetListName(fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
	if(!fList){
		printf("{TDataManager} Warning :  List '%s' not found\n",TSharcFormat::Get()->GetListName(fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
		return;
	}
  TH2F *h;
	if(fChgMat){ // go look for the appropiate charge matrix
    h = (TH2F*) fList->FindObject(TSharcFormat::Get()->GetChgMatName(fChain->GetTitle(),true,fSharcHit->GetDetectorNumber(),fSharcHit->GetFrontStrip()));
    if(h){
      h->Fill(fSharcHit->GetBackStrip(),fSharcHit->GetFrontCharge()/25.0); // *%*%*$#*@#%($@#%(@*#(%*$@#)%(*@$#%()*(@)*%()*#()*@%$^*#()%*$t()@*$#tr*(
    }
  }
	
  return;
}

void TDataManager::FillChargeMats(Option_t *opt){
  fChgMat = true;
  ProcessData();
  fChgMat = false;
}

void TDataManager::MakeChargeSpectrum(UInt_t DET, UInt_t FS, UInt_t BS, Option_t *opt){
  
  TObjectManager *om = TObjectManager::Get();
  TH2F *h = (TH2F*)om->GetObject(TSharcFormat::Get()->GetChgMatName(opt,true,DET,FS),TSharcFormat::Get()->GetListName(DET,FS));
  if(!h)
    return;
  
  TH1D *h1 = h->ProjectionY(TSharcFormat::Get()->GetChgSpecName(opt,true,DET,FS,BS),BS,BS); // project out charge matrix
  h1->SetTitle(TSharcFormat::Get()->GetChgSpecName(opt,true,DET,FS,BS));

  om->GetList(TSharcFormat::Get()->GetListName(DET,FS,BS));
  om->AddObjectToList(h1,TSharcFormat::Get()->GetListName(DET,FS,BS));

  return;
}

void TDataManager::FitChargeSpectrum(UInt_t DET, UInt_t FS, UInt_t BS, Option_t *opt){

  TObjectManager *om = TObjectManager::Get();
  TSharcInput *si = TSharcInput::Get();

  TH1D *h = (TH1D*)om->GetObject(TSharcFormat::Get()->GetChgSpecName(opt,true,DET,FS,BS),TSharcFormat::Get()->GetListName(DET,FS,BS));
  if(!h)
    return;

  UInt_t ngroup = 1;
  h->Rebin(ngroup);//si->GetRebinGroup(opt));

  TSpectrum *s = TFitManager::PeakSearch(h,si->GetIons(opt).size(),si->GetChgSpecResolution(opt)/(double)ngroup,si->GetChgSpecThreshold(opt));
  if(!s)
     return;

  Double_t xmin = si->GetChgSpecFitRangeMin(opt);
  Double_t xmax = si->GetChgSpecFitRangeMax(opt);
  const char *fname = si->GetChgSpecFitFunction(opt);
  std::vector<double> pars = TFitManager::GetParameters(fname,s,si->GetChgSpecResolution(opt)/(double)ngroup);
//  std::vector<std:string> parnames = TFitManager::GetParNames(fname,si->GetNRunPeaks());
  
 // printf("* * * * %s will be fit using %s * * * * * * * *\n",h->GetName(),fname);
  TFitInfo *finfo = TFitManager::FitHist(fname,h,&pars[0],pars.size(),xmin,xmax);

  finfo->SetInfoName(TSharcFormat::Get()->GetFitInfoName(opt,true,DET,FS,BS));
  finfo->SetPeakNames(si->GetIons(opt));
  om->AddObjectToList(finfo,TSharcFormat::Get()->GetListName(DET,FS,BS));
  
  return;
}

void TDataManager::MakeCentroidMat(const char *ion, UInt_t DET, Option_t *opt){
  
  TObjectManager *om = TObjectManager::Get();
  TFitInfo *tfi;
  TH2F *h = (TH2F*)om->GetObject(TSharcFormat::Get()->GetCentMatName(ion,true,DET),TSharcFormat::Get()->GetListName(DET));  // haha TRUE DET
  if(!h)
    return;
  
  for(int FS=0; FS<24; FS++){
    for(int BS=0; BS<8; BS++){
//      printf("Looking for Fitinfo %s...\n",TSharcFormat::Get()->GetFitInfoName(opt,true,DET,FS,BS));
      tfi = (TFitInfo*) om->GetObject(TSharcFormat::Get()->GetFitInfoName(opt,true,DET,FS,BS),TSharcFormat::Get()->GetListName(DET,FS,BS));  // haha TRUE DET FSBS
      if(!tfi)
         continue;
/*      else if(!tfi->GetStatus()){ // check that the spectrum was fit
        printf("This one looks bad!!\n");
        tfi->Print();
        continue;
      }
*/      //printf("FitInfo '%s' ->peak '%s' centroid = %.2f\n",tfi->GetName(),ion,tfi->GetX(tfi->GetPeakNum(ion)));
      h->SetBinContent(FS,BS,tfi->GetX(tfi->GetPeakNum(ion))); // ion should be able to be used to get specific peaks from the fit (as long as they are stored sensibly)
      TF1 *func = tfi->GetFunction();
//      func->Print();
      UInt_t parnum = func->GetParNumber(Form("PEAK%i_MEAN",tfi->GetPeakNum(ion)));
 //     printf("I Got this parameter ->\t %i, %s, %f +-%f\n",parnum,func->GetParName(parnum),func->GetParameter(parnum),func->GetParError(parnum));
      h->SetBinError(FS,BS,func->GetParError(parnum));
//      printf("FILLED DET %i FS %i BS %i\n",DET,FS,BS);
    }
  }
  return; 
}
 
void TDataManager::MakeCalcEnergyMat(const char *ion, UInt_t DET, Option_t *opt){
  
  TObjectManager *om = TObjectManager::Get();
  TSharcInput *si = TSharcInput::Get();
  TH2F *h = (TH2F*)om->GetObject(TSharcFormat::Get()->GetCalcMatName(ion,true,DET),TSharcFormat::Get()->GetListName(DET));  // haha TRUE DET
  if(!h)
     return;
  
  std::vector<double> emeas;
  for(int FS=0; FS<24; FS++){
    for(int BS=0; BS<8; BS++){
     
      emeas = si->GetEmeas(DET,FS,BS,ion,opt);
      if(emeas.at(1)>0.0)
         continue;

      h->SetBinContent(FS,BS,emeas.at(0)); 
      h->SetBinError(FS,BS,100.0); // currently hard coded
      printf("h->GetBinContent(%02i,%02i) = %.2f\n",FS,BS,h->GetBinContent(FS,BS));
      emeas.clear();
    }
  }
  return; 

}
  
void TDataManager::MakeCalGraph(const char *ion, UInt_t DET, UInt_t FS, Option_t *opt){
  
  TObjectManager *om = TObjectManager::Get();
  TSharcFormat *sf = TSharcFormat::Get(); 
  TGraphErrors *g = (TGraphErrors*) om->GetObject(sf->GetCalGraphName(ion,true,DET,FS),sf->GetListName(DET,FS));
  if(!g){
     printf("{TDataManager} Warning :  Graph '%s' not found.\n",sf->GetCalGraphName(ion,true,DET,FS));
     return;
  }

  TH2F *hchg = (TH2F*) om->GetObject(sf->GetCentMatName(ion,true,DET),sf->GetListName(DET)); 
  if(!hchg || hchg->Integral()==0){
     printf("{TDataManager} Warning :  Centroid matrix '%s' not found.\n",sf->GetCentMatName(ion,true,DET));
     return;
  }
  
  TH2F *heng = (TH2F*) om->GetObject(sf->GetCalcMatName(ion,true,DET),sf->GetListName(DET)); 
  if(!heng || heng->Integral()==0){
     printf("{TDataManager} Warning :  Calculated energy matrix '%s' not found.\n",sf->GetCalcMatName(ion,true,DET));
     return;
  }

  for(int BS=0;BS<48;BS++){
     printf("hchg->GetBinContent(FS,BS) = %.2f\t heng->GetBinContent(FS,BS) = %.2f\n",hchg->GetBinContent(FS,BS),heng->GetBinContent(FS,BS));
     if(hchg->GetBinContent(FS,BS)==0 || heng->GetBinContent(FS,BS)==0)
        continue;
     g->SetPoint(BS,hchg->GetBinContent(FS,BS),heng->GetBinContent(FS,BS));
     g->SetPointError(BS,hchg->GetBinError(FS,BS),heng->GetBinError(FS,BS));
  }
  g->SetLineColor(TSharcInput::Get()->GetIonNumber(ion,opt)+3); // will be identical between run and source data
//  g->Dump();

  if(g->GetN()<3)
     printf("{TDataManager} Warning :  There are %i points in '%s'.\n",g->GetN(),sf->GetCalGraphName(ion,true,DET,FS));

  // Fit the graph
//  std::string fitopt = opt;
//  if(fitopt.compare("pol1")==0)
     TFitManager::FitGraph("pol1",g);

  return;
}

void TDataManager::CombineGraphs(UInt_t DET, UInt_t FS, Option_t *opt){

  std::vector<std::string> ionopts;
  std::string optstring = opt;
  
  std::vector<std::string> runions = TSharcInput::Get()->GetRunIons();
  for(int i=0; i<runions.size();i++){
    if(optstring.find(runions.at(i).c_str())!=std::string::npos)
       ionopts.push_back(runions.at(i));
  }
  std::vector<std::string> srcions = TSharcInput::Get()->GetSrcIons();
  for(int i=0; i<srcions.size();i++){
    if(optstring.find(srcions.at(i).c_str())!=std::string::npos)
       ionopts.push_back(srcions.at(i));
  }

  
}
