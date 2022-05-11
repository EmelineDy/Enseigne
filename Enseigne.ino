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
DMX_Tour Tour(PIN_BORDER, NUMBER_OF_BORDER_LEDS) ;                           // Objet Tour
DMX_Lettres Lettres (DMX_ADDRESS) ;                                                       // Objet Lettres
ANIM Animations(DMX_CHANNEL_ANIMATION_SELECT, DMX_CHANNEL_ANIMATION_SPEED, &Tour) ; // objet Animation
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
  Lettres.Set_Chan_RGB(DMX_CHANNEL_LETTERS_RED, DMX_CHANNEL_LETTERS_GREEN, DMX_CHANNEL_LETTERS_BLUE) ;
  Lettres.SetPinRGB(PIN_LETTERS_RED, PIN_LETTERS_GREEN, PIN_LETTERS_BLUE) ; 
  
  // Initialisation écran
  screen.init();
  screen.set(3); // Luminosité faible
  //screen.displayStr(VERSION);
  //screen.displayNum(DMX_ADDRESS);
  isEcranOFF = false;
  timeoutEcran = millis();
  
  // Initialisation DMX
  lastDMXTime = millis();
  //Fait en sorte que la fonction updateDMXTimer soit lancée à chaque réception de trame DMX
  ESP8266DMX.setDataReceivedCallback(&updateDMXTimer);
  ESP8266DMX.startInput();
}


// Routine lancée à chaque réception de trame DMX. Permet de mettre à jour le timer de détection de perte de DMX
void updateDMXTimer(int slots) {
  
  // Si il y a eu un changement d'instruction pour les lettres, met à jour la variable servant de timer
  Lettres.Reset_Timer_Letters();
  
  // Si il y a eu un changement d'instruction pour le tour ou les animations, met à jour la variable servant de timer
  if (Tour.HasChanged() or Animations.HasChanged()) {
    Animations.Set_border() ; 
  }

  // Mise à jour des lettres si le timer a expiré
  Lettres.updateLettersColor();

  // Mise à jour du tour et animations si le timer s'est écoulé
  if (Animations.Check_border()) {
   Tour.UpdateDMX() ; 
   Animations.Update_Anim(); 
  }

  //Si la Frame d'animation actuelle est passée, on passe à la frame suivante
  if (Animations.Check_Frame()) {
    switch(Animations.GET_currentAnimation()) {
      // Pas d'anim
      case 0:
        Animations.Basic();
        break;
      // Chargement
      case 1:      
        Animations.Load();
        break;
      // Chase
      case 2:
        Animations.Chase();
        break;
      // Tour eiffel
      case 3:
        Animations.Eiffel();
        break;
      // Rainbow
      case 4:
        Animations.Rainbow();
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
