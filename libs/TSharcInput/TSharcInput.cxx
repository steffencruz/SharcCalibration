#include<fstream>
#include<string>
#include<sstream>

#include "Globals.h"
#include "TSharcInput.h"

#include "TChannel.h"
#include "TNucleus.h"
#include "TKinematics.h"
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

  if(strncmp(opt,"file",4)==0) {
    PrintInput();
    return;
  }
  bool printall = false;
  if(strcmp(opt,"a")==0)
     printall = true;

  printf(DCYAN "TSharcInput:" RESET_COLOR "\n");
  printf("\t beam A           : \t %i\n",GetA());
  printf("\t beam Z           : \t %i\n",GetZ());
  printf("\t beam N           : \t %i\n",GetN());
  printf("\t beam EperU       : \t %.3f MeV\n",GetBeamEperU());
  printf("\t position offset  : \t [%.2f , %.2f , %.2f] \n",GetPosOffs().X(),GetPosOffs().Y(), GetPosOffs().Z());

  if(printall) printf("\t calibration file : \t %s\n",GetCalFile());
  if(printall) printf("\t target thickness : \t %.3f\n",GetTargetThickness());
  if(printall) printf("\t target material  : \t %s\n",GetTargetMaterial());
  if(printall) printf("\t run datadir      : \t %s\n",GetRunDataDir());
  if(printall) printf("\t src datadir      : \t %s\n",GetSrcDataDir());
}

void TSharcInput::PrintInput() {
  printf("File(%s):\n",fInfileName.c_str());
  printf("%s",fInfileData.c_str());
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

Double_t TSharcInput::GetBeamEnergyMidTarget(){ 
  return TSharcAnalysis::GetBeamEnergyInTarget(GetA(),GetBeamEperU()); 
}

TKinematics *TSharcInput::GetElasticKinematics(const char *ion, Option_t *opt){
  
  TNucleus *n = new TNucleus(ion);
  TKinematics *kin;
  if(!n)
    kin = 0;
  else 
    kin = new TKinematics(GetBeamEnergyMidTarget(),GetBeamSymbol(),ion,ion,GetBeamSymbol());
  
  return kin;
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

  //if(!filename) {
  //  printf("could not open file.\n");
  //  return false;
  //}

  //std::string infilename; 
  //infilename.append(filename);
  //if(infilename.length()==0)
  //  return false;

  //ifstream infile;
  //infile.open(infilename.c_str());
  //if (!infile) {
  //  printf("could not open file.\n");
  //  return false;
  //}

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
       return true;
    }
     //*************************************//
    if (openbrace != std::string::npos) {
       brace_open = true;
      // finputname = line.substr(0, openbrace).c_str();
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

          if(type.compare("Z")==0) {
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
            SetTargetThickness(tempdouble);
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
          } else if(type.compare("RUNCHARGESPECFITRANGE")==0){
            Double_t tempdouble[2]; 
            ss>>tempdouble[0]; ss>>tempdouble[1];
            SetRunChgSpecFitRange(tempdouble[0],tempdouble[1]);
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



