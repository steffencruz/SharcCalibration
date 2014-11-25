#ifndef TSHARCINPUT_H
#define TSHARCINPUT_H

#include <Rtypes.h>
#include <TNamed.h>
#include <TVector3.h>

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
    TSharcInput(bool);


    void trim(std::string *line, const std::string trimchars = " \f\t\n\r\v");
    bool ParseInputFile(const char *);

  public:

    void SetZ(UInt_t &Z)                    { fprotons = Z           ; }
    void SetN(UInt_t &N)                    { fneutrons = N          ; }
    void SetTargetThickness(Double_t &tt)   { ftargthick = tt        ; }
    void SetTargetMaterial(const char *tmp) { ftargmat.assign(tmp)   ; }
    void SetRunDataDir(const char *tmp)     { frundatadir.assign(tmp); }
    void SetSrcDataDir(const char *tmp)     { fsrcdatadir.assign(tmp); }


    UInt_t GetZ()                   { return fprotons            ; }
    UInt_t GetN()                   { return fneutrons           ; }
    UInt_t GetA()                   { return fprotons + fneutrons; }
    Double_t GetTargetThickness()   { return ftargthick          ; }
    const char *GetTargetMaterial() { return ftargmat.c_str()    ; }
    const char *GetRunDataDir()     { return frundatadir.c_str() ; }
    const char *GetSrcDataDir()     { return fsrcdatadir.c_str() ; }

    const char *MakeOutputName(void);

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

    std::string frunchgmat;        // location of run data charge matrix (if available)
    std::string fsrcchgmat;        // location of source data charge matrix (if available)

  ClassDef(TSharcInput,1)

};

#endif

