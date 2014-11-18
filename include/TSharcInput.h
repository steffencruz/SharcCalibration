#ifndef TSHARCINPUT_H
#define TSHARCINPUT_H

#include <TNamed.h>

class TSharcInput : public TNamed  { 
  public:
    TSharcInput();  // default  constructor.
    static TSharcInput *Get();
    virtual ~TSharcInput();   

  private:
    static TSharcInput *fSharcInput;
    TSharcInput(bool);
      

  public:


  private:


  ClassDef(TSharcInput,1)

};

#endif

