
#ifndef TSHARCNAME_H
#define TSHARCNAME_H

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
    static const char *GetChgMatName(Bool_t fullname, Int_t det=-1, Int_t fs=-1)               { return GetObjectName(fChgMatName,det,fs,fullname);   }
//    static const char *GetChgSpecName(UInt_t det, UInt_t fs, UInt_t bs);
    static const char *GetCalGraphName(Bool_t fullname, Int_t det=-1, Int_t fs=-1);            { return GetObjectName(fCalGraphName,det,fs,fullname); }
    static const char *GetMulGraphName(Bool_t fullname, Int_t det=-1, Int_t fs=-1);            { return GetObjectName(fMulGraphName,det,fs,fullname); }
    //Calibrated spectra can be grouped (ie. plot different detectors together)  
    static const char *GetKinMatName(Bool_t fullname,Int_t det=-1, Int_t fs=-1);       { return GetObjectName(fKinMatName,det,fs,fullname); }
    static const char *GetResMatName(Bool_t fullname,Int_t det=-1, Int_t fs=-1);       { return GetObjectName(fResMatName,det,fs,fullname); }
    static const char *GetExcMatName(Bool_t fullname,Int_t det=-1, Int_t fs=-1);       { return GetObjectName(fExcMatName,det,fs,fullname); }
//    static const char *GetExcSpecName(Int_t det=-1, Int_t fs=-1, Int_t bs=-1);

    static const char *GetListName(Int_t det=-1, Int_t fs=-1, Int_t bs = -1); 

    // as ChgSpec and ExcSpec are simply projections, they will already be formatted
    static TObject *CreateObject(const char *objtype);

  private:
    const char *GetObjectName(const char *objtype, UInt_t DET, UInt_t FS, Bool_t fullname = true);

    static std::string fChgMatName   ;// [TH2F] Charge vs BackStrip    [p+d+c],[a]
    static std::string fChgSpecName  ;// [TH1D] Charge                 [p+d+c],[a]
    static std::string fCalGraphName ;// [TGE]  Energy vs Charge       [p+d+a]
    static std::string fMulGraphName ;// [TGE]  Energy vs Charge       [p+d+a]
    static std::string fKinMatName   ;// [TH2F] Energy vs ThetaLab     [p],[d]
    static std::string fResMatName   ;// [TH2F] Energy-Kin vs ThetaLab [p],[d]
    static std::string fExcMatName   ;// [TH2F] Excitation vs ThetaLab [p],[d]
    static std::string fExcSpecName  ;// [TH1D] Excitation             [p],[d]
                

  ClassDef(TSharcFormat,0)
};

#endif
