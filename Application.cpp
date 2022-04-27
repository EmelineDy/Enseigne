#include "Application.h"
#include <iostream>
#include <string>

std::function<void(int)> CallbackHelper;
void Callback (int param)
{
    CallbackHelper(param);
}

Application::Application(){
  debug = 0;

  
  // Définition des objets
  DMX_Tour Tour(DMX_ADDRESS, PIN_BORDER, NUMBER_OF_BORDER_LEDS);
  DMX_Lettres Lettres (DMX_ADDRESS);
  ANIM Animations(DMX_CHANNEL_ANIMATION_SELECT, DMX_CHANNEL_ANIMATION_SPEED, DMX_ADDRESS);
  TM1637 screen(PIN_7SEG_CLK, PIN_7SEG_DIO);
  this->Tour = &Tour;
  this->Lettres = &Lettres;
  this->Animations = &Animations;
  this->screen = &screen;
  
   
}

void Application::Init() {
  // Watchdog disable
  ESP.wdtDisable();
  //Remove watchdog from WiFi chip
  *((volatile uint32_t*) 0x60000900) &= ~(1);
  
  // Initialization des LEDs du tour
  Tour->Init(DMX_CHANNEL_BORDER_RED, DMX_CHANNEL_BORDER_GREEN, DMX_CHANNEL_BORDER_BLUE) ; 
  
  // Initialization des LEDs des lettres
  Lettres->SetPinRGB(PIN_LETTERS_RED, PIN_LETTERS_GREEN, PIN_LETTERS_BLUE) ; 
  Lettres->Set_Chan_RGB(DMX_CHANNEL_LETTERS_RED, DMX_CHANNEL_LETTERS_GREEN, DMX_CHANNEL_LETTERS_BLUE) ; 


  // Initialisation Ecran
  screen->init();
  screen->set(3); // Luminosité faible
  //screen.displayStr(VERSION);
  //screen.displayNum(DMX_ADDRESS);
  isEcranOFF = false;
  timeoutEcran = millis();

  // Initialisation DMX
  lastDMXTime = millis();
  
  CallbackHelper =[&] (int slots) {this->updateDMXTimer(1);};
  ESP8266DMX.setDataReceivedCallback(Callback);
  ESP8266DMX.startInput();
}

void Application::updateDMXTimer(int slots){
  // Si il y a eu un changement d'instruction pour les lettres, met à jour la variable servant de timer
  if (Lettres->Haschanged()) {
    Animations->Set_Text() ;     
  }
  
  // Si il y a eu un changement d'instruction pour le tour ou les animations, met à jour la variable servant de timer
  if (Tour->Haschanged() or Animations->HasChanged()) {
    Animations->Set_Border() ; 
  }

  // Mise à jour des lettres si le timer a expiré
  if (Animations->Check_Text()) {
    Lettres->updateLettersColor();
  }

  // Mise à jour du tour et animations si le timer s'est écoulé
  if (Animations->Check_Border()) {
    Tour->UpdateDMX() ; 
    if (Animations->checkStateAnimation()) { // Si on a changé d'animation on reset les progressions
     Animations->ResetProgress() ; 
    }  
    Animations->SET_tempo(Animations->GET_tempo(Animations->GET_currentAnimation())) ; 
  }

  //Si la Frame d'animation actuelle est passée, on passe à la frame suivante
  if (Animations->Check_Frame()) {
    switch(Animations->GET_currentAnimation()) {
      // Pas d'anim
      case 0:
          //On met simplement toutes les LED à la couleur voulue
          for(int i = 0; i < NUMBER_OF_BORDER_LEDS; i++) {
            Tour->setPixelColor(i, Tour->GET_colorTour());
          }
          Animations->SET_hasAnimationChanged(true);
          Animations->SET_hasAnimationBeenInitialized(true);
        break;
      
      // Chargement
      case 1:
               
        if (!Animations->GET_hasAnimationBeenInitialized() || Animations->GET_currentAnimationFrame() == 0) { //Si on est au début de l'animation, on clear les LED
          Tour->clear();
          Animations->SET_hasAnimationBeenInitialized(true);
        }
        //Si la couleur du tour change durant l'animation, on le prend en compte et on change la couleur de toutes les LED
        if (Tour->GET_hasBorderColorChanged()) {
          for(int i = 0; i < Animations->GET_currentAnimationFrame(); i++) {
            Tour->setPixelColor(i,  Tour->GET_colorTour());
          }
        }

        // Si on est à t+x du début de l'animation, on allume la diode x
        if (Animations->GET_currentAnimationFrame() > 0) {
          Tour->setPixelColor(Animations->GET_currentAnimationFrame() - 1,  Tour->GET_colorTour());
        }

        Animations->SET_currentAnimationFrame( (Animations->GET_currentAnimationFrame() + 1) % NUMBER_OF_BORDER_LEDS);
        Animations->SET_hasAnimationChanged(true);
        break;
      
      // Chase
      case 2:
        Tour->fill(Tour->GET_colorTour());

        //On éteint des LED les unes à la suite de l'autre
        for(int i = 0; i < 10; i++) {
          Tour->setPixelColor(i * NUMBER_OF_BORDER_LEDS / 10 + Animations->GET_currentAnimationFrame(), 0);
        }
        Animations->SET_currentAnimationFrame(Animations->GET_currentAnimationFrame() + 1);
        Animations->SET_hasAnimationChanged (true);
        break;
      // Tour eiffel
      case 3:
        //A chaque fois, on allume quatre LED aléatoirement et on éteint les autres
        Tour->clear();
        Tour->setPixelColor(random(NUMBER_OF_BORDER_LEDS), Tour->GET_colorTour());
        Tour->setPixelColor(random(NUMBER_OF_BORDER_LEDS), Tour->GET_colorTour());
        Tour->setPixelColor(random(NUMBER_OF_BORDER_LEDS), Tour->GET_colorTour());
        Tour->setPixelColor(random(NUMBER_OF_BORDER_LEDS), Tour->GET_colorTour());
        Animations->SET_hasAnimationChanged (true);
        break;
      // Rainbow
      case 4:
        for (int i = 0; i < NUMBER_OF_BORDER_LEDS; i++) {
          int colorOffset = (i + Animations->GET_currentAnimationFrame()) % NUMBER_OF_BORDER_LEDS;
          uint16_t hueValue = map(colorOffset, 0, NUMBER_OF_BORDER_LEDS, 0, 65535);
          uint32_t rgbColor = Tour->ColorHSV(hueValue, 255, 255);
          Tour->setPixelColor(i, rgbColor);
        }
        Animations->SET_currentAnimationFrame(Animations->GET_currentAnimationFrame() + 1);
        Animations->SET_hasAnimationChanged (true);
        break;
    }
    Animations->Check_Frame();
  }
}

void Application::loop(){
  yield();
    if (Animations->GET_hasAnimationChanged()) {
    Tour->show();
    Animations->SET_hasAnimationChanged(false);
  }
}
