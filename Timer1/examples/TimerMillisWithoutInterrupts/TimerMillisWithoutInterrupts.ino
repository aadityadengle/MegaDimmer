//******************************************************************
// Copyright (c) 2013, Carlos Rafael Gimenes das Neves
// This projected is licensed under the terms of the FreeBSD
// License. See LICENSE.txt for more details.
//
// https://github.com/carlosrafaelgn/ArduinoTimer
//
// ATmega168, ATmega328:
// - Using Timer 1 disables PWM (analogWrite) on pins 9 and 10
// ATmega2560:
// - Using Timer 1 disables PWM (analogWrite) on pins 11 and 12
// - Using Timer 3 disables PWM (analogWrite) on pins 2, 3 and 5
// - Using Timer 4 disables PWM (analogWrite) on pins 6, 7 and 8
// - Using Timer 5 disables PWM (analogWrite) on pins 44, 45 and 46
//******************************************************************

#include <Timer1.h>

unsigned short lastTickCount;
unsigned short myMicros;
unsigned long myMillis;

void setup()
{
  // Disable Arduino's default millisecond counter (from now on, millis(), micros(),
  // delay() and delayMicroseconds() will not work)
  disableMillis();
  
  // Prepare Timer1 to count
  // On 16 MHz Arduino boards, this function has a resolution of 4us
  // On 8 MHz Arduino boards, this function has a resolution of 8us
  startCountingTimer1();
  
  // Initialize our counters
  lastTickCount = readTimer1();
  myMicros = 0;
  myMillis = 0;
  
  Serial.begin(9600);
}

void loop()
{
  // readTimer1() returns a maximum value of 65535
  // That means the maximum possible delta one can measure with this
  // function is 262ms on 16 MHz boards, and 524ms on 8 MHz boards
  unsigned short currentTickCount = readTimer1();
  unsigned short delta = currentTickCount - lastTickCount;
  lastTickCount = currentTickCount;
  
  // If you estimate deltaMicros could be > 65 ms, or 65535 us,
  // delta should be cast to unsigned long, and deltaMicros should be
  // created as an unsigned long variable
  // For example: unsigned long deltaMicros = microsFromCounting((unsigned long)delta);
  unsigned short deltaMicros = microsFromCounting(delta);
  
  // Update our micro and millis counters
  myMicros += deltaMicros;
  while (myMicros >= 1000)
  {
    myMicros -= 1000;
    myMillis++;
  }
  
  // Do your work here
  
  Serial.println(myMillis);
}
