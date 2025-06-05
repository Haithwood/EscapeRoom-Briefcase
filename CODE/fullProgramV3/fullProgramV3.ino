#include "FastLED.h"
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// ====== LED Configuration ======
#define NUM_LEDS 101
#define DATA_PIN 8
CRGB leds[NUM_LEDS];

// ====== DFPlayer Serial Communication ======
#define FPSerial Serial1
DFRobotDFPlayerMini myDFPlayer;
static bool trackStarted = false;

#define AUDIO_1 2
#define AUDIO_2 3
#define AUDIO_3 1

// ====== State Control ======
int state = 0;

// ====== Button Setup ======
const int blueButtonPin = 25;
const int redButtonPin = 23;
const int blueLightPin = 24;
const int redLightPin = 22;

// ====== Button State Trackers ======
int buttonStateBlue = 0;
int buttonStateRed = 0;


// ====== Tilt Switch ======
const int tiltPin = 26;
int tiltValue = 0;

// ====== Relay Pins ======
const int siren1Pin = 2;
const int siren2Pin = 3;
const int siren3Pin = 4;
const int siren4Pin = 5;  // broken
const int magBlue5Pin = 6;
const int magRed6Pin = 7;

// ====== Sensor Pins ======
const int internalMaglockPin = A1;
int internalMaglockValue = 1023;



// ====== Case 3 variables ======
// bool trackStarted = false;
uint8_t type;

// ===== Functions ======
void pulseLights(int red, int green, int blue);
void choiceStateLights();

void setup() {
  // ====== Initialize Serial Communication ======
  FPSerial.begin(9600);  // DFPlayer serial
  Serial.begin(115200);  // Debug serial

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo (Arduino Mega)"));
  Serial.println(F("Initializing DFPlayer... (May take 3~5 seconds)"));

  // ====== Initialize DFPlayer Mini ======
  if (!myDFPlayer.begin(FPSerial, true, true)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    while (true) delay(0);  // Stop execution
  }

  myDFPlayer.volume(20);  // Set audio volume (0–30)
  Serial.println(F("DFPlayer Mini online."));

  // ====== Initialize LED Strip ======
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // ====== Set Pin Modes ======
  // Inputs
  pinMode(tiltPin, INPUT);
  pinMode(blueButtonPin, INPUT_PULLUP);
  pinMode(redButtonPin, INPUT_PULLUP);

  // Outputs
  pinMode(blueLightPin, OUTPUT);
  pinMode(redLightPin, OUTPUT);
  digitalWrite(blueLightPin, LOW);
  digitalWrite(redLightPin, LOW);
  //sirens
  pinMode(siren1Pin, OUTPUT);
  pinMode(siren2Pin, OUTPUT);
  pinMode(siren3Pin, OUTPUT);
  digitalWrite(siren1Pin, HIGH);
  digitalWrite(siren2Pin, HIGH);
  digitalWrite(siren3Pin, HIGH);

  pinMode(magBlue5Pin, OUTPUT);
  pinMode(magRed6Pin, OUTPUT);
  digitalWrite(magBlue5Pin, LOW);
  digitalWrite(magRed6Pin, LOW);
}

void loop() {
  // Serial.println("restarting loop");
  // Serial.print("State number:");
  // Serial.println(state);

  switch (state) {
    case 0:  // Wait for maglock
      Serial.println("Starting state 0");
      internalMaglockValue = analogRead(internalMaglockPin);
      // Serial.print("Input maglock voltage level: ");
      Serial.println(internalMaglockValue);
      if (internalMaglockValue <= 50) {
        // Serial.println()
        state = 1;
      }
      //  else {
      //   FastLED.clear();  // lights off
      // }
      break;

    case 1:  // wait for lid to open
      Serial.println("Starting state 1");
      FastLED.setBrightness(100);
      pulseLights(255, 255, 200);  // lights on
      tiltValue = digitalRead(tiltPin);
      // Serial.print("Tilt:");
      // Serial.println(tiltValue);
      if (tiltValue == 0) {
        state = 2;
      }
      break;

    case 2:
      Serial.println("Starting state 2");
      digitalWrite(siren1Pin, LOW);
      digitalWrite(siren2Pin, LOW);
      digitalWrite(siren3Pin, LOW);
      fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0));  // Green
      FastLED.show();

      myDFPlayer.play(AUDIO_1);  // play track001
      waitForAudioToFinish();

      state = 3;
      break;

    case 3:  // Buttons wait
      Serial.println("Starting state 3");
      digitalWrite(blueLightPin, HIGH);
      digitalWrite(redLightPin, HIGH);
      choiceStateLights();

      buttonStateBlue = digitalRead(blueButtonPin);
      buttonStateRed = digitalRead(redButtonPin);
      if (buttonStateBlue == LOW) {
        state = 5;
      }
      if (buttonStateRed == LOW) {
        state = 4;
      }
      break;

    case 4:  // Red state
      Serial.println("Starting state 4");
      fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));  // Red
      FastLED.show();

      myDFPlayer.play(AUDIO_2);  // Play track #2
      waitForAudioToFinish();

      Serial.println("Red selected");
      digitalWrite(magRed6Pin, HIGH);  // Turn off external red maglock
      Serial.println("GAME OVER!!!");

      while (true) {  // Wait here forever while pulsing the lights red
        pulseLights(255, 0, 0);
      }

      break;

    case 5:  // Blue State
      Serial.println("Starting state 5");
      Serial.println("Blue selected");
      digitalWrite(siren1Pin, HIGH);
      digitalWrite(siren2Pin, HIGH);
      digitalWrite(siren3Pin, HIGH);
      fill_solid(leds, NUM_LEDS, CRGB(0, 0, 255));  // Blue
      FastLED.show();

      myDFPlayer.play(AUDIO_3);  // Play track #3
      waitForAudioToFinish();

      digitalWrite(magBlue5Pin, HIGH);  // Turn off external blue maglock
      Serial.println("GAME OVER!!!");

      while (true) {  // Wait here forever while pulsing the lights blue
        pulseLights(0, 0, 255);
      }

      break;
  }
}

void waitForAudioToFinish(){
  type = 0;
  while (type != DFPlayerPlayFinished) {
    if (myDFPlayer.available()) {
      delay(100);
      Serial.println("checking for finished playing");
      type = myDFPlayer.readType();
    }
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
