/**
  ******************************************************************************
  * @file    i2c.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   I2C driver functions.
	******************************************************************************
 **/

#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>



void I2CInit(void);
uint8_t I2CWrite(uint8_t Device_adress, uint8_t *data, uint16_t length);

#endif /* __I2C_H */