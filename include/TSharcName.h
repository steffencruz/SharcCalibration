#ifndef TSHARCNAME_H
#define TSHARCNAME_H

#include <cstdio>
#include <string>

#include <Rtypes.h>
#include <TObject.h>

// Takes care of standardised naming conventions. Useful for structuring output files and easily accessing data
class TSharcName : public TObject {

  public:
    TSharcName(); 
    virtual ~TSharcName();

    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");
/*
    static const char *GetChgMatName  (void)   {return fChgMatName;};
    static const char *GetChgSpecName (void)   {return fChgSpecName;};
    static const char *GetCalGraphName(void)   {return fCalGraphName;};
    static const char *GetMulGraphName(void)   {return fMulGraphName;};
    static const char *GetKinMatName  (void)   {return fKinMatName;};
    static const char *GetKinResName  (void)   {return fKinResName;};
    static const char *GetExcMatName  (void)   {return fExcMatName;};
    static const char *GetExcSpecName (void)   {return fExcSpecName;};
*/

    //Uncalibrated spectra cannot be grouped
    static const char *GetChgMatName(UInt_t det, UInt_t fs);
    static const char *GetChgSpecName(UInt_t det, UInt_t fs, UInt_t bs);
    static const char *GetCalGraphName(UInt_t det, UInt_t fs);
    static const char *GetMulGraphName(UInt_t det, UInt_t fs);
    //Calibrated spectra can be grouped
    static const char *GetKinMatName(Int_t det=-1, Int_t fs=-1);
    static const char *GetResMatName(Int_t det=-1, Int_t fs=-1);
    static const char *GetExcMatName(Int_t det=-1, Int_t fs=-1);
    static const char *GetExcSpecName(Int_t det=-1, Int_t fs=-1, Int_t bs=-1);

    static const char *GetDirName(Int_t det=-1, Int_t fs=-1, Int_t bs = -1); 

  private:
   
    static std::string fChgMatName   ;// [TH2F] Charge vs BackStrip    [p+d+c],[a]
    static std::string fChgSpecName  ;// [TH1D] Charge                 [p+d+c],[a]
    static std::string fCalGraphName ;// [TGE]  Energy vs Charge       [p+d+a]
    static std::string fMulGraphName ;// [TGE]  Energy vs Charge       [p+d+a]
    static std::string fKinMatName   ;// [TH2F] Energy vs ThetaLab     [p],[d]
    static std::string fResMatName   ;// [TH2F] Energy-Kin vs ThetaLab [p],[d]
    static std::string fExcMatName   ;// [TH2F] Excitation vs ThetaLab [p],[d]
    static std::string fExcSpecName  ;// [TH1D] Excitation             [p],[d]
                

  ClassDef(TSharcName,0)
};

#endif
