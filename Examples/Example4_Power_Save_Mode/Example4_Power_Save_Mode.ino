/*
  This example code will walk you through how to enable power save mode on your
  Ambient Light Sensor. When would you use power save mode? The datasheet
  specifies that power save mode is perfect when you want to continuously
  sample ambient light readings from your sensor, as opposed to getting a
  reading once an hour for example. The benefit of power save mode is lower
  power consumption, with current draw in the single digit micro-amp range. 
  Enabling the power save mode decreases the refresh rate of the sensor and so
  sampling should happen at a slower rate. Last thing, the refresh rate is also
  dependent on the integration time setting of the sensor. See the hookup guide 
  for a table showing the correlation of refresh times, current consumption, power 
  mode, and integration mode settings. 

  SparkFun Electronics
  Author: Elias Santistevan
  Date: July 2019

	License: This code is public domain but if you use this and we meet someday, get me a beer! 

	Feel like supporting our work? Buy a board from Sparkfun!
	https://www.sparkfun.com/products/15436

*/

#include <Wire.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"

// Close the jumper on the product to use address 0x10.
#define AL_ADDR 0x48

SparkFun_Ambient_Light light(AL_ADDR);

// Possible values: .125, .25, 1, 2
// Both .125 and .25 should be used in most cases except darker rooms.
// A gain of 2 should only be used if the sensor will be covered by a dark
// glass.
float gain = .125;

// Possible integration times in milliseconds: 800, 400, 200, 100, 50, 25
// Higher times give higher resolutions and should be used in darker light. 
int time = 400;
long luxVal = 0; 

// Power save mode, options range from 1-4. 
// Default is 1.
int powMode = 2; 

void setup(){

  Wire.begin();
  Serial.begin(115200);

  if(light.begin())
    Serial.println("Ready to sense some light!"); 
  else
    Serial.println("Could not communicate with the sensor!");

  // Again the gain and integration times determine the resolution of the lux
  // value, and give different ranges of possible light readings. Check out
  // hoookup guide for more info. 
  light.setGain(gain);
  light.setIntegTime(time);

  Serial.println("Reading settings..."); 
  Serial.print("Gain: ");
  float gainVal = light.readGain();
  Serial.print(gainVal, 3); 
  Serial.print(" Integration Time: ");
  int timeVal = light.readIntegTime();
  Serial.println(timeVal);

  // Power save mode enables low power consumption from your Ambient Light
  // Sensor. You can set modes from 1-4, four being the highest power saving
  // option but slowest refresh rate. The sensor's refresh rate is also
  // dependent on the integration time. For example a setting of 4 with an
  // integration time of 100ms == 4100ms refresh rate but 2 micro-amps current
  // draw. Check hookup guide for more info. 
  light.setPowSavMode(powMode);
  // Let's see that it was set correctly. 
  Serial.print("Power Save Mode: "); 
  int savVal = light.readPowSavMode();
  Serial.println(savVal);
  light.enablePowSave();
  Serial.println("Is power save enabled: ");
  int enaVal = light.readPowSavEnabled(); 

  if(enaVal)
    Serial.println("Yes!");
  else
    Serial.println("No!");  

  // This will power down the sensor and the sensor will draw 0.5 micro-amps of
  // power while shutdown. 
  // light.shutDown();
  // light.powerOn();

  // Give some time to read your settings. 
  delay(1000);

}

void loop(){

  luxVal = light.readLight();
  Serial.print("Ambient Light Reading: ");
  Serial.print(luxVal);
  Serial.println(" Lux");  
  // Sampling at the rate specified in the hookup guide power save mode table. 
  delay(2400);

}
