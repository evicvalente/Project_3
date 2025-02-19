#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

/**
 * @brief Initializes the user interface.
 *
 * Sets default wiper mode and delay values and starts any required timers.
 */
void userInterfaceInit(void);

/**
 * @brief Updates the user interface.
 *
 * Reads the wiper mode and delay selectors, updates the internal state, and refreshes the display.
 */
void userInterfaceUpdate(void);

/**
 * @brief Returns the current wiper mode.
 *
 * @return A pointer to a string representing the current wiper mode ("OFF", "LO", "INT", or "HI").
 */
const char* getWiperMode(void);

/**
 * @brief Returns the current delay setting.
 *
 * @return A pointer to a string representing the current delay setting ("SHORT", "MEDIUM", or "LONG").
 */
const char* getDelaySetting(void);

#endif // _USER_INTERFACE_H_
