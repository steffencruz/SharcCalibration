

TSharc *TDataManager::fSharc = 0;
TTigress *TDataManager::fTigress = 0;
TTriFoil *TDataMAnager::fTrifoil = 0;

void SetChain(const char *treenames){

chain->Add(treenames);

chain->SetBranchAddress("TSharc",&fSharc);
chain->SetBranchAddress("TTigress",&fTigress);
chain->SetBranchAddress("TTriFoil",&fTrifoil);

// other useful chain stuff
//	
// 
}

void ProcessData(TChain *chain=0, FillOptions *fo=0){

if(!chain)
	return;

//FillOptions maybe contains things like index of first and last event to loop over. 

for(int i=0; i<nevents; i++){

	chain->GetEntry(i);
	
	if(ThrowEvent()) // ThrowEvent makes decisions about what is 'good data'
		continue;

	FillHists();
}
	return;
}

Bool_t ThrowEvent()

void FillHists(void){

	fSharc->GetFrontCharge();


}



