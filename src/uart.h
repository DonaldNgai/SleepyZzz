/*
 * uart.h
 *
 *  Created on: Jul 21, 2017
 *      Author: donald
 */

#ifndef UART_H_
#define UART_H_

void Init_UART_PinMux(void);
void setupUART(void);
void putLineUART(const char *send_data);
void getLineUART(char *receive_buffer, uint32_t length);

#endif /* UART_H_ */
