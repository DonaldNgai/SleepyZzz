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

char recv0_buf[512];
#define BUF_LEN 100
	char string_buffer[512];
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

	char char_temp,char_temp2;

	program_init();

	Board_LED_Set(0, false);

	print_to_console("\r\nBeginning of Program\r\n");

//	print_to_console("Sent AT to UART0\r\n");
//	my_sprintf(string_buffer,"r: %x, n: %x",'\r','\n');
//	print_to_console(string_buffer);
//	getLineUART(uart0Handle, recv0_buf, sizeof(recv0_buf));
//	recv0_buf[0] = LPC_UARTD_API->uart_get_char(uart0Handle);
//	recv0_buf[1] = LPC_UARTD_API->uart_get_char(uart0Handle);

	    	int i = 0;

				delay(1000000);
				Board_LED_Set(1, true);

				//		for(i=0; i < 100;i++)
				//		{
//				itoa(23,&char_temp,10);
//				itoa(10,&char_temp2,10);
//				my_sprintf(string_buffer,"AT%c%c",char_temp,char_temp2);
				putLineUART(uart0Handle,"AT\r\n");
//				putLineUART(uart0Handle, string_buffer);
				//		}
//	    	getLineUART(uart0Handle, recv0_buf, sizeof(recv0_buf));
//	    	my_sprintf(string_buffer,"Response: %s\r\n","test");
//	    	print_to_console("Received Response\r\n");
//	    	recv0_buf[sizeof(recv0_buf) - 1] = '\0';	/* Safety */
//			print_to_console(string_buffer);
//			LCD_print_string(LINE_3, recv0_buf);

//    // Force the counter to be placed into memory
//    volatile static int i = 0 ;
//    // Enter an infinite loop, just incrementing a counter
    while(1) {
    	getLineUART(uart0Handle, recv0_buf, sizeof(recv0_buf));
//		recv0_buf[0] = LPC_UARTD_API->uart_get_char(uart0Handle);
//    	while (1)
//    	{
//    	    char ch;
//    	    ch = LPC_UARTD_API->uart_get_char(uart0Handle);
//    	    my_sprintf(string_buffer,"char: %c %d\r\n",ch, ch);
//    	    print_to_console(string_buffer);
//    	    if (ch == '\r') print_to_console("return\r\n");
//    	    if (ch == '\n') print_to_console("new\r\n");
//    	    if (ch == '\n')     // ignore lf's
//    	    {
//				break;      // string now in buf, terminated w/ '\0'
//    	    }
//    	    else
//    	    {
//				recv0_buf[i++] = ch;
//				recv0_buf[i] = '\0';
//    	    }
//    	}
    	my_sprintf(string_buffer,"Received Response %s\r\n",recv0_buf);
//	    	recv0_buf[sizeof(recv0_buf) - 1] = '\0';	/* Safety */
		print_to_console(string_buffer);
		LCD_print_string(LINE_3, recv0_buf);
    }
    return 0 ;
}
