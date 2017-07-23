/*
 * helper.c
 *
 *  Created on: Jul 19, 2017
 *      Author: donald
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "uart.h"

//static char print_buffer[512];
//static char recv1_buf[32];

/* UART handle and memory for ROM API */
UART_HANDLE_T* uart1Handle;

/* Use a buffer size larger than the expected return value of
   uart_get_mem_size() for the static UART handle type */
uint32_t uart1HandleMEM[0x10];

#ifdef ENABLE_DECIMAL
static void ftoa_fixed(char *buffer, double value);
static void ftoa_sci(char *buffer, double value);
#endif
//	/* Get a string for the UART and echo it back to the caller. Data is NOT
//	   echoed back via the UART using this function. */
//	getLineUART(uart1Handle, recv1_buf, sizeof(recv1_buf));
void print_to_console(char* string){
//	strcat(string,"\n\n");
	putLineUART(uart1Handle, string);
}

void setup_usb_console(void){
	/*		USB DEBUG INIT		*/

	/* 115.2KBPS, 8N1, ASYNC mode, no errors, clock filled in later */
	UART_CONFIG_T cfg1 = {
		0,				/* U_PCLK frequency in Hz */
		115200,			/* Baud Rate in Hz */
		1,				/* 8N1 */
		0,				/* Asynchronous Mode */
		NO_ERR_EN		/* Enable No Errors */
	};

	Init_UART_PinMux(SWM_U1_TXD_O,6,SWM_U1_RXD_I,1);
	Chip_UART_Init(LPC_USART1);
	/* Allocate UART handle, setup UART parameters, and initialize UART clocking */
	setupUART((uint32_t) LPC_USART1, &uart1Handle, uart1HandleMEM, sizeof(uart1HandleMEM), cfg1);

}

int my_vsprintf(char *file, char *fmt, va_list arg) {

    int int_temp;
    char char_temp;
    char *string_temp;

#ifdef ENABLE_DECIMAL
    double double_temp;
#endif

    char ch;
    int index = 0;
    int length = 0;

    char buffer[512];

    strcpy(file,"\0"); // Clear string to concatenate with
    ch = fmt[index++];

    while ( ch ) {

        if ( '%' == ch ) {
        	ch = fmt[index++];
            switch (ch) {
                /* %% - print out a single %    */
                case '%':
                    strcat(file, "%");
                    length++;
                    break;

                /* %c: print out a character    */
                case 'c':
                    char_temp = va_arg(arg, int);
                    file[length] = char_temp;
                    file[length+1] = '\0';
                    length++;
                    break;

                /* %s: print out a string       */
                case 's':
                    string_temp = va_arg(arg, char *);
                    strcat(file, string_temp);
                    length += strlen(string_temp);
                    break;

                /* %d: print out an int         */
                case 'd':
                    int_temp = va_arg(arg, int);
                    itoa(int_temp, buffer, 10);
                    strcat(file, buffer);
                    length += strlen(buffer);
                    break;

                /* %x: print out an int in hex  */
                case 'x':
                    int_temp = va_arg(arg, int);
                    itoa(int_temp, buffer, 16);
                    strcat(file, buffer);
                    length += strlen(buffer);
                    break;
#ifdef ENABLE_DECIMAL
                case 'f':
                    double_temp = va_arg(arg, double);
                    ftoa_fixed(buffer, double_temp);
                    strcat(file, buffer);
                    length += strlen(buffer);
                    break;

                case 'e':
                    double_temp = va_arg(arg, double);
                    ftoa_sci(buffer, double_temp);
                    strcat(file, buffer);
                    length += strlen(buffer);
                    break;
#endif
            }
        }
        else {
        	file[length] = ch;
        	file[length+1] = '\0';
            length++;
        }
//        Get next char
        ch = fmt[index++];
    }

    return length;
}

#ifdef ENABLE_DECIMAL
int normalize(double *val) {
    int exponent = 0;
    double value = *val;

    while (value >= 1.0) {
        value /= 10.0;
        ++exponent;
    }

    while (value < 0.1) {
        value *= 10.0;
        --exponent;
    }
    *val = value;
    return exponent;
}

static void ftoa_fixed(char *buffer, double value) {  
    /* carry out a fixed conversion of a double value to a string, with a precision of 5 decimal digits. 
     * Values with absolute values less than 0.000001 are rounded to 0.0
     * Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
     * The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
     * e+308. The C standard requires an implementation to allow a single conversion to produce up to 512 
     * characters, so that's what we really expect as the buffer size.     
     */

    int exponent = 0;
    int places = 0;
    static const int width = 4;

    if (value == 0.0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }         

    if (value < 0.0) {
        *buffer++ = '-';
        value = -value;
    }

    exponent = normalize(&value);

    while (exponent > 0) {
        int digit = value * 10;
        *buffer++ = digit + '0';
        value = value * 10 - digit;
        ++places;
        --exponent;
    }

    if (places == 0)
        *buffer++ = '0';

    *buffer++ = '.';

    while (exponent < 0 && places < width) {
        *buffer++ = '0';
        --exponent;
        ++places;
    }

    while (places < width) {
        int digit = value * 10.0;
        *buffer++ = digit + '0';
        value = value * 10.0 - digit;
        ++places;
    }
    *buffer = '\0';
}

void ftoa_sci(char *buffer, double value) {
	int i = 0;
    int exponent = 0;
    static const int width = 4;

    if (value == 0.0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0.0) {
        *buffer++ = '-';
        value = -value;
    }

    exponent = normalize(&value);

    int digit = value * 10.0;
    *buffer++ = digit + '0';
    value = value * 10.0 - digit;
    --exponent;

    *buffer++ = '.';

    for (i = 0; i < width; i++) {
        int digit = value * 10.0;
        *buffer++ = digit + '0';
        value = value * 10.0 - digit;
    }

    *buffer++ = 'e';
    itoa(exponent, buffer, 10);
}
#endif

int my_sprintf(char *file, char *fmt, ...) {
    va_list arg;
    int length;

    va_start(arg, fmt);
    length = my_vsprintf(file, fmt, arg);
    va_end(arg);
    return length;
}
