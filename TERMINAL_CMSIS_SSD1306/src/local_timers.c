/**
  ******************************************************************************
  * @file    local_timers.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Timer driver functions.
	******************************************************************************
 **/

#include "tim2_handler.h"
#include "tim3_handler.h"
#include "stm32f10x.h"

uint8_t TIM2_ENDUP_FLAG = 0; // Flag to indicate timer has ended
uint8_t TIM3_INTERRUPT = 0;

void TIM2_Start(uint16_t interval_ms) {
    // Enable clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2_ENDUP_FLAG = 0; // Reset the flag before starting the timer

    // Set prescaler and auto-reload for 1 ms tick
    TIM2->PSC = 36000 - 1; // Prescaler to get 2 kHz timer clock (assuming 72 MHz system clock)
    TIM2->ARR = interval_ms * 2 - 1;   // Auto-reload for specified interval (10 kHz / 10 = 1 kHz)

    TIM2->CNT = 0; // Reset counter

    TIM2->CR1 |= TIM_CR1_OPM; // Enable one-pulse mode

    // Enable update interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // Enable TIM2 interrupt in NVIC
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 1);

    // Enable TIM2
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM3_Start(uint16_t interval_ms) {
    // Enable clock for TIM3
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Set prescaler and auto-reload for 1 ms tick
    TIM3->PSC = 36000 - 1; // Prescaler to get 2 kHz timer clock (assuming 72 MHz system clock)
    TIM3->ARR = interval_ms * 2 - 1;   // Auto-reload for specified interval (10 kHz / 10 = 1 kHz)

    TIM3->CNT = 0; // Reset counter

    // Enable update interrupt
    TIM3->DIER |= TIM_DIER_UIE;

    // Enable TIM3 interrupt in NVIC
    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 2);

    // Enable TIM3
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) { // Check for update interrupt
        TIM2->SR &= ~TIM_SR_UIF; // Clear interrupt flag
        TIM2_ENDUP_FLAG = 1; // Set the flag to indicate timer has ended
    }
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) { // Check for update interrupt
        TIM3->SR &= ~TIM_SR_UIF; // Clear interrupt flag
        TIM3_INTERRUPT = 1;
    }
}