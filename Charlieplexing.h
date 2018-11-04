/**
 * @file Charlieplexing.h
 * @author woodenCaliper (you@domain.com)
 * @brief This program is Arduino library for using Charlieplexing method
 * @version 1.0
 * @date 2018-10-31
 *
 * @copyright Copyright (c) 2018 woodenCaliper
 * Licensed under the Apache License, Version 2.0 (the "License").
 * See LICENSE.txt.
 */

#ifndef CHARLIEPLEXING_H_
#define CHARLIEPLEXING_H_

#include <arduino.h>

#ifndef MAX_PIN
#define MAX_PIN 20
#endif

class CharlieplexingClass{
  public:
  CharlieplexingClass();
  
  //setting functions>>
  void begin(byte pinsNumber[], byte numOfPins);
  void setOneShotTime(unsigned long lightOnTime);  
  //<<setting functions

  
  unsigned int getLedId(byte anodePin, byte cathodePin);  
  
  void lightOneShot(unsigned int ledId);
  void multiLightOneShot(unsigned int ledsId[], byte numOfLeds);
  
  void setLedState(unsigned int ledsId[], byte numOfLeds);
  void updateLightingState();

  private:  //functions
  void light(unsigned int ledId, bool lightOn);
  void allLightOff();

  private:  //variables
  byte usePins[MAX_PIN];
  byte numOfUsePin;
  unsigned long lightingTime;
  const int HIGH_INP=-1;

  unsigned int settedLeds[100];
  byte numOfSettedLeds;
  byte drivingUsePinAdrs;
};

#endif