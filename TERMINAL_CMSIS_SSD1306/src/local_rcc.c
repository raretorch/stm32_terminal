/**
  ******************************************************************************
  * @file    local_rcc.с
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Source file for local RCC (Reset and Clock Control) configuration.
	******************************************************************************
 **/

#include "rcc.h"
#include "stm32f10x.h"

ConfigurationInstanceRCC rccConfig;
uint32_t SYSTEM_CORE_CLOCK;

void ConfigurateRCC(ConfigurationInstanceRCC current){

	rccConfig = current;
	
	_Bool HSEStarted = 0;
	_Bool PLLStarted = 0;
	
	//check box for HSE settings
	if(current.SELCLK == HSE){
			RCC->CR |= RCC_CR_HSEON;
	} else return;
	
	//delay for OSC started, after 10000 ticks function interrupt
	for(uint32_t latency = 0;; latency++) {
		if(RCC->CR & RCC_CR_HSERDY) {
			HSEStarted = 1;
			break;
		}
		if(latency > 10000){
			RCC->CR &= ~(RCC_CR_HSEON);
			break;
		}
	}
	
	// configuration for PLL, after wait while PLL started, until get 10000 ticks, then return
	if (HSEStarted){
		RCC->CR |= current.PLLSRC | current.PLLMUL;
		RCC->CR |= RCC_CR_PLLON;
		for(uint32_t latency = 0;; latency++) {
			if(RCC->CR & RCC_CR_PLLRDY) {
				PLLStarted =1;
				break;
			}
			if(latency > 10000){
				RCC->CR &= ~(RCC_CR_PLLON);
				RCC->CR &= ~(RCC_CR_HSEON);
				HSEStarted = 0;
				break;
			}
		}
	}
	
	//configurate rcc dividers for ports and enable
	if(HSEStarted & PLLStarted){
		
		FLASH->ACR |= FLASH_ACR_LATENCY_2;
		
		//reset ports configuration bits
		RCC->CFGR &= ~(0x03FFU << 3);
		
		//set ports bits
		RCC->CFGR |= current.APB2DIV
							| current.APB1DIV
							| current.AHBDIV;
		
		//set SYSCLK source to PLL
		RCC->CFGR |= current.SW;
		
		//enabel clock security system
		RCC->CR |= current.CSSEN;
		
		
		while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));
		
		RCC->CR &= ~(RCC_CR_HSION);

        
	}

	SYSTEM_CORE_CLOCK = HSE_VALUE * ((current.PLLMUL >> 18) + 2);
}

uint32_t GetSystemCoreClock(void){
    return SYSTEM_CORE_CLOCK;
}