#include "globals.h"
#include "arm_book_lib.h"

// Digital inputs
DigitalIn driverSeat(D12);
DigitalIn passengerSeat(D13);
DigitalIn driverBelt(D10);
DigitalIn passengerBelt(D11);
DigitalIn ignitionButton(BUTTON1);

// Digital outputs
DigitalOut ignitionEnabledLED(LED1);
DigitalOut engineLED(LED2);

// Analog input
AnalogIn potentiometer1(A0);
AnalogIn potentiometer2(A1);

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
