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


    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");

  private:
    static TSharcInput *fSharcInput;
    TSharcInput(bool);
      

  public:


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

