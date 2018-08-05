//******************************************************************
// Copyright (c) 2018, Aaditya Dengle, Pune
// This projected is licensed under the terms of the FreeBSD
// License. See LICENSE.txt for more details.
//
// https://github.com/carlosrafaelgn/ArduinoTimer
//
// This Firmware has been developed for controlling the Heaters (About 9KW) on a 220V(rms) 50Hz 3P Y connection Suply.
// Though the system is intended to use with 3 phase supply, one can do the changes and use the system for any other configuration.
// Please refer to readme for more details.
//******************************************************************

#include <Timer1.h>
#include <Timer3.h>
#include <Timer4.h>


#define SYS_PHASE 3                             // Number of phases connected to the System (usually Single Phase(1) or Three Phase(3))

#define AC_FREQ 50                              // AC Line frequency (in India 50Hz, in US 60Hz)
#define AC_PERIOD 1000000/(AC_FREQ*2)           // The time taken from one zero crosssing to the next. The polarity is not important. 

#define MIN_LOWTIME AC_PERIOD*1/100             //Minimum microseconds to keep the output low, any input value bellow this will keep the output always low 
#define MAX_HIGHTIME AC_PERIOD*99/100           //Maximum microseconds to keep the output high, any input value above this will keep the output always high

#if SYS_PHASE == 1
#define ZCPIN_R 21                              // The INT0 pin on AVR (Digital Pin 21 on MEGA2560, Change according to your board or setup), R Phase Zero Crossing Detector input goes here.
#define ZCI_R digitalPinToInterrupt(ZCPIN_R)    // The interrupt number for INT0
#define OCI_R 37                                // R Phase Optocoupler control Pin
#endif

#if SYS_PHASE == 2
#define ZCPIN_R 21                                    // The INT0 pin on AVR (Digital Pin 21 on MEGA2560, Change according to your board or setup), R Phase Zero Crossing Detector input goes here.
#define ZCPIN_Y 20                                    // The INT1 pin on AVR (Digital Pin 20 on MEGA2560, Change according to your board or setup), Y Phase  Zero Crossing Detector input goes here.
#define ZCI_R digitalPinToInterrupt(ZCPIN_R)          // The interrupt number for INT0
#define ZCI_Y digitalPinToInterrupt(ZCPIN_Y)          // The interrupt number for INT1
#define OCI_R 37                                      // R Phase Optocoupler control Pin
#define OCI_Y 36                                      // Y Phase Optocoupler control Pin
#endif

#if SYS_PHASE == 3
#define ZCPIN_R 21                                    // The INT0 pin on AVR (Digital Pin 21 on MEGA2560, Change according to your board or setup), R Phase  Zero Crossing Detector input goes here.
#define ZCPIN_Y 20                                    // The INT1 pin on AVR (Digital Pin 20 on MEGA2560, Change according to your board or setup), Y Phase  Zero Crossing Detector input goes here.
#define ZCPIN_B 19                                    // The INT2 pin on AVR (Digital Pin 19 on MEGA2560, Change according to your board or setup), B Phase  Zero Crossing Detector input goes here.
#define ZCI_R digitalPinToInterrupt(ZCPIN_R)          // The interrupt number for INT0
#define ZCI_Y digitalPinToInterrupt(ZCPIN_Y)          // The interrupt number for INT1
#define ZCI_B digitalPinToInterrupt(ZCPIN_B)          // The interrupt number for INT2
#define OCI_R 37                                      // R Phase Optocoupler control Pin
#define OCI_Y 36                                      // Y Phase Optocoupler control Pin
#define OCI_B 35                                      // B Phase Optocoupler control Pin
#endif


#ifdef ZCPIN_R  
int delay_R = 50;                                     //microninc default delay to grnerate HIGH Pulse
#endif
#ifdef ZCPIN_Y 
int delay_Y = 50;
#endif
#ifdef ZCPIN_B
int delay_B = 50;
#endif

String cmdStr = "";                                   //Controll String Input Buffer
boolean cmdFlg = false;                               //Serial Receive Compleate Flag

void setup() {
  Serial.begin(57600); //Serial communication for setting up the dimming value
  disableMillis();
  #ifdef ZCPIN_R 
  pinMode(ZCPIN_R,INPUT_PULLUP);
  attachInterrupt(ZCI_R,ISR_ZCR,RISING);  
  pinMode(OCI_R, OUTPUT);
  
  #endif
  #ifdef ZCPIN_Y 
  pinMode(ZCPIN_Y,INPUT_PULLUP);
  attachInterrupt(ZCI_Y,ISR_ZCY,RISING);  
  pinMode(OCI_Y, OUTPUT);
 
  #endif
  #ifdef ZCPIN_B 
  pinMode(ZCPIN_B,INPUT_PULLUP);
  attachInterrupt(ZCI_B,ISR_ZCB,RISING);  
  pinMode(OCI_B, OUTPUT);
 
  #endif
  disableMillis();
 
}

void loop() {
 if (cmdFlg) {
    if(cmdStr.startsWith("R")){
      #ifdef ZCPIN_R
      delay_R = map(cmdStr.substring(1).toInt(), 0, 100, MIN_LOWTIME, MAX_HIGHTIME);
      #endif
    }else if(cmdStr.startsWith("Y")){
        #ifdef ZCPIN_Y 
        delay_Y = map(cmdStr.substring(1).toInt(), 0, 100, MIN_LOWTIME, MAX_HIGHTIME);
        #endif
      }else if(cmdStr.startsWith("B")){
          #ifdef ZCPIN_B
          delay_B =map(cmdStr.substring(1).toInt(), 0, 100, MIN_LOWTIME, MAX_HIGHTIME);
          #endif
        }else{
          Serial.print("Input Error!");
        }
      }
    

    
    // clear the string:
    cmdStr = "";
    cmdFlg = false;
  
}
#ifdef ZCPIN_R
void ISR_ZCR(){
 PORTC &= ~_BV(PC0);
startTimer1(AC_PERIOD - delay_R );
  
   
}


ISR(timer1Event){
 resetTimer1();
   PORTC |= _BV(PC0);
  pauseTimer1();
 
}
#endif

#ifdef ZCPIN_Y
void ISR_ZCY(){
 PORTC &= ~_BV(PC1);
startTimer3(AC_PERIOD - delay_R );
  
}

ISR(timer3Event){
 resetTimer3();
   PORTC |= _BV(PC1);
  pauseTimer3();
 
}
#endif

#ifdef ZCPIN_B
void ISR_ZCB(){
 PORTC &= ~_BV(PC2);
startTimer4(AC_PERIOD - delay_B );
  
   
}


ISR(timer4Event){
 resetTimer4();
   PORTC |= _BV(PC2);
  pauseTimer4();
 
}
void serialEvent() {
  //Excpecting 0-100 from the input to end with new line
  while (Serial.available()) {
    char iptChr = (char)Serial.read();
    cmdStr += iptChr;
    if (iptChr == '\n') {
     cmdFlg= true;
    }
  }
}
#endif
