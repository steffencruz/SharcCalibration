
TH1D *hist = 0;
Double_t pars[] = {1,0.0,0.0,30,1500,100,100,50,2700,100,100};

void GetChargeSpectrum(){

  TFile *f = new TFile("/tig/grsmid01_data4/tigress/S1389Work_part2/Steffen/CalUtils/CalibrationsElastic/Tests_Oct28/ChargeMatrix_95Sr_Nentries100000000.root");
  TH2F *h2 = f->FindObjectAny("ChargeVsBackStrip_DET05_FS12");
  hist = h2->ProjectionY("_py5",5,5);

  hist->GetXaxis()->SetRangeUser(500,3500);
}
