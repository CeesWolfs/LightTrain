// Ledstrip trein code (c) 2017 Cees Wolfs
// released under the MIT license

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define NUMPIXELS      10
#define SENSOR_THRESHOLD 400    

#define AFSTAND_TRAJECT 350.0

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int sensor1;
const int sensor2;
int delayval = 500; // delay for half a second
int ms = 0;         // Aantal milliseconden dat de trein over het traject sensor1 -> sensor2 doet
float snelheid;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  for(int i=0;i<NUMPIXELS;i++){
    // Zet de beginkleur op wit
    pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // Moderately bright green color.
  }
  pixels.show(); 
}

void loop() {
  sensor1 = analogRead(A0);
  sensor2 = analogRead(A1);
  while (sensor1 > SENSOR_THRESHOLD) {
    ms++;
    sensor2 = analogRead(A1);
    if(sensor2 < SENSOR_TRESHOLD) break;
    delay(1);
  }
  snelheid = AFSTAND_TRAJECT/(float)ms;
  delayval = 1.7*snelheid; // Snelheid is in cm/ms astand tussen de ledjes is 1,7 seconden dus delay in ms is 1,7*snelheid
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
