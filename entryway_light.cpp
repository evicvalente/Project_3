#include "mbed.h"
#include "arm_book_lib.h"

#include "entryway_light.h"
#include "light_sensor.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut brightnessSensor(LED1);
DigitalOut leftBeamLamp2(A3);
DigitalOut rightBeamLamp2(A4);


//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void brightnessSensorInit()
{
    brightnessSensor = OFF;
    leftBeamLamp2 = LOW;
    rightBeamLamp2 = LOW;
}

void lightUpdated(){
    if (lightSensorUpdate()<0.61){
        delay(500);
        leftBeamLamp2 = HIGH;
        rightBeamLamp2 = HIGH;
    }
    else if (lightSensorUpdate()>0.61){
        delay(1000);
        leftBeamLamp2 = LOW;
        rightBeamLamp2 = LOW;
    }

}
