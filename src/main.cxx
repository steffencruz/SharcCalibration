

#include <cstdio>



#include <TSharcInput.h>


int main(int argc, char **argv) {
	
	printf("srsly world ... again?\n");

  if(!TSharcInput::Get()->InitSharcInput(argv[1]))
     return 1;

  TSharcInput::Get()->Print();
	return 0;
	
}


