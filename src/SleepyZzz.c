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

#define I2C_ADDR_7BIT           (0x60)

#define TICKRATE_HZ (10)	/* 10 ticks per second */
//Main clock and system clock should be 30MHz because we got a 4.99Hz LED frequency when probing pin 7
//Meaning 41.66ns clock period

char print_buffer[50];
sensor_values_t sensorVals;
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
//	Board_LED_Toggle(0);

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
    static int i = 0 ;
    int temp;
    int heartrate;
    int orientation;
    int other;
    // Enter an infinite loop, just incrementing a counter
    sensor_values_t sensors;
    while(1) {
    	delay(500);
    	get_sensor_values(&sensors);
    	LCD_print_integer(LINE_1,sensors.temperature);
    	LCD_print_integer(LINE_2,sensors.heart_rate);
//    	LCD_print_integer(LINE_3,sensors.orientation);
//    	LCD_print_integer(LINE_4,sensors.other);

//    	temp = get_analog_input(0);
//    	heartrate = get_analog_input(1);
//    	orientation = get_analog_input(2);
//    	other = get_analog_input(3);
//    	LCD_print_integer(LINE_1,temp);
//		LCD_print_integer(LINE_2,heartrate);
//		LCD_print_integer(LINE_3,orientation);
//		LCD_print_integer(LINE_4,other);
        i++ ;
    }
    return 0 ;
}
