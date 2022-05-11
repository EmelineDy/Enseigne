#ifndef DMX_TOUR_H
#define DMX_TOUR_H

#include "DMX.h"
#include <Adafruit_NeoPixel.h>


class DMX_Tour : public DMX, public Adafruit_NeoPixel
{
    private:
    uint32_t colorTour;
    uint32_t oldcolorTour;
    bool hasBorderColorChanged;
    int Nb_Led;
  
  public:
  
    //Fait appel au constructeur de DMX et de Adafruit_Neopixel, initialise la variable hasBorderColorChanged
    DMX_Tour(int pin, int nb_Led);

    //Initialise le bandeau avec les canaux DMX, et un séquence d'allumage
    void Init(int Chan_R, int Chan_G, int Chan_B);

    //Met à jour la valeur de colorTour
    void UpdateDMX();
    
    uint32_t GET_colorTour() ; 
    bool GET_hasBorderColorChanged() ; 
    int GET_NbLed();

};

#endif // DMX_TOUR_H
