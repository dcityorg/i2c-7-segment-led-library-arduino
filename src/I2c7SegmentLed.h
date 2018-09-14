
/*
  I2c7SegmentLed.h

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
#include "Arduino.h"
#include "Wire.h"
#elif ARDUINO_ARCH_SAM        // if using an arduino DUE
#include "Arduino.h"
#include "Wire.h"
#elif PARTICLE                 // if using a core, photon, or electron (by particle.io)
#include "Particle.h"
#elif defined(__MK20DX128__) || (__MK20DX256__) || (__MK20DX256__) || (__MK62FX512__) || (__MK66FX1M0__) // if using a teensy 3.0, 3.1, 3.2, 3.5, 3.6
#include "Arduino.h"
#include "Wire.h"
#else                          // if using something else then this may work
#include "Arduino.h"
#include "Wire.h"
#endif


// define macro functions **********************************************************************************
//#define setBit(var, bit)   ((var) |= (byte)(1 << mask))             // set bit - sets the bit (0-7) n the variable var
//#define clearBit(var, bit) ((var) &= (byte)~(1 << mask))            // clear bit - clears the bit (0-7) in the variable var
//#define getBit(var, bit)   (((var) & (byte)(1 << mask))?1:0)        // get bit - returns the state (either 1 or 0) of the bit (0-7) in the variable var
//#define isBitSet(var,bit)  (((var) & (byte)(1 << mask))?1:0)        // is bit set - returns 1 if the bit (0-7) in variable var is 1
//#define isBitClear(var,bit)  (((var) & (byte)(1 << mask))?0:1)      // is bit clear - returns 1 if the bit (0-7) in variable var is 0
#define setMask(var, mask)   ((var) |= (byte)(mask))             // sets the mask bits in the variable var
#define clearMask(var, mask) ((var) &= (byte)~(mask))            // clears the mask bits in the variable var


// Control Register Addresses in the AS1115
// Digit 0-7 are at adresses 1-8
#define REG_DECODE_MODE       0x09     // sets which digits respond to data that is decoded (like BCD or HEX)
#define REG_GLOBAL_INTENSITY  0X0a     // set the brightness for all digits... only bottom 4 bits are used for 16 brightness values
#define REG_SCAN_LIMIT        0x0b     // controls which digits are turned on
#define REG_SHUTDOWN          0x0c     // used to shutdown the display and save power
#define REG_FEATURE           0x0e
#define REG_DISPLAY_TEST_MODE 0x0f
#define REG_DIGIT01_INTENSITY 0x10
#define REG_DIGIT23_INTENSITY 0x11
#define REG_DIGIT45_INTENSITY 0x12
#define REG_DIGIT67_INTENSIGY 0x13

#define REG_DIAGNOSTIC_DIGIT0 0x14
#define REG_DIAGNOSTIC_DIGIT1 0x15
#define REG_DIAGNOSTIC_DIGIT2 0x16
#define REG_DIAGNOSTIC_DIGIT3 0x17
#define REG_DIAGNOSTIC_DIGIT4 0x18
#define REG_DIAGNOSTIC_DIGIT5 0x19
#define REG_DIAGNOSTIC_DIGIT6 0x1a
#define REG_DIAGNOSTIC_DIGIT7 0x1b
#define REG_KEYA              0x1c
#define REG_KEYB              0x1d

#define REG_SELF_ADDRESSING   0x2d     // used to see if the chip will read jumpers to determine it's own i2c address

// Data that can be written to the control registers above

// REG_DECODE_MODE values (type of decode is set in REG_FEATURE)
#define REG_DECODE_MODE_NO_DIGITS 0x00      // no decoding
#define REG_DECODE_MODE_ALL_DIGITS 0xFF     // used for BCD or HEX decoding, bit 0 turns on digit 0 for decoding, etc

// REG_SCAN_LIMIT values (how many digits are displayed)
#define REG_SCAN_LIMIT_1  0x00  // if there is only 1 digit in the display
#define REG_SCAN_LIMIT_2  0x01
#define REG_SCAN_LIMIT_3  0x02
#define REG_SCAN_LIMIT_4  0x03
#define REG_SCAN_LIMIT_5  0x04
#define REG_SCAN_LIMIT_6  0x05
#define REG_SCAN_LIMIT_7  0x06
#define REG_SCAN_LIMIT_8  0x07  // if there are 8 digits in the display

// REG_SHUTDOWN values
#define REG_SHUTDOWN_SHUTDOWN_AND_RESET 0x00
#define REG_SHUTDOWN_SHUTDOWN 0x80
#define REG_SHUTDOWN_NORMAL_AND_RESET 0x01
#define REG_SHUTDOWN_NORMAL 0X81            // this is the normal running values

// REG_SELF_ADDRESSING values, for determinine the chip's i2c address
#define REG_SELF_ADDRESSING_FACTORY_ADDRESS 0x00
#define REG_SELF_ADDRESSING_USER_ADDRESS 0x01

// REG_FEATURE bit values
#define REG_FEATURE_EXTERNAL_CLOCK 0X01   // set bit if using an external clock
#define REG_FEATURE_RESET 0x02            // set bit to reset all registers
#define REG_FEATURE_HEX 0x04              // clear this bit for BCD decoding, set for HEX decoding
#define REG_FEATURE_BLINK 0x10            // set bit to enable blinking of display
#define REG_FEATURE_BLINK_FREQUENCY 0x020 // set bit for 2 second blinking, clear for 1 second blinking
#define REG_FEATURE_SYNC 0x40             // set bit for multiple device blinking
#define REG_FEATURE_BLINK_START 0x80      // set bit to start blinking when display turns on, clear to start blinking when display turns off


#define DECIMAL_POINT_MASK 0x80            // bit to control the decimal point




class I2c7SegmentLed : public Print {       // parent class is Print, so that we can use the print functions
public:

  virtual size_t write(uint8_t);                      // allows the print and println command to work (in Arduino or Particle)
                                                      // write one ascii character to the LED

  I2c7SegmentLed(uint8_t i2cAddress, uint8_t digits); // creates a display object
  void begin();                                       // required to inialize the display. run this first!
  void clear();                                       // clear the display and home the cursor to 1,1
  void home();                                        // move the cursor to home position (1,1)
  void cursorMove(uint8_t digit);                     // move cursor to position the position left of digit 1-8 (positions start at 1)
  void displayOff();                                  // turns off the entire display to save power
  void displayOn();                                   // turns on the entire display
  void setBrightness(uint8_t);                        // set brightness to value 0-15
  void setDecimalPoint(uint8_t digit);                // set the decimal point after this digit (1-8)
  void clearDecimalPoint(uint8_t digit);              // clear the decimal point after this digit (1-8)
  void setSegments(uint8_t digit, uint8_t segments);  // set the digit (1-8) to these segments

private:

  void setRegister(uint8_t, uint8_t);                 // set register to this value
  uint8_t getRegister(uint8_t);                       // read a register from the AS1115

  // private variables

  uint8_t _feature;                 // keep track of the as1115 feature register (we can set and clear bits in it)
  uint8_t _cursorPosition;          // virtual (invisible) cursor is currently to the left of this digit (1-8)
  uint8_t _i2cAddress;              // i2c address of the display
  uint8_t _digits;                  // number of digits in the display (starting at 1)
  uint8_t _segments[9];             // keep track of what segments are set for each digit...
                                    //      used in case we need to set the decimal point for that digit

};


// future functions
/*
void blinkOff();                                                   // stops blinking the display
void blinkOn();                                                    // starts blinking the display
void blinkFast();                                                  // turns on blinking in fast mode
void blinkSlow();                                                  // turns on blinking in slow mode
*/
