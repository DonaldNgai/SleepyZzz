/*
 * adc.c
 *
 *  Created on: Jul 18, 2017
 *      Author: donald
 */

#include "board.h"
#include "i2c.h"
#include "adc.h"

static uint8_t tx_packet; //Unnecessary
static uint8_t rx_packet[NUMBER_OF_ANALOG_INPUTS];

void get_sensor_values(sensor_values_t* sensor_struct)
{
	enable_ADC_auto_increment();
	get_analog_inputs();
	sensor_struct->temperature = 	rx_packet[0];
	sensor_struct->heart_rate = 	rx_packet[1];
	sensor_struct->orientation = 	rx_packet[2];
	sensor_struct->other = 			rx_packet[3];
}

void get_analog_inputs()
{
	SetupXferRecAndExecute(ADC_I2C_ADDRESS, &tx_packet, 1, rx_packet, NUMBER_OF_ANALOG_INPUTS);
}

//This will reset the A/D Channel Number
void enable_ADC_auto_increment()
{
	tx_packet = 1 << AUTO_INCREMENT;
}

//This will reset the A/D Channel Number
void disable_ADC_auto_increment()
{
	tx_packet = 0 << AUTO_INCREMENT;
}

