#include "TFileManager.h"

#include <TKey.h>
#include <TDirectoryFile.h>

TFileManager::TFileManager() { }

TFileManager::~TFileManager() { }

void TFileManager::Print(Option_t *opt) { }

void TFileManager::Clear(Option_t *opt) { }

void TFileManager::ReadList(TList *list, Option_t *opt){
  TObjectManager *om = TObjectManager::Get();
  om->CreateList(list->GetName());
  TIter iter((TList*)list);
  while(TObject *listobj = iter.Next())
    om->AddObjectToList(listobj,list->GetName());
}


void TFileManager::ReadKeys(TDirectoryFile *currentdir) {
   
  TList *listofkeys = currentdir->GetListOfKeys();
  TIter iter(listofkeys);
  
  std::string currentpathname = currentdir->GetPath();
  currentpathname.assign(currentpathname.substr(currentpathname.find('/')+1));
  currentpathname += '/';
  TObjectManager::Get()->CreateList(currentpathname.c_str());
  
  while(TKey *key = (TKey*)iter.Next()) {
    TObject *obj = key->ReadObj();
//    obj->Print();  
    if(obj->InheritsFrom("TSharcInput")){
      TSharcInput *si = (TSharcInput*)obj;
      si->Print();
    } else if(obj->InheritsFrom("TDirectoryFile"))
      ReadKeys((TDirectoryFile*)obj);
    else
      TObjectManager::Get()->AddObjectToList(obj,currentpathname.c_str());
  }
  
  return ;
}

Bool_t TFileManager::ReadFile(const char *fname, Option_t *opt){

  std::string sname = fname;
  if(sname.find(".root")==string::npos){
    printf("{TFileManager} Warning :  '%s' is not a .root file! \n",fname);
    return false;
  }

  TFile *f = new TFile(fname,"READ");
  if(!f->IsOpen()){
    printf("{TFileManager} Warning :  Could not open root file '%s'. \n",fname);
    return false;
  }
  ReadKeys((TDirectoryFile*)f);
  UInt_t nobjread = TObjectManager::Get()->GetMasterList()->GetEntries();
  std::string siname = TSharcInput::Get()->GetInfileName();
  printf("SHARC INPUT NAME IS %s ???!?!!???**^$%^$$%%^$%\n\n",siname.c_str());
  if(nobjread){
    printf("{TFileManager} Loaded %i TObjects into calibration session, and TSharcInput '%s'.\n",nobjread,TSharcInput::Get()->GetInfileName());
    return true;
  } else {
    printf("TFileManager} Warning :  No TSharcInput object was found. Calibration file was unsuccessfully read.\n");
    return false;
  }

}


void TFileManager::WriteList(TFile &file,TList *list, Option_t *opt){
  if(!list)
     return;

  TIter iter(list);
  while(TObject *obj = iter.Next()) {
    file.cd("/");
    if(obj->InheritsFrom("TList")) {
      TList *currentlist = ((TList*)obj);
      const char *dname = currentlist->GetName();
    //  printf("dname = %s \n",dname);
      if(!file.GetDirectory(dname)) {
        file.mkdir(dname);
      }
      file.cd(dname);
      currentlist->Sort();
    }
    obj->Write();
  }

  return;
}

void TFileManager::WriteFile(const char *fname, Option_t *opt) {

  if(!fname)
     fname = TSharcInput::Get()->MakeOutputName();
  TFile file(fname,opt);

  TSharcInput::Get()->Write(); // writes the input parameters
  WriteList(file,TObjectManager::Get()->GetMasterList());
  file.Close();
  return;
}





