/*
 * helper.c
 *
 *  Created on: Jul 23, 2017
 *      Author: donald
 */


void blink_led(int number_of_blinks, int led)
{
	int count = 0;
	int blink_count = 2*number_of_blinks - 1;
	while (blink_count >= 0)
	{
		Board_LED_Toggle(led);
		while(count <= 1000000)
		{
			count ++;
		}
		count = 0;
		blink_count --;
	}
}

void delay(int ticks)
{
	int i = 0;
	while (i <= ticks)
	{
		i++;
	}
}
