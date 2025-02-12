//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"  // Only the ignition module is needed now

//=====[Defines]===============================================================
#define POTENTIOMETER_OVER_TEMP_LEVEL 50

//=====[Declaration and initialization of public global objects]===============
DigitalIn driverSeat(D2);
DigitalIn passengerSeat(D3);
DigitalIn driverBelt(D4);
DigitalIn passengerBelt(D5);
DigitalIn ignitionButton(BUTTON1);

DigitalOut ignitionEnabledLED(LED1);
DigitalOut engineLED(LED2);

// If these analog or digital objects are not used elsewhere, you might remove them too.
AnalogIn potentiometer(A0);

DigitalInOut sirenPin(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============
bool driverState      = OFF;
bool engineState      = OFF; 
bool alarmON          = OFF;
bool endPrint         = OFF; 
bool tryAgain         = OFF; 
bool engineOn         = OFF;
bool ignitionLEDState = OFF;

float potentiometerReading = 0.0;

//=====[Declarations (prototypes) of public functions]=========================
void inputsInit();
void outputsInit();
void uartTask();
float convertAnalog(float analogReading);

//=====[Main function, the program entry point after power on or reset]========
int main()
{
    inputsInit();
    outputsInit();
    // Removed brightnessSensorInit() because it's part of entryway_light

    while (true) {
        uartTask();
        ignitionTask();  // Handles the ignition logic
    }
}

//=====[Implementations of public functions]===================================
void inputsInit()
{
    driverSeat.mode(PullDown);
    passengerSeat.mode(PullDown);
    driverBelt.mode(PullDown);
    passengerBelt.mode(PullDown);
    ignitionButton.mode(PullDown);
    
    sirenPin.mode(OpenDrain);
    sirenPin.input();
}

void outputsInit()
{
    ignitionEnabledLED = OFF;
    engineLED = OFF;
}

void uartTask()
{
    if (driverState == OFF && driverSeat) {
        driverState = ON; 
        uartUsb.write("Welcome to enhanced alarm system model 218-W24\r\n", 48);
    }	

    if (ignitionEnabledLED == OFF && engineState) {
        engineState = OFF; 
        uartUsb.write("Engine started\r\n", 16);
        tryAgain = ON;
    }

    if (alarmON == ON && ignitionButton == ON && !endPrint) {
        alarmON = OFF;  // Corrected assignment from 'alarmON == OFF'
        uartUsb.write("Ignition inhibited\r\n", 20);

        if (driverSeat == OFF) {
            uartUsb.write("Driver seat is not occupied\r\n", 30);
        }

        if (passengerSeat == OFF) {
            uartUsb.write("Passenger seat is not occupied\r\n", 32);
        }

        if (driverBelt == OFF) {
            uartUsb.write("Driver belt is not fastened\r\n", 30);
        }

        if (passengerBelt == OFF) {
            uartUsb.write("Passenger belt is not fastened\r\n", 32);
        }
        endPrint = true;
        uartUsb.write("Try again!\r\n", 12);
    }
}

float convertAnalog(float analogReading)
{
    return (analogReading * 3.3 / 0.01);
}
