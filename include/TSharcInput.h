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

    void SetZ(UInt_t Z) { fprotons = Z;  }
    void SetN(UInt_t N) { fneutrons = N; }



    UInt_t GetZ() { return fprotons; }
    UInt_t GetN() { return fneutrons; }
    UInt_t GetA() { return fprotons + fneutrons; }



  private:
    UInt_t      fprotons;           // beam proton number
    UInt_t      fneutrons;          // beam neutron number
    TVector3    fposition_offset;   // sharc position offset
    Double_t    ftargetthickness;   // target thickness
    std::string ftargetmaterial;    // target material
    std::string frundatadir;        // location of run data
    std::string fsrcdatadir;        // location of source data
    

  ClassDef(TSharcInput,1)

};

#endif

