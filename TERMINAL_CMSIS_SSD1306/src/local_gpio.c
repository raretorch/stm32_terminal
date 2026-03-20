/**
  ******************************************************************************
  * @file    gpio.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   GPIO (General Purpose Input/Output) configuration functions.
	******************************************************************************
 **/

#include "gpio.h"
#include "stm32f10x.h"

void ConfigureUART1Pins(void) {
  // Configure PA9 (TX) as Alternate Function Push-Pull
  GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9); // Clear mode and CNF bits
  GPIOA->CRH |= (GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0); // Output mode, max speed 50 MHz
  GPIOA->CRH |= GPIO_CRH_CNF9_1; // Alternate function push-pull
  
  // Configure PA10 (RX) as Input Floating
  GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10); // Clear mode and CNF bits
  GPIOA->CRH |= GPIO_CRH_CNF10_0; // Input floating
}

void ConfigureI2C1Pins(void) {
  // Configure PB6 (SCL) and PB7 (SDA) as Alternate Function Open-Drain
  GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7); // Clear mode and CNF bits
  GPIOB->CRL |= (GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0); // Output mode, max speed 50 MHz
  GPIOB->CRL |= (GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_1 | GPIO_CRL_CNF7_0); // Alternate function open-drain
  GPIOB->BSRR |= GPIO_BSRR_BS6 | GPIO_BSRR_BS7; // Set PB6 and PB7 high (idle state for I2C)
}

void ConfigureADCPin(void) {
  GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
}

void ConfigureButtonPin(void) {
  GPIOA->CRH &= ~(GPIO_CRH_MODE15 | GPIO_CRH_CNF15);
	GPIOA->CRH |= (0x8U << 28);
	GPIOA->BSRR |= GPIO_BSRR_BS15;
}

void ConfigureGPIOC13Pin(void) {
  // Configure PC13 as Output Push-Pull
  GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13); // Clear mode and CNF bits
  GPIOC->CRH |= (GPIO_CRH_MODE13_1); // Output mode, max speed 2 MHz
  GPIOC->CRH |= GPIO_CRH_CNF13_0; // Push-pull
  GPIOC->BSRR |= GPIO_BSRR_BS13;
}

void EnableGPIOC13Pin(void) {
  GPIOC->BSRR |= GPIO_BSRR_BR13; // Set PC13 low (turn on LED if connected)
}

void DisableGPIOC13Pin(void) {
  GPIOC->BSRR |= GPIO_BSRR_BS13; // Set PC13 high (turn off LED if connected)
}