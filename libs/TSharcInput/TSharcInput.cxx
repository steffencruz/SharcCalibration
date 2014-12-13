#include<fstream>
#include<string>
#include<sstream>

#include "Globals.h"
#include "TSharcInput.h"

#include "TCalibrate.h"
#include "TChannel.h"
#include "TNucleus.h"
#include "TKinematics.h"
#include "TSharc.h"
#include "TSharcAnalysis.h"

ClassImp(TSharcInput)

TSharcInput *TSharcInput::fSharcInput = 0;

TSharcInput *TSharcInput::Get() {
  if(!fSharcInput)
    fSharcInput = new TSharcInput(true);
  return fSharcInput;
}

TSharcInput::TSharcInput() {
  // default constructor, should not be used. //
}

TSharcInput::TSharcInput(bool flag) {
  //do useful things.
  Clear();

}


TSharcInput::~TSharcInput() {  }

void TSharcInput::Print(Option_t *opt) { 

  printf("\n * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
  printf(DCYAN "TSharcInput:" RESET_COLOR "\n");
  printf("\t input file name  : \t %s\n",GetInfileName());

  std::string usropt = opt;
  if(usropt.find("file")!=string::npos) {
    PrintInput();
    return;
  } 
  Bool_t fit=false,data=false,cal=false,src=false,run=false;
  if(usropt.find("fit")!=string::npos) {
     fit=true;
  } else if(usropt.find("data")!=string::npos) {
     data=true;
  } else if(usropt.find("cal")!=string::npos) {
     cal=true;
  } else if(usropt.find("src")!=string::npos) {
     src=true;
  } else if(usropt.find("run")!=string::npos) {
     run=true;
  } else if(usropt.find("all")!=string::npos) {
     fit = true;
     data=true;
     cal=true;
     src=true;
     run=true;
  }
  if(run)          printf("\t beam symbol         : \t %s\n",GetBeamSymbol());
  if(run)          printf("\t beam A              : \t %i\n",GetA());
  if(run)          printf("\t beam Z              : \t %i\n",GetZ());
  if(run)          printf("\t beam N              : \t %i\n",GetN());
  if(run)          printf("\t beam EperU          : \t %.3f MeV\n",GetBeamEperU());
  if(run)          printf("\t position offset     : \t x=%.2f mm , y=%.2f mm , z=%.2f mm \n",GetPosOffs().X(),GetPosOffs().Y(), GetPosOffs().Z());
  if(run)          printf("\t target thickness    : \t %.3f\n",GetTargetThickness());
  if(run)          printf("\t target material     : \t %s\n",GetTargetMaterial());
  if(run){         printf("\t run ions            : \t "); for(int i=0; i<GetRunIons().size(); i++) printf("%s      ",GetRunIons().at(i).c_str());   printf("\n"); }
  if(src){         printf("\t src ions            : \t "); for(int i=0; i<GetSrcIons().size(); i++) printf("%s      ",GetSrcIons().at(i).c_str());   printf("\n"); }
  if(src){         printf("\t src energies        : \t "); for(int i=0; i<GetSrcEnergies().size(); i++) printf("%.2f keV  ",GetSrcEnergies().at(i)); printf("\n"); }
  if(cal)          printf("\t calibration file    : \t %s\n",GetCalFile());
  if(data||run)    printf("\t run datadir         : \t %s\n",GetRunDataDir());
  if(data||src)    printf("\t src datadir         : \t %s\n",GetSrcDataDir());
  if(data)         printf("\t data charge range   : \t [%.2f , %.2f]",GetFrontChargeMin(),GetFrontChargeMax());
  if(fit)          printf("\t src fit range       : \t [%.2f , %.2f]\n",GetSrcChgSpecFitRangeMin(),GetSrcChgSpecFitRangeMax());
  if(fit)          printf("\t run fit range       : \t [%.2f , %.2f]\n",GetRunChgSpecFitRangeMin(),GetRunChgSpecFitRangeMax());
  if(fit)          printf("\t src fit function    : \t %s\n",GetSrcChgSpecFitFunction());
  if(fit)          printf("\t run fit function    : \t %s\n",GetSrcChgSpecFitFunction(),GetRunChgSpecFitFunction());

  printf("\n * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
  return;
}

void TSharcInput::PrintInput() {
  printf("File(%s):\n",fInfileName.c_str());
  printf("%s",GetInfileData());
  printf("\n");
  return;
}

void TSharcInput::Clear(Option_t *opt) { 
   
  fprotons = 0;
  fneutrons = 0;
  ftargthick = 0.0;

  ftargmat.clear(); 
  frundatadir.clear();     
  fsrcdatadir.clear();    
}

bool TSharcInput::InitSharcInput(const char *filename){

  CopyInputFile(filename); 

  bool input = ParseInputFile(GetInfileData());
  if(!input) {
    Clear();
    return input;
  }
 
  SetName(filename);

  // might as well set the beam TNucleus
  fbeam = new TNucleus(GetZ(),GetN());
  
  TChannel::ReadCalFile(GetCalFile());
  if(TChannel::GetNumberOfChannels())
    return true;
  
  return false;
}

bool TSharcInput::InitSharcInput(){
  if(fInfileData.length()<1)
     return false;

  bool input = ParseInputFile(GetInfileData());
  if(!input) {
    Clear();
    return input;
  }
 
  fbeam = new TNucleus(GetZ(),GetN());
  TChannel::ReadCalFile(GetCalFile());
  if(TChannel::GetNumberOfChannels())
    return true;

// initialise some reaction stuff
// if target==cd2, composition is c+d+p, decide this using target input?
  
  return  false;
}

const char *TSharcInput::GetDataDir(Option_t *opt){
  if(strcmp(opt,"Run")==0)
    return GetRunDataDir();
  else if(strcmp(opt,"Src")==0)
    return GetSrcDataDir();
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    return 0;
  }
}

std::vector<std::string> TSharcInput::GetData(Option_t *opt){

  if(strcmp(opt,"Run")==0)
    return GetRunData();
  else if(strcmp(opt,"Src")==0)
    return GetSrcData();
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    std::vector<std::string> s;
    return s;
  }

}

