/*
 * i2c.h
 *
 *  Created on: Jul 11, 2017
 *      Author: donald
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdbool.h>

void Init_I2C_PinMux(void);
void setupI2CMaster(void);
void sendI2CMaster(uint16_t i2c_addr, uint32_t ledStateOut);
void SetupXferRecAndExecute(uint8_t, uint8_t*, uint16_t, uint8_t*, uint16_t);

#endif /* I2C_H_ */
