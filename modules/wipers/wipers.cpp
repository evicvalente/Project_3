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

//=====[Declaration of private defines]========================================
#define DUTY_MIN 0.025f
#define DUTY_MAX 0.098f
#define SERVO_STEP 0.001f      // Servo step increment

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

// Non-blocking servo motion state machine states
enum WiperMotionState {
    SERVO_IDLE,
    SERVO_MOVING_UP,
    SERVO_MOVING_DOWN,
    SERVO_DELAY
};

// Enumerated intermittent mode delays (in milliseconds)
enum IntMode {
    slow   = 8000,
    medium = 6000,
    fast   = 3000,
};

//=====[Declaration and initialization of public global variables]=============
wipersetting currentWiperSetting = wiper_Off;
int intDelay = slow;                // Default intermittent delay
float currentServoPosition = DUTY_MIN;

//=====[Non-blocking State Machine Variables]==================================
static WiperMotionState servoMotionState = SERVO_IDLE; // Declare static variables used for non-blocking servo updates.
static Timer servoTimer;  // Timer to schedule incremental updates

// New initialization function for the wiper module.
void wipersInit(void) {
    servoTimer.start();
}

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
void updateServoMotion();  // Non-blocking servo update function
void wipersTask();

//=====[Non-blocking wait macro]==============================================
#define WAIT_MS(x)  /* Removed blocking wait */

//=====[Implementations of public functions]===================================

/**
 * @brief Non-blocking servo update function.
 *
 * This function is called periodically from wipersTask(). It checks if a fixed
 * time interval has elapsed, then increments or decrements the servo position
 * based on the current motion state.
 */
void updateServoMotion() {
    // Define a step interval (in milliseconds) for each servo update.
    const int stepIntervalMs = 5;
    
    // Use a static local variable to check elapsed time.
    if (servoTimer.elapsed_time().count() < stepIntervalMs * 1000000) { // convert ms to ns
        return;
    }
    servoTimer.reset();
    
    switch(servoMotionState) {
        case SERVO_IDLE:
            // Do nothing in idle state.
            break;
            
        case SERVO_MOVING_UP:
            currentServoPosition += SERVO_STEP;
            if (currentServoPosition >= DUTY_MAX) {
                currentServoPosition = DUTY_MAX;
                // For intermittent mode, transition to delay; otherwise, switch direction.
                if (currentWiperSetting == wiper_Int) {
                    servoMotionState = SERVO_DELAY;
                } else {
                    servoMotionState = SERVO_MOVING_DOWN;
                }
            }
            servo = currentServoPosition;
            break;
            
        case SERVO_MOVING_DOWN:
            currentServoPosition -= SERVO_STEP;
            if (currentServoPosition <= DUTY_MIN) {
                currentServoPosition = DUTY_MIN;
                // For intermittent mode, transition to delay; otherwise, switch direction.
                if (currentWiperSetting == wiper_Int) {
                    servoMotionState = SERVO_DELAY;
                } else {
                    servoMotionState = SERVO_MOVING_UP;
                }
            }
            servo = currentServoPosition;
            break;
            
        case SERVO_DELAY:
            // In delay state, wait until the intermittent delay has elapsed.
            if (servoTimer.elapsed_time().count() >= intDelay * 1000000) { // intDelay in ms -> ns
                // After delay, resume movement.
                // Decide direction based on current position:
                if (fabs(currentServoPosition - DUTY_MAX) < 0.001f) {
                    servoMotionState = SERVO_MOVING_DOWN;
                } else if (fabs(currentServoPosition - DUTY_MIN) < 0.001f) {
                    servoMotionState = SERVO_MOVING_UP;
                } else {
                    // Default to moving down.
                    servoMotionState = SERVO_MOVING_DOWN;
                }
                servoTimer.reset();
            }
            break;
    }
}

/**
 * @brief Updates the wiper setting by reading the current mode from the user interface.
 *
 * Sets the currentWiperSetting and adjusts the servo motion state accordingly.
 */
void updateWiperSetting() {
    // If the engine is off, force wipers off.
    if (engineState == OFF) {
        currentWiperSetting = wiper_Off;
        servoMotionState = SERVO_IDLE;
        servo = DUTY_MIN;
        return;
    }
    
    // Get the current mode from the user interface.
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
    
    // Based on the current setting, set the servo motion state.
    switch (currentWiperSetting) {
        case wiper_Off:
            servoMotionState = SERVO_IDLE;
            servo = DUTY_MIN;
            break;
        case wiper_Low:
            servoMotionState = SERVO_MOVING_UP;
            break;
        case wiper_Hi:
            servoMotionState = SERVO_MOVING_UP;
            break;
        case wiper_Int:
            servoMotionState = SERVO_MOVING_UP;
            break;
    }
}

/**
 * @brief Updates the intermittent mode delay by reading the current delay setting from the user interface.
 */
void updateIntSpeed() {
    // Only update if current mode is intermittent.
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
 * @brief Top-level function to update the wiper system in a non-blocking manner.
 *
 * This function should be called periodically from the main loop.
 */
void wipersTask() {
    // Update the intermittent delay if needed.
    updateIntSpeed();
    // Update the wiper setting from the user interface.
    updateWiperSetting();
    // Update servo motion (non-blocking).
    updateServoMotion();
}

