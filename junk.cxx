
#include <TH1F.h>
#include <TApplication.h>

#include <TSystem.h>

int main(int argc, char **argv) {


   TH1F *h = new TH1F("h","h",100,0,100);
   h->Draw();
   TApplication *app = new TApplication("app",0,0);
   gSystem->ProcessEvents();
   app->Run();
   return 0;
}
