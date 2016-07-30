// 1. need to figure out how to get the const LedSegments into program memory, not ram memory !!!!!!!!



/*
  Important NOTES:
    1. If using Arduino IDE, version 1.5.0 or higher is REQUIRED!
*/

/*
  I2c7SegmentLed.cpp

  Written by: Gary Muhonen  gary@wht.io

  versions
    1.0.0 - 7/31/2016
      Original Release.

  Short Description:
    These files provide software for Arduino and Particle (Photon, Electron, and Core)
    microcontroller boards.  The library files provide useful functions to make it easy
    to communicate with 7 Segment LED display modules that use the AMS AS1115
    LED controller chip. This chip uses the I2C communication protocol. The demo
    program shows the usage of the functions in the library.

    The library files and demo will work with 7 segment LED displays with up to
    8 digits. The LED display must use the AMS AS1115 controller chip.

    See the links below for installation and usage information.

    Project Details:
    * Library installation and usage:                  http://wht.io/portfolio/i2c-7-segment-led-library/
    * .8 inch,  7 Segment module hardware information: http://wht.io/portfolio/i2c-7-segment-led-backpack-dot8inch
    * .56 inch, 7 Segment module hardware information: http://wht.io/portfolio/i2c-7-segment-led-backpack-dot56inch
    * .36 inch, 7 Segment module hardware information: http://wht.io/portfolio/i2c-7-segment-led-backpack-dot36inch

    Software Github repositories (library and demo programs):
    * Arduino library files:      https://github.com/wht-io/i2c-7-segment-led-library-arduino.git
    * Particle library files:     https://github.com/wht-io/i2c-7-segment-led-library-particle.git
    * Raspberry Pi library files: https://github.com/wht-io/i2c-7-segment-led-library-raspberrypi.git

    Hardware Design Github repositories (schematic and board layouts):
    * .8 inch,  7 Segment module design: http://wht.io/portfolio/i2c-7-segment-led-backpack-dot8inch
    * .56 inch, 7 Segment module design: http://wht.io/portfolio/i2c-7-segment-led-backpack-dot56inch
    * .36 inch, 7 Segment module design: http://wht.io/portfolio/i2c-7-segment-led-backpack-dot36inch
*/

/*
  Windy Hill Technology LLC code, firmware, and software is released under the
  MIT License (http://opensource.org/licenses/MIT).

  The MIT License (MIT)

  Copyright (c) 2016 Windy Hill Technology LLC

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*/


#ifdef ARDUINO_ARCH_AVR        // if using an arduino
#include "I2c7SegmentLed.h"
#elif SPARK                    // if using a core, photon, or electron (by particle.io)
#include "I2c7SegmentLed.h"
#else                          // if using something else
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
