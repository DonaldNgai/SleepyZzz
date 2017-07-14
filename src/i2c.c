/*
 * i2c.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: donald
 */

#include "board.h"
#include "i2c.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* I2CM transfer record */
static I2CM_XFER_T  i2cmXferRec;
/* System clock is set to 24MHz, I2C clock is set to 600kHz */
#define I2C_CLK_DIVIDER         (40)
/* 100KHz I2C bit-rate */
#define I2C_BITRATE             (100000)

/* 7-bit I2C addresses of I/O expander */
/* Note: The ROM code requires the address to be between bits [6:0]
         bit 7 is ignored */
#define I2C_ADDR_7BIT           (0x60)

/* SysTick rate in Hz */
#define TICKRATE_HZ             (1000)

#define TASK_LOOP               while (true)
#define EVENT_LED_BUMP          0x01
#define EVENT_LED_TOGGLE        0x02

/* file local variables */
static volatile int intErrCode;
static volatile uint32_t ticks;
static uint8_t txData[16];
static uint8_t rxData[16];
static uint32_t event_flag;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Function to wait for I2CM transfer completion */
static void WaitForI2cXferComplete(I2CM_XFER_T *xferRecPtr)
{
	/* Test for still transferring data */
	while (xferRecPtr->status == I2CM_STATUS_BUSY) {
		/* Sleep until next interrupt */
		__WFI();
	}
}

///* Function sends update to the I/O expander */
//static void sendI2CMaster(uint16_t i2c_addr, uint32_t ledStateOut)
//{
//	int index = 0;
//
//	txData[index++] = (uint8_t) 0x16;							/* I2C device regAddr */
//	txData[index++] = (uint8_t) ((ledStateOut)     & 0xff);		/* I2C device regVal */
//	txData[index++] = (uint8_t) ((ledStateOut >> 8)  & 0xff);		/* I2C device regVal */
//	txData[index++] = (uint8_t) ((ledStateOut >> 16) & 0xff);		/* I2C device regVal */
//	txData[index++] = (uint8_t) ((ledStateOut >> 24) & 0xff);		/* I2C device regVal */
//
//	SetupXferRecAndExecute(i2c_addr, txData, 5, rxData, 0);
//}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle I2C interrupt by calling I2CM interrupt transfer handler
 * @return	Nothing
 */
void I2C_IRQHandler(void)
{
	/* Call I2CM ISR function with the I2C device and transfer rec */
	Chip_I2CM_XferHandler(LPC_I2C, &i2cmXferRec);
}

/* Initializes pin muxing for I2C interface */
void Init_I2C_PinMux(void)
{
	/* Enable the clock to the Switch Matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);

	/* Connect the I2C_SDA and I2C_SCL signals to port pins(P0.10, P0.11) */
	Chip_SWM_MovablePinAssign(SWM_I2C_SDA_IO, 10);
	Chip_SWM_MovablePinAssign(SWM_I2C_SCL_IO, 11);

	/* Disable the clock to the Switch Matrix to save power */
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}

/* Setup I2C handle and parameters */
void setupI2CMaster()
{
	/* Enable I2C clock and reset I2C peripheral */
	Chip_I2C_Init(LPC_I2C);

	/* Setup clock rate for I2C */
	Chip_I2C_SetClockDiv(LPC_I2C, I2C_CLK_DIVIDER);

	/* Setup I2CM transfer rate */
	Chip_I2CM_SetBusSpeed(LPC_I2C, I2C_BITRATE);

	/* Enable Master Mode */
	Chip_I2CM_Enable(LPC_I2C);
}

/* Function to setup and execute I2C transfer request */
void SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize)
{
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;

//	Using Interrupts
//	Chip_I2CM_Xfer(LPC_I2C, &i2cmXferRec);
//	/* Enable Master Interrupts */
//	Chip_I2C_EnableInt(LPC_I2C, I2C_INTENSET_MSTPENDING | I2C_INTENSET_MSTRARBLOSS | I2C_INTENSET_MSTSTSTPERR);
//	/* Wait for transfer completion */
//	WaitForI2cXferComplete(&i2cmXferRec);
//	/* Clear all Interrupts */
//	Chip_I2C_ClearInt(LPC_I2C, I2C_INTENSET_MSTPENDING | I2C_INTENSET_MSTRARBLOSS | I2C_INTENSET_MSTSTSTPERR);

//  Using Polling
	Chip_I2CM_XferBlocking(LPC_I2C, &i2cmXferRec);
}

