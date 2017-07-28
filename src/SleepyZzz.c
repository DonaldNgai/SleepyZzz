/*
===============================================================================
 Name        : SleepyZzz.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#include <string.h>
#include <stdlib.h>
#include "switch_matrix.h"
#include "i2c.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "wifi.h"
#include "print.h"
#include "helper.h"
#include "SleepyZzz.h"

#define TICKRATE_HZ (10)	/* 10 ticks per second */
//Main clock and system clock should be 30MHz because we got a 4.99Hz LED frequency when probing pin 7
//Meaning 41.66ns clock period

sensor_values_t sensorVals;

/* UART handle and memory for ROM API */
UART_HANDLE_T* uart0Handle;

/* Use a buffer size larger than the expected return value of
   uart_get_mem_size() for the static UART handle type */
uint32_t uart0HandleMEM[0x10];

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */

void SysTick_Handler(void)
{


}

void program_init(void){

	#if defined (__USE_LPCOPEN)
		// Read clock settings and update SystemCoreClock variable
		SystemCoreClockUpdate();
	#if !defined(NO_BOARD_LIB)
		// Set up and initialize all required blocks and
		// functions related to the board hardware
		Board_Init();
	#endif
	#endif

	reset_switch_matrix();

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	/*		LCD INIT		*/

	/* Setup I2C pin muxing */
	Init_I2C_PinMux();
	/* Allocate I2C handle, setup I2C rate, and initialize I2C clocking */
	setupI2CMaster();
	/* Enable the interrupt for the I2C */
	NVIC_EnableIRQ(I2C_IRQn);
	lcd_clear();
	turn_on_blinking_cursor();

	setup_usb_console();

	/*		WIFI INIT		*/

	UART_CONFIG_T cfg0 = {
			0,				/* U_PCLK frequency in Hz */
			115200,			/* Baud Rate in Hz */
			1,				/* 8N1 */
			0,				/* Asynchronous Mode */
			NO_ERR_EN		/* Enable No Errors */
		};

	Init_UART_PinMux(SWM_U0_TXD_O,4,SWM_U0_RXD_I,0);
	Chip_UART_Init(LPC_USART0);
	/* Allocate UART handle, setup UART parameters, and initialize UART clocking */
	setupUART((uint32_t) LPC_USART0, &uart0Handle, uart0HandleMEM, sizeof(uart0HandleMEM), cfg0);

}

int main(void) {

	char recv0_buf[100];
	char string_buffer[100];

	program_init();

	Board_LED_Set(0, false);

	print_to_console("\r\nBeginning of Program\r\n");

	//echo_to_console();

	//get_line_from_console(recv0_buf,sizeof(recv0_buf));

//  How to print to console and use sprintf
//	print_to_console("Sent AT to UART0\r\n");
//	my_sprintf(string_buffer,"r: %x, n: %x",'\r','\n');

//	Get line from WIFI Module
//	getLineUART(uart0Handle, recv0_buf, sizeof(recv0_buf));
// 	Get char from WIFI Module
//	recv0_buf[0] = LPC_UARTD_API->uart_get_char(uart0Handle);
// 	How to put a line to Wifi Module
// 	putLineUART(uart0Handle,"AT\r\r\n\r\n\0");
//
// 	getLineUART(uart0Handle, recv0_buf, sizeof(recv0_buf));
//
// 	my_sprintf(string_buffer,"Received: %s", recv0_buf);
// 	print_to_console(string_buffer);

   // Force the counter to be placed into memory
	volatile static int i = 0 ;

	while(1)
	{
		// Enter an infinite loop, just incrementing a counter
		i++;

		if (i > 1000000){
			putLineUART(uart0Handle,"AT\r\r\n\r\n\0");
			Board_LED_Toggle(1);
		}

//		get_line_from_console(recv0_buf,sizeof(recv0_buf));
//
//		putLineUART(uart0Handle,recv0_buf);
//
		getLineUART(uart0Handle, recv0_buf, sizeof(recv0_buf));

//		my_sprintf(string_buffer,"Received: %s\n", recv0_buf);

		print_to_console(recv0_buf);

		if (i > 1000000){
			putLineUART(uart0Handle,"AT\r\r\n\r\n\0");
			Board_LED_Toggle(1);
		}
	}
    return 0 ;
}
