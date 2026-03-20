/**
  ******************************************************************************
  * @file    tim3_handler.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   TIM3 handler functions.
	******************************************************************************
 **/

#ifndef __TIM3_HANDLER_H
#define __TIM3_HANDLER_H

#include <stdint.h>

extern uint8_t TIM3_INTERRUPT;

void TIM3_Start(uint16_t interval_ms);
void TIM3_IRQHandler(void);

#endif /* __TIM3_HANDLER_H */