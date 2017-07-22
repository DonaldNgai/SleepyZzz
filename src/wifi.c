/*
 * wifi.c
 *
 *  Created on: Jul 21, 2017
 *      Author: donald
 */

#include <string.h>
#include "board.h"
#include "lcd.h"
#include "uart.h"
#include "wifi.h"
#include "SleepyZzz.h"

const char* ssid     = "SSID HERE";
const char* password = "PASSWORD HERE";
char path[] = "/";
char host[] = "echo.websocket.org";
static char tx_data[100] = "";

bool join_network()
{
//	char* tx_data_ptr = tx_data;
	strcat(tx_data, "AT+CWJAP = ");
//	LCD_print_string(LINE_1, tx_data);
	strcat(tx_data, ssid);
	strcat(tx_data, ", ");
	strcat(tx_data, password);
	strcat(tx_data, "\r\n");
//	LCD_print_string(LINE_3, tx_data);
	print_to_console(tx_data);
	return false;
}

bool disconnect_from_network()
{
	return false;
}

bool handshake_with_server()
{
	return false;
}

bool connect_to_server()
{
	return false;
}

bool get_data_from_server()
{
	return false;
}

bool send_data_from_server()
{
	return false;
}
