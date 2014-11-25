#ifndef TDATAMANAGER_H
#define TDATAMANAGER_H

#include<TNamed.h>
#include<RTypes.h>
#include<TList.h>

#include<TSharc.h>
#include<TTigress.h>
#include<TTriFoil.h>

// If TSharcInput is the brains, this class is the muscle of the package. It handles looping over data, creating histograms and applying the TSharcInput user settings

// This class uses TSharcInput to make histograms consistently and efficiently. It should not be used without a TSharcInput instance as it has no default options or behaviour -> it is completely directed by TSharcInput. Its primary function is ProcessData, which loops over the chain entries and calls FillHists each time. In FillHists the histograms of interest are located and data is added appropriately. ProcessData is the only function that loops over chain entries, an dso it should be called as little as possible (ideally once). FillHists therefore is equipped with a means to locate and fill one or many different histograms with the contents of the detector branches (TSharc,TTigress,TTriFoil). 
// We need to fill charge histograms in ProcessData (sharc_hits.front_charge,detectornumber,frontstrip,backstrip)
// We (eventually) need to fill pad histograms, and maybe even trees with everything useful..?
class TDataManager: public TNamed {

	public:
		TDataManager();
		virtual void ~TDataManager();

		virtual void Print(Option_t *opt = "");
		virtual void Clear(Option_t *opt = "");
		
    void ApplySharcInput(Option_t *opt = "");
    void ProcessData(Option_t *opt = "");
		void   FillHists(Option_t *opt = ""); 
    Bool_t ThrowEvent(Option_t opt = "");

    static TList *MakeEnergyMat(UInt_t &det); // do we give this all the parameters, or does it locate the TSharcInput instance and extract them?

	private:
		static TSharc *fSharc;
		static TTigress *fTigress;
		static TTriFoil *fTrifoil;

		static TSharcHit *fSharcHit;
		static TTigressHit *fTigressHit;
		static TTriFoilHit *fTrifoilHit;
		
	public: // SharcInput parameter setters  
		void  AddToChain(const char *argv);

  private: // SharcInputParameters
    static TChain *fchain;

    static Bool_t fFCsel; // front charge selection flag
    Double_t fFrontCharge_min;
    Double_t fFrontCharge_max;
    static Bool_t fBCsel; // back charge selection flag
    Double_t fBackCharge_min;
    Double_t fBackCharge_max;
    static Bool_t fPCsel; // pad charge selection flag
    Double_t fPadCharge_min;
    Double_t fPadCharge_max;

    // It's more efficient to just store these because they require some calculation which needn't be repeated throughout the code
    Double_t fFrontCharge;
    Double_t fBackCharge;
    Double_t fPadCharge;

  ClassDef(TDataManager,0);
}

#endif
