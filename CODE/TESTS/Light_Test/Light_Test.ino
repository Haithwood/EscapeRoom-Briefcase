// Test script for neo-pixel LEDs
// Before testing make sure that the 12 volt power-
// supply is plugged in & the pin is correctly connected

#include "FastLED.h"

#define NUM_LEDS 101  // number of LEDs
#define DATA_PIN 8    // LED pin

CRGB leds[NUM_LEDS];

void pulseLights(int red, int green, int blue);
void choiceStateLights();

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  FastLED.clear();
  //comment one of the funcions to test them
  pulseLights(0, 255, 0);
  // choiceStateLights();
}

void pulseLights(int red, int green, int blue) {
  float r, g, b;

  // fade in
  for (int i = 0; i <= 255; i++) {
    r = (i / 256.0) * red;
    g = (i / 256.0) * green;
    b = (i / 256.0) * blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(2);
  }

  // fade out
  for (int i = 255; i >= 0; i--) {
    r = (i / 256.0) * red;
    g = (i / 256.0) * green;
    b = (i / 256.0) * blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(2);
  }
}
void choiceStateLights() {
  // Blue section: LEDs 0–10
  for (int i = 0; i <= 10; i++) {
    leds[i].setRGB(0, 0, 255);
  }

  // Red section: LEDs 11–33
  for (int i = 11; i <= 33; i++) {
    leds[i].setRGB(255, 0, 0);
  }

  // Blue section: LEDs 34–55
  for (int i = 34; i <= 55; i++) {
    leds[i].setRGB(0, 0, 255);
  }

  // Red section: LEDs 56–73
  for (int i = 56; i <= 73; i++) {
    leds[i].setRGB(255, 0, 0);
  }

  // Blue section: LEDs 74–90
  for (int i = 74; i <= 90; i++) {
    leds[i].setRGB(0, 0, 255);
  }

  // Red section: LEDs 91–101
  for (int i = 91; i <= 101; i++) {
    leds[i].setRGB(255, 0, 0);
  }
  FastLED.show();
}


