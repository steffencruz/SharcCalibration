#ifndef TFITINFO_H
#define TFITINFO_H

#include <vector>
#include <string>

#include <TNamed.h>
#include <Rtypes.h>
#include <TF1.h>

class TFitInfo : public TNamed {

  public:
    TFitInfo();
    TFitInfo(TF1 *func, Double_t *xvals, Double_t *yvals, UInt_t npeaks,Int_t *indices, Bool_t flag=true, const char *infoname=""); 
    virtual ~TFitInfo();

    void Clear(Option_t *opt = "");
    void Print(Option_t *opt = "");

  public:
    UInt_t GetNPeaks()                            { return fnpeaks                  ; }
    Double_t GetX(int i)                          { return fxvalues.at(i)           ; }
    Double_t GetY(int i)                          { return fyvalues.at(i)           ; }
    Double_t GetXE(int i)                         { return fxevalues.at(i)          ; }
    Double_t GetYE(int i)                         { return fyevalues.at(i)          ; }
    TF1 *GetFunction()                            { return &ffunction               ; }
    Bool_t GetStatus()                            { return fgood                    ; }
    Double_t GetChi2(UInt_t i)                    { return fchi2.at(i)              ; } // crazxty
    Double_t GetNormChi2(UInt_t i)                { return fnormchi2.at(i)          ; } // crazxty
    
    const char *GetPeakName(int i)                { return fpeaknames.at(i).c_str() ; }
    UInt_t GetPeakNum(const char *name);
    Double_t GetParm(int peaknumber,const char *parname, const char *ion);


  private:
    void SetNPeaks(UInt_t &n)                     { fnpeaks = n                     ; }
    void SetX(Double_t &xvalue)                   { fxvalues.push_back(xvalue)      ; }
    void SetY(Double_t &yvalue)                   { fyvalues.push_back(yvalue)      ; }
    void SetXY(Double_t xvalue,Double_t yvalue) { SetX(xvalue); SetY(yvalue)      ; }
    void SetFunction(TF1 *func)                   { ffunction = *func               ; }
    void SetXE(Double_t &xevalue)                    { fxevalues.push_back(xevalue)   ; }
    void SetYE(Double_t &yevalue)                    { fyevalues.push_back(yevalue)   ; }
    void SetXYE(Double_t xevalue,Double_t yevalue) { SetXE(xevalue); SetYE(yevalue) ; }
    
    void SetStatus(bool flag)                      { fgood = flag                    ; }
    void SetChi2(Double_t chi2)                    { fchi2.push_back(chi2)           ; }
    void SetNormChi2(Double_t chi2)                { fnormchi2.push_back(chi2)       ; }
    
    void SetPeakName(int i, const char *name)      { fpeaknames.at(i) = name         ; }

  public:
    void SetPeakNames(std::vector<std::string> peaknames,const char *opt);
    void SetInfoName(const char *infoname);

  private:
    UInt_t fnpeaks;
    bool fgood;
    std::vector<Double_t> fchi2;
    std::vector<Double_t> fnormchi2;
    TF1  ffunction;
    std::vector<Double_t> fxvalues;
    std::vector<Double_t> fyvalues;
    std::vector<Double_t> fxevalues;
    std::vector<Double_t> fyevalues;
    std::vector<std::string> fpeaknames;

  ClassDef(TFitInfo,1)
};


#endif
