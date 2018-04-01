/*
* Copyright (c) Anders Heed 2018
*
* This code is based on a sketch downloaded from https://gist.github.com/madc/4474559#file-morse-ino
* Adapted for the Adafruit HUZZAH ESP8266 board, using the red on-board LED at Pin #0
* Tested with Arduino IDE v1.8.5
*/


#include "morse.h"

#define BLINK_DEBUG_OUTPUT(x) (Serial.println(x))
//#define BLINK_DEBUG_OUTPUT(x)

//Define the LED Pin
#define PIN_OUT        0
#define LED_ON LOW
#define LED_OFF HIGH

//Define unit length in ms
#define UNIT_LENGTH    250

#define BLINK_STATE_OFF 0
#define BLINK_STATE_ON  1


//Build a struct with the morse code mapping
static const struct {const char letter, *code;} MorseMap[] =
{
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', "-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { ' ', "     " }, //Gap between word, seven units 
    
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },
    
  { '.', "·–·–·–" },
  { ',', "--..--" },
  { '?', "..--.." },
  { '!', "-.-.--" },
  { ':', "---..." },
  { ';', "-.-.-." },
  { '(', "-.--." },
  { ')', "-.--.-" },
  { '"', ".-..-." },
  { '@', ".--.-." },
  { '&', ".-..." },
};


MorseBlinker::MorseBlinker()
{
    blinkstate = BLINK_STATE_OFF;
    waitTargetTime = 0;
    charIndex = -1;
}

String MorseBlinker::Encode(const char *string)
{
  size_t i, j;
  String morseWord = "";
  
  for( i = 0; string[i]; ++i )
  {
    for( j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j )
    {
      if( toupper(string[i]) == MorseMap[j].letter )
      {
        morseWord += MorseMap[j].code;
        break;
      }
    }
    morseWord += " "; //Add tailing space to separate the chars
  }

  return morseWord;  
}

void MorseBlinker::HandleBlink()
{
  int currentTime = millis();
  
  if(currentTime < waitTargetTime)
  {
    return;
  }

  if(blinkstate == BLINK_STATE_ON)
  {
    digitalWrite( PIN_OUT, LED_OFF );
    blinkstate = BLINK_STATE_OFF;
    waitTargetTime = currentTime + UNIT_LENGTH;
  }
  else
  {
    if(++charIndex >= morseWord.length())
    {
      BLINK_DEBUG_OUTPUT( "Restarting" );
      charIndex = 0;
    }

    waitTargetTime = currentTime + UNIT_LENGTH;
    
    switch( morseWord[charIndex] )
    {
      case '.': //dit
        digitalWrite( PIN_OUT, LED_ON );
        blinkstate = BLINK_STATE_ON;
        BLINK_DEBUG_OUTPUT( ".  dit" );
        break;

      case '-': //dah
        digitalWrite( PIN_OUT, LED_ON );
        blinkstate = BLINK_STATE_ON;
        waitTargetTime = currentTime + 3 * UNIT_LENGTH;
        BLINK_DEBUG_OUTPUT( "-  dah" );
        break;

      case ' ': //gap
        BLINK_DEBUG_OUTPUT( "gap" );
        break;
    }
  }
    
  
}

void MorseBlinker::Setup()
{

  morseWord = Encode( "SOS " );
  waitTargetTime = 0;
  pinMode( PIN_OUT, OUTPUT );
  digitalWrite( PIN_OUT, LED_OFF );

}

void MorseBlinker::SetMorseWord(String encodedWord)
{
  morseWord = encodedWord;
  charIndex = -1;
}


