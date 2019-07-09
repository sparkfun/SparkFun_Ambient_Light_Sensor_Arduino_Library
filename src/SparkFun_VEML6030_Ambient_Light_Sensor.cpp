/*
  This is a library for SparkFun's VEML6030 Ambient Light Sensor (Qwiic)
  By: Elias Santistevan
  Date: July 2019
  License: This code is public domain but you buy me a beer if you use this and 
  we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
 */

#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"

SparkFun_Ambient_Light::SparkFun_Ambient_Light(uint8_t address){  _address = address; } //Constructor for I2C

bool SparkFun_Ambient_Light::begin( TwoWire &wirePort )
{
  
  _i2cPort = &wirePort;

  _i2cPort->beginTransmission(_address);
  uint8_t _ret = _i2cPort->endTransmission();
  if( !_ret )
    return true; 
  else 
    return false; 

}

// REG0x00, bits [12:11]
// This function sets the gain for the Ambient Light Sensor. Possible values
// are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
// sensors is behind dark glass, where as the lowest setting should be used in
// dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
void SparkFun_Ambient_Light::setGain(double gainVal){

  uint8_t bits; 

  if (gain == 1)
    bits = 0; 
  else if (gain == 2)
    bits = 1;
  else if (gain == .125)
    bits = 2;
  else if (gain == .25)
    bits = 3; 
  else
    return; 

  _writeRegister(SETTING_REG, GAIN_MASK, bits, GAIN_POS); 

}

// REG0x00, bits [12:11]
// This function reads the gain for the Ambient Light Sensor. Possible values
// are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
// sensors is behind dark glass, where as the lowest setting should be used in
// dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
double SparkFun_Ambient_Light::readGain(){
 
  uint16_t regVal = _readRegister(SETTING_REG); // Get register
  regVal &= (~GAIN_MASK); // Invert the gain mask to _keep_ the gain
  regVal = (regVal >> GAIN_POS); // Move values to front of the line. 
  
  if (regVal == 2)
    return .125;
  else if (regVal == 3)
    return .25
  else:    
    return regVal; 
  
}

// REG0x00, bits[9:6]
// This function sets the integration time (the saturation time of light on the
// sensor) of the ambient light sensor. Higher integration time leads to better
// resolution but slower refresh times. 
void SparkFun_Ambient_Light::setIntegTime(uint16_t time){ 
 
  uint8_t bits;

  if (time == 100) // Default setting.
    bits = 0; 
  else if (time == 200)
    bits = 1; 
  else if (time == 400)
    bits = 3; 
  else if (time == 800)
    bits = 4; 
  else if (time == 50)
    bits = 8; 
  else if (time == 25)
    bits = 12; 
  else
    return;

  _writeRegister(SETTING_REG, INTEG_MASK, bits, INTEG_POS);  
  uint8_t regVal = readIntegTime();
  currInteg = regVal; 

}

// REG0x00, bits[9:6]
// This function reads the integration time (the saturation time of light on the
// sensor) of the ambient light sensor. Higher integration time leads to better
// resolution but slower refresh times. 
uint8_t SparkFun_Ambient_Light::readIntegTime(){

  uint16_t regval = _readRegister(SETTING_REG); 
  regVal &= (~INTEG_MASK); 
  regVal = (regVal >> INTEG_POS); 
  currInteg = regVal; 
  return regVal;

}

// REG0x00, bits[5:4]
// This function sets the persistence protect number. 
void SparkFun_Ambient_Light::setProtect(uint8_t protVal){

  uint8_t bits; 

  if (protVal == 1)
    bits = 0; 
  else if (protVal == 2)
    bits = 1;
  else if (protVal == 4)
    bits = 2;
  else if (protVal == 8)
    bits = 3;
  else
    return;

  _writeRegister(SETTING_REG, PERS_PROT_MASK, bits, PERS_PROT_POS); 

}

// REG0x00, bits[5:4]
// This function reads the persistence protect number. 
uint8_t SparkFun_Ambient_Light::readProtect(){

  uint16_t regval = _readRegister(SETTING_REG); 
  regVal &= (~PERS_PROT_MASK); 
  regVal = (regVal >> PERS_PROT_POS); 
  return regVal;

}

// REG0x00, bit[1]
// This function enables the Ambient Light Sensor's interrupt. 
void SparkFun_Ambient_Light::enableInt(){

  _writeRegister(SETTING_REG, INT_MASK, INT_ENABLE, INT_EN_POS); 

}

// REG0x00, bit[1]
// This function disables the Ambient Light Sensor's interrupt. 
void SparkFun_Ambient_Light::disableInt(){

  _writeRegister(SETTING_REG, INT_MASK, INT_DISABLE, INT_EN_POS); 

}

// REG0x00, bit[1]
// This function checks if the interrupt is enabled or disabled. 
uint8_t SparkFun_Ambient_Light::readIntSetting(){

  uint16_t regval = _readRegister(SETTING_REG); 
  regVal &= (~INT_MASK); 
  regVal = (regVal >> INT_EN_POS); 
  return regVal;

}

// REG0x00, bit[0]
// This function powers down the Ambient Light Sensor. The light sensor will
// hold onto the last light reading which can be acessed while the sensor is 
// shut down. 0.5 micro Amps are consumed while shutdown. 
void SparkFun_Ambient_Light::shutDown(){

  _writeRegister(SETTING_REG, SD_MASK, ALS_SD, SD_MASK);

}

// REG0x00, bit[0]
// This function powers up the Ambient Light Sensor. The last value that was
// read during shut down will be overwritten on the sensor's subsequent read.
// After power up, a small 4ms delay is applied to give time for the internal
// osciallator and signal processor to power up.   
void SparkFun_Ambient_Light::powerOn(){

  _writeRegister(SETTING_REG, SD_MASK, ALS_POWER_ON, SD_MASK);
  delay(4);

}

// This function writes to a 16 bit register. Paramaters include the register's address, a mask 
// for bits that are ignored, the bits to write, and the bits' starting
// position.
void SparkFun_Ambient_Light::_writeRegister(uint8_t _wReg, uint16_t _mask, uint8_t _bits, uint8_t _startPosition)
{
  
  uint16_t _i2cWrite; 

  _i2cWrite = readRegister(_wReg); // Get the current value of the register
  _i2cWrite &= _mask; // Mask the position we want to write to.
  _i2cWrite |= (_bits << _startPosition);  // Place the given bits to the variable
  _i2cPort->beginTransmission(_address); // Start communication.
  _i2cPort->write(_wReg); // at register....
  _i2cPort->write(_i2cWrite); // Write register...
  _i2cPort->endTransmission(); // End communcation.

}

// This function reads a 16 bit register. It takes the register's
// address as its' parameter.
uint16_t SparkFun_Ambient_Light::_readRegister(uint8_t _reg)
{

  uint16_t _regValue; 

  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(_reg); // Moves pointer to register.
  _i2cPort->endTransmission(false); // 'False' here sends a restart message so that bus is not released
  _i2cPort->requestFrom(_address, 2); // Two reads for 16 bit registers
  _regValue = _i2cPort->read(); // LSB
  _regValue |= uint16_t(_i2cPort->read()) << 8; //MSB
  return(_regValue);

}

