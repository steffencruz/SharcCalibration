

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
const char *DoCalibrate(Option_t *opt);

int main(int argc, char **argv) {
	
   printf("srsly world ... again?\n");

   TCalibrate *cal = TCalibrate::Get();

   cal->SetFlag("DEBUG",true);
   cal->SetFlag("Quick",false);
   cal->SetFlag("CentroidsOnly",false);
   cal->SetFlag("DontSave",false);

   std::string strarg = argv[1];
   if(strarg.find(".root")!=std::string::npos)
      cal->LoadCal(argv[1]); // resumes calibration status [sets internal flags]   ///Sets the flags (Decidedes where to get data.)

   if(!cal->GetFlag("SharcInput") && !cal->InitDeltaCal(argv[1])) { 
     printf("{TCalibrate} Warning :  Bad input file, '%s'... You're better than this.\n",argv[1]);
     return 1;
   }

   DoCalibrate("Run");
   DoCalibrate("Src");

   //cal->SaveCal();
   printf("PROGRAM ENDING.\n");

   return 0;
}


const char *DoCalibrate(Option_t *opt) {

   TCalibrate *cal = TCalibrate::Get();

   if(!cal->GetFlag("ChgMats",opt)){       //If I got a root file(with charge mats).
      if(!cal->MakeChargeMatrices(opt))
         return "MakeChargeMatrices";
      
      cal->SetFlag("ChgMats",true,opt);
      cal->Print();
      if(!cal->GetFlag("DontSave"))
         cal->SaveCal("CurrentCal_ChgMats.root"); 
   }
/************************************************/

   cal->SetFlag("ChgSpecs",false,opt);   
   if(!cal->GetFlag("ChgSpecs",opt) || !cal->GetFlag("ChgFits",opt)){  // looks for fitted spectra.
      if(!cal->GetCentroidsFromData(opt))
         return "GetCentroidsFromData";
      
      cal->SetFlag("ChgSpecs",true,opt);
      cal->SetFlag("ChgFits",true,opt);
      cal->Print();
      if(!cal->GetFlag("DontSave"))
         cal->SaveCal("CurrentCal_ChgFits.root"); 
   }
/************************************************/   
   cal->SetFlag("CentMats",false,opt);   
   if(!cal->GetFlag("CentMats",opt)){               // looks for centroid mats.... may not be sorted. 
      if(!cal->PutCentroidsInMatrix(opt))
         return "PutCentroidsInMatrix";
      
      cal->SetFlag("CentMats",true,opt);
      cal->Print();
      if(!cal->GetFlag("DontSave"))
         cal->SaveCal("CurrentCal_CentMats.root"); 
   }
/************************************************/
   if(cal->GetFlag("CentroidsOnly"))
      return "CentroidsFinished";
/************************************************/

   if(!cal->GetFlag("CalcMats",opt)){
      if(!cal->PutEnergiesInMatrix(opt))
         return "PutEnergiesInMatrix";
      
      cal->SetFlag("CalcMats",true,opt);
      cal->Print();
      if(!cal->GetFlag("DontSave"))
         cal->SaveCal("CurrentCal_CalcMats.root"); 
   }
/************************************************/


   if(!cal->GetFlag("CalGraphs",opt)){
      if(!cal->ProduceCalGraphs(opt))
         return "ProduceCalGraphs";
      
      cal->SetFlag("CalGraphs",true,opt);
      cal->Print();
      if(!cal->GetFlag("DontSave"))
         cal->SaveCal("CurrentCal_CalGraphs.root"); 
   }
/************************************************/

/*
   if(!cal->GetFlag("MulGraphs",opt)){
      if(!cal->ProduceMulGraphs(opt))
         return "ProduceMulGraphs";
      
      cal->SetFlag("MulGraphs",true,opt);
      cal->Print();
      if(cal->GetFlag("DontSave"))
         cal->SaveCal("CurrentCal_MulGraphs.root"); 
   }
   */
/************************************************/


   return "";
};


