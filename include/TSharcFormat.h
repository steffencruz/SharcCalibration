
#ifndef TSHARCFORMAT_H
#define TSHARCFORMAT_H

#include <cstdio>
#include <string>

#include <Rtypes.h>
#include <TObject.h>

// more useful copy of TSharcName, but with ability to apply standard formatting to names and histograms
class TSharcFormat : public TObject {

  public:
    TSharcFormat(); 
    virtual ~TSharcFormat();

    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");

    //Uncalibrated spectra cannot be grouped
    static const char *GetChgMatName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)                { return GetObjectName(fChgMatName,path,det,fs);      }
    static const char *GetCentMatName(Bool_t path=false, Int_t det=-1)                            { return GetObjectName(fCentMatName,path,det);        }
    static const char *GetCalcMatName(Bool_t path=false, Int_t det=-1)                            { return GetObjectName(fCalcMatName,path,det);        }
//    static const char *GetChgSpecName(UInt_t det, UInt_t fs, UInt_t bs);
    static const char *GetCalGraphName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)              { return GetObjectName(fCalGraphName,path,det,fs);    }
    static const char *GetMulGraphName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)              { return GetObjectName(fMulGraphName,path,det,fs);    }
    //Calibrated spectra can be grouped (ie. plot different detectors together)  
    static const char *GetKinMatName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)                { return GetObjectName(fKinMatName,path,det,fs);      }
    static const char *GetResMatName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)                { return GetObjectName(fResMatName,path,det,fs);      }
    static const char *GetExcMatName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)                { return GetObjectName(fExcMatName,path,det,fs);      }
//    static const char *GetExcSpecName(Int_t det=-1, Int_t fs=-1, Int_t bs=-1);

    static const char *GetListName(Int_t det=-1, Int_t fs=-1, Int_t bs = -1); 
    static const char *GetFitInfoName(Bool_t path=false, Int_t det=-1, Int_t fs=-1, Int_t bs=-1)  { return GetObjectName(fFitInfoName,path,det,fs,bs);  }

    // as ChgSpec and ExcSpec are simply projections, they will already be formatted
    static TObject *CreateObject(const char *objtype, UInt_t DET, UInt_t FS);

  private:
    static const char *GetObjectName(std::string objtype, Bool_t path = false, Int_t DET=-1, Int_t FS=-1, Int_t BS=-1);

    static std::string fChgMatName   ;    // [TH2F] Charge vs BackStrip    [p+d+c],[a]
    static std::string fChgSpecName  ;    // [TH1D] Charge                 [p+d+c],[a]
    static std::string fCentMatName  ;    // [TH2F] Centroid               [p+d+c],[a]
    static std::string fCalcMatName  ;    // [TH2F] Calculated energy      [p+d+c],[a]
    static std::string fCalGraphName ;    // [TGE]  Energy vs Charge       [p+d+a]
    static std::string fMulGraphName ;    // [TGE]  Energy vs Charge       [p+d+a]
    static std::string fKinMatName   ;    // [TH2F] Energy vs ThetaLab     [p],[d]
    static std::string fResMatName   ;    // [TH2F] Energy-Kin vs ThetaLab [p],[d]
    static std::string fExcMatName   ;    // [TH2F] Excitation vs ThetaLab [p],[d]
    static std::string fExcSpecName  ;    // [TH1D] Excitation             [p],[d]
                
    static std::string fFitInfoName  ;    // [TFitInfo] contaier for fit results
  
  ClassDef(TSharcFormat,0)
};

#endif
