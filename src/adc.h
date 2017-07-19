/*
 * adc.h
 *
 *  Created on: Jul 18, 2017
 *      Author: donald
 */

#ifndef ADC_H_
#define ADC_H_

	#define ADC_I2C_ADDRESS	0x0b1001111
	#define ADC_POR_VALUE	0x80h

	typedef enum {
		AIN0,
		AIN1,
		AIN2,
		AIN3,
		NUMBER_OF_ANALOG_INPUTS
	}AIN_VALUE;

	typedef struct sensorValues{
		double temperature;
		double heart_rate;
		double orientation;
		double other;
	}sensor_values_t;

	typedef enum ADC_CONTROL_BYTE{
		AD_CHANNEL_NUMBER,
		AUTO_INCREMENT=2,
		ANALOG_INPUT_PROGRAM,
		ANALOG_OUTPUT_ENABLE=6
	};

	void get_sensor_values(sensor_values_t* sensor_struct);

#endif /* ADC_H_ */
