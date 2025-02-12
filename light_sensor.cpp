//=====[Libraries]=============================================================

#include "mbed.h"

#include "light_sensor.h"

//=====[Declaration of private defines]========================================

#define LIGHT_SENSOR_NUMBER_OF_AVG_SAMPLES    10

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

AnalogIn lightSensor(A2);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

float lightReadingsArray[LIGHT_SENSOR_NUMBER_OF_AVG_SAMPLES];

// static float averageLight( float analogReading );

//=====[Implementations of public functions]===================================

void lightSensorInit()
{
    int i;
    
    for( i=0; i < LIGHT_SENSOR_NUMBER_OF_AVG_SAMPLES ; i++ ) {
        lightReadingsArray[i] = 0;
    }
}

float lightSensorUpdate()
{
    static int lightSampleIndex = 0;
    float lightReadingsSum = 0.0;
    float lightReadingsAverage = 0.0;

    int i = 0;

    lightReadingsArray[lightSampleIndex] = lightSensor.read();
       lightSampleIndex++;
    if ( lightSampleIndex >= LIGHT_SENSOR_NUMBER_OF_AVG_SAMPLES) {
        lightSampleIndex = 0;
    }
    
   lightReadingsSum = 0.0;
    for (i = 0; i < LIGHT_SENSOR_NUMBER_OF_AVG_SAMPLES; i++) {
        lightReadingsSum = lightReadingsSum + lightReadingsArray[i];
    }
    lightReadingsAverage = lightReadingsSum / LIGHT_SENSOR_NUMBER_OF_AVG_SAMPLES;

    return lightReadingsAverage;
}
