/*
 * lcd.cpp

 *
 *  Created on: Jul 11, 2017
 *      Author: donald
 */
#include <stdint.h>
#include "board.h"
#include "i2c.h"

#define LCD_i2c_address	0x28 //80
#define LCD_max_data_rate 100 //KHz

uint8_t tx_packet[3];
uint8_t rx_packet; //Unnecessary

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void send_packet(int number_of_bytes)
{
	SetupXferRecAndExecute(LCD_i2c_address, tx_packet, number_of_bytes, &rx_packet, 0);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void turn_on_lcd()
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x41;
	send_packet(2);	
}

void turn_off_lcd()
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x42;
	send_packet(2);	
}

void set_lcd_cursor(unsigned int x)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x45;
	tx_packet[2] = x;
	send_packet(3);	
}

// void lcd_clear_line(unsigned int x)
// {
// 	unsigned int i;
	
// 	for (i = 0; i < x; i++) {
// 		//Write blank character x times
// 		tx_packet[i] = 0x20;
// 	}
// 	send_packet(x);
// }

void lcd_clear(void)
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x51;
	send_packet(2);
}

void LCP_print_char(char c)
{
	tx_packet[0] = (int) c;
	send_packet(1);	
}

void LCD_print_string(int row, char* string)
{
	while (*string) {
    	LCP_print_char(*string++);
  	}
}

void show_lcd_i2c_address()
{
	tx_packet[0] = 0xFE;
	tx_packet[1] = 0x72;
	send_packet(2);	
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
}


