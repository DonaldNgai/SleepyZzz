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
static uint8_t rx_packet[NUMBER_OF_ANALOG_INPUTS+1];

void get_sensor_values(sensor_values_t* sensor_struct)
{
	tx_packet = 0;
	enable_ADC_auto_increment();
	get_analog_inputs();
	sensor_struct->temperature = 	rx_packet[1];
	sensor_struct->heart_rate = 	rx_packet[2];
	sensor_struct->orientation = 	rx_packet[3];
	sensor_struct->other = 			rx_packet[4];
}

void get_analog_inputs()
{
	SetupXferRecAndExecute(ADC_I2C_ADDRESS, &tx_packet, 1, rx_packet, NUMBER_OF_ANALOG_INPUTS+1);
}

uint8_t get_analog_input(int port)
{
	tx_packet = port;
	SetupXferRecAndExecute(ADC_I2C_ADDRESS, &tx_packet, 1, rx_packet, 2);
	return rx_packet[1];
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

float convert_temperature(int temp){
	return ((temp-8)*1.34)-41;
}

