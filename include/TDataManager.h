#ifndef TDATAMANAGER_H
#define TDATAMANAGER_H

#include<TNamed.h>
#include<Rtypes.h>
#include<TList.h>
#include<TChain.h>

#ifndef __CINT__
#include "TSharc.h"
#include "TTigress.h"
#include "TTriFoil.h"
#include "TSharcHit.h"
#include "TTigressHit.h"
#else
class TSharc;
class TTigress;
class TTriFoil;
class TSharcHit;
class TTigressHit;
#endif

#include<TSharcInput.h>
#include<TSharcName.h>
#include<TObjectManager.h>

// If TSharcInput is the brains, this class is the brawns of the package. It handles looping over data, creating histograms and applying the TSharcInput user settings
// No setters or getters, just MUSCLE!! locates & puts data into histograms.
class TDataManager: public TNamed {

	public:
		TDataManager();
		virtual ~TDataManager();
    static TDataManager *Get();

		virtual void Print(Option_t *opt = "");
		virtual void Clear(Option_t *opt = "");
		
    void ApplySharcInput(Option_t *opt = "");

    void FillChargeMats(Option_t *opt = "");
    
    void MakeChargeSpectrum(UInt_t DET, UInt_t FS, UInt_t BS, Option_t *opt = "");
    void FitChargeSpectrum(UInt_t DET, UInt_t FS, UInt_t BS, Option_t *opt = "");

    void MakeCentroidMat(const char *ion, UInt_t DET, Option_t *opt);
    void MakeCalcEnergyMat(const char *ion, UInt_t DET, Option_t *opt);
    void MakeCalGraph(const char *ion, UInt_t DET, UInt_t FS, Option_t *opt = "");

    void CombineGraphs(UInt_t DET, UInt_t FS, Option_t *opt = "");

	private:
	  static TDataManager *fDataManager;
    TDataManager(Bool_t);

    void ProcessData(Option_t *opt = "");
		void   FillHists(Option_t *opt = ""); 
    Bool_t ThrowEvent(Option_t *opt = "");
  
    static TSharc *fSharc;
		static TTigress *fTigress;
		static TTriFoil *fTrifoil;

		static TSharcHit *fSharcHit;
		static TTigressHit *fTigressHit;
		
	public: // SharcInput parameter setters  
    void NewChain(Option_t *opt = ""); // creates chain and sets branches appropriately
		Bool_t SetChain(Option_t *opt);
    void AddToChain(const char *tree, const char *dir = "");
    TChain *GetChain() { return fChain; }

  private: // SharcInputParameters
    TChain *fChain;
    
    Bool_t fChgMat; // flag to fill charge matrices

    Bool_t fFCsel; // front charge selection flag
    Double_t fFrontCharge_min;
    Double_t fFrontCharge_max;
    Bool_t fBCsel; // back charge selection flag
    Double_t fBackCharge_min;
    Double_t fBackCharge_max;
    Bool_t fPCsel; // pad charge selection flag
    Double_t fPadCharge_min;
    Double_t fPadCharge_max;

  ClassDef(TDataManager,0);
};

#endif
