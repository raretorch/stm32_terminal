/**
  ******************************************************************************
  * @file    local_i2c.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   I2C driver functions.
	******************************************************************************
 **/

#include "gpio.h"
#include "i2c.h"
#include "sys_tick.h"
#include "usart.h"
#include "tim2_handler.h"
#include "stm32f10x.h"

void I2CInit(void) {
	ConfigureI2C1Pins(); // Configure GPIO pins for I2C1

	// reset bit to reset I2C from error or for initializing
	I2C1->CR1 |= I2C_CR1_SWRST;
  	I2C1->CR1 &= ~I2C_CR1_SWRST;
	

	I2C1->CR2 = 36; // frequency 36 MHz
	I2C1->CCR = 180;  /*
									  T_sm_mode = 2 * CCR * T_pclk = 1 / 200000
									  CCR = T_sm_mode / 2 * T_pclk = 36000000 / 200000 = 180
									  */
	
	I2C1->TRISE = 37; /*
										T_pclk = 1 / 36000000 => nanoseconds
										T_pclk = 1000000000 / 36000000 = 27.78 ns
										trise = 1000 / 27.78 = 36.997
										*/
	// enable peripherial
	I2C1->CR1 |= I2C_CR1_PE;
}

uint8_t I2CWrite(uint8_t Device_adress, uint8_t *data, uint16_t length){

	I2C1->CR1 &= ~I2C_CR1_STOP;
	I2C1->CR1 |= I2C_CR1_ACK;
	
	TIM2_Start(20);
	while(I2C1->SR2 & I2C_SR2_BUSY){
		if(TIM2_ENDUP_FLAG){
			I2C1->CR1 &= ~I2C_CR1_PE;
			USART1_DebugSendString("I2C: Line is busy, turn of I2C.\n");
			break;
		}
	} // wait until bus is not busy

	I2C1->CR1 |= I2C_CR1_PE;
  	I2C1->CR1 |= I2C_CR1_START; // send a start bit

	// waiting for start bit condition for timeout ending in ms
	TIM2_Start(200);
	while (!(I2C1->SR1 & I2C_SR1_SB)){
		if (TIM2_ENDUP_FLAG){
			USART1_DebugSendString("I2C: Start bit doesn't set.\n");
			I2C1->CR1 |= I2C_CR1_STOP;
			return 0;
		}
	}
	(void)I2C1->SR1;
		
	I2C1->DR = (uint8_t)(Device_adress << 1) | 0;

	TIM2_Start(50);
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){
		if(TIM2_ENDUP_FLAG){
			USART1_DebugSendString("I2C: Address do not match.\n");
			I2C1->CR1 |= I2C_CR1_STOP;
			return 0;
		}
	}
	(void)I2C1->SR1;
    (void)I2C1->SR2;

	for(uint16_t i = 0; i < length; i++) {
		TIM2_Start(50);
        while(!(I2C1->SR1 & I2C_SR1_TXE)){
			if(TIM2_ENDUP_FLAG){
				if (I2C1->SR1 & I2C_SR1_AF){
					USART1_DebugSendString("I2C: Acknowledge failure.\n");
					return 0;
				}
				USART1_DebugSendString("I2C: Timeout for TXE waiting select next byte.\n");
				break;
			}
		}
        I2C1->DR = data[i];
    }
	
	TIM2_Start(50);
	while(!(I2C1->SR1 & I2C_SR1_BTF)){
		if(TIM2_ENDUP_FLAG){
			USART1_DebugSendString("I2C: Error on last byte transfer\n");
			return 0;
		}
	}
	I2C1->CR1 |= I2C_CR1_STOP;
	return 1;
}