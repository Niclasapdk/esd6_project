#include <ezdsp5535.h>
#include <ezdsp5535_i2c.h>
#include "lcd.h"

#define LCD_I2C_ADDR (0x27)

// flags
#define LCD_RS  (0x01)
#define LCD_RW  (0x02)
#define LCD_EN  (0x04)
#define LCD_LED (0x08)
static Uint8 LCD_flags = 0;

// Display ON/OFF control 37 microseconds
#define LCD_DISPLAY_CONTROL (0x08)
#define LCD_BLINK_ON        (0x01)
#define LCD_CURSOR_ON       (0x02)
#define LCD_DISPLAY_ON      (0x04)

// Set cursor
#define LCD_SET_DDRRAM_ADDR (0x80)


// writes byte to lcd driver
void LCD_writebyte(Uint8 c) {
	Uint16 buf[2];
	Uint8 fuck;
	fuck = LCD_flags | LCD_EN;
	buf[0] = (c&0xf0) | fuck;
	buf[1] = ((c&0x0f)<<4) | fuck;
	EZDSP5535_I2C_write(LCD_I2C_ADDR, buf, 2);
}

// writes a character to lcd display
void LCD_write(Uint8 c) {
	LCD_flags = LCD_RS;
	LCD_writebyte(c);
}

void LCD_blink() {
	LCD_flags = 0;
	LCD_writebyte(LCD_DISPLAY_CONTROL | LCD_BLINK_ON);
}

void LCD_noblink() {
	LCD_flags = 0;
	LCD_writebyte(LCD_DISPLAY_CONTROL);
}

// No sanity checks on this
// if you call with OOB indices, you're dumb
// obviously, row must be [0:3] and column must be [0:15]
void LCD_setCursor(Uint8 column, Uint8 row) {
    static const Uint8 rowOffsets[] = {0x00, 0x40, 0x14, 0x54};
    Uint8 newAddr;
	LCD_flags = 0;
	newAddr = rowOffsets[row] + column;
	LCD_writebyte(LCD_SET_DDRRAM_ADDR | newAddr);
}
