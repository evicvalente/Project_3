//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "display.h"


//=====[Declaration of private defines]========================================

// LCD instruction set commands
#define DISPLAY_IR_CLEAR_DISPLAY          0b00000001
#define DISPLAY_IR_ENTRY_MODE_SET         0b00000100
#define DISPLAY_IR_DISPLAY_CONTROL        0b00001000
#define DISPLAY_IR_FUNCTION_SET           0b00100000
#define DISPLAY_IR_SET_DDRAM_ADDR         0b10000000

// Entry mode options
#define DISPLAY_IR_ENTRY_MODE_SET_INCREMENT 0b00000010
#define DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT  0b00000000

// Display control options
#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON  0b00000100
#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF 0b00000000
#define DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF    0b00000000
#define DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF     0b00000000

// Function set options (we use 8-bit mode, 2 lines, 5x8 dots)
#define DISPLAY_IR_FUNCTION_SET_8BITS    0b00010000
#define DISPLAY_IR_FUNCTION_SET_2LINES   0b00001000
#define DISPLAY_IR_FUNCTION_SET_5x8DOTS  0b00000000

// DDRAM addresses for a 20x2 LCD display
#define DISPLAY_20x2_LINE1_FIRST_CHARACTER_ADDRESS  0x00
#define DISPLAY_20x2_LINE2_FIRST_CHARACTER_ADDRESS  0x40

// RS and RW values
#define DISPLAY_RS_INSTRUCTION 0
#define DISPLAY_RS_DATA        1

#define DISPLAY_RW_WRITE 0
#define DISPLAY_RW_READ  1

// Pin assignments (adjust if needed)
#define DISPLAY_PIN_RS  4
#define DISPLAY_PIN_RW  5
#define DISPLAY_PIN_EN  6
#define DISPLAY_PIN_D0  7  
#define DISPLAY_PIN_D1  8  
#define DISPLAY_PIN_D2  9  
#define DISPLAY_PIN_D3 10
#define DISPLAY_PIN_D4 11
#define DISPLAY_PIN_D5 12 
#define DISPLAY_PIN_D6 13 
#define DISPLAY_PIN_D7 14 

//=====[Declaration and initialization of public global objects]=============
DigitalOut displayD0( D0 );
DigitalOut displayD1( D1 );
DigitalOut displayD2( D2 );
DigitalOut displayD3( D3 );
DigitalOut displayD4( D4 );
DigitalOut displayD5( D5 );
DigitalOut displayD6( D6 );
DigitalOut displayD7( D7 );
DigitalOut displayRs( D8 );
DigitalOut displayEn( D9 );

//=====[Declarations (prototypes) of private functions]========================
static void displayPinWrite( uint8_t pinName, int value );
static void displayDataBusWrite( uint8_t dataByte );
static void displayCodeWrite( bool type, uint8_t dataBus );

// Custom string compare function (static so it's private to this file)
static int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

//=====[Implementations of public functions]===================================

void displayInit()
{
    // Wait for the LCD to power up
    delay(50);
    
    // Function set (8-bit mode)
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                     DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_8BITS);
    delay(5);
            
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                     DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_8BITS);
    delay(1); 

    displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                     DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_8BITS);
    delay(1);  

    // Set function: 8 bits, 2 lines, 5x8 dots
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                     DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_8BITS | 
                     DISPLAY_IR_FUNCTION_SET_2LINES | DISPLAY_IR_FUNCTION_SET_5x8DOTS);
    delay(1);         

    // Turn display off
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                     DISPLAY_IR_DISPLAY_CONTROL |
                     DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF |
                     DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF |
                     DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF);
    delay(1);          

    // Clear display
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, DISPLAY_IR_CLEAR_DISPLAY);
    delay(1); 

    // Set entry mode: increment, no display shift
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                     DISPLAY_IR_ENTRY_MODE_SET |
                     DISPLAY_IR_ENTRY_MODE_SET_INCREMENT |
                     DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT);
    delay(1);           

    // Turn display on (without cursor or blink)
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                     DISPLAY_IR_DISPLAY_CONTROL |
                     DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON |
                     DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF |
                     DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF);
    delay(1);
}

/**
 * @brief Sets the cursor to a specific (column, row) position.
 */
void displayCharPositionWrite(uint8_t charPositionX, uint8_t charPositionY)
{
    switch(charPositionY) {
        case 0:
            displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                DISPLAY_IR_SET_DDRAM_ADDR | (DISPLAY_20x2_LINE1_FIRST_CHARACTER_ADDRESS + charPositionX));
            delay(1);
            break;
        case 1:
            displayCodeWrite(DISPLAY_RS_INSTRUCTION, 
                DISPLAY_IR_SET_DDRAM_ADDR | (DISPLAY_20x2_LINE2_FIRST_CHARACTER_ADDRESS + charPositionX));
            delay(1);
            break;
        default:
            break;
    }
}

