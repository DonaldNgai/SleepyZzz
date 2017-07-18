/*
 * lcd.cpp

 *
 *  Created on: Jul 11, 2017
 *      Author: donald
 */
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"
#include "board.h"
#include "i2c.h"

#define hundredMicroSeconds	100
#define hundredFiftyMilliSeconds	150000
#define fourHundredMilliSeconds		400000
#define numberOfColumns	20
#define restrictLineChange

uint8_t tx_packet[3];
uint8_t rx_packet[3]; //Unnecessary

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void send_packet(int number_of_bytes)
{
	SetupXferRecAndExecute(LCD_i2c_address, tx_packet, number_of_bytes, rx_packet, 0);
}

static void delay(int microSeconds)
{
	int i = 0;
	while (i <= 100)
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
	delay(hundredMicroSeconds);
}

 void lcd_clear_line(lcd_lines line)
 {
 	int i;

 	set_lcd_cursor(line);
	
 	for (i = 0; i < numberOfColumns; i++) {
 		//Write blank character x times
 		tx_packet[i] = 0x20;
 	}
 	send_packet(numberOfColumns);
 	delay(hundredMicroSeconds*numberOfColumns);
 }

void lcd_clear(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x51;
	send_packet(2);
	delay(hundredFiftyMilliSeconds);
}

void LCD_print_char(char c)
{
	tx_packet[0] = (uint8_t) c;
	send_packet(1);
	delay(hundredMicroSeconds);
}

//Make sure to null terminate string
void LCD_print_string(lcd_lines line, char* string)
{
	#ifdef restrictLineChange
		int count = 0;
	#endif

	set_lcd_cursor(line);

	while (*string) {
		LCD_print_char((char)*string++);
		#ifdef restrictLineChange
			count ++;
			if (count == numberOfColumns)
			{
				break;
			}
		#endif
  	}
}

void LCD_print_integer(lcd_lines line, int number)
{
	char print_buffer[numberOfColumns];
	itoa(number, print_buffer, 10);
	LCD_print_string(line, print_buffer);
}

void show_lcd_i2c_address(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x72;
	send_packet(2);	
	delay(fourHundredMilliSeconds);
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
	delay(hundredMicroSeconds);
}

void turn_on_blinking_cursor(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x4B;
	send_packet(2);
	delay(hundredFiftyMilliSeconds);
}

void turn_off_blinking_cursor(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x4C;
	send_packet(2);
	delay(hundredFiftyMilliSeconds);
}
