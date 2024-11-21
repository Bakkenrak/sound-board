#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/13, /*tx =*/15);
#define FPSerial softSerial

DFRobotDFPlayerMini myDFPlayer;
int sensorValue = 0;  // variable to store the value coming from the sensor

void printDetail(uint8_t type, int value);

void setup()
{
  FPSerial.begin(9600);

  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30
}

void loop()
{
  // read the value from the sensor:
  sensorValue = analogRead(A0);
  int buttonNumber = mapToButtonNumber(sensorValue);
  Serial.print(F("Sensor : "));
  Serial.print(sensorValue);
  Serial.print(F(" -> Button: "));
  Serial.println(buttonNumber);

  if (buttonNumber > 0) {
    //myDFPlayer.play(buttonNumber);
    myDFPlayer.playFolder(0, buttonNumber);
    delay(1000);
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value){
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

int mapToButtonNumber(int sensorValue){
  switch(sensorValue){
    case 75 ... 125:
      return 1;
    case 175 ... 225:
      return 2;
    case 255 ... 305:
      return 3;
    case 345 ... 395:
      return 4;
    case 425 ... 475:
      return 5;
    case 525 ... 575:
      return 6;
    case 725 ... 775:
      return 7;
    case 875 ... 925:
      return 8;
    case 1000 ... 1024:
      return 9;
    default:
      Serial.print(F("Cannot map value to button: "));
      Serial.println(sensorValue);
      return 0;
  }
}