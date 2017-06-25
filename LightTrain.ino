// Ledstrip trein code (c) 2017 Cees Wolfs
// released under the MIT license

#include "gamma_correctie.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            4       // Ledjes worden bestuurd vanaf deze pin
#define NUMPIXELS      10      // Aantal ledjes in de neopixel ledstring
#define SENSOR_THRESHOLD 400   // Waarde waaronder de sensor komt als er een trein langsgaat, expirimenteel bepaald

#define IDLE_TIME 300L*1000L   // Na 5 minuten moeten de lampjes weer uit gaan
#define WIT kleur(255,255,255) 
#define UIT kleur(0,0,0)

#if (delayval >= 200)
#define KLEUR kleur(0, 200, 0) // Groen de trein gaat langzaam
#elif (delayval >= 150)
#define KLEUR kleur(200, 200, 0) // Geel de trein gaat niet snel
#elif (delayval >= 100)
#define KLEUR kleur(200, 120, 0) // Oranje de trein gaat vrij snel 
#else
#define KLEUR kleur(200, 0, 0)   // Rood de trein gaat vol gas
#endif

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int delayval = 200; // delay for half a second
unsigned long laatste_trein;   // Aantal milliseconden sinds de laatste keer dat de trein langskwam.



void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  for (int i = 0; i < NUMPIXELS; i++) {
    // Zet de beginkleur op wit
    pixels.setPixelColor(i, WIT);
  }
  pixels.show();
  laatste_trein = millis();
}

void loop() {
  int sensor1 = analogRead(A0);
  int aantal_leds;
  boolean trein_aangekomen = false;
  int ms = 0;         // Aantal milliseconden dat de trein erover doet om sensor 1 te passeren.

  while (sensor1 < SENSOR_THRESHOLD) { // Sensor is laag, de trein komt langs
    ms++;
    delay(1);
    trein_aangekomen = true;  // Er is een trein aangekomen
    sensor1 = analogRead(A0); // Herhaal deze loop totdat de waarde waar hoog is, dus totdat de trein voorbij is
  }
  aantal_leds = ms / delayval; // Het aantal ledjes dat de trein breedt is hoelang de trein overover doet gedeeld door hoelang een ledje duurt
  if (trein_aangekomen) {          // Als er een trein langsgekomen is doe het volgende:
    for (int i = 0; i < NUMPIXELS + aantal_leds; i++) { // Beetje ingewikkeld, tel tot het aantal ledjes + het aantal ledjes dat de trein lang is
      int achterste_led = i - aantal_leds;    // Dit doe ik zo om ook het terugschakelen van de ledjes te regelen
      if (achterste_led >= 0) {               // Als de trein helemaal op de ledstrip is moeten er weer lampjes op de rust kleur worden gezet
        pixels.setPixelColor(achterste_led, WIT);
      }
      if (i < NUMPIXELS) {                    // Zet de lampjes die met de trein meegaan op de kleur aangeven door de snelheid, zie bovenin
        pixels.setPixelColor(i, KLEUR);
      }
      pixels.show();
      delay(delayval);                        // Wacht delayval milliseconden
    }
    laatste_trein = millis();                 // Reset als het ware de timer die het uitschakelen van de ledjes regelt, zie hieronder
  }
  if ((millis() - laatste_trein) > IDLE_TIME) { // Als de trein IDLE_TIME milliseconden niet is langsgeweest, zet de lampjes uit
    for (int i = 0; i < NUMPIXELS; i++) {
      // Zet de lampjes uit
      pixels.setPixelColor(i, UIT);
    }
    pixels.show();
  }
}
