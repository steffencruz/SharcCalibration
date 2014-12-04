
#ifndef TSHARCFORMAT_H
#define TSHARCFORMAT_H

#include <cstdio>
#include <string>

#include <Rtypes.h>
#include <TObject.h>

// more useful copy of TSharcName, but with ability to apply standard formatting to names and histograms
class TSharcFormat : public TObject {

  public:
    static TSharcFormat *Get();
    virtual ~TSharcFormat();

    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");

    //Uncalibrated spectra cannot be grouped
    const char *GetChgMatName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)                { return GetObjectName(fChgMatName.c_str(),path,det,fs);      }
    const char *GetCentMatName(Bool_t path=false, Int_t det=-1)                            { return GetObjectName(fCentMatName.c_str(),path,det);        }
    const char *GetCalcMatName(Bool_t path=false, Int_t det=-1)                            { return GetObjectName(fCalcMatName.c_str(),path,det);        }
    const char *GetChgSpecName(Bool_t path=false, Int_t det=-1, Int_t fs=-1, Int_t bs=-1)  { return GetObjectName(fChgSpecName.c_str(),path,det,fs,bs);  }
    const char *GetCalGraphName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)              { return GetObjectName(fCalGraphName.c_str(),path,det,fs);    }
    const char *GetMulGraphName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)              { return GetObjectName(fMulGraphName.c_str(),path,det,fs);    }
    //Calibrated spectra can be grouped (ie. plot different detectors together)  
    const char *GetKinMatName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)                { return GetObjectName(fKinMatName.c_str(),path,det,fs);      }
    const char *GetResMatName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)                { return GetObjectName(fResMatName.c_str(),path,det,fs);      }
    const char *GetExcMatName(Bool_t path=false, Int_t det=-1, Int_t fs=-1)                { return GetObjectName(fExcMatName.c_str(),path,det,fs);      }
//    static const char *GetExcSpecName(Int_t det=-1, Int_t fs=-1, Int_t bs=-1);

    const char *GetListName(Int_t det=-1, Int_t fs=-1, Int_t bs = -1); 
    const char *GetFitInfoName(Bool_t path=false, Int_t det=-1, Int_t fs=-1, Int_t bs=-1)  { return GetObjectName(fFitInfoName.c_str(),path,det,fs,bs);  }

    // as ChgSpec and ExcSpec are simply projections, they will already be formatted
    TObject *CreateObject(const char *objtype, UInt_t DET, UInt_t FS);

  private:
    TSharcFormat(); 
    static TSharcFormat *fSharcFormat;
    const char *GetObjectName(const char*, Bool_t path = false, Int_t DET=-1, Int_t FS=-1, Int_t BS=-1);

    static const std::string fChgMatName  ;    // [TH2F] Charge vs BackStrip    [p+d+c],[a]
    static const std::string fChgSpecName ;    // [TH1D] Charge                 [p+d+c],[a]
    static const std::string fCentMatName ;    // [TH2F] Centroid               [p+d+c],[a]
    static const std::string fCalcMatName ;    // [TH2F] Calculated energy      [p+d+c],[a]
    static const std::string fCalGraphName;    // [TGE]  Energy vs Charge       [p+d+a]
    static const std::string fMulGraphName;    // [TGE]  Energy vs Charge       [p+d+a]
    static const std::string fKinMatName  ;    // [TH2F] Energy vs ThetaLab     [p],[d]
    static const std::string fResMatName  ;    // [TH2F] Energy-Kin vs ThetaLab [p],[d]
    static const std::string fExcMatName  ;    // [TH2F] Excitation vs ThetaLab [p],[d]
    static const std::string fExcSpecName ;    // [TH1D] Excitation             [p],[d]
     
    static const std::string fFitInfoName  ;    // [TFitInfo] contaier for fit results
  
  ClassDef(TSharcFormat,0)
};

#endif
