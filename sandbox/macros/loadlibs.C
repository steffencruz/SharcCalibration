
void loadlibs(){
  gSystem->Load("libSpectrum.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libFitInfo.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libFitManager.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libObjectManager.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libDataManager.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libSharcName.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libFileWriter.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libSharcInput.so");
}
