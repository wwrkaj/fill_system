/*
 * this is the program to run on the 'slave' arduino
 * 
 * to use connect to blue usb from master arduino and open serial monitor
 * 
 * enter the desired program number in the serial monitor
 * 
 * Connect HC12 "RXD" pin to Arduino Digital Pin 4
 * Connect HC12 "TXD" pin to Arduino Digital Pin 5
 * Connect HC12 "Set" pin to Arduino Digital Pin 6
 * 
 * By Josh Wewerka
 * 
 * Program descriptions:
    Program 0) Check System 
      Checks the status of all sensors and valves, return data 
      Sweeps valves open to close, return data
      End program
    Program 1) Fill 
      Open FTFV verify status, return data
      Open STFV verify status, return data
      Wait until run tank reaches above 0.6kg (+-.25kg) and above 600psi, return data loop
      Close FTFV verify status, return data
      Close STFV verify status, return data
      End program
    Program 2) Open Run Valve
      Enter arming mode wait for arm command, return data
      Arming command received wait for start command, return data
      Start command received open RV check for stop command, return data
      If stop command received close RV, return data
      End program
    Program a) Abort 
      Close FTFV and RV, return data
      Open LBV and FTFV, return data loop
      If pressure == 0 or end command received close valves, return data
      End program
    Program r) Report
      Return data only 
      End program
*/
#include "HX711.h"
// make sure you add the hx711 library by Bogdan Necula
// hx711 can be found by searching in the manage libraries under the tools menu

#include <Servo.h>
#include <SoftwareSerial.h>

const byte HC12RxdPin = 4;                  // Receive Pin on HC12
const byte HC12TxdPin = 5;                  // Transmit Pin on HC12

const int pressurePin = A0;   // input pin for pressure tansducer 

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;
char arm_value = 'n';
HX711 scale;

SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port
Servo LBV;  // create LBV servo object to control a servo pin 7
Servo STFV; // create STFV servo object                   pin 8 
Servo FTFV; // create FTFV servo object                   pin 9
Servo RV;   // create RV servo object                     pin 10 
// twelve servo objects can be created on most boards


void setup() {
  pinMode(pressurePin, INPUT);  // set up analog pin for pressure sensor
  LBV.attach(7);  // attaches the LBV servo on pin 7 to the servo object
  STFV.attach(8); // attach STFV servo to pin 8
  FTFV.attach(9); // attach FTFV servo to pin 9
  RV.attach(10);  // attach RV servo to pin 10 
   
  Serial.begin(9600);   // open serial port for computer
  HC12.begin(9600);     // open serial port to HC12
  delay(2000);          // wait for a serial ports to fully setup
  HC12.write(" Do you want to arm the Fill System? enter 'y' to arm... ");
  arm_value = HC12.read();
  while(arm_value!='y'){
    arm_value = HC12.read();
    //wait for y
    delay(500);
  }
          closeLBV();
          delay(1000);
          closeSTFV();
          delay(1000);
          closeFTFV();
          delay(1000);
          closeRV();
          delay(100);
  HC12.write(" System armed, Enter Command: ");
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  //set up load cell communications
}

void loop() {
  if(HC12.available()>0){
    int program = HC12.read();
      switch(program){
        case '0':   // program zero = default close valve
                    // good for both savox valves!
          openLBV();
          delay(1000);
          openSTFV();
          delay(1000);
          openFTFV();
          delay(1000);
          openRV();
          break;
        case '1':   //program one = open
                    // good for both savox values FTFV & RV
          closeLBV();
          delay(1000);
          closeSTFV();
          delay(1000);
          closeFTFV();
          delay(1000);
          closeRV();
          break;
        case '2': //program two = get load cell data
          
            // check pressure sensor
            // this needs fixed
            // ambient raw 100~98 = 0psi
            // 211 = 144psi
            // 208 = 138psi
            // 206 = 136psi
            for(int i=0; i<10; ++i){
              byte raw = analogRead(pressurePin);
              delay(100);
              HC12.print(" raw: ");
              HC12.println(raw);
              
              Serial.print("raw:  ");
              Serial.println(raw);
              delay(100);
            }
           break;
         case '3': // LBV (should also be used to test STFV!)
          openRV();
          break;
        case '4': //STFV
          closeRV();
          break;
      }
    
  }
}

// all funtion will be place below the loop here

void openLBV(){
  HC12.write(" opening LBV...");
          LBV.write(130);
          delay(150);
          LBV.write(130);
          delay(150);
          LBV.write(130);
          delay(150);
          LBV.write(130);
          delay(150);
          LBV.write(130);
          delay(150);
          LBV.write(130);
          delay(150);
          HC12.write(" LBV open... ");
}
void closeLBV(){
  HC12.write(" closing LBV..");
          LBV.write(0);
          delay(150);
          LBV.write(0);
          delay(150);
          LBV.write(0);
          delay(150);
          LBV.write(0);
          delay(150);
          LBV.write(0);
          delay(150);
          LBV.write(0);
          delay(150);
          HC12.write(" LBV closed...");
}
void openSTFV(){
  HC12.write(" opening STFV...");
          STFV.write(140);
          delay(150);
          STFV.write(140);
          delay(150);
          STFV.write(140);
          delay(150);
          STFV.write(140);
          delay(150);
          STFV.write(140);
          delay(150);
          STFV.write(140);
          delay(150);
          HC12.write(" STFV open... ");
}
void closeSTFV(){
  HC12.write(" closing STFV..");
          STFV.write(0);
          delay(150);
          STFV.write(0);
          delay(150);
          STFV.write(0);
          delay(150);
          STFV.write(0);
          delay(150);
          STFV.write(0);
          delay(150);
          STFV.write(0);
          delay(150);
          HC12.write(" STFV closed...");
}
void openFTFV(){
  HC12.write(" opening FTFV...");
          FTFV.write(100);       // 100 degrees = full open
          delay(150);
          FTFV.write(100);
          delay(150);
          FTFV.write(100);
          delay(150);
          FTFV.write(100);
          delay(150);
          FTFV.write(100);
          delay(150);
          FTFV.write(100);
          delay(150);
          HC12.write(" FTFV open... ");
}
void closeFTFV(){
  HC12.write(" closing FTFV...");
          FTFV.write(10);       // 10 degrees = closed
          delay(150);
          FTFV.write(10);
          delay(150);
          FTFV.write(10);
          delay(150);
          FTFV.write(10);
          delay(150);
          FTFV.write(10);
          delay(150);
          FTFV.write(10);
          delay(150);
          HC12.write(" FTFV closed..."); 
}
void openRV(){
  HC12.write(" opening RV...");
          RV.write(100);       // 100 degrees = full open
          delay(150);
          RV.write(100);
          delay(150);
          RV.write(100);
          delay(150);
          RV.write(100);
          delay(150);
          RV.write(100);
          delay(150);
          RV.write(100);
          delay(150);
          HC12.write(" RV open... ");
}
void closeRV(){
  HC12.write(" closing RV...");
          RV.write(10);       // 10 degrees = closed
          delay(150);
          RV.write(10);
          delay(150);
          RV.write(10);       // 10 degrees = closed
          delay(150);
          RV.write(10);
          delay(150);
          RV.write(10);       // 10 degrees = closed
          delay(150);
          RV.write(10);
          delay(150);
          HC12.write(" RV closed..."); 
}
