/*
 * lcd.cpp

 *
 *  Created on: Jul 11, 2017
 *      Author: donald
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lcd.h"
#include "board.h"
#include "i2c.h"

#define CLOCK_PERIOD_IN_NANOSECONDS	33
#define WRITE_CHAR_DELAY_NANOSECONDS	30000
#define WRITE_CHAR_DELAY_TICKS	WRITE_CHAR_DELAY_NANOSECONDS/CLOCK_PERIOD_IN_NANOSECONDS

#define NUMBER_OF_COLUMNS	20
#define NUMBER_OF_ROWS	4
#define restrictLineChange

static uint8_t tx_packet[3];
static uint8_t rx_packet; //Unnecessary

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void send_packet(int number_of_bytes)
{
	SetupXferRecAndExecute(LCD_i2c_address, tx_packet, number_of_bytes, &rx_packet, 0);
}

static void delay(int ticks)
{
	int i = 0;
	while (i <= ticks)
	{
		i++;
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void set_lcd_cursor(lcd_lines x)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x45;
	tx_packet[2] = (uint8_t) x;
	send_packet(3);	
	delay(WRITE_CHAR_DELAY_TICKS);
}

 void lcd_clear_line(lcd_lines line)
 {
 	int i;

 	set_lcd_cursor(line);
	
 	for (i = 0; i < NUMBER_OF_COLUMNS; i++) {
 		//Write blank character x times
 		tx_packet[i] = 0x20;
 	}
 	send_packet(NUMBER_OF_COLUMNS);
 	delay(WRITE_CHAR_DELAY_TICKS*NUMBER_OF_COLUMNS);
 }

void lcd_clear(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x51;
	send_packet(2);
	delay(WRITE_CHAR_DELAY_TICKS*(NUMBER_OF_COLUMNS*NUMBER_OF_ROWS));
}

void LCD_print_char(char c)
{
	tx_packet[0] = (uint8_t) c;
	send_packet(1);
	delay(WRITE_CHAR_DELAY_TICKS);
}

//Make sure to null terminate string
//Ex: LCD_print_string(LINE_2, "Hello World!\0");
void LCD_print_string(lcd_lines line, char* string)
{
	#ifdef restrictLineChange
		int count = 0;
	#endif

	string[strlen(string)] = '\0';	/* Safety */

	set_lcd_cursor(line);

	while (*string) {
		LCD_print_char((char)*string++);
		#ifdef restrictLineChange
			count ++;
			if (count == NUMBER_OF_COLUMNS)
			{
				break;
			}
		#endif
  	}
}

//Ex: LCD_print_integer(LINE_2, 999);
void LCD_print_integer(lcd_lines line, int number)
{
	char print_buffer[NUMBER_OF_COLUMNS];
	itoa(number, print_buffer, 10);
	LCD_print_string(line, print_buffer);
}

void show_lcd_i2c_address(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x72;
	send_packet(2);	
	delay(WRITE_CHAR_DELAY_TICKS*NUMBER_OF_COLUMNS);
}

//param brightness: must be between 1 and 8.
void set_lcd_backlight_brightness(int brightness)
{
	//Make sure we have a valid brightness
	if (brightness > 8 || brightness < 1)
	{
		return;
	}
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x53;
	tx_packet[2] = brightness;
	send_packet(3);	
	delay(WRITE_CHAR_DELAY_TICKS);
}

void turn_on_blinking_cursor(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x4B;
	send_packet(2);
	delay(WRITE_CHAR_DELAY_TICKS);
}

void turn_off_blinking_cursor(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x4C;
	send_packet(2);
	delay(WRITE_CHAR_DELAY_TICKS);
}
