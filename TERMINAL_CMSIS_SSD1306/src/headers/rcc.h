/**
  ******************************************************************************
  * @file    rcc.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Header file for RCC (Reset and Clock Control) configuration.
	******************************************************************************
 **/

#ifndef __RCC_H
#define __RCC_H

#include <stdint.h>

#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */


/**/
typedef struct {
	uint32_t SW;
	uint32_t PLLMUL;
	uint32_t PLLSRC;
	uint32_t CSSEN;
	uint32_t AHBDIV;
	uint32_t APB1DIV;
	uint32_t APB2DIV;
	enum CLK {HSE, LSE, HSI, LSI} SELCLK;
} ConfigurationInstanceRCC;
/**/

extern ConfigurationInstanceRCC rccConfig;
extern uint32_t SYSTEM_CORE_CLOCK;

void ConfigurateRCC(ConfigurationInstanceRCC);

uint32_t GetSystemCoreClock(void);

#endif