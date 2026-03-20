/**
  ******************************************************************************
  * @file    gpio.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Header file for GPIO (General Purpose Input/Output) configuration.
	******************************************************************************
 **/

#ifndef __GPIO_H
#define __GPIO_H

void ConfigureUART1Pins(void);

void ConfigureI2C1Pins(void);

void ConfigureGPIOC13Pin(void);

void ConfigureADCPin(void);

void ConfigureButtonPin(void);

void EnableGPIOC13Pin(void);

void DisableGPIOC13Pin(void);

#endif // __GPIO_H