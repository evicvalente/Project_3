//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"
#include "ignition.h"
#include <ctime>

//=====[Declaration of private defines]========================================
#define DUTY_MIN
#define DUTY_MAX 
#define HI_PERIOD 
#define LOW_PERIOD
#define Time_Increment 2000 // adjust as needed 

//=====[Declaration of private data types]=====================================


//=====[Declaration and initialization of public global objects]===============
PwmOut servo(PF_9);
AnalogIn wiper_potentiometer1(A0);
AnalogIn wiper_potentiometer2(A1);

//=====[Declaration of external public global variables]=======================


//=====[Declaration and initialization of public global variables]=============
float potentiometerReading = 0.0;
float Set_Duty_Cycle = DUTY_MIN;

//=====[Declaration and initialization of private global variables]============


//=====[Declarations (prototypes) of private functions]========================


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
    slow,
    medium,
    fast,
};

//=====[Implementations of public functions]===================================
void servo_move() {
    if (wipersetting = wiper_Off) {
        return;
    }
    if (wipersetting = wiper_High) {
        servo = //equation to control speed

    }
    if (wipersetting = wiper_Low) {
        servo = //equation to control speed

    }
    if (wipersetting = wiper_Int) {
        ???
    }
}












void change_wiper_state() {
if (checkenginestate() == ON) {
switch (wipersetting) {
    case wiper_Off:
    return;

    case wiper_Hi:
    // wipers on at full speed 


    case wiper_Low:
    // wipers on low speed 


    case wiper_Int:
        switch (IntMode) {
            case slow: // 8 second delay
            // wipers move at full speed with 8 seconds between cycles

            case medium: // 6 second delay
            // wipers move at full speed with 6 seconds between cycles

            case fast: // 8 second delay
            // wipers move at full speed with 3 seconds between cycles


        }
}

}
}