std::vector<std::string> TSharcInput::GetIons(Option_t *opt){

  if(strcmp(opt,"Run")==0)
    return GetRunIons();
  else if(strcmp(opt,"Src")==0)
    return GetSrcIons();
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    std::vector<std::string> s; 
    return s;
  }
}

Double_t TSharcInput::GetChgSpecFitRangeMin(Option_t *opt){

  if(strcmp(opt,"Run")==0)
    return GetRunChgSpecFitRangeMin();
  else if(strcmp(opt,"Src")==0)
    return GetSrcChgSpecFitRangeMin();
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    return 0;
  }
}

Double_t TSharcInput::GetChgSpecFitRangeMax(Option_t *opt){

  if(strcmp(opt,"Run")==0)
    return GetRunChgSpecFitRangeMax();
  else if(strcmp(opt,"Src")==0)
    return GetSrcChgSpecFitRangeMax();
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    return 0;
  }
}

const char *TSharcInput::GetChgSpecFitFunction(Option_t *opt){

  if(strcmp(opt,"Run")==0)
    return GetRunChgSpecFitFunction();
  else if(strcmp(opt,"Src")==0)
    return GetSrcChgSpecFitFunction();
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    return 0;
  }
}

Double_t TSharcInput::GetChgSpecResolution(Option_t *opt){

  if(strcmp(opt,"Run")==0)
    return GetRunChgSpecResolution();
  else if(strcmp(opt,"Src")==0)
    return GetSrcChgSpecResolution();
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    return 0;
  }
}

UInt_t TSharcInput::GetIonNumber(const char *ion, Option_t *opt){

  for(int i=0; i<GetIons(opt).size(); i++)
     if(GetIons(opt).at(i).compare(ion)==0)
        return i;

  return 0;// maybe return  null value and not zero (although that woud cause a seg fault..)
}

Double_t TSharcInput::GetChgSpecThreshold(Option_t *opt){

  if(strcmp(opt,"Run")==0)
    return GetRunChgSpecThreshold();
  else if(strcmp(opt,"Src")==0)
    return GetSrcChgSpecThreshold();
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    return 0;
  }
}

Double_t TSharcInput::GetBeamEnergyMidTarget(){ 
  return TSharcAnalysis::GetBeamEnergyInTarget(GetA(),GetBeamEperU()); 
}

TKinematics *TSharcInput::GetRunKinematics(const char *ion, Option_t *opt){
  
  TNucleus *n = new TNucleus(ion);
  TKinematics *kin;
  if(!n)
    kin = 0;
  else 
    kin = new TKinematics(GetBeamEnergyMidTarget()*1e-3,GetBeamSymbol(),ion,ion,GetBeamSymbol());
  
  return kin;
}

