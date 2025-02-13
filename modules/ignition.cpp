//=====[Libraries]=============================================================
#include "ignition.h"
#include "mbed.h"
#include "arm_book_lib.h"
#include "globals.h"

//=====[Declaration of private defines]========================================
// (No private defines for ignition)

//=====[Declaration of private data types]=====================================
// (No private data types for ignition)

//=====[Declaration and initialization of public global objects]===============
// (No new public global objects in this module)

//=====[Declaration of external public global variables]=======================
// (None)

//=====[Declaration and initialization of public global variables]=============
// (None)

//=====[Declaration and initialization of private global variables]============
// (None)

//=====[Declarations (prototypes) of private functions]========================
// (None)

//=====[Implementations of public functions]===================================
void ignitionTask(void)
{
    // Turn ignition LED on if all seat and belt sensors are active
    if ( driverSeat && passengerSeat && driverBelt && passengerBelt ) {
        ignitionEnabledLED = ON;
    }

    // If ignition is enabled and the ignition button is pressed,
    // start the engine (engine LED on) and disable ignition LED.
    if ( ignitionEnabledLED && ignitionButton ) {
        engineLED = ON;
        engineState = ON;
        ignitionEnabledLED = OFF;
        ignitionLEDState = ON;
    }

    // If the engine LED is on, update the engine state and configure the siren pin.
    if ( engineLED ) {
        engineOn = ON;
        ignitionLEDState = ON;
        sirenPin.input();
    }

    // If ignition is not enabled, but the ignition button is pressed and
    // no engine or alarm is active, then sound the buzzer.
    if ( !ignitionEnabledLED && ignitionButton && (alarmON == OFF) && !ignitionLEDState ) {
        sirenPin.output();
        sirenPin = LOW;
        alarmON = ON;
    }
}

//=====[Implementations of private functions]==================================
// (No private functions)