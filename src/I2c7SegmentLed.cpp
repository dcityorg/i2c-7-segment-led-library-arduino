
/*
  I2c7SegmentLed.cpp

  Written by: Gary Muhonen  gary@dcity.org

  versions
    1.0.0 - 7/31/2016 Original Release.
    1.0.1 - 8/27/2018 Transfer to GM, and some minor changes

    Short Description:

        These files provide a software library and demo program for the Arduino and Particle
        microcontroller boards.

        The library files provide useful functions to make it easy
        to communicate with 7 Segment LEDs
        that use the I2C communication protocol. The demo
        program shows the usage of the functions in the library.

        The 7 Segment LED must connect to the I2C bus using a AMS AS1115 controller chip.
        A backback board with the AMS AS1115 chip is available and details are in the link below.

    https://www.dcity.org/portfolio/i2c-7-segment-led-library/
    This link has details including:
        * software library installation for use with Arduino, Particle and Raspberry Pi boards
        * list of functions available in these libraries
        * a demo program (which shows the usage of most library functions)
        * info on 7 segment LED displays that work with this software
        * hardware design for a backpack board for 7 segment LEDs, available on github
        * info on backpack “bare” pc boards available from OSH Park.


    License Information:  https://www.dcity.org/license-information/


    NOTES:
        1. If using Arduino IDE, version 1.5.0 or higher is REQUIRED!

*/

// include files... some boards require different include files
#ifdef ARDUINO_ARCH_AVR        // if using an arduino
#include "I2c7SegmentLed.h"
#elif ARDUINO_ARCH_SAM         // if using a arduino DUE
#include "I2c7SegmentLed.h"
#elif PARTICLE                 // if using a core, photon, or electron (by particle.io)
#include "I2c7SegmentLed.h"
#elif defined(__MK20DX128__) || (__MK20DX256__) || (__MK20DX256__) || (__MK62FX512__) || (__MK66FX1M0__) // if using a teensy 3.0, 3.1, 3.2, 3.5, 3.6
#include "I2c7SegmentLed.h"
#else                          // if using something else then this may work
#include "I2c7SegmentLed.h"
#endif


// default values for the LED segments for all 128 ASCII characters
// the first value is for ASCII character 0, then 1, etc
// each byte contains the 7 LED segments and the decimal point, arranged as (from MSB to LSB)
//    DP G F E D C B A   (DP, middle, top left, btm left, btm, btm right, top right, top)
// if a bit is a '1', then that segment of the led will be turned on.
const uint8_t LedSegments[] = {
0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110010,  // Ascii decimal:0-7       hex:00-07
0b01111110,0b01111011,0b01111101,0b00011111,0b00001101,0b00111101,0b01101111,0b01000111,  // Ascii decimal:8-15      hex:08-0F
0b01111110,0b00000110,0b01101101,0b01001111,0b00010111,0b01011011,0b01111011,0b00011110,  // Ascii decimal:16-23     hex:10-17
0b01111111,0b01011111,0b01101111,0b01110011,0b01100001,0b01100111,0b01111101,0b00111001,  // Ascii decimal:24-31     hex:18-1f
0b00000000,0b00110000,0b00100010,0b01000001,0b01001001,0b00100101,0b00110001,0b00000010,  // Ascii decimal:32-39     hex:20-27
0b01001010,0b01101000,0b01000010,0b00000111,0b00000100,0b00000001,0b00000000,0b00100101,  // Ascii decimal:40-47     hex:28-2F
0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110010,  // Ascii decimal:48-55     hex:30-37
0b01111111,0b01111011,0b01001000,0b01011000,0b01000011,0b00001001,0b01100001,0b01100101,  // Ascii decimal:56-63     hex:38-3F
0b01111101,0b01110111,0b01111111,0b01001110,0b00111101,0b01001111,0b01000111,0b01011110,  // Ascii decimal:64-71     hex:40-47
0b00110111,0b00000110,0b00111100,0b01010111,0b00001110,0b01010100,0b01110110,0b01111110,  // Ascii decimal:72-79     hex:48-4F
0b01100111,0b01101011,0b01100110,0b01011011,0b00001111,0b00111110,0b00111110,0b00101010,  // Ascii decimal:80-87     hex:50-57
0b00110111,0b00111011,0b01101101,0b00011110,0b00010011,0b00110110,0b01100010,0b00001000,  // Ascii decimal:88-95     hex:58-5F
0b00100000,0b01111101,0b00011111,0b00001101,0b00111101,0b01101111,0b01000111,0b01111011,  // Ascii decimal:96-103    hex:60-67
0b00010111,0b00000100,0b00011000,0b01010111,0b00000110,0b00010100,0b00010101,0b00011101,  // Ascii decimal:104-111   hex:68-6F
0b01100111,0b01110011,0b00000101,0b01011011,0b00001111,0b00011100,0b00011100,0b00010100,  // Ascii decimal:112-119   hex:70-77
0b00110111,0b00111011,0b01101101,0b01001011,0b01010101,0b01100011,0b01000000,0b00000000   // Ascii decimal:120-127   hex:78-7F
};



// class constructor
I2c7SegmentLed::I2c7SegmentLed(uint8_t i2cAddress, uint8_t digits)
{
  _i2cAddress          = i2cAddress;      // address of the display
  _digits              = digits;          // number of digits in this display
}


// public functions


