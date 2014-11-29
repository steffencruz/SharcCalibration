#include<fstream>
#include<string>
#include<sstream>

#include "Globals.h"
#include "TSharcInput.h"

#include "TChannel.h"
#include "TNucleus.h"

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

void TSharcInput::Clear(Option_t *opt) { 
   
  fprotons = 0;
  fneutrons = 0;
  ftargthick = 0.0;

  ftargmat.clear(); 
  frundatadir.clear();     
  fsrcdatadir.clear();    
}

bool TSharcInput::InitSharcInput(const char *filename){
  
  bool input = ParseInputFile(filename);
  if(!input)
    return input;
  
  TChannel::ReadCalFile(GetCalFile());
  if(TChannel::GetNumberOfChannels())
    return true;

// initialise some reaction stuff
// if target==cd2, composition is c+d+p, decide this using target input?
  fbeam = new TNucleus(GetZ(),GetN());
  freaction[0] = new TKinematics(GetBeamEperU()*GetA(),"p",fbeam->GetSymbol(),fbeam->GetSymbol(),"p");
  freaction[1] = new TKinematics(GetBeamEperU()*GetA(),"d",fbeam->GetSymbol(),fbeam->GetSymbol(),"d");
  
  return  false;
}

TKinematics *TSharcInput::GetKinematics(Option_t *opt){
  std::string ion = opt;
  if(ion.compare("p")==0)
     return freaction[0];
  else if(ion.compare("d")==0)
     return freaction[1];
  else 
     return 0;
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

Bool_t TSharcInput::ParseInputFile(const char *filename){
  //Make TChannels from a cal file.

  if(!filename) {
    printf("could not open file.\n");
    return false;
  }

  std::string infilename; 
  infilename.append(filename);
  if(infilename.length()==0)
    return false;

  ifstream infile;
  infile.open(infilename.c_str());
  if (!infile) {
    printf("could not open file.\n");
    return false;
  }

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