/**
 * @brief Writes a string starting at the current cursor position.
 */
void displayStringWrite(const char * str)
{
    while (*str) {
        displayCodeWrite(DISPLAY_RS_DATA, *str++);
    }
}

/**
 * @brief Clears the display.
 */
void displayClear(void)
{
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, DISPLAY_IR_CLEAR_DISPLAY);
    delay(2);
}

/**
 * @brief Displays the current windshield wiper mode.
 *
 * For "INT" mode, the delay setting is shown on the second line.
 */
void displayWiperMode(const char* mode, const char* delaySetting)
{
    displayClear();
    
    // Write first line: "Wiper Mode: <mode>"
    displayCharPositionWrite(0, 0);
    displayStringWrite("Wiper Mode: ");
    displayStringWrite(mode);
    
    // If intermittent mode, display delay setting on second line.
    if(my_strcmp(mode, "INT") == 0) {
        displayCharPositionWrite(0, 1);
        displayStringWrite("Delay: ");
        displayStringWrite(delaySetting);
    }
}

/**
 * @brief Displays a generic message on the first line.
 */
void displayMessage(const char* message)
{
    displayClear();
    displayCharPositionWrite(0, 0);
    displayStringWrite(message);
}

//=====[Implementations of private functions]==================================

/**
 * @brief Writes an instruction or data byte to the LCD.
 *
 * @param type     DISPLAY_RS_INSTRUCTION or DISPLAY_RS_DATA.
 * @param dataBus  The data byte to send.
 */
static void displayCodeWrite( bool type, uint8_t dataBus )
{
    if ( type == DISPLAY_RS_INSTRUCTION )
        displayPinWrite( DISPLAY_PIN_RS, DISPLAY_RS_INSTRUCTION );
    else
        displayPinWrite( DISPLAY_PIN_RS, DISPLAY_RS_DATA );
    
    displayPinWrite( DISPLAY_PIN_RW, DISPLAY_RW_WRITE );
    displayDataBusWrite( dataBus );
}

/**
 * @brief Writes a value to one of the defined pins.
 *
 * @param pinName The pin identifier (use the defined DISPLAY_PIN_ constants).
 * @param value   The value to write (0 or 1).
 */
static void displayPinWrite( uint8_t pinName, int value )
{
    switch(pinName) {
        case DISPLAY_PIN_D0: displayD0 = value; break;
        case DISPLAY_PIN_D1: displayD1 = value; break;
        case DISPLAY_PIN_D2: displayD2 = value; break;
        case DISPLAY_PIN_D3: displayD3 = value; break;
        case DISPLAY_PIN_D4: displayD4 = value; break;
        case DISPLAY_PIN_D5: displayD5 = value; break;
        case DISPLAY_PIN_D6: displayD6 = value; break;
        case DISPLAY_PIN_D7: displayD7 = value; break;
        case DISPLAY_PIN_RS: displayRs = value; break;
        case DISPLAY_PIN_EN: displayEn = value; break;
        // DISPLAY_PIN_RW is handled in displayCodeWrite
        default: break;
    }
}

/**
 * @brief Writes an 8-bit value to the LCD data bus.
 *
 * Data is latched on the falling edge of the Enable pin.
 *
 * @param dataBus The 8-bit data to write.
 */
static void displayDataBusWrite( uint8_t dataBus )
{
    // Begin with Enable low
    displayPinWrite( DISPLAY_PIN_EN, OFF );
    
    displayPinWrite( DISPLAY_PIN_D7, (dataBus & 0b10000000) ? 1 : 0 );
    displayPinWrite( DISPLAY_PIN_D6, (dataBus & 0b01000000) ? 1 : 0 );
    displayPinWrite( DISPLAY_PIN_D5, (dataBus & 0b00100000) ? 1 : 0 );
    displayPinWrite( DISPLAY_PIN_D4, (dataBus & 0b00010000) ? 1 : 0 );
    displayPinWrite( DISPLAY_PIN_D3, (dataBus & 0b00001000) ? 1 : 0 );
    displayPinWrite( DISPLAY_PIN_D2, (dataBus & 0b00000100) ? 1 : 0 );  
    displayPinWrite( DISPLAY_PIN_D1, (dataBus & 0b00000010) ? 1 : 0 );      
    displayPinWrite( DISPLAY_PIN_D0, (dataBus & 0b00000001) ? 1 : 0 );
    
    // Latch data by pulsing Enable high then low
    displayPinWrite( DISPLAY_PIN_EN, ON );
    delay(1);
    displayPinWrite( DISPLAY_PIN_EN, OFF);
    delay(1);
}


