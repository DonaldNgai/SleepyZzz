/*
 * helper.c
 *
 *  Created on: Jul 19, 2017
 *      Author: donald
 */


void blink_led(int number_of_blinks, int led)
{
//	Board_LED_Set(0, true);
//	Board_LED_Set(2, false);
	int count = 0;
	int blink_count = 2*number_of_blinks - 1;
	while (blink_count >= 0)
	{

//		Board_LED_Set(2, true);
		Board_LED_Toggle(led);
		while(count <= 1000000)
		{
			count ++;
		}
		count = 0;
		blink_count --;
	}
//	Board_LED_Set(1, true);
}



