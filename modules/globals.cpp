#include "globals.h"
#include "arm_book_lib.h"

// Digital inputs
DigitalIn driverSeat(D2);
DigitalIn passengerSeat(D3);
DigitalIn driverBelt(D4);
DigitalIn passengerBelt(D5);
DigitalIn ignitionButton(BUTTON1);

// Digital outputs
DigitalOut ignitionEnabledLED(LED1);
DigitalOut engineLED(LED2);

// Analog input
AnalogIn potentiometer(A0);

// Digital in/out
DigitalInOut sirenPin(PE_10);

// Serial communication
UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

// Global state variables
bool driverState      = OFF;
bool engineState      = OFF;
bool alarmON          = OFF;
bool endPrint         = OFF;
bool tryAgain         = OFF;
bool engineOn         = OFF;
bool ignitionLEDState = OFF;

// Other globals
float potentiometerReading = 0.0;
