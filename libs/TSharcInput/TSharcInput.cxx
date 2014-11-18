#include "Globals.h"
#include "TSharcInput.h"

ClassImp(TSharcInput)

TSharcInput *TSharcInput::fSharcInput = 0;

TSharcInput *TSharcInput::Get() {
  if(!fSharcInput)
    fSharcInput = new TSharcInput(true);
  return fSharcInput;
}

TSharcInput::TSharcInput() {
  // default constructor, should not be used. //
}

TSharcInput::TSharcInput(bool flag) {
  //do useful things.

}


TSharcInput::~TSharcInput() {  }



void TSharcInput::Print(Option_t *opt) { 

  printf(DCYAN "TSharcInput:" RESET_COLOR "\n");
  printf("\t beam A : \t %i\n",10);



}


void TSharcInput::Clear(Option_t *opt) {  }
