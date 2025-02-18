//=====[#include guards - begin]===============================================

#ifndef _IGNITION_H_
#define _IGNITION_H_

//=====[Declaration of public defines]=========================================
// (No ignition-specific defines)

//=====[Declaration of public data types]======================================
// (No ignition-specific data types)

//=====[Declarations (prototypes) of public functions]=========================
void updateWiperSetting();
void updateIntSpeed();
void moveServoSmoothly(float targetPosition, int speedDelay);
void servo_move();

//=====[#include guards - end]=================================================

#endif // _IGNITION_H_
