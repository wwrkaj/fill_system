/*
 * this is the program to run on the 'slave' arduino
 * 
 * Connect HC12 "RXD" pin to Arduino Digital Pin 4
 * Connect HC12 "TXD" pin to Arduino Digital Pin 5
 * Connect HC12 "Set" pin to Arduino Digital Pin 6
 * 
 * By Josh Wewerka
*/
#include "HX711.h"
// make sure you add the hx711 library by Bogdan Necula
// hx711 can be found by searching in the manage libraries under the tools menu

#include <Servo.h>
#include <SoftwareSerial.h>

const byte HC12RxdPin = 4;                  // Recieve Pin on HC12
const byte HC12TxdPin = 5;                  // Transmit Pin on HC12

const int pressurePin = A0;   // inout pin for pressure tansducer 

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;
char arm_value = 'n';
HX711 scale;

SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards


void setup() {
  pinMode(pressurePin, INPUT);  // set up analog pin for pressure sensor
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
   // need to add in all 4 servos and their locations
  Serial.begin(9600);   // open serial port for computer
  HC12.begin(9600);     //open serial port to HC12
  delay(2000);
 /* HC12.write(" Do you want to arm the Fill System? enter 'y' to arm... ");
  arm_value = HC12.read();
  while(arm_value!='y'){
    arm_value = HC12.read();
    //wait for y
    delay(500);
  }
  HC12.write(" System armed, Enter Command: ");
  */
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  //set up load cell communications
}

void loop() {
  if(HC12.available()>0){
    int program = HC12.read();
      switch(program){
        case '0':   // program zero = default close all
                    // good for both savox valves!
          myservo.write(10);
          delay(150);
          myservo.write(10);
          delay(150);
          myservo.write(10);
          delay(150);
          myservo.write(10);
          delay(150);
          myservo.write(10);
          delay(150);
          myservo.write(10);
          HC12.write("...Running program 0: closing valve ");
          delay(1000);    //delay in ms needed for max close time under load
          HC12.write(" done...");
          break;
        case '1':   //program one = open
                    // good for both savox values
          myservo.write(100);
          delay(150);
          myservo.write(100);
          delay(150);
          myservo.write(100);
          delay(150);
          myservo.write(100);
          delay(150);
          myservo.write(100);
          delay(150);
          myservo.write(100);
          delay(150);
          HC12.write("...Running program 1: opening valve ");
          delay(1000);    //delay time in ms needed for max open time under load
          HC12.write(" done...");
          break;
        case '2': //program two = get load cell data
          if (scale.is_ready()) {
              long reading = scale.read();
              HC12.write("Running program 2: HX711 reading =  ");
              HC12.write(reading);
              Serial.print("load cell raw: ");
              Serial.println(reading);
              delay(1000);
              HC12.write(" not done...");
            } else {
              HC12.write("HX711 not found check load cell...");
            }
            // check pressure sensor
            // this needs fixed
            for(int i=0; i<10; ++i){
              int raw = analogRead(pressurePin);
              HC12.write(raw);
              Serial.print("raw:  ");
              Serial.println(raw);
              delay(10);
            }
           break;
         case '3': //LBV
          // opens properly at 130. tested.
          HC12.write(" opening LBV...");
          myservo.write(130);
          delay(150);
          myservo.write(130);
          delay(150);
          myservo.write(130);
          delay(2500);
          // 0 degrees is closed
          HC12.write("closing LBV..");
          myservo.write(0);
          delay(150);
          myservo.write(0);
          delay(150);
          myservo.write(0);
          delay(300);
          HC12.write("  done...");
          break;
        case '4': //STFV
          // close
          HC12.write(" opening STFV...");
          myservo.write(0);
          delay(150);
          myservo.write(0);
          delay(150);
          myservo.write(0);
          delay(2500);
          //open
          HC12.write("closing STFV..");
          myservo.write(90);
          delay(150);
          myservo.write(90);
          delay(150);
          myservo.write(90);
          delay(300);
          HC12.write("  done...");
          break;
      }
    
  }
}
