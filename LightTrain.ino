// Ledstrip trein code (c) 2017 Cees Wolfs
// released under the MIT license

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            4
#define NUMPIXELS      10
#define SENSOR_THRESHOLD 400    

#define IDLE_TIME 300*1000
#define WIT pixels.Color(255,255,255)
#define UIT pixels.Color(0,0,0)

#if (delayval >= 200)
#define KLEUR pixels.Color(0, 150, 0)
#elif (delayval < 200) 
#define KLEUR pixels.Color(200,144,0)
#elif (delayval < 150) 
#define KLEUR pixels.Color(200, 100, 0)
#else 
#define KLEUR pixels.Color(200, 0, 0)
#endif

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int sensor1;
int delayval = 200; // delay for half a second
int ms = 0;         // Aantal milliseconden dat de trein erover doet om sensor 1 te passeren.
unsigned long laatste_trein;   // Aantal milliseconden sinds de laatste keer dat de trein langskwam.
int aantal_leds;
boolean trein_langs;


void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  for(int i=0;i<NUMPIXELS;i++){
    // Zet de beginkleur op wit
    pixels.setPixelColor(i, WIT);
  }
  pixels.show(); 
}

void loop() {
  sensor1 = analogRead(A0);
  while (sensor1 < SENSOR_THRESHOLD) {
    ms++;
    delay(1);
    trein_langs = true;
    sensor1 = analogRead(A0);
  }
  aantal_leds = ms/delayval;
  if(trein_langs) {
    for(int i=0;i<NUMPIXELS+aantal_leds;i++){
      int achterste_led = i - aantal_leds;
      if(achterste_led >= 0) {
        pixels.setPixelColor(achterste_led, WIT);
      }
      if (i < NUMPIXELS) {
        pixels.setPixelColor(i, KLEUR);
      } 
      pixels.show();
      delay(delayval); 
    }
    trein_langs = false;
    ms = 0;
    laatste_trein = millis();
  }
  if((millis() - laatste_trein) > IDLE_TIME) {
    // De trein is al lang niet langsgeweest, zet de lampjes uit
    for(int i=0;i<NUMPIXELS;i++){
      // Zet de lampjes uit
      pixels.setPixelColor(i, UIT);
    }
    pixels.show();
  }
}
