//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "user_interface.h"
#include "globals.h"    // For shared globals (e.g., potentiometer)
#include "display.h"
#include <chrono>
using namespace std::chrono;

//=====[Declaration of private defines]========================================
#define UI_UPDATE_INTERVAL_MS 1000  // Refresh interval for the display

//=====[Declaration and initialization of private global variables]============
static char currentWiperMode[5] = "OFF";     // Valid modes: "OFF", "LO", "INT", "HI"
static char currentDelaySetting[8] = "MEDIUM"; // Valid settings: "SHORT", "MEDIUM", "LONG"

// Analog input for the intermittent delay time selector (assumed on A1)
static AnalogIn delaySelector(A1);

// Timer for periodic UI updates
static Timer uiTimer;

//=====[Declarations (prototypes) of private functions]========================
static void updateWiperMode(void);
static void updateDelaySetting(void);
static void updateDisplay(void);

// Custom string copy function
static void ui_strcpy(char *dest, const char *src) {
    while((*dest++ = *src++));
}

// Custom string compare function
static int ui_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
         s1++;
         s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

//=====[Implementations of public functions]===================================

void userInterfaceInit(void)
{
    // Set initial default values using our custom functions.
    ui_strcpy(currentWiperMode, "OFF");
    ui_strcpy(currentDelaySetting, "MEDIUM");
    
    // Start the UI timer.
    uiTimer.start();
}

void userInterfaceUpdate(void)
{
    // Update wiper mode based on the potentiometer (shared global 'potentiometer' from globals.h)
    updateWiperMode();
    
    // If the mode is "INT" (intermittent), update the delay setting from delaySelector.
    if (ui_strcmp(currentWiperMode, "INT") == 0) {
        updateDelaySetting();
    }
    
    // Update the LCD display with the current mode (and delay if applicable).
    updateDisplay();
}

const char* getWiperMode(void)
{
    return currentWiperMode;
}

const char* getDelaySetting(void)
{
    return currentDelaySetting;
}

//=====[Implementations of private functions]==================================

// Reads the potentiometer value (0.0 to 1.0) and maps it to a wiper mode.
static void updateWiperMode(void)
{
    float value = potentiometer.read();
    
    if (value < 0.25f) {
        ui_strcpy(currentWiperMode, "OFF");
    } else if (value < 0.5f) {
        ui_strcpy(currentWiperMode, "LO");
    } else if (value < 0.75f) {
        ui_strcpy(currentWiperMode, "INT");
    } else {
        ui_strcpy(currentWiperMode, "HI");
    }
}

// Reads the delay selector value (0.0 to 1.0) and maps it to a delay setting.
static void updateDelaySetting(void)
{
    float value = delaySelector.read();
    
    if (value < 0.33f) {
        ui_strcpy(currentDelaySetting, "SHORT");
    } else if (value < 0.67f) {
        ui_strcpy(currentDelaySetting, "MEDIUM");
    } else {
        ui_strcpy(currentDelaySetting, "LONG");
    }
}

// Updates the LCD display with the current wiper mode and, if applicable, the delay setting.
static void updateDisplay(void)
{
    // Use the Chrono-based elapsed_time method instead of read_ms()
    if (duration_cast<milliseconds>(uiTimer.elapsed_time()).count() >= UI_UPDATE_INTERVAL_MS) {
        uiTimer.reset();
        // displayWiperMode() is assumed to clear the display and then:
        // - On line 0: "Wiper Mode: <mode>"
        // - On line 1: "Delay: <delay>" if mode is "INT"
        displayWiperMode(currentWiperMode, currentDelaySetting);
    }
}
