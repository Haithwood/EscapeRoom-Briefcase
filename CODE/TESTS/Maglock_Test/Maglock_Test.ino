// Test to check that the Maglock in the suitcase works.
// Locked(external 12v on): lights red \ Unlocked(external 12v off): Lights Blue
// Before running make sure all wires are correctly connected & internal 12V power supply is on.


#include <FastLED.h>

#define NUM_LEDS 101
#define DATA_PIN 9
CRGB leds[NUM_LEDS];

const int internalMaglockPin = A1;
int internalMaglockValue = 1023;
int state = 0;

void pulseLights(int red, int green, int blue);


void setup() {
  Serial.begin(9600);
  // pinMode(analogPin, INPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  switch (state) {
    case 0:  // Wait for maglock
      internalMaglockValue = analogRead(internalMaglockPin);
      Serial.print("Input maglock voltage level: ");
      Serial.println(internalMaglockValue);
      if (internalMaglockValue <= 50) {
        // FastLED.setBrightness(50);
        pulseLights(255, 0, 0);  // Red pulse
      } else {
        pulseLights(0, 0, 255);  // Blue pulse
      }
      break;
  }
}

void pulseLights(int red, int green, int blue) {
  float r, g, b;

  // fade in
  for (int i = 0; i <= 255; i++) {
    r = (i / 256.0) * red;
    g = (i / 256.0) * green;
    b = (i / 256.0) * blue;
    fill_solid(leds, NUM_LEDS, CRGB((int)r, (int)g, (int)b));
    FastLED.show();
    delay(2);
  }

  // fade out
  for (int i = 255; i >= 0; i--) {
    r = (i / 256.0) * red;
    g = (i / 256.0) * green;
    b = (i / 256.0) * blue;
    fill_solid(leds, NUM_LEDS, CRGB((int)r, (int)g, (int)b));
    FastLED.show();
    delay(2);
  }
}