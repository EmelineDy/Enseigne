/************************
    Enseigne LATE SHOW
      ENFOIROS X GEI
      NodeMCU ESP8266
           2022
 ************************/

#define VERSION "2-02"

// Ajout des bibliothèques
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>    // Leds cercle
#include <LXESP8266UARTDMX.h>     // DMX  (https://github.com/claudeheintz/LXESP8266DMX)
#include <TM1637.h>               // 7 segments (Grove 4-Digit Display)

#include "DMX_Lettres.h"
#include "DMX_Tour.h"
#include "ANIM.h"


// Définition des PIN utilisés
#define PIN_BORDER 2 // => D4 DATA du tour
#define PIN_7SEG_CLK 12 // => D6
#define PIN_7SEG_DIO 13 // => D7
#define PIN_LETTERS_RED 4 // => D2
#define PIN_LETTERS_GREEN 15 // => D8
#define PIN_LETTERS_BLUE 14 // => D5

/* PATCH DMX
   Canal 1 -> Rouge Lettres
   Canal 2 -> Vert Lettres
   Canal 3 -> Bleu Lettres
   Canal 4 -> Choix animation Tour
   Canal 5 -> Vitesse animation Tour
   Canal 6 -> Rouge Tour
   Canal 7 -> Vert Tour
   Canal 8 -> Bleu Tour
*/

//Définition des canaux DMX
#define DMX_ADDRESS 1

#define DMX_CHANNEL_LETTERS_RED 0
#define DMX_CHANNEL_LETTERS_GREEN 1
#define DMX_CHANNEL_LETTERS_BLUE 2
#define DMX_CHANNEL_ANIMATION_SELECT 3
#define DMX_CHANNEL_ANIMATION_SPEED 4
#define DMX_CHANNEL_BORDER_RED 5
#define DMX_CHANNEL_BORDER_GREEN 6
#define DMX_CHANNEL_BORDER_BLUE 7

//Variables propres à notre enseigne
#define NUMBER_OF_BORDER_LEDS 124
#define NUMBER_OF_ANIMATIONS 5

// Permet d'afficher 'Err' sur l'ecran
/*const*/ int8_t err[] = { 0x0, 0x79, 0x50, 0x50 };

// Définition des objets
DMX_Tour Tour(DMX_ADDRESS, PIN_BORDER, NUMBER_OF_BORDER_LEDS) ;                           // Objet Tour
DMX_Lettres Lettres (DMX_ADDRESS) ;                                                       // Objet Lettres
ANIM Animations(DMX_CHANNEL_ANIMATION_SELECT, DMX_CHANNEL_ANIMATION_SPEED, DMX_ADDRESS) ; // objet Animation
TM1637 screen(PIN_7SEG_CLK, PIN_7SEG_DIO);                                                // Objet écran


unsigned long lastDMXTime;        // Permet la détection de perte de DMX
unsigned long timeoutEcran;       // Permet d'éteindre l'écran
boolean isEcranOFF;
int debug = 0 ;                   // Variable aide au debug

void updateDMXTimer(int slots);

void setup() {  
  // Watchdog disable
  ESP.wdtDisable();
  //Remove watchdog from WiFi chip
  *((volatile uint32_t*) 0x60000900) &= ~(1);
  
  // Initialization des LEDs du tour
  Tour.Init(DMX_CHANNEL_BORDER_RED, DMX_CHANNEL_BORDER_GREEN, DMX_CHANNEL_BORDER_BLUE) ; 
  
  // Initialization des LEDs des lettres
  Lettres.SetPinRGB(PIN_LETTERS_RED, PIN_LETTERS_GREEN, PIN_LETTERS_BLUE) ; 
  Lettres.Set_Chan_RGB(DMX_CHANNEL_LETTERS_RED, DMX_CHANNEL_LETTERS_GREEN, DMX_CHANNEL_LETTERS_BLUE) ; 


  // Initialisation Ecran
  screen.init();
  screen.set(3); // Luminosité faible
  //screen.displayStr(VERSION);
  //screen.displayNum(DMX_ADDRESS);
  isEcranOFF = false;
  timeoutEcran = millis();

  // Initialisation DMX
  lastDMXTime = millis();
  ESP8266DMX.setDataReceivedCallback(&updateDMXTimer);
  ESP8266DMX.startInput();
}



