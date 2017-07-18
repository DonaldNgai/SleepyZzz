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

#define I2C_ADDR_7BIT           (0x60)

//0x28 or 40
#define TICKRATE_HZ (10)	/* 10 ticks per second */
//Main clock and system clock should be 30MHz because we got a 4.99Hz LED frequency when probing pin 7
//Meaning 41.66ns clock period

char print_buffer[50];

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
//	Board_LED_Toggle(0);
//	Board_LED_Set(0, true);


//	LCD_print_char('b');

	//Works
//	LCD_print_char('H');
//	LCD_print_char('e');
//	LCD_print_char('l');
//	LCD_print_char('l');
//	LCD_print_char('o');
//
//	LCD_print_char('a');
//	turn_on_blinking_cursor();
	//	turn_off_lcd();
	//	turn_on_lcd();
//	show_lcd_i2c_address();
//	set_lcd_backlight_brightness(8);
//	lcd_clear();

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

//	reset_switch_matrix();

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	/* Setup I2C pin muxing */
	Init_I2C_PinMux();

	/* Allocate I2C handle, setup I2C rate, and initialize I2C clocking */
	setupI2CMaster();

	/* Enable the interrupt for the I2C */
	NVIC_EnableIRQ(I2C_IRQn);

	lcd_clear();
}

int main(void) {

	int num = 100;
	program_init();

//	sprintf(print_buffer,"hi %d", 10);
//	LCD_print_string(LINE_1, print_buffer);
	LCD_print_integer(LINE_2, 999);
//	lcd_clear_line(LINE_1);

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
