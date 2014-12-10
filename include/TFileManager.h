#ifndef TFILEMANAGER_H
#define TFILEMANAGER_H

#include <TObject.h>
#include <TFile.h>
#include <TList.h>

#include <TSharcInput.h>
#include <TObjectManager.h>

// Takes care of nice output file structures with nested directories
// Responsible for putting stuff back into objectmanager and sharcinput in read mode
class TFileManager : public TObject {

  public:
    TFileManager();
    virtual ~TFileManager();

    virtual void Print(Option_t *opt="");
    virtual void Clear(Option_t *opt="");

    static Bool_t ReadFile(const char *fname=0, Option_t *opt = "READ");
    static void WriteFile(const char *fname=0, Option_t *opt = "RECREATE");

  private:
    static void ReadKeys(TDirectoryFile *currentdir);
    static void ReadList(TList *list, Option_t *opt = "");
    static void WriteList(TFile &file, TList *list, Option_t *opt = "");

  ClassDef(TFileManager,0);
};

#endif