std::vector<double> TSharcInput::GetEmeas(UInt_t DET, UInt_t FS, UInt_t BS,const char *ion, Option_t *opt){

  Double_t efull = 0.0;
  std::vector<double> emeas;
  TVector3 position;
  char p = (char)std::tolower(ion[0]); // get lowercase char for ion 'p','d','a'...

  if(strcmp(opt,"Run")==0){
    position = TSharc::GetPosition(DET,FS,BS,GetPosOffs().X(),GetPosOffs().Y(),GetPosOffs().Z());
    TKinematics *kin = GetRunKinematics(Form("%c",p));
    efull = kin->ELab(position.Theta(),2)*1e3;
    emeas = TSharcAnalysis::GetMeasuredEnergy(position,DET,efull,p);
  }
  else if(strcmp(opt,"Src")==0){
    position = TSharc::GetPosition(DET,FS,BS); // no position offset
    efull = GetSrcEnergies().at(GetIonNumber(ion,opt));
    emeas = TSharcAnalysis::GetMeasuredEnergy(position,DET,efull,p,"no_target");// no target energy loss
  }
  else{
    printf("{TSharcInput} Warning : Invalid input '%s'. Generalised getters only work for input of 'Run' or 'Src'.\n",opt);
    return emeas;
  }
  
  if(TCalibrate::Get()->GetFlag("DEBUG"))
    printf("[DET = %i FS = %i BS = %i]\t theta = %.2f [deg]\t efull = %.2f [keV]\t emeas[0] = %.2f [keV]\t emeas[1] = %.2f [keV]\n",DET,FS,BS,position.Theta()*TMath::RadToDeg(),efull,emeas.at(0),emeas.at(1));
  return emeas;
}

void TSharcInput::trim(std::string * line, const std::string trimChars) {
  //Removes the the string "trimCars" from  the string 'line'
  if (line->length() == 0)
    return;
  std::size_t found = line->find_first_not_of(trimChars);

  if (found != std::string::npos)
    *line = line->substr(found, line->length());
  found = line->find_last_not_of(trimChars);

  if (found != std::string::npos)
    *line = line->substr(0, found + 1);

  return;
}

Bool_t TSharcInput::CopyInputFile(const char *filename){

  if(!filename)
     return false;
  
  std::ifstream infile;
  infile.open(filename);
  infile.seekg(0,std::ios::end);
  int length = infile.tellg();
  char buffer[length];

  if(length<1)
     return false;

  infile.seekg(0,std::ios::beg);
  infile.read(buffer,length);
  
  fInfileName.assign(filename);
  fInfileData.assign(buffer);

  return true;
}

