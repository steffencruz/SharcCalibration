#ifndef TFITINFO_H
#define TFITINFO_H

#include <TNamed.h>

class TFitInfo : public TNamed {

  public:
    TFitInfo()
    virtual ~TFitInfo();

    void Clear(Option_t *opt);
    void Print(Option_t *opt);

    Int_t GetNParm() { return fxvalues.size();}

    void SetX(double &xvalue) { fxvalues.push_back(xvalue); }
    void SetX(double &yvalue) { fxvalues.push_back(yvalue); }
    void SetXY(double &xvalue,double &yvalue) { SetX(xvalue); SetY(yvalue); }

    Double_t GetX(int i) { return xvalue.at(i); }
    Double_t GetY(int i) { return yvalue.at(i); }

    void SetStatus(bool flag) { fgood = flag; }
    bool Status() { return fgood; }
  
    void SetFunction(TF1 *func) {ffunction = func; }
    TF1 *GetFunction()          {return &ffunction; }

  private:
    bool fgood;
    TF1  ffunction;
    std::vector<double> fxvalues;
    std::vector<double> fyvalues;

  ClassDef(TFitInfo,1)
};


#endif
