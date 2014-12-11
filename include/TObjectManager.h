#ifndef TOBJECTMANAGER_H
#define TOBJECTMANAGER_H

#include <Rtypes.h>
#include <TObject.h>
#include <TList.h>
#include <TH1.h>
#include <TGraphErrors.h>

// Currently contains all plots necessary to carry out dE calibrations and analyse results
class TObjectManager : public TObject  { 
  public:
    TObjectManager();  // default  constructor.
    static TObjectManager *Get();
    virtual ~TObjectManager();   

    //Int_t LocatePlots(const char *datadir = "");
    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");

  private:
    static TObjectManager *fObjectManager;
    TObjectManager(bool);

    TList *fMasterList;

  public:

    void AddObjectToList(TObject *object,const char *listname);

    TObject *GetObject(const char *oname, const char *listname=0);
    UInt_t CountObjectsOfType(const char *oname, TList *list=0);

    TList *GetList(const char *listname);

    TList *GetMasterList() { return fMasterList; }
    

 ClassDef(TObjectManager,1)

};

#endif

