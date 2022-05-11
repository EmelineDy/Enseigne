
#include "DMX.h"

DMX::DMX(int Ad){
    DMX_ADDRESS = Ad;
}

DMX::DMX(){}

void DMX::Set_Chan_RGB(int Chan_R, int Chan_G, int Chan_B){
    DMX_CHAN_RED = Chan_R;
    DMX_CHAN_GREEN = Chan_G;
    DMX_CHAN_BLUE = Chan_B;    
}

short DMX::dmxToPwm(short dmxValue) {
  return 1024 - map(dmxValue, 0, 255, 1, 1023);
}

bool DMX::HasChanged(){
  boolean flag = false;

  //On boucle de 0 à 2 pour parcourir le tableau currentDMXCommand, qu'on met à jour
  // On observe le DMX sur le canal DMX_ADDRESS + i + DMX_CHAN_RED car on a choisi de positionner les canaux RGB les un à la suite de l'autre  
  for (size_t i = 0; i < 3; i++) {
    if (currentDMXCommand[i] != ESP8266DMX.getSlot(DMX_ADDRESS + i + DMX_CHAN_RED)) {
      currentDMXCommand[i] = ESP8266DMX.getSlot(DMX_ADDRESS + i + DMX_CHAN_RED);
      flag = true;
    }
  }
  return flag;
}

int DMX::DMX_ADDRESS = 0;
