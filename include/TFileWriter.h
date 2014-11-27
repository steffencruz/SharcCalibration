#ifndef TFILEWRITER_H
#define TFILEWRITER_H

#include <TObject.h>
#include <TFile.h>
#include <TList.h>

#include <TSharcInput.h>
#include <TObjectManager.h>


class TFileWriter : public TObject {

  public:
    TFileWriter();
    virtual ~TFileWriter();

    virtual void Print(Option_t *opt="");
    virtual void Clear(Option_t *opt="");

    static void WriteList(TFile &file, TList *list, Option_t *opt = "");
    static void WriteFile(const char *fname=0, Option_t *opt = "RECREATE");

  ClassDef(TFileWriter,0);
};

#endif
