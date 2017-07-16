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
/* Turn on LED to indicate an error */
static void errorI2C(void)
{
	Board_LED_Set(0, true);
	while (1) {}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
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

/* Master transfer state change handler handler */
uint32_t Chip_I2CM_XferHandlerMine(LPC_I2C_T *pI2C, I2CM_XFER_T *xfer)
{

	uint32_t status = Chip_I2CM_GetStatus(pI2C);
	/* Master Lost Arbitration */
	if (status & I2C_STAT_MSTRARBLOSS) {
		/* Set transfer status as Arbitration Lost */
		xfer->status = I2CM_STATUS_ARBLOST;
		/* Clear Status Flags */
		Chip_I2CM_ClearStatus(pI2C, I2C_STAT_MSTRARBLOSS);
	}
	/* Master Start Stop Error */
	else if (status & I2C_STAT_MSTSTSTPERR) {
		/* Set transfer status as Bus Error */
		xfer->status = I2CM_STATUS_BUS_ERROR;
		/* Clear Status Flags */
		Chip_I2CM_ClearStatus(pI2C, I2C_STAT_MSTSTSTPERR);
	}
	/* Master is Pending */
	else if (status & I2C_STAT_MSTPENDING) {
		/* Branch based on Master State Code */
		switch (Chip_I2CM_GetMasterState(pI2C)) {
		/* Master idle */
		case I2C_STAT_MSTCODE_IDLE:
			/* Do Nothing */
			break;

		/* Receive data is available */
		case I2C_STAT_MSTCODE_RXREADY:
			/* Read Data */
			*xfer->rxBuff++ = pI2C->MSTDAT;
			xfer->rxSz--;
			if (xfer->rxSz) {
				/* Set Continue if there is more data to read */
				Chip_I2CM_MasterContinue(pI2C);
			}
			else {
				/* Set transfer status as OK */
				xfer->status = I2CM_STATUS_OK;
				/* No data to read send Stop */
				Chip_I2CM_SendStop(pI2C);
			}
			break;

		/* Master Transmit available */
		case I2C_STAT_MSTCODE_TXREADY:
			if (xfer->txSz) {
				/* If Tx data available transmit data and continue */
				pI2C->MSTDAT = *xfer->txBuff++;
				xfer->txSz--;
				Chip_I2CM_MasterContinue(pI2C);
			}
			else {
				/* If receive queued after transmit then initiate master receive transfer*/
				if (xfer->rxSz) {
					/* Write Address and RW bit to data register */
					Chip_I2CM_WriteByte(pI2C, (xfer->slaveAddr << 1) | 0x1);
					/* Enter to Master Transmitter mode */
					Chip_I2CM_SendStart(pI2C);
				}
				else {
					/* If no receive queued then set transfer status as OK */
					xfer->status = I2CM_STATUS_OK;
					/* Send Stop */
					Chip_I2CM_SendStop(pI2C);
				}
			}
			break;

		case I2C_STAT_MSTCODE_NACKADR:
			/* Set transfer status as NACK on address */
			xfer->status = I2CM_STATUS_NAK_ADR;
			Chip_I2CM_SendStop(pI2C);
			break;

		case I2C_STAT_MSTCODE_NACKDAT:
			/* Set transfer status as NACK on data */
			xfer->status = I2CM_STATUS_NAK_DAT;
			Chip_I2CM_SendStop(pI2C);
			break;

		default:
			/* Default case should not occur*/
			xfer->status = I2CM_STATUS_ERROR;
			break;
		}
	}
	else {
		/* Default case should not occur */
		xfer->status = I2CM_STATUS_ERROR;
	}

	return xfer->status != I2CM_STATUS_BUSY;
}

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

/* Function to setup and execute I2C transfer request */
void SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize)
{
	int count = 0;
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;

//	Chip_I2CM_XferBlocking(LPC_I2C, &i2cmXferRec);.

	Board_LED_Set(1, (LPC_I2C->STAT & I2C_STAT_MSTPENDING));
	while(count <= 5000000)
	{
		count ++;
	}

	uint32_t ret = 0;
	/* start transfer */
	Chip_I2CM_Xfer(LPC_I2C, &i2cmXferRec);

	while (ret == 0) {
//		blink_led((LPC_I2C->STAT & I2C_STAT_MSTPENDING));
		Board_LED_Set(1, (LPC_I2C->STAT & I2C_STAT_MSTPENDING));
		ret = Chip_I2CM_XferHandler(LPC_I2C, &i2cmXferRec);
		/* wait for status change interrupt */
		while (!Chip_I2CM_IsMasterPending(LPC_I2C)) {
			// Not pending
//			blink_led(1);
			blink_led((LPC_I2C->STAT & 0x7 << 1), count);
			count = count + 1 % 3;
		}
		/* call state change handler */


	}

}

/* Function sends update to the I/O expander */
void sendI2CMaster(uint16_t i2c_addr, uint32_t ledStateOut)
{
	int index = 0;

	txData[index++] = (uint8_t) 0x16;							/* I2C device regAddr */
	txData[index++] = (uint8_t) ((ledStateOut)     & 0xff);		/* I2C device regVal */
	txData[index++] = (uint8_t) ((ledStateOut >> 8)  & 0xff);		/* I2C device regVal */
	txData[index++] = (uint8_t) ((ledStateOut >> 16) & 0xff);		/* I2C device regVal */
	txData[index++] = (uint8_t) ((ledStateOut >> 24) & 0xff);		/* I2C device regVal */

	SetupXferRecAndExecute(i2c_addr, txData, 5, rxData, 0);
}

