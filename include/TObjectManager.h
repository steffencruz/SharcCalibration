#ifndef THISTMANAGER_H
#define THISTMANAGER_H

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
    void CreateList(const char *listname);

    TObject *GetObject(const char *oname, const char *listname=0);

  private:
    
    // Standardised plot names
    static std::string fChgMatName   ;// [TH2F] Charge vs BackStrip    [p+d+c],[a]
    static std::string fChgSpecName  ;// [TH1D] Charge                 [p+d+c],[a]
    static std::string fCalGraphName ;// [TGE]  Energy vs Charge       [p+d+a]
    static std::string fMulGraphName ;// [TGE]  Energy vs Charge       [p+d+a]
    static std::string fKinMatName   ;// [TH2F] Energy vs ThetaLab     [p],[d]
    static std::string fKinResName   ;// [TH2F] Energy-Kin vs ThetaLab [p],[d]
    static std::string fExcMatName   ;// [TH2F] Excitation vs ThetaLab [p],[d]
    static std::string fExcSpecName  ;// [TH1D] Excitation             [p],[d]
/*
    // Associated lists
    TList *fChgMatList[16];
    TList *fChgSpecList[16];
    TList *fCalGraphList[16];
    TList *fMulGraphList[16];
    TList *fKinMatList[16];
    TList *fKinResList[16];
    TList *fExcMatList[16];
    TList *fExcSpecList[16];
*/
  ClassDef(TObjectManager,1)

};

#endif

