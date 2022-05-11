#include "DMX_Tour.h"

DMX_Tour::DMX_Tour(int pin, int nb_Led):DMX(), Adafruit_NeoPixel(nb_Led, pin, NEO_GRB + NEO_KHZ800){
  hasBorderColorChanged = true ;   
  this->Nb_Led = nb_Led;
  }

void DMX_Tour::Init(int Chan_R, int Chan_G, int Chan_B){

  Set_Chan_RGB(Chan_R, Chan_G, Chan_B) ; 
  begin();
  setBrightness(255); // Luminosité à fond
  clear();
  show();
}

void DMX_Tour::UpdateDMX(){
    colorTour = Color(currentDMXCommand[0], currentDMXCommand[1], currentDMXCommand[2]);
    hasBorderColorChanged = (colorTour != oldcolorTour);
    oldcolorTour = colorTour;
}

uint32_t DMX_Tour::GET_colorTour(){
  return colorTour ; 
}

bool DMX_Tour::GET_hasBorderColorChanged(){
  return hasBorderColorChanged ; 
}

int DMX_Tour::GET_NbLed(){
  return Nb_Led;
}
