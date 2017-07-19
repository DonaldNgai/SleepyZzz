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
// 30Mhz, divide by 50 will give 600kHz I2C clock rate
#define I2C_CLK_DIVIDER         (40)
/* 100KHz I2C bit-rate */
// This mean 850k of bits per I2C clock
// Hence SCL is Chip_Clock_GetSystemClockRate() / (Chip_I2C_GetClockDiv(pI2C) * busSpeed);
// Because we need to know how many ticks on the main clock need to go by per I2C tick
#define I2C_BITRATE             (85000) //85kHz

/* file local variables */
static volatile int intErrCode;
static volatile uint32_t ticks;

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

	Chip_I2CM_XferBlocking(LPC_I2C, &i2cmXferRec);

}

/* Initializes pin muxing for I2C interface */
void Init_I2C_PinMux(void)
{
	/* Enable the clock to the Switch Matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);

	/* Connect the I2C_SDA and I2C_SCL signals to port pins(P0.10, P0.11) */
	Chip_SWM_MovablePinAssign(SWM_I2C_SDA_IO, 10);
	Chip_SWM_MovablePinAssign(SWM_I2C_SCL_IO, 11);

	/* Enable Fast Mode Plus for I2C pins */
	Chip_IOCON_PinSetI2CMode(LPC_IOCON, IOCON_PIO10, PIN_I2CMODE_FASTPLUS);
	Chip_IOCON_PinSetI2CMode(LPC_IOCON, IOCON_PIO11, PIN_I2CMODE_FASTPLUS);

	/* Disable the clock to the Switch Matrix to save power */
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}

