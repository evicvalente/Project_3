//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "entryway_light.h"
#include "light_sensor.h"

//=====[Defines]===============================================================


//=====[Declaration and initialization of public global objects]===============

DigitalIn driverSeat(D2);
DigitalIn passengerSeat(D3);
DigitalIn driverBelt(D4);
DigitalIn passengerBelt(D5);
DigitalIn ignitionButton(BUTTON1);

DigitalOut ignitionEnabledLED(LED1);
DigitalOut engineLED(LED2);


AnalogIn potentiometer(A0);

DigitalInOut sirenPin(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool driverState = OFF;
bool engineState = OFF; 
bool alarmON = OFF;
bool endPrint = OFF; 
bool tryAgain = OFF; 
bool engineOn = OFF;
bool ignitionLEDState = OFF; 

float potentiometerReading = 0.0;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();
void uartTask();
void ledActivation();


//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    //brightnessSensorInit();
    while (true) {
        uartTask();
        ledActivation();
        //lightBeamActivation();
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

void ledActivation() {

    if ( driverSeat && passengerSeat && driverBelt && passengerBelt ) {
        ignitionEnabledLED = ON;
    }


    if (ignitionEnabledLED && ignitionButton) {
        engineLED = ON;
        engineState = ON;
        ignitionEnabledLED = OFF;
        ignitionLEDState = ON;
    }


    if (engineLED){
        engineOn = ON; 
        ignitionLEDState = ON;
        sirenPin.input(); 
    }


    if (!ignitionEnabledLED && ignitionButton && alarmON == OFF && !ignitionLEDState) {
        sirenPin.output();
        sirenPin = LOW;
        alarmON = ON;
    }
}

void lightBeamActivation(){
    potentiometerReading = potentiometer.read();
    if (engineLED && potentiometerReading<0.3){
        leftBeamLamp = HIGH;
        rightBeamLamp = HIGH;
    }

    if (engineLED && potentiometerReading>0.3 && potentiometer.read()<0.6){
        leftBeamLamp = LOW;
        rightBeamLamp = LOW;
    }
    
    if (engineLED && potentiometerReading>0.3 && potentiometer.read()>0.6){
        lightUpdated();    
        }  
}


void uartTask(){
    if ( driverState == OFF && driverSeat ) {
        driverState = ON; 
        uartUsb.write( "Welcome to enhanced alarm system model 218-W24\r\n", 48 );
    }	

    if (ignitionEnabledLED == OFF && engineState ){
        engineState = OFF; 
        uartUsb.write( "Engine started\r\n", 16 );
        tryAgain = ON;
    }

    if (alarmON == ON && ignitionButton == ON && !endPrint){
        alarmON == OFF;
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


