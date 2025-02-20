//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"
#include "alarm.h" 
#include "display.h"
#include "user_interface.h"
#include "globals.h"
#include "wipers.h"

//=====[Defines]===============================================================
#define POTENTIOMETER_OVER_TEMP_LEVEL 50

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
    displayInit();          // Initialize the 2x20 display
    userInterfaceInit();    // Initialize the user interface module
    wipersInit();           // Initialize the wiper module (start servoTimer)

    while (true) {
        uartTask();
        alarmTask();    
        ignitionTask();
        userInterfaceUpdate();  // Update the user interface (wiper mode and delay)
        wipersTask();           // Non-blocking wiper update
    }
}

//=====[Implementations of public functions]===================================
void inputsInit()
{
    // Initialize digital inputs
    driverSeat.mode(PullDown);
    passengerSeat.mode(PullDown);
    driverBelt.mode(PullDown);
    passengerBelt.mode(PullDown);
    ignitionButton.mode(PullDown);
    
    // Initialize digital in/out
    sirenPin.mode(OpenDrain);
    sirenPin.input();
}

void outputsInit()
{
    // Set initial LED states
    ignitionEnabledLED = OFF;
    engineLED = OFF;
}

void uartTask() {
    static bool welcomeDisplayed = false;
    static bool engineStartedDisplayed = false;

    // Display welcome message once when driver sits down.
    if (!welcomeDisplayed && driverState == OFF && driverSeat) {
        driverState = ON;
        uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n", 48);
        welcomeDisplayed = true;
    }

    // When the engine is running, display "Engine started" only once.
    if (!engineStartedDisplayed && engineState == ON) {
        uartUsb.write("Engine started\r\n", 16);
        engineStartedDisplayed = true;
        tryAgain = ON;
    }

    // Reset the engine started flag if engine is off.
    if (engineState == OFF) {
        engineStartedDisplayed = false;
    }
}


float convertAnalog(float analogReading)
{
    return (analogReading * 3.3 / 0.01);
}

