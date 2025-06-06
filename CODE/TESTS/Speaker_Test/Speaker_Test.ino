#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// connect TX/RX to pins 18/19
// yellow goes to 18/ yellow goes to 19
#define FPSerial Serial1

// Revise the mp3 files on the SD card to match the relevant order 
// Figuring out what the order is requires testing once the audio- 
// files are replaced, preferably with a sheet of paper.

#define AUDIO_1 2
#define AUDIO_2 3
#define AUDIO_3 1

DFRobotDFPlayerMini myDFPlayer;
// static bool trackStarted = false;
uint8_t type;

void printDetail(uint8_t type, int value);
void waitForAudioToFinish();

void setup() {
  FPSerial.begin(9600);  // Start Serial1 for DFPlayer
  Serial.begin(115200);  // Start Serial (USB) for debugging

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo (Arduino Mega)"));
  Serial.println(F("Initializing DFPlayer... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(FPSerial, true, true)) {  // Use Serial1 to communicate with DFPlayer
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    while (true) {
      delay(0);  // Infinite loop to halt the program
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(10);  // Set volume (0-30)
}

void loop() {
  Serial.println("Loop starting");

  myDFPlayer.play(AUDIO_2);  // change the audio number to test the relevant file
  waitForAudioToFinish();

  Serial.println("Audio file finished playing");
}

void waitForAudioToFinish() {
  type = 0;
  while (type != DFPlayerPlayFinished) {
    if (myDFPlayer.available()) {
      delay(100);
      Serial.println("checking for finished playing");
      type = myDFPlayer.readType();
    }
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