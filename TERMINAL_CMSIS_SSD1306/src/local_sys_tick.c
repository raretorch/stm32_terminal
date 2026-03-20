/**
  ******************************************************************************
  * @file    sys_tick.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   SysTick implementation for STM32F10x microcontroller.
	******************************************************************************
 **/

 #include "sys_tick.h"
 #include "rcc.h"
 #include "stm32f10x.h"

 uint32_t debouncer = 0;

 void SysTick_Handler(void) {
    if (tickCounter > 0) {
			tickCounter--;
	}

    if (GPIOA->IDR & GPIO_IDR_IDR15) {
        debouncer--;
        if (debouncer < 0) {
            debouncer = 0;
        }
    } else {
        Debounce();
    }
 }

 void Delay(uint32_t ticks) {
     tickCounter = ticks;
     while (tickCounter);
 }

 void Debounce(){
    debouncer++;
    if (debouncer > 10) {
        debouncer = 10;
    }
 }

 void SysTick_Init(SysTickMode mode) {
     switch (mode) {
         case SEC:
             TicksPerInterrupt = SYSTEM_CORE_CLOCK / 1;
             SysTick_Config(SYSTEM_CORE_CLOCK);
             break;
         case MS:
             TicksPerInterrupt = SYSTEM_CORE_CLOCK / 1000;
             SysTick_Config(SYSTEM_CORE_CLOCK / 1000);
             break;
         case NS:
             TicksPerInterrupt = SYSTEM_CORE_CLOCK / 1000000;
             SysTick_Config(SYSTEM_CORE_CLOCK / 1000000);
             break;
     }
 }

 SysTickMode GetSysTickMode(void) {
     if (TicksPerInterrupt == SYSTEM_CORE_CLOCK / 1) {
         return SEC;
     } else if (TicksPerInterrupt == SYSTEM_CORE_CLOCK / 1000) {
         return MS;
     } else if (TicksPerInterrupt == SYSTEM_CORE_CLOCK / 1000000) {
         return NS;
     }
     return MS; // Default to milliseconds if unknown
 }
