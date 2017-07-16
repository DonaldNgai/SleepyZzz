/*
 * switch_matrix.c
 *
 *  Created on: Jul 14, 2017
 *      Author: donald
 */

#include "board.h"

#define NUMBER_OF_PIN_ASSIGNS	8
#define PIN_ASSIGN_RESET_VALUE	0xFFFFFFFF
#define PIN_ENABLE_RESET_VALUE	0x1B3
#define SDA_PIN	 10
#define SCL_PIN	 11


void reset_switch_matrix()
{
	int count;

	/* Enable the clock to the Switch Matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);

	//Reset Pin Assignments
	for (count = 0; count < NUMBER_OF_PIN_ASSIGNS; count++)
	{
		LPC_SWM->PINASSIGN[count] = PIN_ASSIGN_RESET_VALUE;
	}

	//Reset Pin Functions
	LPC_SWM->PINENABLE0 = PIN_ENABLE_RESET_VALUE;

	/* Disable the clock to the Switch Matrix to save power */
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}

void assign_I2C_pins()
{
	//Assign SDA
	LPC_SWM->PINASSIGN[7] = SDA_PIN << 24;

	//Assign SCL
	LPC_SWM->PINASSIGN[8] = SCL_PIN;
}


