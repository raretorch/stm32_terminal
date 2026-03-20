/**
  ******************************************************************************
  * @file    sys_tick.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   SysTick header.
	******************************************************************************
 **/

 #include <stdint.h>

#ifndef SYS_TICK_H
#define SYS_TICK_H

static uint32_t tickCounter;
static uint32_t TicksPerInterrupt;
extern uint32_t debouncer;

typedef enum {
    SEC,
    MS,
    NS
} SysTickMode;

void SysTick_Handler(void);

void Debounce();

void Delay(uint32_t ticks);

void SysTick_Init(SysTickMode);

SysTickMode GetSysTickMode(void);

#endif /* SYS_TICK_H */