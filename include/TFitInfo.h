#ifndef TFITINFO_H
#define TFITINFO_H

#include <vector>

#include <TNamed.h>
#include <Rtypes.h>
#include <TF1.h>

class TFitInfo : public TNamed {

  public:
    TFitInfo();
    TFitInfo(TF1 *func,Float_t *xvals, Float_t *yvals, UInt_t npeaks, Bool_t flag=true); 
    virtual ~TFitInfo();

    void Clear(Option_t *opt = "");
    void Print(Option_t *opt = "");

  public:
    Int_t GetNParm() { return fxvalues.size();}

    void SetX(Float_t &xvalue) { fxvalues.push_back(xvalue); }
    void SetY(Float_t &yvalue) { fxvalues.push_back(yvalue); }
    void SetXY(Float_t &xvalue,Float_t &yvalue) { SetX(xvalue); SetY(yvalue); }

    Float_t GetX(int i) { return fxvalues.at(i); }
    Float_t GetY(int i) { return fyvalues.at(i); }

    void SetInfoName(const char *infoname = "");

    void SetStatus(bool flag) { fgood = flag; }
    bool Status() { return fgood; }
  
    void SetFunction(TF1 *func) {ffunction = *func; }
    TF1 *GetFunction()          {return &ffunction; }

  private:
    bool fgood;
    TF1  ffunction;
    std::vector<Float_t> fxvalues;
    std::vector<Float_t> fyvalues;

  ClassDef(TFitInfo,1)
};


#endif