Bool_t TSharcInput::ParseInputFile(const char *filedata){
  //Make TChannels from a cal file.
  std::istringstream infile(filedata);

  std::string line;
  int linenumber = 0;
  bool brace_open = false;
  std::string name;

  while (std::getline(infile, line)) {
    linenumber++;
    trim(&line);
    int comment = line.find("//");
    if (comment != std::string::npos) 
      line = line.substr(0, comment);
    
    if (!line.length())
      continue;
   
    int openbrace = line.find("{");   
    int closebrace = line.find("}");                               
    int colon = line.find(":");

    if(openbrace  == std::string::npos && 
       closebrace == std::string::npos &&
       colon  == std::string::npos)
       continue;

     //*************************************//
    if (closebrace != std::string::npos) {
       brace_open = false;
       return CheckInput();
    }
     //*************************************//
    if (openbrace != std::string::npos) {
       brace_open = true;
    }
     //*************************************//
    if (brace_open) {
       int ntype = line.find(":");
       if (ntype != std::string::npos) {
          std::string type = line.substr(0, ntype);
          line = line.substr(ntype + 1, line.length());
          trim(&line);
          std::istringstream ss(line);
          int j = 0;
          while (type[j]) {
            char c = *(type.c_str() + j);
            c = toupper(c);
            type[j++] = c;
          }
          trim(&type);
          //printf("type[%s]:\t%s\n", type.c_str(),line.c_str());
          if(type.compare("DELTACALSTYLE")==0){
             if(line.find("Src")!=std::string::npos)
                SetSrcDeltaCalFlag(true);
             if(line.find("Run")!=std::string::npos)
                SetRunDeltaCalFlag(true);
          } else if(type.compare("Z")==0) {
            UInt_t tempint; ss>>tempint;
            SetZ(tempint);
          } else if(type.compare("N")==0) {
            UInt_t tempint; ss>>tempint;
            SetN(tempint);
          } else if(type.compare("EPERU")==0) {
            Double_t tempdouble; ss>>tempdouble;
            SetBeamEperU(tempdouble);
          } else if(type.compare("SHARCPOSOFFSETXYZ")==0) {
            Double_t tempdouble[3]; 
            ss>>tempdouble[0]; ss>>tempdouble[1]; ss>>tempdouble[2];
            SetPosOffs(tempdouble[0],tempdouble[1],tempdouble[2]);
          } else if(type.compare("TARGETMATERIAL")==0){
            SetTargetMaterial(line.c_str());
          } else if(type.compare("TARGETTHICKNESS")==0){
            Double_t tempdouble; ss>>tempdouble;
            TSharcAnalysis::SetTarget(tempdouble); // IT HAPPENS NOW!!
            SetTargetThickness(tempdouble);
          } else if(type.compare("SRCIONTYPE")==0){
            AddSrcIon(line.c_str());
          } else if(type.compare("SRCIONENERGY")==0){
            Double_t tempdouble; 
            while(ss>>tempdouble)
              AddSrcEnergy(tempdouble);
          } else if(type.compare("RUNIONTYPE")==0){
            AddRunIon(line.c_str());
          } else if(type.compare("RUNDATADIR")==0){
            SetRunDataDir(line.c_str());
          } else if(type.compare("SRCDATADIR")==0){
            SetSrcDataDir(line.c_str());
          } else if(type.compare("RUNDATA")==0){
            AddRunData(line.c_str());
          } else if(type.compare("SRCDATA")==0){
            AddSrcData(line.c_str());
          } else if(type.compare("CALFILE")==0){
            SetCalFile(line.c_str());
          } else if(type.compare("RUNCHARGESPECFUNCTION")==0){
            SetRunChgSpecFunction(line.c_str());
          } else if(type.compare("RUNCHARGESPECRESOLUTION")==0){
            Double_t tempdouble; ss>>tempdouble;
            SetRunChgSpecResolution(tempdouble);
          } else if(type.compare("RUNCHARGESPECTHRESHOLD")==0){
            Double_t tempdouble; ss>>tempdouble;
            SetRunChgSpecThreshold(tempdouble);
          } else if(type.compare("RUNCHARGESPECFITRANGE")==0){
            Double_t tempdouble[2]; 
            ss>>tempdouble[0]; ss>>tempdouble[1];
            SetRunChgSpecFitRange(tempdouble[0],tempdouble[1]);
          } else if(type.compare("SRCCHARGESPECFUNCTION")==0){
            SetSrcChgSpecFunction(line.c_str());
          } else if(type.compare("SRCCHARGESPECRESOLUTION")==0){
            Double_t tempdouble; ss>>tempdouble;
            SetSrcChgSpecResolution(tempdouble);
          } else if(type.compare("SRCCHARGESPECTHRESHOLD")==0){
            Double_t tempdouble; ss>>tempdouble;
            SetSrcChgSpecThreshold(tempdouble);
          } else if(type.compare("SRCCHARGESPECFITRANGE")==0){
            Double_t tempdouble[2]; 
            ss>>tempdouble[0]; ss>>tempdouble[1];
            SetSrcChgSpecFitRange(tempdouble[0],tempdouble[1]);
          } else if(type.compare("FRONTCHARGEMINMAX")==0){
            Double_t tempdouble[2]; 
            ss>>tempdouble[0]; ss>>tempdouble[1];
            SetFrontChargeMinMax(tempdouble[0],tempdouble[1]);
          } else if(type.compare("BACKCHARGEMINMAX")==0){
            Double_t tempdouble[2]; 
            ss>>tempdouble[0]; ss>>tempdouble[1];
            SetBackChargeMinMax(tempdouble[0],tempdouble[1]);
          } else if(type.compare("PADCHARGEMINMAX")==0){
            Double_t tempdouble[2]; 
            ss>>tempdouble[0]; ss>>tempdouble[1];
            SetPadChargeMinMax(tempdouble[0],tempdouble[1]);
         } 
       }
    }
  }
  return false;
}


const char *TSharcInput::MakeOutputName(){

  char outname[256];
  sprintf(outname,"Cal_%iSr.root",GetA());
  std::string sname = outname;

  return sname.c_str();
}


Bool_t TSharcInput::CheckInput(){
  // make sure the essentials paramaters are present, and that vectors are correct lengths etc
  if( 1 == 1)
     return true;
}




void TSharcInput::Streamer(TBuffer &R__b) {

  //make stuff read/write nice.... ooh, you read/write nice.
  UInt_t R__s, R__c;
  if(R__b.IsReading()) {  //reading.
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    TNamed::Streamer(R__b);
    { TString R__str; R__str.Streamer(R__b); fInfileName = R__str.Data(); }
    { TString R__str; R__str.Streamer(R__b); fInfileData = R__str.Data(); }
    InitSharcInput();   
    fSharcInput = this;
    R__b.CheckByteCount(R__s, R__c, TSharcInput::IsA());
  } else {                   //writing.
    R__c = R__b.WriteVersion(TSharcInput::IsA(), kTRUE);
    TNamed::Streamer(R__b);
    { TString R__str = fInfileName.c_str(); R__str.Streamer(R__b);}
    { TString R__str = fInfileData.c_str(); R__str.Streamer(R__b);}
    R__b.SetByteCount(R__c, kTRUE);
  }
}



