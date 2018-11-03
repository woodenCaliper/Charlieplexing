#include "Charlieplexing.h"

/**
 * @brief Construct a new Charlieplexing Class:: Charlieplexing Class object
 *
 */
CharlieplexingClass::CharlieplexingClass(){
  numOfUsePin=0;
  lightingTime=1000; //us
}



/**
 * @brief Set using arduino port. This function same setUsePins().
 *
 * @param[in] pinsNumber
 * @param[in] numOfPins
 */
inline void CharlieplexingClass::begin(byte pinsNumber[], byte numOfPins){
  setUsePins(pinsNumber, numOfPins);
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
 * @brief Set using arduino pin.
 *
 * @param[in] pin  pin that connecting to LED
 */
void CharlieplexingClass::setUsePin(byte pin){
  usePins[numOfUsePin]=pin;
  numOfUsePin++;
  pinMode(pin, INPUT);
}

/**
 * @brief Set using arduino pins.
 *
 * @param[in] pins[]     any pins that connecting to LED
 * @param[in] numOfUsePins  length of pins array
 */
void CharlieplexingClass::setUsePins(byte pins[], byte numOfUsePins){
  for(byte i=0; i<numOfUsePins; i++){
    setUsePin(pins[i]);
  }
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
 * @param[in] anodePin     pin Number connected to the LED's anode.
 * @param[in] cathodePin   pin Number connected to the LED's cathode.
 * @param[in] lightOn       true:light on, false:light off
 */
void CharlieplexingClass::light(byte anodePin, byte cathodePin, bool lightOn){
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
 * @brief Direct LED pin control.
 *
 * @param[in] ledId   LED ID by getLedId() function.
 * @param[in] lightOn true:light on, false:light off
 */
inline void CharlieplexingClass::light(unsigned int ledId, bool lightOn){
  light(ledId>>8, ledId&0x0f, lightOn);
}

/**
 * @brief Light on the LED. -> wait for time that you set in setOneShotTime() function. -> light off the LED.
 *
 * @param[in] anodePin
 * @param[in] cathodePin
 */
void CharlieplexingClass::lightOneShot(byte anodePin, byte cathodePin){
  light(anodePin, cathodePin, true);
  delayMicroseconds(lightingTime);
  light(anodePin, cathodePin, false);
}
/**
 * @brief Light on the LED. -> wait for time that you set in setOneShotTime() function. -> light off the LED.
 *
 * @param[in] ledId
 */
inline void CharlieplexingClass::lightOneShot(unsigned int ledId){
  lightOneShot(ledId>>8, ledId&0x0f);
}

/**
 * @brief Control multi LEDs. but cannot be light on the LEDs using same pin.
 *
 * @param[in] ledsId[]    address of LEDs ID array by getLedId() function.
 * @param[in] numOfLeds   length of ledsId array
 * @param[in] lightOn     true:lightOn, false:lightOff
 */
void CharlieplexingClass::multiLight(unsigned int ledsId[], byte numOfLeds, bool lightOn){
  for(byte i=0; i<numOfLeds; i++){
    light(ledsId[i], lightOn);
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
    for(byte j=0; j<numOfLeds; j++){
      if(usePins[i]==(byte)ledsId[j]){  //usePins[i]がcathodeなのを選択
        afterLightLeds[afterLedsCount]=ledsId[j];
        afterLedsCount++;
        alreadyOnCount++;
      }
    }
    multiLight(afterLightLeds, afterLedsCount, true);
    delayMicroseconds(lightingTime);
    allLightOff();
    if(alreadyOnCount==numOfLeds){
      return;
    }
    afterLedsCount=0;
  }
}
