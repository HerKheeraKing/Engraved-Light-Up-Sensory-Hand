/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_NeoPixel.h>


#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#define PIN 21
#define NUMPIXELS 7

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Initializing adafruit object(num of pixels, data pin, pixel type)
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

// color changing values
uint16_t timechange = 2000;
uint16_t stayblue = 300;
uint32_t currenttime[] = { 0, 0, 0, 0, 0, 0 };
bool istouched[] = { false, false, false, false, false, false };


void setup() {

  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)

  Serial.begin(9600);


  while (!Serial) {  // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1)
      ;
  }
  Serial.println("MPR121 found!");
}

void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();
  //pixels.clear(); // Set all pixel colors to 'off'


  // >>>>>>>>>>>>LIGHT UP EACH LED<<<<<<<<<<<<<<<
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for (int i = 6; i < 12; i++) {  // For each pixel...

    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      Serial.print(i);
      Serial.println(" touched");

      istouched[i - 6] = true;
    }

    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
      Serial.print(i);
      Serial.println(" released");
      istouched[i - 6] = false;

      // Here we're using a moderately bright green color:
    }
  }

  for (int i = 6; i < 12; i++) {

    if (istouched[i - 6] == true) {  // turn on
      // Lerping technique (blue -> red)
      if (currenttime[i - 6] < stayblue) {
        pixels.setPixelColor(i - 6, pixels.Color(0, 0, 255));       

        if (i == 11) {
          pixels.setPixelColor(i - 5, pixels.Color(0, 0, 255));    
        }
      } else {
        int cool = map(currenttime[i - 6], stayblue, timechange, 0, 255);  

        // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i - 6, pixels.Color(cool, 0, 255 - cool));   

        if (i == 11) {
          pixels.setPixelColor(i - 5, pixels.Color(cool, 0, 255 - cool));
        }
      }

      if (currenttime[i - 6] < timechange) {
        currenttime[i - 6]++;
      }
    } else {  // turn off
      pixels.setPixelColor(i - 6, pixels.Color(0, 0, 0));

      if (i == 11) {
        pixels.setPixelColor(i - 5, pixels.Color(0, 0, 0));
      }

      currenttime[i - 6] = 0;
    }
  }


  // reset our state
  lasttouched = currtouched;
  pixels.show();  // Send the updated pixel colors to the hardware.
  delay(1);
}
