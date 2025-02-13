//=====[Libraries]=============================================================
#include "alarm.h"
#include "mbed.h"
#include "arm_book_lib.h"
#include "globals.h"

//=====[Implementations of public functions]===================================
void alarmTask(void)
{
    // If the alarm is active, the ignition button is pressed, and we haven't printed the alarm message
    if (alarmON == ON && ignitionButton == ON && !endPrint) {
        alarmON = OFF;  // Reset the alarm flag
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

//=====[Implementations of private functions]==================================
// (None)

