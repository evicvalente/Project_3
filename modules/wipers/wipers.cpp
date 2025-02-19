//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"
#include "user_interface.h"  // For getWiperMode() and getDelaySetting()
#include "globals.h"
#include <cmath>             // For fabs()
#include "ThisThread.h"
#include <chrono>
using namespace std::chrono;

//---- Macro to provide wait_ms() functionality ----
#define wait_ms(x) ThisThread::sleep_for(milliseconds(x))

//=====[Declaration of private defines]========================================
#define DUTY_MIN 0.025f
#define DUTY_MAX 0.098f

//=====[Declaration and initialization of public global objects]=============
PwmOut servo(PF_9);

//=====[Enumerations]==========================================================
// Enumerated wiper settings
enum wipersetting {
    wiper_Off,
    wiper_Low,
    wiper_Hi,
    wiper_Int,
};

// Enumerated intermittent mode delays (in milliseconds)
enum IntMode {
    slow   = 8000,
    medium = 6000,
    fast   = 3000,
};

//=====[Declaration and initialization of public global variables]=============
wipersetting currentWiperSetting = wiper_Off;
int intDelay = slow;
float currentServoPosition = DUTY_MIN;

//=====[Custom String Compare Function]======================================

/**
 * @brief Compares two null-terminated strings.
 *
 * @param s1 First string.
 * @param s2 Second string.
 * @return An integer less than, equal to, or greater than zero if s1 is found,
 *         respectively, to be less than, to match, or be greater than s2.
 */
static int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
         s1++;
         s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

//=====[Function Prototypes]===================================================
void updateWiperSetting();
void updateIntSpeed();
void moveServoSmoothly(float targetPosition, int speedDelay);
void servo_move();
void wipersTask();

//=====[Implementations of public functions]===================================

/**
 * @brief Gradually moves the servo from the current position toward the target position.
 *
 * @param targetPosition The desired servo duty cycle.
 * @param speedDelay Delay (in ms) between steps.
 */
void moveServoSmoothly(float targetPosition, int speedDelay) {
    // Adjust in small steps until within a tolerance of 0.001
    while (fabs(currentServoPosition - targetPosition) > 0.001f) {
        if (targetPosition > currentServoPosition)
            currentServoPosition += 0.001f;
        else
            currentServoPosition -= 0.001f;
        servo = currentServoPosition;
        wait_ms(speedDelay);
    }
}

/**
 * @brief Moves the servo based on the current wiper setting.
 */
void servo_move() {
    // Use engineState (from globals.h) to determine if the engine is running.
    // If the engine is off, force the wipers off.
    if (engineState == OFF) {
        servo = DUTY_MIN;
        return;
    }
    
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
            wait_ms(intDelay);
            moveServoSmoothly(DUTY_MIN, 10);
            wait_ms(intDelay);
            break;
    }
}

/**
 * @brief Updates the wiper setting by reading the current mode from the user interface.
 */
void updateWiperSetting() {
    // If the engine is off, force the wiper setting to off.
    if (engineState == OFF) {
        currentWiperSetting = wiper_Off;
        return;
    }
    
    // Get the current mode from the user interface module.
    const char* modeStr = getWiperMode();
    
    if (my_strcmp(modeStr, "OFF") == 0) {
        currentWiperSetting = wiper_Off;
    } else if (my_strcmp(modeStr, "LO") == 0) {
        currentWiperSetting = wiper_Low;
    } else if (my_strcmp(modeStr, "HI") == 0) {
        currentWiperSetting = wiper_Hi;
    } else if (my_strcmp(modeStr, "INT") == 0) {
        currentWiperSetting = wiper_Int;
    }
    
    // Once the setting is updated, perform the servo movement.
    servo_move();
}

/**
 * @brief Updates the intermittent mode delay by reading the current delay setting from the user interface.
 */
void updateIntSpeed() {
    // Only update delay if the current mode is intermittent.
    if (currentWiperSetting != wiper_Int) return;
    
    const char* delayStr = getDelaySetting();
    
    if (my_strcmp(delayStr, "SHORT") == 0) {
        intDelay = slow;
    } else if (my_strcmp(delayStr, "MEDIUM") == 0) {
        intDelay = medium;
    } else if (my_strcmp(delayStr, "LONG") == 0) {
        intDelay = fast;
    }
}

/**
 * @brief Top-level function to update the wiper system.
 *
 * Call this function periodically from your main loop.
 */
void wipersTask() {
    updateIntSpeed();
    updateWiperSetting();
}
