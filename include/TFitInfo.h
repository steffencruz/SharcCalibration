#ifndef TFITINFO_H
#define TFITINFO_H

#include <vector>

#include <TNamed.h>
#include <Rtypes.h>
#include <TF1.h>

class TFitInfo : public TNamed {

  public:
    TFitInfo();
    TFitInfo(TF1 *func, Double_t *xvals, Double_t *yvals, UInt_t npeaks, Bool_t flag=true); 
    virtual ~TFitInfo();

    void Clear(Option_t *opt = "");
    void Print(Option_t *opt = "");

  public:
    Int_t GetNParm() { return fxvalues.size();}

    void SetX(Double_t &xvalue) { fxvalues.push_back(xvalue); }
    void SetY(Double_t &yvalue) { fxvalues.push_back(yvalue); }
    void SetXY(Double_t &xvalue,Double_t &yvalue) { SetX(xvalue); SetY(yvalue); }

    Double_t GetX(int i) { return fxvalues.at(i); }
    Double_t GetY(int i) { return fyvalues.at(i); }

    void SetInfoName(const char *infoname = "");

    void SetStatus(bool flag) { fgood = flag; }
    bool Status() { return fgood; }
  
    void SetFunction(TF1 *func) {ffunction = *func; }
    TF1 *GetFunction()          {return &ffunction; }

  private:
    bool fgood;
    TF1  ffunction;
    std::vector<Double_t> fxvalues;
    std::vector<Double_t> fyvalues;

  ClassDef(TFitInfo,1)
};


#endif
