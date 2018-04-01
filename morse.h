/*
* Copyright (c) Anders Heed 2018
*
* This code is based on a sketch downloaded from https://gist.github.com/madc/4474559#file-morse-ino
* Adapted for the Adafruit HUZZAH ESP8266 board, using the red on-board LED at Pin #0
* Tested with Arduino IDE v1.8.5
*/


#ifndef _MORSE_H_
#define _MORSE_H_

#include <Arduino.h>

class MorseBlinker
{
  public:

  MorseBlinker();

  void Setup();

  void HandleBlink();

  void SetMorseWord(String encodedWord);

  static String Encode(const char *string);


protected:
  String morseWord;
  int blinkstate;
  int waitTargetTime;
  int charIndex;

};




#endif //_MORSE_H_

