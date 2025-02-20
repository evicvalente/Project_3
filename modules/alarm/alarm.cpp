//=====[Libraries]=============================================================
#include "alarm.h"
#include "mbed.h"
#include "arm_book_lib.h"
#include "globals.h"

//=====[Implementations of public functions]===================================
void alarmTask(void)
{
    static bool alarmMessageDisplayed = false;
    
    // Only process if the alarm is active.
    if (alarmON == ON) {
        // If all required conditions are met AND the ignition button is pressed,
        // clear the alarm (i.e. allow a new start attempt).
        if (driverSeat && passengerSeat && driverBelt && passengerBelt && ignitionButton) {
            alarmON = OFF;
            endPrint = false;
            alarmMessageDisplayed = false;
            sirenPin.input();  // Stop the buzzer
        }
        // Otherwise, if the ignition button is pressed and the alarm message hasn't been displayed,
        // display the error messages.
        else if (ignitionButton && !alarmMessageDisplayed) {
            uartUsb.write("Ignition inhibited\r\n", 20);
            if (!driverSeat) {
                uartUsb.write("Driver seat is not occupied\r\n", 30);
            }
            if (!passengerSeat) {
                uartUsb.write("Passenger seat is not occupied\r\n", 32);
            }
            if (!driverBelt) {
                uartUsb.write("Driver belt is not fastened\r\n", 30);
            }
            if (!passengerBelt) {
                uartUsb.write("Passenger belt is not fastened\r\n", 32);
            }
            uartUsb.write("Try again!\r\n", 12);
            alarmMessageDisplayed = true;
        }
    }
}



//=====[Implementations of private functions]==================================
// (None)
