
#include <TObjectManager.h>

ClassImp(TObjectManager)

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


TList *TObjectManager::GetList(const char *listname) {

  TList *list = (TList*)fMasterList->FindObject(listname);
  if(!list) {
    list = new TList();
    list->SetName(listname);
    fMasterList->Add(list);
  }
  return list;
}

void TObjectManager::AddObjectToList(TObject *object, const char * listname) {
  TList *list = (TList*)fMasterList->FindObject(listname);
  if(!list) {
    GetList(listname);
    //printf("{TObjectManager} Warning: list '%s' not found!!\n",listname);
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
    printf("{TObjectManager} Warning :  list '%s' not found!!\n",listname);
    return obj;
  }
  obj = list->FindObject(oname);
  if(!obj) {
    printf("{TObjectManager} Warning :  Object '%s' not found in list '%s'!!\n",oname,listname);
  }
  return obj;
}

UInt_t TObjectManager::CountObjectsOfType(const char *oname, TList *list){
  if(!list)
    list = fMasterList;

  std::string str;
  UInt_t n=0;
  TIter iter(list);
  while(TObject *obj = iter.Next()){
 
    if(obj->InheritsFrom("TList")){
       TList *sublist = (TList*) obj;
       n+=CountObjectsOfType(oname,sublist);
    }
    str.assign(obj->GetName());
    if(str.find(oname)!=std::string::npos) // if this basic name is matched, increment the counter
      n++;
  }
  return n;
}
