#ifndef DMX_H
#define DMX_H

#include <LXESP8266UARTDMX.h>

class DMX
{
  private : 
    int DMX_CHAN_RED;
    int DMX_CHAN_BLUE;
    int DMX_CHAN_GREEN;

  protected :
    static int DMX_ADDRESS;
    //Instructions du DMX 
    short currentDMXCommand[3] ;

  public:
    // Le constructeur initialise l'adresse de la table DMX avec le paramètre d'entrée
    DMX(int Ad); 
    DMX();

    // Initialise les channel DMX dédiés au instructions de couleurs
    void Set_Chan_RGB(int Chan_R, int Chan_G, int Chan_B) ; 

    //Transformation d'un signal DMX en PWM
    short dmxToPwm(short dmxValue) ; 

    //Vérifie si les instructions ont changé et les met à jour si c'est le cas
    virtual bool HasChanged();
};

#endif // DMX_H
