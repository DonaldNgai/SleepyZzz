/*
 * helper.h
 *
 *  Created on: Jul 23, 2017
 *      Author: donald
 */

#ifndef PRINT_H_
#define PRINT_H_

//#define ENABLE_DECIMAL

void setup_usb_console(void);
void print_to_console(char* string);
void get_line_from_console(char* recv_buffer, int buffer_size);
void echo_to_console(void);
int my_sprintf(char *file, char *fmt, ...);

#endif /* PRINT_H_ */
