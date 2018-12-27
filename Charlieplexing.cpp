/**
 * @file Charlieplexing.cpp
 * @author woodenCaliper
 * @brief
 * @version 0.2
 * @date 2018-12-27
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "Charlieplexing.h"

/**
 * @brief Construct. Set use arduino pins.
 *
 * @param[in] pins[]     any pins that connecting to LED
 * @param[in] numOfPins  length of pins array
 */
CharlieplexingClass::CharlieplexingClass(const byte pins[], byte numOfPins){
  lightingTime=1000; //us
  numOfUsePin = numOfPins;
  for(byte i=0; i<numOfPins; i++){
    usePins[i]=pins[i];
    setPinState(i, HIGH_INP);
  }
  for(byte i=0; i<CHARLIEPLEXING_MAX_USE_PIN; i++){
    for(byte j=0; j<CHARLIEPLEXING_MAX_USE_PIN; j++){
      reservationPinState[i][j] = HIGH_INP;
    }
  }
  drivingAnodePinIndex=0;
  needUpdateTimes=0;
}

/**
 * @brief Allocate unique LED ID.
 *
 * @param[in] anodePin arduino pin conected to LED anode
 * @param[in] cathodePin arduino pin conected to LED cathode
 * @return [unsigned int] unique LED ID
 */
unsigned int CharlieplexingClass::getLedId(byte anodePin, byte cathodePin){
  byte anodeIndex, cathodeIndex;
  for(byte i=0; i<numOfUsePin; i++){
    if(usePins[i] == anodePin){
      anodeIndex=i;
    }
    else if(usePins[i] == cathodePin){
      cathodeIndex=i;
    }
  }
  return ((unsigned int)anodeIndex<<8) | cathodeIndex;
}

/**
 * @brief Set light on time, when called OneShot() function.
 * @param[in] lightOnTime unit is micro second. default 1000.
 */
void CharlieplexingClass::setOneShotTime(unsigned long lightOnTime){
  lightingTime=lightOnTime;
}

void CharlieplexingClass::setPinState(byte usePinIndex, byte state){
  usePinState[usePinIndex] = state;
  switch(state){
    case LOW:
      pinMode(usePins[usePinIndex], OUTPUT);
      digitalWrite(usePins[usePinIndex], LOW);
    break;
    case HIGH:
      pinMode(usePins[usePinIndex], OUTPUT);
      digitalWrite(usePins[usePinIndex], HIGH);
    break;
    case HIGH_INP:
      pinMode(usePins[usePinIndex], INPUT);
    break;
  }
}

/**
 * @brief Light off the all LED.
 * @param
 */
void CharlieplexingClass::allLightOff(){
  for(byte i=0; i<numOfUsePin; i++){
    pinMode(usePins[i], INPUT);
  }
}

/**
 * @brief Light on the LED. -> wait for time that you set in setOneShotTime() function. -> light off the LED.
 *
 * @param[in] ledId
 */
void CharlieplexingClass::lightOneShot(unsigned int ledId){
  setPinState(ledId>>8, HIGH);
  setPinState((byte)ledId, LOW);
  delayMicroseconds(lightingTime);
  setPinState(ledId>>8, HIGH_INP);
  setPinState((byte)ledId, HIGH_INP);
}

/**
 * @brief Light multi LEDs that in order(dynamic lighting).
 *
 * @param[in] ledsId[]    address of LEDs ID array by getLedId() function.
 * @param[in] numOfLeds   length of ledsId array
 *
 * @details WARN: if this use, overwrite the config in setLedState() function.
 */
void CharlieplexingClass::lightOneShot(const unsigned int ledsId[], byte numOfLeds){
  setLedState(ledsId, numOfLeds);
  drivingAnodePinIndex=0;
  for(byte i=0; i<needUpdateTimes; i++){
    updateLightingState();
    delayMicroseconds(lightingTime);
  }
}

/**
 * @brief set LEDs for light in updateLedState() function.
 *
 * @param[in] ledsId[]  address of LEDs ID array by getLedId() function.
 * @param[in] numOfLeds length of ledsId array
 */
void CharlieplexingClass::setLedState(const unsigned int ledsId[], byte numOfLeds){
  for(byte i=0; i<numOfUsePin; i++){
    for(byte j=0; j<numOfUsePin; j++){
      //対角要素はHIGH, それ以外はHIGH_INPで初期化
      reservationPinState[i][j] = i==j ? HIGH : HIGH_INP;
    }
  }
  //ledsIdからreservationPinStateの2次元配列に変換
  for(byte i=0; i<numOfLeds; i++){
    reservationPinState[ledsId[i]>>8][ledsId[i]&0xff] = LOW;
  }
  //update回数を最小にするために、光らせない行を削除
  byte toIndex = 0;
  for(byte fromIndex=0; fromIndex<numOfUsePin; fromIndex++){
    for(byte j=0; j<numOfUsePin; j++){ //横の配列探索
      if(reservationPinState[fromIndex][j] == LOW){ //LOWが一つでもあればLEDは光る
        for(byte k=0; k<numOfUsePin; k++){ //横配列のコピー
          reservationPinState[toIndex][k] = reservationPinState[fromIndex][k];
        }
        toIndex++;
        break;
      }
    }
  }
  needUpdateTimes = toIndex;
  if(drivingAnodePinIndex>needUpdateTimes){
    drivingAnodePinIndex = 0;
  }
}

/**
 * @brief light LED function for use timer. repeat this at regular intervals
 * @param
 * @details I recommend do this at about 2ms intervals
 */
void CharlieplexingClass::updateLightingState(){
  if(needUpdateTimes>=0){
    for(byte i=0; i<numOfUsePin; i++){
      if(usePinState[i] != reservationPinState[drivingAnodePinIndex][i]){
        setPinState(i, HIGH_INP); //ちらつき防止のため、変更されるピンだけHIGH_INPにする
      }
    }
    for(byte i=0; i<numOfUsePin; i++){
      byte state = reservationPinState[drivingAnodePinIndex][i];
      if(state != HIGH_INP){
        setPinState(i, state);
      }
    }
    drivingAnodePinIndex = drivingAnodePinIndex+1==needUpdateTimes ? 0 : drivingAnodePinIndex+1;
  }
}