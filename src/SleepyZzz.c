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

char print_buffer[50];
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

	get_sensor_values(&sensorVals);

//	LCD_print_integer(LINE_1,(int) sensorVals.temperature);
//	LCD_print_integer(LINE_2,(int) sensorVals.heart_rate);
//	LCD_print_integer(LINE_3,(int) sensorVals.orientation);
//	LCD_print_integer(LINE_4,(int) sensorVals.other);

}

//clock runs at 30MHz
void delay(int ms)
{
	int k = 0;
	int j = 0;
	while (k <= ms)
	{
		j = 0;
		while (j <= 3000){
			j++;
		}
		k++;
	}
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

//	turn_on_blinking_cursor();
}

int main(void) {

	program_init();
//	LCD_print_integer(LINE_2,SystemCoreClock);
//	LCD_print_string(LINE_3,"Hello World!\0");
    // Force the counter to be placed into memory
    int temp;
    int heartrate;
    // Enter an infinite loop, just incrementing a counter
    sensor_values_t sensors;
    while(1) {
    	delay(500);
    	get_sensor_values(&sensors);
    	LCD_print_integer(LINE_1,sensors.temperature);
    	LCD_print_integer(LINE_2,sensors.heart_rate);
    }
    return 0 ;
}
