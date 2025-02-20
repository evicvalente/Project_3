//=====[#include guards - begin]===============================================

#ifndef wipers_h
#define wipers_h

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================
void updateWiperSetting();
void updateIntSpeed();
void moveServoSmoothly(float targetPosition, int speedDelay);
void servo_move();
void wipersTask();

void wipersInit(void);
//=====[#include guards - end]=================================================

#endif // wipers_h