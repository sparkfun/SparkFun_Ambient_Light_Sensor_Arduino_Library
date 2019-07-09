#ifndef _SPARKFUN_VEML6030_H_
#define _SPARKFUN_VEML6030_H_

#include <Wire.h>
#include <Arduino.h>

#define ENABLE 0x01
#define DISABLE 0x00
#define INCORR_PARAM 0xFF

typdef uint8_t i2cAddress;
typdef uint8_t setting; 
const i2cAddress defAddr = 0x48;
const i2cAddress altAddr = 0x10;
setting currGain; 
setting currInteg; 

enum VEML6030_16BIT_REGISTERS {

  SETTING_REG            = 0x00,
  H_THRESH_REG,
  L_THRESH_REG,
  POWER_SAVE_REG, 
  AMBIENT_LIGHT_DATA_REG,
  WHITE_LIGHT_DATA_REG,
  INTERRUPT_REG

};

enum VEML6030_16BIT_REG_MASKS {
  
  GAIN_MASK              = 0xE7FF,
  INTEG_MASK             = 0xFC3F,
  PERS_PROT_MASK         = 0xFFCF,
  INT_MASK               = 0xFFFD,
  SD_MASK                = 0xFFFE,
  POW_SAVE_EN            = 0x06, // Most of this register is reserved
  POW_SAVE_MASK          = 0x01, // Most of this register is reserved
  INT_HIGH_MASK          = 0x8000,    
  INT_LOW_MASK           = 0x4000
  
};

enum SETTING_REG_POS {

  SHUT_POS               = 0x00,
  INT_EN_POS             = 0x01,
  PERS_PROT_POS          = 0x04,
  INTEG_POS              = 0x06,
  GAIN_POS               = 0x11

};

enum SETTING_REG_VALUES {

  ALS_POWER_ON           = 0x00, // wait 4ms after startup before first measurement.
  ALS_SD,
  INT_DISABLE            = 0x00,
  INT_ENABLE,

};

enum POWER_SAVE_REG_POS {
  
  PSM_EN_POS             = 0x00,
  PSM_POS

};

enum INTERRUPT_REG_POS {

  INT_HIGH_POS           = 0xE,
  INT_LOW_POS

};

uint8_t gainValue = 1; // Gather this from the register on startup, though it is default.
uint8_t standGain[]  = { 2, 1, 4, 8 };
double standReso[]   = { .0072, .0144, .0288, .05276, .1152, .2304 };
uint16_t standInteg[] = {   800,   400,   200,    100,   50 ,    25 };

class SparkFun_Ambient_Light
{  
  public:

    SparkFun_Ambient_Light(uint8_t address); // I2C Constructor

    bool begin(TwoWire &wirePort = Wire); // begin function

    // REG0x00, bits [12:11]
    // This function sets the gain for the Ambient Light Sensor. Possible values
    // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
    // sensors is behind dark glass, where as the lowest setting should be used in
    // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
    void setGain(double gainVal);

    // REG0x00, bits [12:11]
    // This function reads the gain for the Ambient Light Sensor. Possible values
    // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
    // sensors is behind dark glass, where as the lowest setting should be used in
    // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
    double readGain();

    // REG0x00, bits[9:6]
    // This function sets the integration time (the saturation time of light on the
    // sensor) of the ambient light sensor. Higher integration time leads to better
    // resolution but slower refresh times. 
    void setIntegTime(uint16_t time);

    // REG0x00, bits[9:6]
    // This function reads the integration time (the saturation time of light on the
    // sensor) of the ambient light sensor. Higher integration time leads to better
    // resolution but slower refresh times. 
    uint8_t readIntegTime();

    // REG0x00, bits[5:4]
    // This function sets the persistence protect number. 
    void setProtect(uint8_t protVal);

    // REG0x00, bits[5:4]
    // This function reads the persistence protect number. 
    uint8_t readProtect();

    // REG0x00, bit[1]
    // This function enables the Ambient Light Sensor's interrupt. 
    void enableInt();

    // REG0x00, bit[1]
    // This function disables the Ambient Light Sensor's interrupt. 
    void disableInt();

    // REG0x00, bit[1]
    // This function checks if the interrupt is enabled or disabled. 
    uint8_t readIntSetting();

    // REG0x00, bit[0]
    // This function powers down the Ambient Light Sensor. The light sensor will
    // hold onto the last light reading which can be acessed while the sensor is 
    // shut down. 0.5 micro Amps are consumed while shutdown. 
    void shutDown();

    // REG0x00, bit[0]
    // This function powers up the Ambient Light Sensor. The last value that was
    // read during shut down will be overwritten on the sensor's subsequent read.
    // After power up, a small 4ms delay is applied to give time for the internal
    // osciallator and signal processor to power up.   
    void powerOn();

  private:

    uint8_t _address;

    // This function writes to a 16 bit register. Paramaters include the register's address, a mask 
    // for bits that are ignored, the bits to write, and the bits' starting
    // position.
    void _writeRegister(uint8_t _wReg, uint16_t _mask, uint8_t _bits, uint8_t _startPosition);

    // This function reads a 16 bit register. It takes the register's
    // address as its' parameter.
    uint16_t _readRegister(uint8_t _reg);

    TwoWire *_i2cPort;
};
#endif
