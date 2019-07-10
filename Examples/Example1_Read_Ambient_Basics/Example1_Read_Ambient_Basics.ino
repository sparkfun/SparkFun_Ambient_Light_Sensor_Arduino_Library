/*
*/

#include <Wire.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"

#define AL_ADDR 0x48

SparkFun_Ambient_Light light(AL_ADDR);

void setup(){

  Wire.begin();
  Serial.begin(115200);

  if(light.begin())
    Serial.println("Ready to read some light!"); 
  else
    Serial.println("Could not communicate with the sensor!");
}

void loop(){
}
