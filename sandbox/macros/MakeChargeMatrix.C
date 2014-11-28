TH2F *h;

TSharcInput *si;
TObjectManager *om;
TDataManager *dm;

void MakeChargeMatrix(const char *inputname = "exampleinput.sharc", UInt_t nentries=0){

  si = TSharcInput::Get();   // parses user settings

  if(!si->InitSharcInput(inputname))
     return;
  si->Print("a");
  printf("\n - - -- - - - - -- - - - - - -- - -  - -  -- - - -  - - -\n");
  om = TObjectManager::Get();// container for histogram 
  dm = TDataManager::Get();// Sets up necessary variables to loop over data

  UInt_t DET = 5;
  UInt_t FS = 12;

  // make list
  const char *listname = TSharcName::GetListName(DET,FS);
  printf("listname = %s\n",listname);
  om->CreateList(listname);

// currently nobody is making the empty histograms.. who should do it?
  const char *chgmatname = (const char *)TSharcName::GetChgMatName(DET,FS);
  printf("chgmatname = %s\n",chgmatname);
  
  h = new TH2F(chgmatname,chgmatname,48,0,48,16000,0,16000);
  om->AddObjectToList(h,listname);

  dm->ProcessData(); // when this loops over data it should locate the chgmat(DET=5,FS=12) object and fill it.

//  TFileWriter::WriteFile(); // uses TSharcName output file convention and writes TObjectManager's master list

  return;
}
