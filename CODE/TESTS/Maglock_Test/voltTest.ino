#include <FastLED.h>

#define NUM_LEDS 101
#define DATA_PIN 8

CRGB leds[NUM_LEDS];

const int analogPin = A0;

void setup() {
  Serial.begin(9600);
  pinMode(analogPin, INPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  int sensorValue = analogRead(analogPin);
  Serial.print("Raw sensor value: ");
  Serial.println(sensorValue);

  if (sensorValue == 0) {
    pulseLights(255, 0, 0);  // Red pulse
  } else {
    pulseLights(0, 0, 255);  // Blue pulse
  }

  delay(100);  // Optional: prevent spamming Serial Monitor
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