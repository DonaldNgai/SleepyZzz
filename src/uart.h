/*
 * uart.h
 *
 *  Created on: Jul 21, 2017
 *      Author: donald
 */

#ifndef UART_H_
#define UART_H_

void Init_UART_PinMux(CHIP_SWM_PIN_MOVABLE_T UART_TX, uint8_t tx_pin, CHIP_SWM_PIN_MOVABLE_T UART_RX, uint8_t rx_pin);
void setupUART(uint32_t uartBase, UART_HANDLE_T** uartHandle, uint32_t *uartHandleMEM, uint8_t mem_size, UART_CONFIG_T cfg);
void putLineUART(UART_HANDLE_T* uartHandle, const char *send_data);
void getLineUART(UART_HANDLE_T* uartHandle, char *receive_buffer, uint32_t length);

#endif /* UART_H_ */
