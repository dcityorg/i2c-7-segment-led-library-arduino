/*
  Important NOTES:
    1. If using Arduino IDE, version 1.5.0 or higher is REQUIRED!
*/

/*
  I2c7SegmentLedDemo.ino

  Written by: Gary Muhonen  gary@wht.io

  versions
    1.0.0 - 3/19/2016
      Original release.

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
  This demo program is public domain. You may use it for any purpose.
  NO WARRANTY IS IMPLIED.
*/


#ifdef ARDUINO_ARCH_AVR         // if using an arduino
#include <I2c7SegmentLed.h>
#include <Wire.h>
#elif SPARK                     // if using a core, photon, or electron (by particle.io)
#include "I2c7SegmentLed/I2c7SegmentLed.h"  // use this if the library files are in the particle repository of libraries
//#include "I2c7SegmentLed.h"     // use this if the library files are in the same folder as this demo program
#else                           // if using something else
#endif

#define LEDADDRESS     0x03                    // i2c address for the led display

#define TESTNUM        2                       // number of times to run each test

// In this demo program we are testing a four digit, 7 segment, LED display.

// create an led object at i2c address and with 4 digits
I2c7SegmentLed led(LEDADDRESS, 4);    // create an led object for 4 digits.

void setup()
{
  Serial.begin(9600);
  Serial.print("Setting up the 7 Segment LED display");
  Wire.begin();                     // initialize i2c
  led.begin();                      // initialize the led

}


// change the constant TESTNUM above to control how many times each test is run.

void loop()
{

  // display some typical messages on led
  led.clear();
  led.print("Typ");
  uint8_t temp = 72;
  float pH = 7.02;
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
      led.clear();
      led.print("T=");
      led.print(temp);
      delay(1000);
      led.clear();
      led.print("pH=");
      delay(1000);
      led.clear() ;
      led.print(pH);
      delay(1000);
  }


  // test displaying ASCII strings
  led.clear();
  led.print("ASCI");
  delay(1000);
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.clear();
    led.print("ABCD");
    delay(1000);
    led.clear();
    led.print("EFGH");
    delay(1000);
    led.clear();
    led.print("abcd");
    delay(1000);
    led.clear();
    led.print("efgh");
    delay(1000);
  }

  // test displaying floating point numbers containing decimal points
  led.clear();
  led.print("FLT");
  delay(1000);
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.clear();
    led.print(.678,3);
    delay(1000);
    led.clear();
    led.print(5.678,3);
    delay(1000);
    led.clear();
    led.print(56.78);
    delay(1000);
    led.clear();
    led.print(567.8);
    delay(1000);
    led.clear();
    led.print(5678.);
    delay(1000);

  }


  // test displaying all ASCII characters
  led.clear();
  led.print("ASCI");
  delay(1000);
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    for (uint8_t j=0; j<=127; ++j) {
      led.clear();
      led.write(j);
      delay(100);
    }
  }


  // test the clear command
  led.clear();
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.print("Clr");
    delay(1000);
    led.clear();
    delay(1000);
  }



  // test the cursor home command
  led.clear();
  led.print("HOME");
  delay(1000);
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.clear();
    led.print("HOME");
    delay(1000);
    led.home();
    delay(1000);
    led.print("1");
    delay(500);
    led.print("2");
    delay(500);
    led.print("3");
    delay(500);
    led.print("4");
    delay(500);
  }



  // test the cursor move command
  led.clear();
  led.print("MOVE");
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.clear();
    led.print("MOVE");
    delay(1000);
    led.cursorMove(4);
    led.print(" ");
    delay(1000);
    led.cursorMove(3);
    led.print(" ");
    delay(1000);
    led.cursorMove(2);
    led.print(" ");
    delay(1000);
    led.cursorMove(1);
    led.print(" ");
    delay(1000);
  }



  // test the display on/off commands
  led.clear();
  led.print("OFF");
  delay(1000);
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.displayOn();
    delay(1000);
    led.displayOff();
    delay(1000);
  }
  led.displayOn();


  // test the set segments command, where you control the individual segments of the LED
  // The 7 LED segments and the decimal point are arranged as (from MSB to LSB)
  //    DP A B C D E F G   (DP, top, top right, btm right, btm, btm left, top left, middle)
  led.clear();
  led.print("SEG");
  delay(1000);
  led.clear();
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.setSegments(1, 0x36);      // turn on side segments
    delay(1000);
    led.setSegments(1, 0x48);      // turn on top and bottom segments
    delay(1000);
  }



  // test the setBrightness command.
  led.clear();
  led.print("BRIT");
  delay(1000);
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.setBrightness(0x0);      // set brightness to min
    delay(1000);
    led.setBrightness(0xf);      // set brightness to max
    delay(1000);
  }



  // test the setDecimalPoint and clearDecimalPoint command.
  led.clear();
  led.print("DP");
  delay(1000);
  for (uint8_t i = 0; i < TESTNUM; ++i)
  {
    led.setDecimalPoint(3);        // set the dp on the 1st digit
    delay(1000);
    led.clearDecimalPoint(3);      // clear the dp on the 1st digit
    delay(1000);
  }







}
