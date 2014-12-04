

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
  cal->DeltaCal(argv[1]);
	
  return 0;
	
}


