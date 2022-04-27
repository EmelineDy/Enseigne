#ifndef APP_H
#define APP_H

#include "DMX.h"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>    // Leds cercle
#include <LXESP8266UARTDMX.h>     // DMX  (https://github.com/claudeheintz/LXESP8266DMX)
#include <TM1637.h>               // 7 segments (Grove 4-Digit Display)

#include "DMX_Lettres.h"
#include "DMX_Tour.h"
#include "ANIM.h"

#include <functional>

typedef void (*Cb)(int);

extern std::function<void(int)> CallbackHelper;

 void Callback (int param);

class Application {
  private:
    // Définition des PIN utilisés
    int PIN_BORDER = 2; // => D4 DATA du tour
    int PIN_7SEG_CLK = 12; // => D6
    int PIN_7SEG_DIO = 13; // => D7
    int PIN_LETTERS_RED = 4; // => D2
    int PIN_LETTERS_GREEN = 15; // => D8
    int PIN_LETTERS_BLUE = 14; // => D5
    
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
    int DMX_ADDRESS = 1;
    int DMX_CHANNEL_LETTERS_RED = 0;
    int DMX_CHANNEL_LETTERS_GREEN = 1;
    int DMX_CHANNEL_LETTERS_BLUE = 2;
    int DMX_CHANNEL_ANIMATION_SELECT = 3;
    int DMX_CHANNEL_ANIMATION_SPEED = 4;
    int DMX_CHANNEL_BORDER_RED = 5;
    int DMX_CHANNEL_BORDER_GREEN = 6;
    int DMX_CHANNEL_BORDER_BLUE = 7;
    
    //Variables propres à notre enseigne
    int NUMBER_OF_BORDER_LEDS = 124;
    int NUMBER_OF_ANIMATIONS = 5;
    DMX_Tour* Tour ;                           // Objet Tour
    DMX_Lettres* Lettres ;                                                       // Objet Lettres
    ANIM* Animations ; // objet Animation
    TM1637 *screen;                                                // Objet écran  
    
    int debug;
    unsigned long lastDMXTime;        // Permet la détection de perte de DMX
    unsigned long timeoutEcran;       // Permet d'éteindre l'écran
    boolean isEcranOFF;// Variable aide au debug
//    /*const*/ int8_t err[] = { 0x0, 0x79, 0x50, 0x50 }; // Permet d'afficher 'Err' sur l'ecran

  public:
    //Fait appel au constructeur de DMX et de Adafruit_Neopixel, initialise la variable hasBorderColorChanged
    Application();

    //Initialise le bandeau avec les canaux DMX, et un séquence d'allumage
    void Init();

    // Routine lancée à chaque réception de trame DMX. Permet de mettre à jour le timer de détection de perte de DMX
    void updateDMXTimer(int slots);

    //Met à jour la valeur de colorTour
    void loop();
};

#endif // DMX_APP
