/*
  This example code will walk you through how to set the interrupts on the
  SparkFun VEML6030 Ambient Light Sensor. 
  
  SparkFun Electronics
  Author: Elias Santistevan
  Date: July 2019

	License: This code is public domain but if you use this and we meet someday, get me a beer! 

	Feel like supporting our work? Buy a board from Sparkfun!
	https://www.sparkfun.com/products/15436

*/

#include <Wire.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"

#define AL_ADDR 0x48

SparkFun_Ambient_Light light(AL_ADDR);

// Possible values: .125, .25, 1, 2
// Both .125 and .25 should be used in most cases except darker rooms.
// A gain of 2 should only be used if the sensor will be covered by a dark
// glass.
float gain = .125;

// Possible integration times in milliseconds: 800, 400, 200, 100, 50, 25
// Higher times give higher resolutions and should be used in darker light. 
int time = 100;
long luxVal = 0; 


// Interrupt settings. 
long lowThresh = 20; 
long highThresh = 400; 
int numbValues = 1;

// Interrupt pin
int intPin = 3; 

void setup(){

  Wire.begin();
  Serial.begin(115200);
  pinMode(intPin, INPUT);
  
Serial.println(float(time));
Serial.println(time);
Serial.println(float("1"));
  if(light.begin())
    Serial.println("Ready to sense some light!"); 
  else
    Serial.println("Could not communicate with the sensor!");

  // Again the gain and integration times determine the resolution of the lux
  // value, and give different ranges of possible light readings. Check out
  // hoookup guide for more info. The gain/integration time also affects 
  // interrupt threshold settings so ALWAYS set these first. 
  light.setGain(gain);
  light.setIntegTime(time);

  Serial.println("Reading settings..."); 
  Serial.print("Gain: ");
  float gainVal = light.readGain();
  Serial.print(gainVal, 3); 
  Serial.print(" Integration Time: ");
  int timeVal = light.readIntegTime();
  Serial.println(timeVal);

  // Set both low and high thresholds, they take values in lux.
  light.setIntLowThresh(lowThresh);
  light.setIntHighThresh(highThresh);
  // Let's check that they were set correctly. 
  // There are some rounding issues inherently.
  Serial.print("Low Threshold: ");
  long lowVal = light.readLowThresh();
  Serial.print(lowVal);
  Serial.print(" High Threshold: ");
  long highVal = light.readHighThresh();
  Serial.println(highVal);

  // Now we enable the interrupt, now that he thresholds are set. 
  light.enableInt();
  Serial.print("Is interrupt enabled: ");
  light.readIntSetting() ? Serial.println("Yes") : Serial.println("No");


  // This setting modifies the number of times a value has to fall below or
  // above the threshold before the interrupt fires! Values include 1, 2, 4 and
  // 8. 
  light.setProtect(numbValues);
  Serial.print("Number of values that must fall below/above threshold before interupt occurrs: ");
  int protectVal = light.readProtect();
  Serial.println(protectVal);
  Serial.println("-------------------------------------------------");
  
  // Give some time to read our settings on startup.
  delay(3000);
}

void loop(){

  luxVal = light.readLight();
  Serial.print("Ambient Light Reading: ");
  Serial.print(luxVal);
  Serial.println(" Lux");  

  if (digitalRead(intPin) == LOW){
    int intVal = light.readInterrupt();
    if (intVal == 1)
      Serial.println("High threshold crossed!");
    else if (intVal == 2){
      Serial.println("Low threshold crossed!");
    }
  }

  delay(200);
}
