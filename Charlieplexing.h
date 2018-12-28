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

#ifndef CHARLIEPLEXING_MAX_USE_PIN
#define CHARLIEPLEXING_MAX_USE_PIN 20
#endif

class CharlieplexingClass{
  public:
  CharlieplexingClass(const byte pins[], byte numOfPins);

  void setOneShotTime(unsigned long lightOnTime);
  unsigned int getLedId(byte anodePin, byte cathodePin);

  void allLightOff();
  void lightOneShot(unsigned int ledId);
  void lightOneShot(const unsigned int ledsId[], byte numOfLeds);

  void resetLedState();
  void addLedState(const unsigned int ledsId[], byte numOfLeds);
  void setLedState(const unsigned int ledsId[], byte numOfLeds);
  void updateLightingState();


  private:  //functions
  void setPinState(byte , byte);
  // void light(unsigned int ledId, bool lightOn);

  private:  //variables
  byte usePins[CHARLIEPLEXING_MAX_USE_PIN];
  byte usePinState[CHARLIEPLEXING_MAX_USE_PIN];
  byte numOfUsePin;
  unsigned long lightingTime;

  byte reservationPinState[CHARLIEPLEXING_MAX_USE_PIN][CHARLIEPLEXING_MAX_USE_PIN];//横がusePinの状態、縦はupdateするときの順番
  byte needUpdateTimes;
  byte drivingAnodePinIndex;

  const static byte HIGH_INP=2;
};

#endif