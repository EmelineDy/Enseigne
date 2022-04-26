#ifndef ANIM_H
#define ANIM_H

#include <LXESP8266UARTDMX.h>

class ANIM : public DMX
{

  private : 

    int TIMEOUT_DMX ; 
  
  //Timer d'expiration
  int last_text_update_time;
  int last_border_update_time;
  int last_frame_time;
  int tempo ;

  //Canaux DMX associés à l'animation et sa vitesse
  int channel_anim ; 
  int channel_speed ; 

  byte currentAnimation; // Numéro d'animation en cours
  int currentAnimationFrame;// Etape en cours dans l'animation

  bool hasAnimationBeenInitialized;
  bool hasAnimationChanged; 
  int NUMBER_OF_ANIMATION ; 

  public:
  //Initialise les canaux d'animation et de vitesse, ainsi que l'adresse
  ANIM(int channel_a, int channel_speed, int Ad);

  //Renvoie le numéro de l'animation en fonction du signal sur le canal DMX dédié à l'animation
  byte getAnimation() ; 

  //Renvoie la valeur  du timer associé à l'animation, en fonction de l'instruction de vitesse
  int GET_tempo(byte animation) ;

   //Remplace la fonction hasChanged de la classe DMX, pour vérifier les instructions sur les canaux d'animation et de vitesse
  bool HasChanged() ;

  //Remet à zéro la progression de l'animation
  void ResetProgress() ;

  //Vérifie que le timer associé au étape d'animation / au tour / aux lettres est expiré
  bool Check_Frame() ; 
  bool Check_Border() ;
  bool Check_Text() ; 

  //Remet à zéro le timer d'étape d'animation / du tour / des lettres
  void Set_Frame() ; 
  void Set_Border() ; 
  void Set_Text() ;

  //Vérifie si l'animation enregistrée est celle demandée par la table de mixage 
  bool checkStateAnimation() ; 

  bool GET_hasAnimationBeenInitialized () ; 
  byte GET_currentAnimation() ; 
  int GET_currentAnimationFrame() ;
  bool GET_hasAnimationChanged() ; 

  void SET_tempo(int t) ;
  void SET_hasAnimationBeenInitialized(bool state) ; 
  void SET_hasAnimationChanged (bool state) ;  
  void SET_currentAnimationFrame (int current_frame) ; 
};

#endif // ANIM_H
