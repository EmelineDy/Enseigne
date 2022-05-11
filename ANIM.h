#ifndef ANIM_H
#define ANIM_H

#include <LXESP8266UARTDMX.h>
#include "DMX_Tour.h"

class ANIM : public DMX
{

  private : 

  //Timer d'expiration
  int last_frame_time;
  int last_border_update_time;
  int tempo ; 

  int channel_anim ; 
  int channel_speed ; 

  byte currentAnimation; // Numéro d'animation en cours
  int currentAnimationFrame;// Etape en cours dans l'animation

  bool hasAnimationBeenInitialized;
  bool hasAnimationChanged; 
  int NUMBER_OF_ANIMATION ; 

  DMX_Tour* Tour;

  public:

  //Initialise les canaux d'animation et de vitesse, ainsi que l'adresse et relie la classe à un Tour qui doit être animé
  ANIM(int channel_a, int channel_speed, DMX_Tour * Tour);

  //Renvoie le numéro de l'animation en fonction du signal sur le canal DMX dédié à l'animation
  byte getAnimation() ; 

  //Renvoie la valeur  du timer associé à l'animation, en fonction de l'instruction de vitesse
  int GET_tempo(byte animation) ;

  //Remplace la fonction hasChanged de la classe DMX, pour vérifier les instructions sur les canaux d'animation et de vitesse
  virtual bool HasChanged() ;

  //Remet à zéro la progression de l'animation
  void ResetProgress() ;

  //Met à jour l'animation lorsqu'il y a un changement
  void Update_Anim();

  //Différentes animations possibles
  void Basic(); //Couleurs classique
  void Load(); //Effet de chargement
  void Chase(); //Effet de poursuite
  void Eiffel(); //Effet tour Effeil avec des led qui clignotent
  void Rainbow(); //Effet d'arc-en ciel qui bouge

  //Vérifie que le timer associé au étape d'animation est expiré
  bool Check_Frame() ; 

  //Remet à zéro le timer d'étape
  void Set_Frame() ; 

  //Vérifie que le timer associé au Tour est expiré
  bool Check_border() ; 

  //Remet à zéro le timer Tour
  void Set_border() ; 
  
  void SET_hasAnimationChanged (bool state) ; 
  byte GET_currentAnimation() ;  
  bool GET_hasAnimationChanged() ; 

};

#endif // ANIM_H
