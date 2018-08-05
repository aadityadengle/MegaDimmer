# MegaDimmer
An AC line dimmer firmware on ATMega2560 for 220Vrms 50Hz 3P Y Supply. (Firmware only)

This Firmware has been developed for controlling the Heaters (About 9KW) on a 220V(rms) 50Hz 3P Y connection Suply.
Though the system is intended to use with 3 phase supply, one can do the changes and use the system for any other configuration.
This firmware can be used for controlling any resistive load. Motors to be tested yet.  

1. System Parameters
1.1 SYS_PHASE: Number of connected phases. (1 = Single Phase; 2 = Two Phase; 3 = Three Phase)
1.2 AC_FREQ : Supply Frequency (e.g. India = 50 Hz, US = 60 Hz)

2. System Supply Conditions
2.1 Frequency needs to be setted correctly. 
2.2 The AC supply needs to be in Y connection.
2.3 Supply Voltage dose not matter.

3. Hardware Requirement:
3.1 Schematics shall be uploaded sooner. Till then, you will require, zero crossing detector for each phase, triac optocoupler for each phase, a ATMega2560 with supporting hardware, AND KNOWDGE ABOUT WHAT YOU ARE DOING.

4. Connections:
4.1 All Zero Crossing inputs (normally sinking) to be given to PD0, PD1, PD2 for R,Y,B phases respectively. (on Mega2560 these are Pin 21, 20,19 respectively)
4.2 The triac controlling pulses shall appear on PC0, PC1, PC2 pins. (on Mega2560 these are Pin 37,36,35 respectively)
4.3 5V DC supply for the chip. if you have plan to use same supply for the rest of the components then min. 3A supply require.

5. Controlling.
5.1 0 is dark and 99 is bright.
5.2 Pass the "Xn" with '/n' on serial to controll the dimming. X is the phase name (i.e. R/Y/B); n is the number between (0 to 99).
5.3 Any other input will return Input error and nothing will change.
5.4 Should use Serial0 (RXD0/TXD0) only.

6. Thanks to:
   Carlos Rafael Gimenes das Neves for his Timing Library (https://github.com/carlosrafaelgn/ArduinoTimer)
   DXARTS for the hardware references (https://dxarts.washington.edu/wiki/ac-dimmer-circuit)
