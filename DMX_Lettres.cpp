#include <Arduino.h>
#include "DMX_Lettres.h"

DMX_Lettres::DMX_Lettres( int Ad):DMX(Ad){
  last_text_update_time = 0;
}

void DMX_Lettres::SetPinRGB(int Pin_R, int Pin_G, int Pin_B){
    PIN_LETTERS_RED = Pin_R;
    PIN_LETTERS_BLUE = Pin_B;
    PIN_LETTERS_GREEN = Pin_G;

    pinMode(PIN_LETTERS_RED, OUTPUT);
    pinMode(PIN_LETTERS_GREEN, OUTPUT);
    pinMode(PIN_LETTERS_BLUE, OUTPUT);
}

void DMX_Lettres::setLettersColor(int pin, int value) {
  analogWrite(pin, value);
}

void DMX_Lettres::updateLettersColor() {

  if ((millis() - last_text_update_time) > 0) {
    short lettersRed = dmxToPwm(currentDMXCommand[0]);
    short lettersGreen = dmxToPwm(currentDMXCommand[1]);
    short lettersBlue = dmxToPwm(currentDMXCommand[2]);
    
    setLettersColor(PIN_LETTERS_RED, lettersRed);
    setLettersColor(PIN_LETTERS_GREEN, lettersGreen);
    setLettersColor(PIN_LETTERS_BLUE, lettersBlue);
  }
}

void DMX_Lettres::Reset_Timer_Letters() {
  if (this->HasChanged()) {
    last_text_update_time = millis();
  }
}
