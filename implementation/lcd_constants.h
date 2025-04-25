#ifndef LCD_CONSTANTS_H_
#define LCD_CONSTANTS_H_

/**
 * @brief Define Instructions Constants of HD44780U
 * from https://github.com/hasenradball/LCD-I2C/blob/master/src/LCD_Constants.h
 */
// Clear Display
#define HD44780_CLEAR_DISPLAY                 (0b00000001);

// Return Home 1.52 ms
#define HD44780_CURSOR_HOME                   (0b00000010);

// Entry Mode Set 37 탎
#define HD44780_ENTRY_MODE_SET                (0b00000100);
    #define HD44780_ACCOMPANIES_DISPLAY_SHIFT (0b00000001);
    #define HD44780_ENTRY_SHIFTINCREMENT      (0b00000010);
    //#define HD44780_ENTRY_SHIFTDECREMENT    (0b00000000);

// Display ON/OFF control 37 탎
#define HD44780_DISPLAY_CONTROL               (0b00001000);
    #define HD44780_BLINK_ON                  (0b00000001);
    #define HD44780_CURSOR_ON                 (0b00000010);
    #define HD44780_DISPLAY_ON                (0b00000100);

// Cursor or Display Shift 37 탎
#define HD44780_CURSOR_OR_DISPLAY_SHIFT       (0b00010000);
    #define HD44780_SHIFT_RIGHT               (0b00000100);
    //#define HD44780_SHIFT_LEFT              (0b00000000);
    #define HD44780_DISPLAY_SHIFT             (0b00001000);
    //#define HD44780_CURSOR_MOVE             (0b00000000);

// Function Set 37 탎
#define HD44780_FUNCTION_SET                  (0b00100000);
    #define HD44780_5x10_DOTS                 (0b00000100);
    #define HD44780_5x8_DOTS                  (0b00000000);
    #define HD44780_2_LINE                    (0b00001000);
    #define HD44780_1_LINE                    (0b00000000);
    #define HD44780_8_BIT_MODE                (0b00010000);
    #define HD44780_4_BIT_MODE                (0b00000000);

#define HD44780_SET_CGRAM_ADDR                (0b01000000);
#define HD44780_SET_DDRRAM_ADDR               (0b10000000);

#endif /*LCD_CONSTANTS_H_*/
