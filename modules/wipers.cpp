//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"
#include <ctime>

//=====[Declaration of private defines]========================================
#define DUTY_MIN 0.025
#define DUTY_MAX 0.098

//=====[Declaration and initialization of public global objects]===============
PwmOut servo(PF_9);
AnalogIn wiper_potentiometer1(A0);
AnalogIn wiper_potentiometer2(A1);

//=====[Enumerations]==========================================================

// Makes states for each of the wiper settings
enum wipersetting {
    wiper_Off,
    wiper_Hi,
    wiper_Low,
    wiper_Int,
};

// Makes states for the int mode delays 
enum IntMode {
    slow = 8000,
    medium = 6000,
    fast = 3000,
};

//=====[Declaration and initialization of public global variables]=============
wipersetting currentWiperSetting = wiper_Off;
int intDelay = slow;
float currentServoPosition = DUTY_MIN;

//=====[Function Prototypes]===================================================
void updateWiperSetting();
void updateIntSpeed();
void moveServoSmoothly(float targetPosition, int speedDelay);
void servo_move();

//=====[Implementations of public functions]===================================

void moveServoSmoothly(float targetPosition, int speedDelay) {
    while (currentServoPosition != targetPosition) {
        currentServoPosition += (targetPosition > currentServoPosition);
        servo = currentServoPosition;
        sleep_for(speedDelay);

    }
}



void servo_move() {
    if (checkenginestate() == OFF) {
        servo = DUTY_MIN;
        return;

    switch (currentWiperSetting) {
        case wiper_Off:
            servo = DUTY_MIN;
            break;
        
        case wiper_Hi:
            moveServoSmoothly(DUTY_MAX, 5);
            moveServoSmoothly(DUTY_MIN, 5);
            break;

        case wiper_Low:
            moveServoSmoothly(DUTY_MAX, 15);
            moveServoSmoothly(DUTY_MIN, 15);
            break;

        case wiper_Int:
            moveServoSmoothly(DUTY_MAX, 5);
            sleep_for(intDelay);
            moveServoSmoothly(DUTY_MIN, 10);
            sleep_for(intDelay);
            break;
    }
}

void updateWiperSetting() {
    if (checkenginestate() == OFF) {
        currentWiperSetting = wiper_Off;
        return;
    }

    float potValue1 = wiper_potentiometer1.read();

    if (potValue1 < 0.25) {
        currentWiperSetting = wiper_Off;
    }
    if (potValue1 < 0.5) {
        currentWiperSetting = wiper_Low;
    }
    if (potValue1 < 0.75) {
        currentWiperSetting = wiper_Hi;
    }
    else {
        currentWiperSetting = wiper_Int;
    }
    servo_move();
}

void updateIntSpeed() {
    float potValue2 = wiper_potentiometer2.read()

    if (potValue2 < 0.33) {
        intDelay = slow;
    }
    if (potValue2 < 0.66) {
        intDelay = medium;
    }
    else {
        intDelay = fast;
    }
}
