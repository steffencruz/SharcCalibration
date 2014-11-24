

#include <cstdio>



#include <TSharcInput.h>


void SetOptions();

int main(int argc, char **argv) {
	
	printf("srsly world ... again?\n");

  if(!TSharcInput::Get()->InitSharcInput(argv[1]))
     return 1;

  TSharcInput::Get()->Print("a");
	
  return 0;
	
}

void SetOptions(){

  

  return;
}

