#include<fstream>
#include<string>
#include<sstream>

#include "Globals.h"
#include "TSharcInput.h"

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

  printf(DCYAN "TSharcInput:" RESET_COLOR "\n");
  printf("\t beam A : \t %i\n",GetA());
  printf("\t beam Z : \t %i\n",GetZ());
  printf("\t beam N : \t %i\n",GetN());

}

void TSharcInput::Clear(Option_t *opt) { 
   
   fprotons = 0;
   fneutrons = 0;
   
}





bool TSharcInput::InitSharcInput(const char *filename){

  return ParseInputFile(filename);
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

bool TSharcInput::ParseInputFile(const char *filename){
  //Makes TChannels from a cal file.

  if(!filename) {
    printf("could not open file.\n");
    return false;
  }

  std::string infilename; 
  infilename.append(filename);
  printf("infilename.c_str() = %s", infilename.c_str() );
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

          if(type.compare("Z")==0) {
            int tempint; ss>>tempint;
            SetZ(tempint);
          } else if(type.compare("N")==0) {
            int tempint; ss>>tempint;
            SetN(tempint);
          }
       }
    }
  }
  return false;
}
