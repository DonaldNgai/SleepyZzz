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
#include "print.h"
#include "SleepyZzz.h"

const char* ssid     = "SSID HERE";
const char* password = "PASSWORD HERE";
char path[] = "/";
char host[] = "echo.websocket.org";
static char tx_data[512] = "";

static UART_HANDLE_T* uartHandle;
static char recv_buf[32];

#define CONNECTION_TYPE	"\"TCP\""
#define CONNECTION_ADDRESS	"\"10.1.10.2\""
#define CONNECTION_PORT	23
#define	CONNECTION_ID	1
//#define MULTIPLE_CONNECTION_MODE
#define STATION_MODE	"1"
// 1~7200
#define TCP_KEEP_ALIVE	7200

#define OK_RESPONSE 	"OK"
#define SEND_RESPONSE	"SEND OK"

/*****************************************************************************
 * Helper functions
 ****************************************************************************/

//bool terminate_string(char* string)
//{
//	strcat(string, "\r\n\0");
//}

void send_command_to_module(char* tx_data)
{
	putLineUART(uartHandle,tx_data);
}

bool confirm_response(char* response)
{
	bool result = false;
	// getLineUART(uartHandle, recv_buf, sizeof(recv_buf));
	// result = (strcmp(recv_buf,response) == 0) ? true : false;
	return result;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

void setup_wifi_module(UART_HANDLE_T* uartHandle)
{
	uartHandle = uartHandle;
}

/*****************************************************************************
 * Wi-Fi functions
 ****************************************************************************/

bool set_module_mode()
{
	my_sprintf(tx_data, "AT+CWMODE = %s\r\n",STATION_MODE);
	send_command_to_module(tx_data);
	return confirm_response(OK_RESPONSE);
}

bool set_autoconnect_to_AP(bool enable)
{
	my_sprintf(tx_data, "AT+CWAUTOCONN = %d\r\n",enable);
	send_command_to_module(tx_data);
	return confirm_response(OK_RESPONSE);
}

bool connect_to_AP()
{
	my_sprintf(tx_data, "AT+CWJAP = %s, %s\r\n",ssid,password);
	send_command_to_module(tx_data);
	return confirm_response(OK_RESPONSE);
}

bool disconnect_from_AP()
{
	my_sprintf(tx_data, "AT+CWQAP\r\n",ssid,password);
	send_command_to_module(tx_data);
	return confirm_response(OK_RESPONSE);
}

/*****************************************************************************
 * TCP/IP functions
 ****************************************************************************/
bool establish_tcp_connection()
{

#ifdef MULTIPLE_CONNECTION_MODE
	my_sprintf(tx_data, "AT+CIPSTART= %d, %s, %s, %d, %d\r\n",CONNECTION_ID,CONNECTION_TYPE,CONNECTION_ADDRESS,CONNECTION_PORT,TCP_KEEP_ALIVE);
#else
	my_sprintf(tx_data, "AT+CIPSTART= %s, %s, %d, %d\r\n",CONNECTION_TYPE,CONNECTION_ADDRESS,CONNECTION_PORT,TCP_KEEP_ALIVE);
#endif

	send_command_to_module(tx_data);
	return confirm_response(OK_RESPONSE);
}

bool close_tcp_connection()
{
	#ifdef MULTIPLE_CONNECTION_MODE
		my_sprintf(tx_data, "AT+CIPCLOSE= %d\r\n",CONNECTION_ID);
	#else
		my_sprintf(tx_data, "AT+CIPCLOSE\r\n");
	#endif
	send_command_to_module(tx_data);
	return confirm_response(OK_RESPONSE);
}

void get_local_ip_address(char* response)
{
	my_sprintf(tx_data, "AT+CIFSR\r\n");
	send_command_to_module(tx_data);

	// getLineUART(uartHandle, recv_buf, sizeof(recv_buf));
	// Response should be +CIFSR:ip OK
	// *response = "";
}

bool send_data_to_server(char* tx_data)
{
	#ifdef MULTIPLE_CONNECTION_MODE
		my_sprintf(tx_data, "AT+CIPSEND= %d\r\n",CONNECTION_ID);
	#else
		my_sprintf(tx_data, "AT+CIPSEND\r\n");
	#endif

//		confirm_response(">")
//		Start sending data
		// TODO length of data
	// strcat(tx_data, strlen());
	return confirm_response(SEND_RESPONSE);
}

// bool receive_data_from_server(char* tx_data)
// {
// 	strcat(tx_data, "AT+CIPSEND= ");
// 	#ifdef MULTIPLE_CONNECTION_MODE
// 		strcat(tx_data, CONNECTION_ID);
// 		strcat(tx_data, ",");
// 	#endif
// 		// TODO length of data
// 	// strcat(tx_data, strlen());
// 	terminate_string(tx_data);
// 	return confirm_response(SEND_RESPONSE);
// }

/*****************************************************************************
 * Websocket functions
 ****************************************************************************/

// bool send_websocket_handshake_request()
// {
// 	strcat(tx_data, "AT+CWJAP = ");
// 	strcat(tx_data, ssid);
// 	strcat(tx_data, ", ");
// 	strcat(tx_data, password);
// 	terminate_string(tx_data);
/*GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==
Sec-WebSocket-Protocol: chat, superchat
Sec-WebSocket-Version: 13
Origin: http://example.com*/
// }

// bool establish_websocket()
// {
// 	// strcat(tx_data, "AT+CIPSTART=?");
// 	// send_data_to_server(tx_data);
// 	strcat(tx_data, "AT+CIPSTART= ");
// 	#ifdef MULTIPLE_CONNECTION_MODE
// 		strcat(tx_data, CONNECTION_ID);
// 		strcat(tx_data, ",");
// 	#endif
// 	strcat(tx_data, CONNECTION_TYPE);
// 	strcat(tx_data, ",");
// 	strcat(tx_data, CONNECTION_ADDRESS);
// 	strcat(tx_data, ",");
// 	strcat(tx_data, CONNECTION_PORT);
// 	// strcat(tx_data, ",");
// 	terminate_string(tx_data);
// 	// strcat(tx_data, " (AT+CIPMUX=0)");
// // 	<type>,<addr>,<port> 
// // AT+CIPSTART= <id><type>,<addr>,<port> (AT+CIPMUX=1)
// 	return confirm_response(OK_RESPONSE);
// }
