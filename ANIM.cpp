
#include "DMX.h"
#include "ANIM.h"

ANIM::ANIM (int channel_a, int channel_speed, DMX_Tour * Tour) : DMX(){
  
  channel_anim = channel_a ; 
  channel_speed = channel_speed ; 
  this->Tour = Tour ;
    
  last_border_update_time = 0;
  last_frame_time = 0 ; 
  tempo = 0 ; 

  currentAnimation = 0 ;
  currentAnimationFrame = 0 ;
 
  hasAnimationBeenInitialized = false ; 
  hasAnimationChanged = true ;

  NUMBER_OF_ANIMATION = 5 ; 
}

//Dans le cas de la classe ANIM, currentDMXCommand[0] correspond au canal de l'animation et currentDMXCommand[1] au canal de vitesse de l'animation

byte ANIM::getAnimation(){
  // 255 / Number_of_animation permet de créer des intervalles correspondants aux différentes animations
  //On récupère ensuite la valeur envoyer sur le canal DMX et la compare au intervalles pour déterminer l'animation voulue
  byte out = (byte) currentDMXCommand[0] / (255 / NUMBER_OF_ANIMATION);
  if (out <= NUMBER_OF_ANIMATION) return out;

  //Dans le cas où l'instruction DMX vaut plus de 255 (ce qui n'est pas sensé être possible), on considère qu'on veut l'animation 5
  else return (NUMBER_OF_ANIMATION - 1);
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
    if (currentDMXCommand[i] != ESP8266DMX.getSlot(DMX_ADDRESS + i + channel_anim )) {
      currentDMXCommand[i] = ESP8266DMX.getSlot(DMX_ADDRESS + i + channel_anim);
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

void ANIM::Update_Anim(){
  if (getAnimation() != currentAnimation) { // Si on a changé d'animation on reset les progressions
     ResetProgress(); 
  }
  //On change la valeur de tempo pour correspondre à la tempo de l'animation voulue
  tempo = GET_tempo(currentAnimation);  
}

void ANIM::Basic(){
  //On met simplement toutes les LED à la couleur voulue
  for(int i = 0; i < (Tour->GET_NbLed()); i++) {
    Tour->setPixelColor(i, Tour->GET_colorTour());
  }
  hasAnimationChanged = true;
  hasAnimationBeenInitialized = true;
}

void ANIM::Load(){
  if (!hasAnimationBeenInitialized || currentAnimationFrame == 0) { //Si on est au début de l'animation, on clear les LED
    Tour->clear();
    hasAnimationBeenInitialized = true;
  }
  //Si la couleur du tour change durant l'animation, on le prend en compte et on change la couleur de toutes les LED
  if (Tour->GET_hasBorderColorChanged()) {
    for(int i = 0; i < currentAnimationFrame; i++) {
      Tour->setPixelColor(i, Tour->GET_colorTour());
    }
  }

  // Si on est à t+x du début de l'animation, on allume la diode x
  if (currentAnimationFrame > 0) {
    Tour->setPixelColor(currentAnimationFrame - 1,  Tour->GET_colorTour());
  }

  currentAnimationFrame = (currentAnimationFrame + 1) % Tour->GET_NbLed();
  hasAnimationChanged = true;
}

void ANIM::Chase(){
  Tour->fill(Tour->GET_colorTour()); //On allume toutes les LED

  //On éteint des LED les unes à la suite de l'autre
  for(int i = 0; i < 10; i++) {
    Tour->setPixelColor(i * Tour->GET_NbLed() / 10 + currentAnimationFrame, 0);
  }
  currentAnimationFrame = currentAnimationFrame + 1;
  hasAnimationChanged = true;
}

void ANIM::Eiffel(){
  //A chaque fois, on allume quatre LED aléatoirement et on éteint les autres
  Tour->clear();
  Tour->setPixelColor(random(Tour->GET_NbLed()), Tour->GET_colorTour());
  Tour->setPixelColor(random(Tour->GET_NbLed()), Tour->GET_colorTour());
  Tour->setPixelColor(random(Tour->GET_NbLed()), Tour->GET_colorTour());
  Tour->setPixelColor(random(Tour->GET_NbLed()), Tour->GET_colorTour());
  hasAnimationChanged = true;
}

void ANIM::Rainbow(){
  for (int i = 0; i < Tour->GET_NbLed(); i++) {
    int colorOffset = (i + currentAnimationFrame) % Tour->GET_NbLed();
    uint16_t hueValue = map(colorOffset, 0, Tour->GET_NbLed(), 0, 65535);
    uint32_t rgbColor = Tour->ColorHSV(hueValue, 255, 255);
    Tour->setPixelColor(i, rgbColor);
  }
  currentAnimationFrame = currentAnimationFrame + 1;
  hasAnimationChanged = true;
}

bool ANIM::Check_Frame(){
  return ((millis() - last_frame_time) > tempo) ; 
}

void ANIM::Set_Frame(){
  last_frame_time = millis(); 
}

bool ANIM::Check_border(){
  return ((millis() - last_border_update_time) > 0) ; 
}

void ANIM::Set_border(){
  last_border_update_time = millis(); 
}


void ANIM::SET_hasAnimationChanged (bool state){
  hasAnimationChanged = state ; 
}

byte ANIM::GET_currentAnimation (){
  return currentAnimation ; 
}

bool ANIM::GET_hasAnimationChanged(){
  return hasAnimationChanged ; 
}
