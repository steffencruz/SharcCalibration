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
    TFitInfo(TF1 *func, Double_t *xvals, Double_t *yvals, UInt_t npeaks, Bool_t flag=true, const char *infoname=""); 
    virtual ~TFitInfo();

    void Clear(Option_t *opt = "");
    void Print(Option_t *opt = "");

  public:
    UInt_t GetNPeaks()                            { return fnpeaks                  ; }
    Double_t GetX(int i)                          { return fxvalues.at(i)           ; }
    Double_t GetY(int i)                          { return fyvalues.at(i)           ; }
    TF1 *GetFunction()                            { return &ffunction               ; }
    Bool_t GetStatus()                            { return fgood                    ; }
    const char *GetPeakName(int i)                { return fpeaknames.at(i).c_str() ; }
    UInt_t GetPeakNum(const char *name);

  private:
    void SetNPeaks(UInt_t &n)                     { fnpeaks = n                     ; }
    void SetX(Double_t &xvalue)                   { fxvalues.push_back(xvalue)      ; }
    void SetY(Double_t &yvalue)                   { fxvalues.push_back(yvalue)      ; }
    void SetXY(Double_t &xvalue,Double_t &yvalue) { SetX(xvalue); SetY(yvalue)      ; }
    void SetFunction(TF1 *func)                   { ffunction = *func               ; }
    void SetStatus(bool flag)                     { fgood = flag                    ; }
    void SetPeakName(int i, const char *name)     { fpeaknames.at(i) = name         ; }

  public:
    void SetPeakNames(std::vector<std::string> peaknames);
    void SetInfoName(const char *infoname);

  private:
    UInt_t fnpeaks;
    bool fgood;
    TF1  ffunction;
    std::vector<Double_t> fxvalues;
    std::vector<Double_t> fyvalues;
    std::vector<std::string> fpeaknames;

  ClassDef(TFitInfo,1)
};


#endif
