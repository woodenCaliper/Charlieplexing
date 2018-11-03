#include "Charlieplexing.h"

/**
 * @brief Construct a new Charlieplexing Class:: Charlieplexing Class object
 *
 */
CharlieplexingClass::CharlieplexingClass(){
  numOfUsePort=0;
  lightingTime=1000; //us
}



/**
 * @brief Set using arduino port. This function same setUsePorts().
 *
 * @param[in] ports
 * @param[in] numOfPorts
 */
inline void CharlieplexingClass::begin(byte ports[], byte numOfPorts){
  setUsePorts(ports, numOfPorts);
}

/**
 * @brief Allocate unique LED ID.
 *
 * @param[in] anodePort
 * @param[in] cathodePort
 * @return[in] unsigned int unique LED ID
 */
unsigned int CharlieplexingClass::getLedId(byte anodePort, byte cathodePort){
  return ((unsigned int)anodePort<<8) | cathodePort;
}

/**
 * @brief Set using arduino port.
 *
 * @param[in] port  port that connecting to LED
 */
void CharlieplexingClass::setUsePort(byte port){
  usePorts[numOfUsePort]=port;
  numOfUsePort++;
  pinMode(port, INPUT);
}

/**
 * @brief Set using arduino ports.
 *
 * @param[in] ports[]     any ports that connecting to LED
 * @param[in] numOfPorts  length of ports array
 */
void CharlieplexingClass::setUsePorts(byte ports[], byte numOfPorts){
  for(byte i=0; i<numOfPorts; i++){
    setUsePort(ports[i]);
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
 * @brief Direct LED port control.
 *
 * @param[in] anodePort     port Number connected to the LED's anode.
 * @param[in] cathodePort   port Number connected to the LED's cathode.
 * @param[in] lightOn       true:light on, false:light off
 */
void CharlieplexingClass::light(byte anodePort, byte cathodePort, bool lightOn){
  if(lightOn){  //anode:HIGH, cathode:LOW
    pinMode(anodePort, OUTPUT);
    digitalWrite(anodePort, HIGH);
    pinMode(cathodePort, OUTPUT);
    digitalWrite(cathodePort, LOW);
  }
  else{    //set high inpedance
    pinMode(anodePort, INPUT);
    pinMode(cathodePort, INPUT);
  }
}

/**
 * @brief Direct LED port control.
 *
 * @param[in] ledId   LED ID by getLedId() function.
 * @param[in] lightOn true:light on, false:light off
 */
inline void CharlieplexingClass::light(unsigned int ledId, bool lightOn){
  light(ledId>>8, ledId&0x0f, lightOn);
}

/**
 * @brief Light on LED and light off after wait time, you set setOneShotTime() function.
 *
 * @param[in] anodePort
 * @param[in] cathodePort
 */
void CharlieplexingClass::lightOneShot(byte anodePort, byte cathodePort){
  light(anodePort, cathodePort, true);
  delayMicroseconds(lightingTime);
  light(anodePort, cathodePort, false);
}
/**
 * @brief Light on LED and light off after wait time, you set setOneShotTime() function.
 *
 * @param[in] ledId
 */
inline void CharlieplexingClass::lightOneShot(unsigned int ledId){
  lightOneShot(ledId>>8, ledId&0x0f);
}

/**
 * @brief Light multi LEDs. but cannot be light LEDs using same port.
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
 * @brief Light off all LED.
 * @param
 */
void CharlieplexingClass::allLightOff(){
  for(byte i=0; i<numOfUsePort; i++){
    pinMode(usePorts[i], INPUT);
  }
}

/**
 * @brief Light multi LEDs that in order(dynamic lighting).
 *
 * @param[in] ledsId[]    address of LEDs ID array by getLedId() function.
 * @param[in] numOfLeds   length of ledsId array
 * @details take processing time about (oneShotTime)*(Number of using port)+300us
 */
void CharlieplexingClass::multiLightOneShot(unsigned int ledsId[], byte numOfLeds){
  byte alreadyOnCount=0;
  unsigned int afterLightLeds[MAX_PORT];
  byte afterLedsCount=0;

  allLightOff();
  for(byte i=0; i<numOfUsePort; i++){
    for(byte j=0; j<numOfLeds; j++){
      if(usePorts[i]==(byte)ledsId[j]){  //usePorts[i]がcathodeなのを選択
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
