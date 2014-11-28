#include "TFileWriter.h"



TFileWriter::TFileWriter() { }

TFileWriter::~TFileWriter() { }

void TFileWriter::Print(Option_t *opt) { }


void TFileWriter::Clear(Option_t *opt) { }


void TFileWriter::WriteList(TFile &file,TList *list, Option_t *opt){
  if(!list)
     return;

  TIter iter(list);
  while(TObject *obj = iter.Next()) {
    file.cd("/");
    if(obj->InheritsFrom("TList")) {
      TList *currentlist = ((TList*)obj);
      const char *dname = currentlist->GetName();
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

void TFileWriter::WriteFile(const char *fname, Option_t *opt) {

  if(!fname)
     fname = TSharcInput::Get()->MakeOutputName();
  TFile file(fname,opt);

  TSharcInput::Get()->Write(); // writes the input parameters
  WriteList(file,TObjectManager::Get()->GetMasterList());
  file.Close();
  return;
}





