/*
 * lcd.h
 *
 *  Created on: Jul 11, 2017
 *      Author: donald
 */

#ifndef LCD_H_
#define LCD_H_

#define LCD_i2c_address	0x28 //40
//#define LCD_i2c_address	0x50 //80
#define LCD_max_data_rate 100 //KHz

typedef enum {
	LINE_1 = 0x00,
	LINE_2 = 0x40,
	LINE_3 = 0x14,
	LINE_4 = 0x54
}lcd_lines;

void set_lcd_cursor(lcd_lines x);
void lcd_clear(void);
void LCD_print_char(char c);
void LCD_print_string(lcd_lines line, char* string);
void LCD_print_integer(lcd_lines line, int number);
void show_lcd_i2c_address(void);
void set_lcd_backlight_brightness(int brightness);
void turn_on_blinking_cursor(void);
void turn_off_blinking_cursor(void);

#endif /* LCD_H_ */
