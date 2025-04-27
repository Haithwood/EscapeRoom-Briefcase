#include "FastLED.h"

#define NUM_LEDS 90  // number of LEDs
#define DATA_PIN 23   // LED pin

CRGB leds[NUM_LEDS];

int state = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

void loop() {
  //suitcase closed
  if (state == 0){
    pulseLights(255,255,255);
  }
  //suitcase opened (tilt switch)
  if (state == 1){
    // light sequence after opening suitcase
  }
  //red button pressed
  if (state == 2){
    //light sequence for pressing red button
  }
  //blue button pressed
  if (state == 3){
    // light sequence for pressing blue button
  }
}

void pulseLights(int red, int green, int blue) {
  float r, g, b;

  // fade in
  for(int i = 0; i <= 255; i++) {
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(2);
  }

  // fade out
  for(int i = 255; i >= 0; i--) {
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(2);
  }
}