// Routine lancée à chaque réception de trame DMX. Permet de mettre à jour le timer de détection de perte de DMX
void updateDMXTimer(int slots) {
  
  // Si il y a eu un changement d'instruction pour les lettres, met à jour la variable servant de timer
  if (Lettres.Haschanged()) {
    Animations.Set_Text() ;     
  }
  
  // Si il y a eu un changement d'instruction pour le tour ou les animations, met à jour la variable servant de timer
  if (Tour.Haschanged() or Animations.HasChanged()) {
    Animations.Set_Border() ; 
  }

  // Mise à jour des lettres si le timer a expiré
  if (Animations.Check_Text()) {
    Lettres.updateLettersColor();
  }

  // Mise à jour du tour et animations si le timer s'est écoulé
  if (Animations.Check_Border()) {
    Tour.UpdateDMX() ; 
    if (Animations.checkStateAnimation()) { // Si on a changé d'animation on reset les progressions
     Animations.ResetProgress() ; 
    }  
    Animations.SET_tempo(Animations.GET_tempo(Animations.GET_currentAnimation())) ; 
  }

  //Si la Frame d'animation actuelle est passée, on passe à la frame suivante
  if (Animations.Check_Frame()) {
    switch(Animations.GET_currentAnimation()) {
      // Pas d'anim
      case 0:
          //On met simplement toutes les LED à la couleur voulue
          for(int i = 0; i < NUMBER_OF_BORDER_LEDS; i++) {
            Tour.setPixelColor(i, Tour.GET_colorTour());
          }
          Animations.SET_hasAnimationChanged(true);
          Animations.SET_hasAnimationBeenInitialized(true);
        break;
      
      // Chargement
      case 1:
               
        if (!Animations.GET_hasAnimationBeenInitialized() || Animations.GET_currentAnimationFrame() == 0) { //Si on est au début de l'animation, on clear les LED
          Tour.clear();
          Animations.SET_hasAnimationBeenInitialized(true);
        }
        //Si la couleur du tour change durant l'animation, on le prend en compte et on change la couleur de toutes les LED
        if (Tour.GET_hasBorderColorChanged()) {
          for(int i = 0; i < Animations.GET_currentAnimationFrame(); i++) {
            Tour.setPixelColor(i,  Tour.GET_colorTour());
          }
        }

        // Si on est à t+x du début de l'animation, on allume la diode x
        if (Animations.GET_currentAnimationFrame() > 0) {
          Tour.setPixelColor(Animations.GET_currentAnimationFrame() - 1,  Tour.GET_colorTour());
        }

        Animations.SET_currentAnimationFrame( (Animations.GET_currentAnimationFrame() + 1) % NUMBER_OF_BORDER_LEDS);
        Animations.SET_hasAnimationChanged(true);
        break;
      
      // Chase
      case 2:
        Tour.fill(Tour.GET_colorTour());

        //On éteint des LED les unes à la suite de l'autre
        for(int i = 0; i < 10; i++) {
          Tour.setPixelColor(i * NUMBER_OF_BORDER_LEDS / 10 + Animations.GET_currentAnimationFrame(), 0);
        }
        Animations.SET_currentAnimationFrame(Animations.GET_currentAnimationFrame() + 1);
        Animations.SET_hasAnimationChanged (true);
        break;
      // Tour eiffel
      case 3:
        //A chaque fois, on allume quatre LED aléatoirement et on éteint les autres
        Tour.clear();
        Tour.setPixelColor(random(NUMBER_OF_BORDER_LEDS), Tour.GET_colorTour());
        Tour.setPixelColor(random(NUMBER_OF_BORDER_LEDS), Tour.GET_colorTour());
        Tour.setPixelColor(random(NUMBER_OF_BORDER_LEDS), Tour.GET_colorTour());
        Tour.setPixelColor(random(NUMBER_OF_BORDER_LEDS), Tour.GET_colorTour());
        Animations.SET_hasAnimationChanged (true);
        break;
      // Rainbow
      case 4:
        for (int i = 0; i < NUMBER_OF_BORDER_LEDS; i++) {
          int colorOffset = (i + Animations.GET_currentAnimationFrame()) % NUMBER_OF_BORDER_LEDS;
          uint16_t hueValue = map(colorOffset, 0, NUMBER_OF_BORDER_LEDS, 0, 65535);
          uint32_t rgbColor = Tour.ColorHSV(hueValue, 255, 255);
          Tour.setPixelColor(i, rgbColor);
        }
        Animations.SET_currentAnimationFrame(Animations.GET_currentAnimationFrame() + 1);
        Animations.SET_hasAnimationChanged (true);
        break;
    }
    Animations.Set_Frame();
  }
}


void loop() {
   yield();
    if (Animations.GET_hasAnimationChanged()) {
    Tour.show();
    Animations.SET_hasAnimationChanged(false);
  }

}
