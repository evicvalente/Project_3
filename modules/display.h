//=====[#include guards - begin]=============================================
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "mbed.h"

//=====[Declaration of public function prototypes]=============================
/**
 * @brief Initializes the 2x20 LCD display.
 */
void displayInit(void);

/**
 * @brief Clears the display.
 */
void displayClear(void);

/**
 * @brief Sets the cursor to the given (column, row) position.
 *
 * For a 20x2 display, row 0 is the first line and row 1 is the second.
 *
 * @param charPositionX Column position (0-19)
 * @param charPositionY Row position (0 or 1)
 */
void displayCharPositionWrite(uint8_t charPositionX, uint8_t charPositionY);

/**
 * @brief Writes a null-terminated string to the display at the current cursor position.
 *
 * @param str Pointer to the string to display.
 */
void displayStringWrite(const char * str);

/**
 * @brief Displays the current windshield wiper mode.
 *
 * Clears the display and writes on line 0: "Wiper Mode: <mode>"
 * If mode is "INT", then on line 1 it writes "Delay: <delaySetting>".
 *
 * @param mode         A string representing the mode ("HI", "LO", "INT", or "OFF").
 * @param delaySetting A string representing the delay setting ("SHORT", "MEDIUM", or "LONG").
 *                     This is used only if mode is "INT".
 */
void displayWiperMode(const char* mode, const char* delaySetting);

/**
 * @brief Displays a generic message on the LCD.
 *
 * The message is shown on the first line.
 *
 * @param message The message string.
 */
void displayMessage(const char* message);

#endif // _DISPLAY_H_
//=====[#include guards - end]===============================================

