/**
  ******************************************************************************
  * @file    tim2_handler.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   TIM2 handler functions.
	******************************************************************************
 **/

#ifndef __TIM2_HANDLER_H
#define __TIM2_HANDLER_H

#include <stdint.h>

extern uint8_t TIM2_ENDUP_FLAG;

void TIM2_Start(uint16_t interval_ms);
void TIM2_IRQHandler(void);

#endif /* __TIM2_HANDLER_H */