/**
 * @file Charlieplexing.h
 * @author woodenCaliper (you@domain.com)
 * @brief This program is Arduino library for using Charlieplexing method
 * @version 1.0
 * @date 2018-10-31
 *
 * @copyright Copyright (c) 2018 woodenCaliper
 * This software is released under the MIT License, see LICENSE.txt.
 */

#ifndef CHARLIEPLEXING_H_
#define CHARLIEPLEXING_H_

#include <arduino.h>

#ifndef MAX_PORT
#define MAX_PORT 20
#endif

class CharlieplexingClass{
  public:
  CharlieplexingClass();

  unsigned int getLedId(byte anodePort, byte cathodePort);
  void setUsePort(byte port);
  void setUsePorts(byte ports[], byte numOfPorts);
  void setOneShotTime(unsigned long lightOnTime);

  void light(byte anodePort, byte cathodePort, bool lightOn);
  inline void light(unsigned int ledId, bool lightOn);
  void lightOneShot(byte anodePort, byte cathodePort);
  inline void lightOneShot(unsigned int ledId);

  void multiLight(unsigned int ledsId[], byte numOfLeds, bool lightOn);
  void multiLightOneShot(unsigned int ledsId[], byte numOfLeds);
  void allLightOff();

  private:
  byte usePorts[MAX_PORT];
  byte numOfUsePort;
  unsigned long lightingTime;
  const int HIGH_INP=-1;
};

#endif