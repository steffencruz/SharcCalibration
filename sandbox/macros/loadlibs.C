
void loadlibs(){
  gSystem->Load("libSpectrum.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libSharcFormat.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libFitInfo.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libFitManager.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libObjectManager.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libDataManager.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libFileManager.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libSharcInput.so");
  gSystem->Load("/home/tiguser/packages/SharcCalibration/libs/libCalibrate.so");
}
