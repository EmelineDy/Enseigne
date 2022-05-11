#ifndef DMX_LETTRES_H
#define DMX_LETTRES_H

#include "DMX.h"


class DMX_Lettres : public DMX
{
  private:
    // Les LEDs des lettres ont trois PIN de data RGB
    int PIN_LETTERS_RED;
    int PIN_LETTERS_BLUE;
    int PIN_LETTERS_GREEN;
    int last_text_update_time;
  
  public:
    //Le constructeur fait appel au constructeur de DMX
    DMX_Lettres(int Ad);

    //Initialise les PIN de données
    void SetPinRGB(int Pin_R, int Pin_G, int Pin_B);

    //Ecrit sur un pin une valeur de 0 à 255 correspondant à la couleur
    void setLettersColor(int pin, int value) ; 

    //Envoie les instructions DMX, transformées en PWM, sur les trois PIN de data
    void updateLettersColor() ;   

    //Remet à zéro le timer des lettres si il y a eu un changement d'instruction
    void Reset_Timer_Letters();
};

#endif // DMX_LETTRES_H
