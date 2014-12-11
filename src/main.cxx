

#include <cstdio>

#include <TCalibrate.h>
#include <TDataManager.h>
#include <TFitInfo.h>
#include <TFitManager.h>
#include <TFileWriter.h>
#include <TObjectManager.h>
#include <TSharcFormat.h>
#include <TSharcInput.h>


void SetOptions();

int main(int argc, char **argv) {
	
	printf("srsly world ... again?\n");

  TCalibrate *cal = TCalibrate::Get();
  std::string strarg = argv[1];
  if(strarg.find(".root")!=std::string::npos)
    cal->LoadCal(argv[1]); // resumes calibration status [sets internal flags]
  
  cal->DeltaCal(argv[1]);// now just continue
	
  return 0;
	
}


