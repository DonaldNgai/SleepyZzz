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

uint8_t txData[16];
uint8_t rxData[16];

/*****************************************************************************
 * Public functions
 ****************************************************************************/

void clear_lcd()
{

}

void turn_off_lcd()
{
	txData[0] = 0xFE; // Prefix
	txData[1] = 0x53; // Command
	txData[2] = 0x06; // Command
//	SetupXferRecAndExecute(LCD_i2c_address, txData, 2, rxData, 0);
}

void LCD_print_string()
{

//	SetupXferRecAndExecute((uint16_t)LCD_i2c_address, txData, 5, rxData, 0);
//	turn_off_lcd();uint16_t AddressI2C, bool ledStateOut, bool address10Bit
//	sendI2CMaster(LCD_i2c_address, 0, 0);
	txData[0] = 0xFE; // Prefix
	txData[1] = 0x72; // Command
//	txData[0] = 0xFE; // Prefix
//		txData[1] = 0x53; // Command
//		txData[2] = 0x06; // Command

	SetupXferRecAndExecute(LCD_i2c_address, txData, 2, rxData, 0);

}

