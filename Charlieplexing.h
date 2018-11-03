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
  
  inline void begin(byte pinsNumber[], byte numOfPins);

  unsigned int getLedId(byte anodePin, byte cathodePin);
  void setUsePin(byte pin);
  void setUsePins(byte pins[], byte numOfpins);
  void setOneShotTime(unsigned long lightOnTime);
  

  void light(byte anodePin, byte cathodePin, bool lightOn);
  inline void light(unsigned int ledId, bool lightOn);
  void lightOneShot(byte anodePin, byte cathodePin);
  inline void lightOneShot(unsigned int ledId);

  void multiLight(unsigned int ledsId[], byte numOfLeds, bool lightOn);
  void multiLightOneShot(unsigned int ledsId[], byte numOfLeds);
  void allLightOff();

  private:
  byte usePins[MAX_PIN];
  byte numOfUsePin;
  unsigned long lightingTime;
  const int HIGH_INP=-1;
};

#endif