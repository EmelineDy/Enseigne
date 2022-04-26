#include "DMX.h"
#include "ANIM.h"
#include <LXESP8266UARTDMX.h>

ANIM::ANIM (int channel_a, int channel_speed, int Ad) : DMX(Ad){
  
  channel_anim = channel_a ; 
  channel_speed = channel_speed ; 
    
  last_text_update_time = 0;
  last_border_update_time = 0;

  TIMEOUT_DMX = 0 ; 
  last_frame_time = 0 ; 
  tempo = 0 ; 

  currentAnimation = 0 ; 
  currentAnimationFrame = 0 ;

  hasAnimationBeenInitialized = false ; 
  hasAnimationChanged = true ;

  NUMBER_OF_ANIMATION = 5 ; 
}

//Dans le cas de la classe ANIM, currentDMXCommand[0] correspond au canal de l'animation et currentDMXCommand[1] au cnaal de vitesse de l'animation

byte ANIM::getAnimation(){
  // 255 / Number_of_animation permet de créer des intervalles correspondants aux différentes animations
  //On récupère ensuite la valeur envoyer sur le canal DMX et la compare au intervalles pour déterminer l'animation voulue
  byte out = (byte) currentDMXCommand[0] / (255 / NUMBER_OF_ANIMATION);
  if (out < NUMBER_OF_ANIMATION) return out; // Si read(addr) = 255 return max value
  else 
    return (NUMBER_OF_ANIMATION - 1);
}

int ANIM::GET_tempo (byte animation) {
  switch (animation) {
    case 0:
      return 1000;
    case 1:
      return map(currentDMXCommand[1], 0, 255, 2500, 250); // map between 0.5 and 5 seconds
    case 2:
      return 1010 - map(currentDMXCommand[1], 0, 255, 10, 1000); // map between 0.05 and 3 seconds
    case 3:
      return 1010 - map(currentDMXCommand[1], 0, 255, 10, 1000);
    case 4:
      return 1010 - map(currentDMXCommand[1], 0, 255, 10, 1000);
  }
}

bool ANIM::HasChanged(){
   boolean flag = false;
  for (size_t i = 0; i < 2 ; i++) {
    if (currentDMXCommand[i] != ESP8266DMX.getSlot(this->DMX_ADDRESS + i + channel_anim )) {
      currentDMXCommand[i] = ESP8266DMX.getSlot(this->DMX_ADDRESS + i + channel_anim);
      flag = true;
    }
  }
  return flag;
}

void ANIM::ResetProgress(){
   currentAnimationFrame = 0; // On recommence à l'étape 1
   currentAnimation = getAnimation(); // On set la nouvelle animation
   hasAnimationBeenInitialized = false;  
}

bool ANIM::Check_Frame(){
  return (millis() - last_frame_time > tempo) ; 
}

bool ANIM::Check_Border(){
  return (millis() - last_border_update_time > TIMEOUT_DMX) ; 
}

bool ANIM::Check_Text(){
  return (millis() - last_text_update_time > TIMEOUT_DMX) ; 
}

void ANIM::Set_Frame(){  
  last_frame_time = millis(); 
}

void ANIM::Set_Border(){
  last_border_update_time = millis(); 
}

void ANIM::Set_Text(){
  last_text_update_time = millis();
}

bool ANIM::checkStateAnimation(){
return ( getAnimation() != currentAnimation) ;  
}

bool ANIM::GET_hasAnimationBeenInitialized (){
  return hasAnimationBeenInitialized ;
}

byte ANIM::GET_currentAnimation (){
  return currentAnimation ; 
}

int ANIM::GET_currentAnimationFrame(){
  return currentAnimationFrame ; 
}

bool ANIM::GET_hasAnimationChanged(){
  return hasAnimationChanged ; 
}

void ANIM::SET_tempo (int t){
  tempo = t ; 
}

void ANIM::SET_hasAnimationBeenInitialized(bool state){
  hasAnimationBeenInitialized = state ; 
}

void ANIM::SET_hasAnimationChanged (bool state){
  hasAnimationChanged = state ; 
}

void ANIM::SET_currentAnimationFrame (int current_frame){
  currentAnimationFrame = current_frame ;   
}
