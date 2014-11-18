
#include <TObjectManager.h>

ClassImp(TObjectManager)



std::string TObjectManager::fChgMatName   = "ChargeMatrix";       // [TH2F] Charge vs BackStrip    [p+d+c],[a]
std::string TObjectManager::fChgSpecName  = "ChargeSpectrum";     // [TH1D] Charge                 [p+d+c],[a]
std::string TObjectManager::fCalGraphName = "CalibrationGraph";   // [TGE]  Energy vs Charge       [p+d+a]
std::string TObjectManager::fMulGraphName = "MultiGraph";         // [TGE]  Energy vs Charge       [p+d+a]
std::string TObjectManager::fKinMatName   = "KinematicsMatrix";   // [TH2F] Energy vs ThetaLab     [p],[d]
std::string TObjectManager::fKinResName   = "KinematicsResidual"; // [TH2F] Energy-Kin vs ThetaLab [p],[d]
std::string TObjectManager::fExcMatName   = "ExcitationMatrix";   // [TH2F] Excitation vs ThetaLab [p],[d]
std::string TObjectManager::fExcSpecName  = "ExcitationMatrix";   // [TH1D] Excitation             [p],[d]



TObjectManager *TObjectManager::fObjectManager = 0;

TObjectManager *TObjectManager::Get() {
  if(!fObjectManager)
    fObjectManager = new TObjectManager(true);
  
  return fObjectManager;
}

TObjectManager::TObjectManager() : fMasterList(0) {
     // default constructor, should not be used. //
 }
     
TObjectManager::TObjectManager(bool flag) : fMasterList(0) {
  //do useful things.
    Clear();
    fMasterList = new TList();
    fMasterList->SetName("MasterList");
}


TObjectManager::~TObjectManager() { 
  if(fMasterList)
     fMasterList->Delete();
}

void TObjectManager::Print(Option_t *opt) { }

void TObjectManager::Clear(Option_t *opt) { }

//UInt_t TObjectManager::LocatePlots(const char *datadir){}


void TObjectManager::CreateList(const char *listname) {
  TList *list = new TList();
  list->SetName(listname);
  fMasterList->Add(list);
}

void TObjectManager::AddObjectToList(TObject *object, const char * listname) {
  TList *list = (TList*)fMasterList->FindObject(listname);
  if(!list) {
    printf("Warning: list %s not found!!\n",listname);
    return;
  }
  list->Add(object);
  return;
}

TObject *TObjectManager::GetObject(const char *oname, const char *listname) {
  TList  *list = 0;
  TObject *obj = 0;
  if(!listname)
    list = fMasterList;
  else
    list = (TList*)fMasterList->FindObject(listname);
  if(!list) {
    printf("Warning: list %s not found!!\n",listname);
    return obj;
  }
  obj = list->FindObject(oname);
  if(!obj) {
    printf("Warning: object %s not found in list %s!!\n",oname,listname);
  }
  return obj;
}





