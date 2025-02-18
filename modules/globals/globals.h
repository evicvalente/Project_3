#ifndef GLOBALS_H
#define GLOBALS_H

#include "mbed.h"

// Digital inputs
extern DigitalIn driverSeat;
extern DigitalIn passengerSeat;
extern DigitalIn driverBelt;
extern DigitalIn passengerBelt;
extern DigitalIn ignitionButton;

// Digital outputs
extern DigitalOut ignitionEnabledLED;
extern DigitalOut engineLED;

// Analog input
extern AnalogIn potentiometer;

// Digital in/out
extern DigitalInOut sirenPin;

// Serial communication
extern UnbufferedSerial uartUsb;

// Global state variables
extern bool driverState;
extern bool engineState;
extern bool alarmON;
extern bool endPrint;
extern bool tryAgain;
extern bool engineOn;
extern bool ignitionLEDState;

// Other globals
extern float potentiometerReading;

#endif // GLOBALS_H

