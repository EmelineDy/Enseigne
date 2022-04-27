 /************************
    Enseigne LATE SHOW
      ENFOIROS X GEI
      NodeMCU ESP8266
           2022
 ************************/

#define VERSION "2-02"

// Ajout des bibliothèques
#include "Application.h"


Application App;

void setup() { 
  App.Init() ;
}
  
void loop() {
   App.loop();
}
