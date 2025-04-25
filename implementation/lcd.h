#ifndef LCD_H_
#define LCD_H_

void LCD_clear();
void LCD_setCursor(Uint8 column, Uint8 row);
void LCD_write(Uint8 c);
void LCD_blink();
void LCD_noblink();
void LCD_init();

#endif /*LCD_H_*/