void I2c7SegmentLed::begin()
{

  // Start talking to the AS1115 chips, as they will be i2c address 0 at powerup
  Wire.beginTransmission(0x00);
  Wire.write(REG_SHUTDOWN);
  Wire.write(REG_SHUTDOWN_NORMAL);
  Wire.endTransmission();
  delay(20);
  // tell all as1115 chips to use their hardware jumpered i2c address
  Wire.beginTransmission(0x00);
  Wire.write(REG_SELF_ADDRESSING);
  Wire.write(REG_SELF_ADDRESSING_USER_ADDRESS);
  Wire.endTransmission();
  delay(20);

  // reset the AS1115 chip and the feature register
  setRegister(REG_SHUTDOWN, REG_SHUTDOWN_NORMAL_AND_RESET);

  // display all digits, full brightness, decoded using the hex font
  setBrightness(15);
  setRegister(REG_SCAN_LIMIT,_digits-1);   // set number of digits in use
  setRegister(REG_DECODE_MODE,REG_DECODE_MODE_NO_DIGITS);   // we won't use their decoder

  _feature             = 0;             // starting value for the _feature register
  setRegister(REG_FEATURE,_feature);    // initialize the feature register

  clear();


}



// functions to interface with higher level Arduino and Particle functions (like Print and Println)

// this allows the print command to be used to write to the display...
// e.g. if your display class is myLed, then you can use  myLed.print(12.4);  to write to the led
inline size_t I2c7SegmentLed::write(uint8_t value)
{
  // if we are not past the number of digits that we have
  if (_cursorPosition <= _digits) {
    if (value == '.') {
      if (_cursorPosition == 1)
        setDecimalPoint(_cursorPosition++);     // set the dp for digit 1 and inc cursorPosition
      else
        setDecimalPoint(_cursorPosition-1);     // set the dp for the previous digit
    }
    else {
      _segments[_cursorPosition] = LedSegments[value];     // save the segments to local storage
      setSegments(_cursorPosition++, LedSegments[value]);   // write the segments to the display
    }
  }

  return 1;         // we have printed one character
}


// main functions

// clear and home the display
void I2c7SegmentLed::clear()
{
  for(uint8_t i=1; i<=_digits; i++){
    _segments[i] = 0x00;     // save the segments to local storage
    setSegments(i,0x00);   // set all segments to off
  }
  _cursorPosition = 1;  // reset the cursor to home
}


// home the invisible virtual cursor to position 1 (so that the next char written will go at this position)
void I2c7SegmentLed::home()
{
  cursorMove(1);
}


// move cursor to new postion digit (starts at 1)
void I2c7SegmentLed::cursorMove(uint8_t digit)
{
  if ((digit <= _digits) && (digit >=1)) {
    _cursorPosition = digit;
  }
}


// display off (reduces the current draw)
void I2c7SegmentLed::displayOff()
{
  setRegister(REG_SHUTDOWN,REG_SHUTDOWN_SHUTDOWN);
}

// display on
void I2c7SegmentLed::displayOn()
{
  setRegister(REG_SHUTDOWN,REG_SHUTDOWN_NORMAL);
}


// set the brightness of all digits (value range is 0-15 decimal)
void I2c7SegmentLed::setBrightness(uint8_t value)
{
  setRegister(REG_GLOBAL_INTENSITY, value);
}


// turn on the decimal point for digit specified
void I2c7SegmentLed::setDecimalPoint(uint8_t digit)
{
  uint8_t currentSegments;
  if ((digit <= _digits) && (digit >=1)) {
    currentSegments = _segments[digit];        // get the led segments for this digit
    setMask(currentSegments,DECIMAL_POINT_MASK);
    setSegments(digit, currentSegments);   // rewrite the segments
  }
}

// turn off the decimal point for this digit
void I2c7SegmentLed::clearDecimalPoint(uint8_t digit)
{
  uint8_t currentSegments;
  if ((digit <= _digits) && (digit >=1)) {
    currentSegments = _segments[digit];        // get the led segments for this digit
    clearMask(currentSegments,DECIMAL_POINT_MASK);
    setSegments(digit, currentSegments);   // rewrite the segments
  }
}

// turn on the specified segments for this digit
void I2c7SegmentLed::setSegments(uint8_t digit, uint8_t segments)
{
  if ((digit <= _digits) && (digit >=1)) {
    setRegister(digit, segments);
    _segments[digit] = segments;
  }
}


// private functions ********************************

// write value to this AS1115 register
void I2c7SegmentLed::setRegister(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(_i2cAddress);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// read data from this AS1115 register
uint8_t I2c7SegmentLed::getRegister(uint8_t reg)
{
  uint8_t dataRead;
  Wire.beginTransmission(_i2cAddress);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)_i2cAddress, (uint8_t)1);   // read one byte
  dataRead = Wire.read();
  return dataRead;
}



// future functions
/*

// blink on/off
void I2c7SegmentLed::blinkOff()
{
  clearBit(_feature, REG_FEATURE_BLINK);
  setRegister(REG_FEATURE,_feature);
}


void I2c7SegmentLed::blinkOn()
{
  setBit(_feature, REG_FEATURE_BLINK);
  setRegister(REG_FEATURE,_feature);
}

// blink fast/slow
void I2c7SegmentLed::blinkFast()
{
  setBit(_feature, REG_FEATURE_BLINK_FREQUENCY);
  setRegister(REG_FEATURE,_feature);
}


void I2c7SegmentLed::blinkSlow()
{
  clearBit(_feature, REG_FEATURE_BLINK_FREQUENCY);
  setRegister(REG_FEATURE,_feature);
}
*/
