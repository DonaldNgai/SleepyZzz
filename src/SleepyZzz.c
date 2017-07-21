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

#include "switch_matrix.h"
#include "i2c.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"

#define I2C_ADDR_7BIT           (0x60)

#define TICKRATE_HZ (10)	/* 10 ticks per second */
//Main clock and system clock should be 30MHz because we got a 4.99Hz LED frequency when probing pin 7
//Meaning 41.66ns clock period

char print_buffer[50];
sensor_values_t sensorVals;

char recv_buf[32];
/* Use a buffer size larger than the expected return value of
   uart_get_mem_size() for the static UART handle type */
uint32_t uartHandleMEM[0x10];
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
//	Board_LED_Toggle(0);
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

	/* Setup I2C pin muxing */
	Init_I2C_PinMux();

	/* Allocate I2C handle, setup I2C rate, and initialize I2C clocking */
	setupI2CMaster();

	/* Enable the interrupt for the I2C */
	NVIC_EnableIRQ(I2C_IRQn);

	lcd_clear();

	turn_on_blinking_cursor();
}

int main(void) {

	program_init();
//	LCD_print_integer(LINE_1,SystemCoreClock);
//	LCD_print_string(LINE_3,"Hello World!\0");

	Init_UART_PinMux(SWM_U0_TXD_O,6,SWM_U0_RXD_I,1);
	Chip_UART_Init(LPC_USART0);
	Board_LED_Set(0, false);

	/* 115.2KBPS, 8N1, ASYNC mode, no errors, clock filled in later */
		UART_CONFIG_T cfg = {
			0,				/* U_PCLK frequency in Hz */
			115200,			/* Baud Rate in Hz */
			1,				/* 8N1 */
			0,				/* Asynchronous Mode */
			NO_ERR_EN		/* Enable No Errors */
		};

	/* Allocate UART handle, setup UART parameters, and initialize UART
	   clocking */
	setupUART(uartHandleMEM, sizeof(uartHandleMEM), cfg);

	/* Transmit the welcome message and instructions using the
	   putline function */
	putLineUART("LPC8XX USART API ROM polling Example\r\n");
	putLineUART("Enter a string, press enter (CR+LF) to echo it back:\r\n");

	/* Get a string for the UART and echo it back to the caller. Data is NOT
	   echoed back via the UART using this function. */
	getLineUART(recv_buf, sizeof(recv_buf));
	recv_buf[sizeof(recv_buf) - 1] = '\0';	/* Safety */
	if (strlen(recv_buf) == (sizeof(recv_buf) - 1)) {
		putLineUART("**String was truncated, input data longer than "
					"receive buffer***\r\n");
	}
	putLineUART(recv_buf);

	/* Transmit the message for byte/character part of the exampel */
	putLineUART("\r\nByte receive with echo: "
				"Press a key to echo it back. Press ESC to exit\r");

//	/* Endless loop until ESC key is pressed */
//	recv_buf[0] = '\n';
//	while (recv_buf[0] != ESCKEY) {
//		/* Echo it back */
//		LPC_UARTD_API->uart_put_char(uartHandle, recv_buf[0]);
//
//		/* uart_get_char will block until a character is received */
//		recv_buf[0] = LPC_UARTD_API->uart_get_char(uartHandle);
//	}

	/* Transmit the message for byte/character part of the exampel */
	putLineUART("\r\nESC key received, exiting\r\n");

//    // Force the counter to be placed into memory
//    volatile static int i = 0 ;
//    // Enter an infinite loop, just incrementing a counter
//    while(1) {
//        i++ ;
//    }
    return 0 ;
}
