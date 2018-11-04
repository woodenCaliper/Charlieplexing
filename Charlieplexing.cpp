#include "Charlieplexing.h"

/**
 * @brief Construct a new Charlieplexing Class:: Charlieplexing Class object
 *
 */
CharlieplexingClass::CharlieplexingClass(){
  numOfUsePin=0;
  lightingTime=1000; //us

  for(byte i=0; i<100; i++){
    settedLeds[i]=0;
  }
  numOfSettedLeds=0;
  drivingUsePinAdrs=0;
}



/**
 * @brief Set using arduino pins.
 *
 * @param[in] pins[]     any pins that connecting to LED
 * @param[in] numOfUsePins  length of pins array
 */
void CharlieplexingClass::begin(byte pins[], byte numOfPins){
  
  numOfUsePin = numOfPins;
  for(byte i=0; i<numOfPins; i++){
    usePins[i]=pins[i];
    pinMode(pins[i], INPUT);
  }
}

/**
 * @brief Allocate unique LED ID.
 *
 * @param[in] anodePin
 * @param[in] cathodePin
 * @return[in] unsigned int unique LED ID
 */
unsigned int CharlieplexingClass::getLedId(byte anodePin, byte cathodePin){
  return ((unsigned int)anodePin<<8) | cathodePin;
}

/**
 * @brief Set light on time, when called OneShot() function.
 *
 * @param[in] lightOnTime unit is micro second. default 1000
 */
void CharlieplexingClass::setOneShotTime(unsigned long lightOnTime){
  lightingTime=lightOnTime;
}



/**
 * @brief Direct LED pin control.
 *
 * @param[in] ledId   LED ID by getLedId() function.
 * @param[in] lightOn true:light on, false:light off
 */
void CharlieplexingClass::light(unsigned int ledId, bool lightOn){
  byte anodePin = ledId>>8;
  byte cathodePin = ledId&0x0f;

  if(lightOn){  //anode:HIGH, cathode:LOW
    pinMode(anodePin, OUTPUT);
    digitalWrite(anodePin, HIGH);
    pinMode(cathodePin, OUTPUT);
    digitalWrite(cathodePin, LOW);
  }
  else{    //set high inpedance
    pinMode(anodePin, INPUT);
    pinMode(cathodePin, INPUT);
  }
}

/**
 * @brief Light on the LED. -> wait for time that you set in setOneShotTime() function. -> light off the LED.
 *
 * @param[in] ledId
 */
void CharlieplexingClass::lightOneShot(unsigned int ledId){
  light(ledId, true);
  delayMicroseconds(lightingTime);
  light(ledId, false);
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
 * @brief Light multi LEDs that in order(dynamic lighting).
 *
 * @param[in] ledsId[]    address of LEDs ID array by getLedId() function.
 * @param[in] numOfLeds   length of ledsId array
 * @details take processing time about (oneShotTime)*(Number of using pin)+300us
 */
void CharlieplexingClass::multiLightOneShot(unsigned int ledsId[], byte numOfLeds){
  byte alreadyOnCount=0;
  unsigned int afterLightLeds[MAX_PIN];
  byte afterLedsCount=0;

  allLightOff();
  for(byte i=0; i<numOfUsePin; i++){
    for(byte i=0; i<numOfLeds; i++){
      if(usePins[i]==(byte)ledsId[i]){  //usePins[i]がcathodeなのを選択、ledsIdの右8bitとチェック
        afterLightLeds[afterLedsCount]=ledsId[i];
        afterLedsCount++;
        alreadyOnCount++;
      }
    }
    for(byte i=0; i<afterLedsCount; i++){
      light(afterLightLeds[i], true);
    } 
    
    delayMicroseconds(lightingTime);
    allLightOff();
    if(alreadyOnCount==numOfLeds){
      return;
    }
    afterLedsCount=0;
  }
}

/**
 * @brief set LEDs for light in updateLedState() function.
 * 
 * @param[in] ledsId[]  address of LEDs ID array by getLedId() function.
 * @param[in] numOfLeds length of ledsId array
 */
void CharlieplexingClass::setLedState(unsigned int ledsId[], byte numOfLeds){
  for(byte i=0; i<numOfLeds; i++){
    settedLeds[i]=ledsId[i];
  }
  numOfSettedLeds = numOfLeds;
  drivingUsePinAdrs=0;
}

/**
 * @brief light LED function for use timer. repeat this at regular intervals
 * @param 
 */
void CharlieplexingClass::updateLightingState(){
  allLightOff();
  for(byte i=0; i<numOfSettedLeds; i++){ //光らせたいLEDを全探査
    if(usePins[drivingUsePinAdrs]==(byte)settedLeds[i]){  //usePins[i]がcathodeなのを選択、ledsIdの右8bitとチェック
      light(settedLeds[i], true);
    }
  }
  drivingUsePinAdrs++;
  drivingUsePinAdrs%=numOfUsePin;
}