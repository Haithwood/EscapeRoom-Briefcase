#include "FastLED.h"
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#define FPSerial Serial1
#define NUM_LEDS 78  // number of LEDs
#define DATA_PIN 8   // LED pin

int tiltPin = 26; 

CRGB leds[NUM_LEDS];
DFRobotDFPlayerMini myDFPlayer;

void printDetail(uint8_t type, int value);
void pulseLights(int red, int green, int blue);

int state = 0;
int tiltValue = 0;

void setup() {
  Serial.begin(9600);
  Serial.begin(115200);  // Start Serial (USB) for debugging

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo (Arduino Mega)"));
  Serial.println(F("Initializing DFPlayer... (May take 3~5 seconds)"));
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  pinMode(tiltPin, INPUT);

  if (!myDFPlayer.begin(FPSerial, true, true)) {  // Use Serial1 to communicate with DFPlayer
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    while (true) {
      delay(0);  // Infinite loop to halt the program
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(20);  // Set volume (0-30)
}

void loop() {
  //suitcase closed
  Serial.println(state);
  tiltValue = digitalRead(tiltPin);
  if (state == 0){
    FastLED.clear();
    pulseLights(255,255,255);
    if (tiltValue < 1){
      state++;
    }
  }
  //suitcase opened (tilt switch)
  if (state == 1){ // light sequence after opening suitcase
    FastLED.clear();
    pulseLights(0,255,0);
    myDFPlayer.play(1);     // Play first MP3

    if (myDFPlayer.available()) {
      uint8_t type = myDFPlayer.readType();
      int value = myDFPlayer.read();

      if (type == DFPlayerPlayFinished) {
        state++;  // Increment state when a track finishes
        Serial.println("Track finished! next state... ");
      }
    }
  }
  //red button pressed
  if (state == 2){
    //light sequence for pressing red button
    FastLED.clear();
    pulseLights(255,0,0);
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

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
