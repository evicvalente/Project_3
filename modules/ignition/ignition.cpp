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
    static bool prevButtonState = false;
    static int cycleCount = 0;  // Counts push events (rising edges)
    bool currentButtonState = ignitionButton;  // Read current button state

    // --- When engine is off, update the ignition enabled LED based on seat and belt sensors ---
    if (engineState == OFF) {
        if (driverSeat && passengerSeat && driverBelt && passengerBelt) {
            ignitionEnabledLED = ON;   // Green LED on when conditions met
        } else {
            ignitionEnabledLED = OFF;
        }
    }

    // --- Rising Edge: Button pressed ---
    if (!prevButtonState && currentButtonState) {
        cycleCount++;  // Count every press

        if (engineState == OFF) {
            if (ignitionEnabledLED) {
                // Conditions met: start engine on first push.
                engineLED = ON;           // Blue LED on
                engineState = ON;         // Engine running
                ignitionEnabledLED = OFF; // Turn off green LED
                ignitionLEDState = ON;
            } else {
                // Conditions NOT met: trigger alarm.
                if (alarmON == OFF) {
                    sirenPin.output();
                    sirenPin = LOW;
                    alarmON = ON;
                    // Optionally, send error messages via UART here.
                }
                // Reset cycleCount so that a failed start does not count as a push–release cycle.
                cycleCount = 0;
            }
        }
    }

    // --- Falling Edge: Button released ---
    if (prevButtonState && !currentButtonState) {
        // If the engine is running and this is the second complete push–release cycle, stop the engine.
        if ((engineState == ON) && (cycleCount == 2)) {
            engineLED = OFF;       // Turn off blue LED
            engineState = OFF;     // Mark engine as off
            engineOn = OFF;
            ignitionLEDState = OFF;
            cycleCount = 0;        // Reset for next cycle
        }
    }

    // Save the current button state for next iteration.
    prevButtonState = currentButtonState;
}


//=====[Implementations of private functions]==================================
// (No private functions)