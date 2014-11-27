#ifndef TSHARCINPUT_H
#define TSHARCINPUT_H

#include <Rtypes.h>
#include <TNamed.h>
#include <TVector3.h>


// this class is the brains of the whole package. It is the only class containing specific values and settings
class TSharcInput : public TNamed  { 
  public:
    TSharcInput();  // default  constructor.
    static TSharcInput *Get();
    virtual ~TSharcInput();   

    bool InitSharcInput(const char*);

    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");

  private:
    static TSharcInput *fSharcInput;
    TSharcInput(Bool_t);

    void trim(std::string *line, const std::string trimchars = " \f\t\n\r\v");
    bool ParseInputFile(const char *);

  public:

    void SetZ(UInt_t &Z)                    { fprotons = Z           ; }
    void SetN(UInt_t &N)                    { fneutrons = N          ; }
    void SetTargetThickness(Double_t &tt)   { ftargthick = tt        ; }
    void SetTargetMaterial(const char *tmp) { ftargmat.assign(tmp)   ; }
    void SetRunDataDir(const char *tmp)     { frundatadir.assign(tmp); }
    void SetSrcDataDir(const char *tmp)     { fsrcdatadir.assign(tmp); }
    void AddRunData(std::string tmp)        { frundata.push_back(tmp); }
    void AddSrcData(std::string tmp)        { fsrcdata.push_back(tmp); }

    void SetFrontChargeMinMax(Double_t chgmin = 0.0, Double_t chgmax = 0.0) 
         {fFrontCharge_min = chgmin;  fFrontCharge_max = chgmax;}
    void SetBackChargeMinMax(Double_t chgmin = 0.0, Double_t chgmax = 0.0) 
         {fBackCharge_min = chgmin;  fBackCharge_max = chgmax;}
    void SetPadChargeMinMax(Double_t chgmin = 0.0, Double_t chgmax = 0.0) 
         {fPadCharge_min = chgmin;  fPadCharge_max = chgmax;}

    
    UInt_t GetZ()                   { return fprotons            ; }
    UInt_t GetN()                   { return fneutrons           ; }
    UInt_t GetA()                   { return fprotons + fneutrons; }
    Double_t GetTargetThickness()   { return ftargthick          ; }
    const char *GetTargetMaterial() { return ftargmat.c_str()    ; }
    const char *GetRunDataDir()     { return frundatadir.c_str() ; }
    const char *GetSrcDataDir()     { return fsrcdatadir.c_str() ; }
    std::vector<std::string> GetRunData() { return frundata; }
    std::vector<std::string> GetSrcData() { return fsrcdata; }

    Double_t GetFrontChargeMin()    { return fFrontCharge_min    ; }
    Double_t GetFrontChargeMax()    { return fFrontCharge_max    ; }
    Double_t GetBackChargeMin()     { return fBackCharge_min     ; }
    Double_t GetBackChargeMax()     { return fBackCharge_max     ; }
    Double_t GetPadChargeMin()      { return fPadCharge_min      ; }
    Double_t GetPadChargeMax()      { return fPadCharge_max      ; }

    const char *MakeOutputName();

    void SetRunChgMat(const char *tmp)   { frunchgmat.assign(tmp); }
    void SetSrcChgMat(const char *tmp)   { fsrcchgmat.assign(tmp); }
    const char *GetRunChgMat()     { return frunchgmat.c_str()   ; }
    const char *GetSrcChgMat()     { return fsrcchgmat.c_str()   ; }

  private:
    UInt_t      fprotons;           // beam proton number
    UInt_t      fneutrons;          // beam neutron number
    TVector3    fposition_offset;   // sharc position offset
    Double_t    ftargthick;         // target thickness
    std::string ftargmat;           // target material
    std::string frundatadir;        // location of run data
    std::string fsrcdatadir;        // location of source data

    std::vector<std::string> frundata;
    std::vector<std::string> fsrcdata;

    std::string frunchgmat;        // location of run data charge matrix (if available)
    std::string fsrcchgmat;        // location of source data charge matrix (if available)

    // settings to apply when looping over data
    Double_t fFrontCharge_min;  // minimum front charge
    Double_t fFrontCharge_max;  // maximum front charge
    Double_t fBackCharge_min;  // "" back charge
    Double_t fBackCharge_max;
    Double_t fPadCharge_min;  // "" pad charge/
    Double_t fPadCharge_max;

  ClassDef(TSharcInput,1)

};

#endif

