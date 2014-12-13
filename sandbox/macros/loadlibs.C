
void loadlibs(){
  gSystem->Load("libSpectrum.so");

  gSystem->Load("${PROGDIR}/SharcAnalysis/libSharcAnalysis.so");
  gSystem->Load("${PROGDIR}/SharcCalibration/libs/libSharcFormat.so");
  gSystem->Load("${PROGDIR}/SharcCalibration/libs/libFitInfo.so");
  gSystem->Load("${PROGDIR}/SharcCalibration/libs/libFitManager.so");
  gSystem->Load("${PROGDIR}/SharcCalibration/libs/libObjectManager.so");
  gSystem->Load("${PROGDIR}/SharcCalibration/libs/libDataManager.so");
  gSystem->Load("${PROGDIR}/SharcCalibration/libs/libFileManager.so");
  gSystem->Load("${PROGDIR}/SharcCalibration/libs/libSharcInput.so");
  gSystem->Load("${PROGDIR}/SharcCalibration/libs/libCalibrate.so");

}
