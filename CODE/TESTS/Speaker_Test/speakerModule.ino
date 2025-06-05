#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

//connect TX/RX to pins 18/19
#define FPSerial Serial1

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int state = 0;

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
  myDFPlayer.play(1);     // Play first MP3
}

void loop() {
  if (state == 0) {
    if (myDFPlayer.available()) {
      uint8_t type = myDFPlayer.readType();
      int value = myDFPlayer.read();

      if (type == DFPlayerPlayFinished) {
        state++;  // Increment state when a track finishes
        Serial.println("Track finished! next state... ");
      }

      // Optional: still print all details
      printDetail(type, value);
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