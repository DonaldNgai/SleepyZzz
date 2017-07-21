/*
 * uart.c
 *
 *  Created on: Jul 21, 2017
 *      Author: donald
 */


#include "board.h"
#include <string.h>

/*Had to add dialout to groups in order to connect to serial port via putty
 * https://github.com/simondlevy/BreezyLidar/issues/3
 * Also, TX:6 and RX:1 is for connecting UART to the usb connected to host pc
 * TX:4 and RX:0 will connect the UART to the digital pins 0 and 1
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* UART handle and memory for ROM API */
static UART_HANDLE_T *uartHandle;

/* Receive buffer */
#define RECV_BUFF_SIZE 32

/* ASCII code for escape key */
#define ESCKEY			27
#define	UART_CLOCK_DIV	1

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Turn on LED to indicate an error */
static void errorUART(void)
{
	Board_LED_Set(0, true);
	while (1) {}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* UART Pin mux function - note that SystemInit() may already setup your
   pin muxing at system startup */
void Init_UART_PinMux(CHIP_SWM_PIN_MOVABLE_T UART_TX, uint8_t TX_PIN, CHIP_SWM_PIN_MOVABLE_T UART_RX, uint8_t RX_PIN)
{
	/* Enable the clock to the Switch Matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);

	Chip_Clock_SetUARTClockDiv(UART_CLOCK_DIV);

	/* Connect the U0_TXD_O and U0_RXD_I signals to port pins(P0.4, P0.0) */
	Chip_SWM_DisableFixedPin(SWM_FIXED_ACMP_I1);
	Chip_SWM_MovablePinAssign(UART_TX, TX_PIN);
	Chip_SWM_MovablePinAssign(UART_RX, RX_PIN);

	/* Disable the clock to the Switch Matrix to save power */
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}

/* Setup UART handle and parameters */
void setupUART(uint32_t *uartHandleMEM, uint8_t mem_size, UART_CONFIG_T cfg)
{
	uint32_t frg_mult;

	/* Perform a sanity check on the storage allocation */
	if (LPC_UARTD_API->uart_get_mem_size() > mem_size) {
		/* Example only: this should never happen and probably isn't needed for
		   most UART code. */
		errorUART();
	}

	/* Setup the UART handle */
	uartHandle = LPC_UARTD_API->uart_setup((uint32_t) LPC_USART0, (uint8_t *) uartHandleMEM);
	if (uartHandle == NULL) {
		errorUART();
	}

	/* Need to tell UART ROM API function the current UART peripheral clock speed */
	cfg.sys_clk_in_hz = Chip_Clock_GetMainClockRate()/UART_CLOCK_DIV;

	/* Initialize the UART with the configuration parameters */
	frg_mult = LPC_UARTD_API->uart_init(uartHandle, &cfg);
	if (frg_mult) {
		Chip_SYSCTL_SetUSARTFRGDivider(0xFF);	/* value 0xFF should be always used */
		Chip_SYSCTL_SetUSARTFRGMultiplier(frg_mult);
	}
}

/* Send a string on the UART terminated by a NULL character using
   polling mode. */
void putLineUART(const char *send_data)
{
	UART_PARAM_T param;

	param.buffer = (uint8_t *) send_data;
	param.size = strlen(send_data);

	/* Polling mode, do not append CR/LF to sent data */
	param.transfer_mode = TX_MODE_SZERO;
	param.driver_mode = DRIVER_MODE_POLLING;

	/* Transmit the data */
	if (LPC_UARTD_API->uart_put_line(uartHandle, &param)) {
		errorUART();
	}
}

/* Receive a string on the UART terminated by a LF character using
   polling mode. */
void getLineUART(char *receive_buffer, uint32_t length)
{
	UART_PARAM_T param;

	param.buffer = (uint8_t *) receive_buffer;
	param.size = length;

	/* Receive data up to the CR/LF character in polling mode. Will
	   truncate at length if too long.	*/
	param.transfer_mode = RX_MODE_CRLF_RECVD;
	param.driver_mode = DRIVER_MODE_POLLING;

	/* Receive the data */
	if (LPC_UARTD_API->uart_get_line(uartHandle, &param)) {
		errorUART();
	}
}
