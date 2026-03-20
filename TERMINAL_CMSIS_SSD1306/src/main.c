/**
  ******************************************************************************
  * @file    main.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Main program body for STM32F10x microcontroller.
	******************************************************************************
 **/

#include "stm32f10x.h"
#include "rcc.h"
#include "sys_tick.h"
#include "usart.h"
#include "i2c.h"
#include "ssd1306.h"
#include "window_handler.h"
#include "tim3_handler.h"
#include "graph_label_handler.h"
#include "acp.h"

uint8_t SYSTEM_MODE = 0;
uint8_t SYSTEM_MODE_INTERRUPT = 0;

int main(void) {
  
  // Enable clocks for GPIOA, GPIOB, GPIOC, USART1, and I2C1
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
  AFIO->EXTICR[3] &= ~AFIO_EXTICR4_EXTI15_PA;
	EXTI->FTSR |= EXTI_FTSR_TR15;
	EXTI->IMR |= EXTI_IMR_MR15;
  EXTI->RTSR &= ~EXTI_RTSR_TR15;
	NVIC_EnableIRQ(EXTI15_10_IRQn);
  NVIC_SetPriority(EXTI15_10_IRQn, 1);
    
  ConfigurationInstanceRCC Config = {
    .SELCLK = HSE,
	  .SW = RCC_CFGR_SW_PLL,
	  .PLLMUL = RCC_CFGR_PLLMULL9,
	  .PLLSRC = RCC_CFGR_PLLSRC_HSE,
	  .CSSEN = RCC_CR_CSSON,
	  .AHBDIV = RCC_CFGR_HPRE_DIV1,
	  .APB1DIV = RCC_CFGR_PPRE1_DIV2,
	  .APB2DIV = RCC_CFGR_PPRE2_DIV1
  };
    
  ConfigurateRCC(Config);
  SysTick_Init(MS);
  ADC_Init();
  ConfigureButtonPin();

  USART1_Init(7500); // Initialize USART1 with baud rate of 115200 (assuming PCLK2 is 72 MHz)
  USART1_SystemStartup();

  TIM3_Start(50);

  I2CInit(); // Initialize I2C1
  SSD1306_Init(); // Initialize the SSD1306 OLED display
  SSD1306_UpdateScreen(); // Update the display with initial buffer content

  Window_Init(
    1, 
    appModes.mode1[0].x, 
    appModes.mode1[0].y, 
    appModes.mode1[0].width, 
    appModes.mode1[0].height);
  Graph_label_init(1, 0, 0);
  Window_Render(1);

  Window_Init(
  2, 
  appModes.mode1[1].x, 
  appModes.mode1[1].y, 
  appModes.mode1[1].width, 
  appModes.mode1[1].height);
  Window_Draw_Raretorch(2, 0);
  Window_Render(2);

  Window_Init(
  3, 
  appModes.mode1[2].x, 
  appModes.mode1[2].y, 
  appModes.mode1[2].width, 
  appModes.mode1[2].height);
  Graph_label_init(3, 0, 1);
  Window_Render(3);
  SSD1306_UpdateScreen();

  while (1) {
    if(TIM3_INTERRUPT) {

        TIM3_INTERRUPT = 0;
        uint16_t adcv = (uint16_t)ADCValue();
        curResist = (adcv + 1) / 4;
        if(curResist > 990){
          curResist = 990;
        } else if (curResist < 20) {
          curResist = 0;
        }
        curResist /= 10;

        if (SYSTEM_MODE_INTERRUPT) {
          if (SYSTEM_MODE == 1) {
              Window_TransformObject(
                1, 
                1, 
                0, 
                appModes.mode1[0].x + LEFT_OFFSET, 
                appModes.mode1[0].y + UPPER_OFFSET, 
                appModes.mode1[0].x + appModes.mode1[0].width - RIGHT_OFFSET, 
                appModes.mode1[0].y + appModes.mode1[0].height - DOWN_OFFSET, 
                appModes.mode2[0].x - (appModes.mode1[0].width - appModes.mode2[0].width) + LEFT_OFFSET, 
                appModes.mode2[0].y + UPPER_OFFSET, 
                appModes.mode2[0].x + appModes.mode2[0].width - RIGHT_OFFSET, 
                appModes.mode2[0].y + appModes.mode2[0].height - DOWN_OFFSET);
              Window_Change(
                1, 
                appModes.mode2[0].x, 
                appModes.mode2[0].y, 
                appModes.mode2[0].width, 
                appModes.mode2[0].height);
              Graph_label_init(1, curResist, 0);

              Window_TransformObject(
                2, 
                1, 
                0, 
                appModes.mode1[1].x + LEFT_OFFSET, 
                appModes.mode1[1].y + UPPER_OFFSET, 
                appModes.mode1[1].x + appModes.mode1[1].width - RIGHT_OFFSET, 
                appModes.mode1[1].y + appModes.mode1[1].height - DOWN_OFFSET, 
                appModes.mode2[1].x - (appModes.mode1[1].width - appModes.mode2[1].width) + LEFT_OFFSET, 
                appModes.mode2[1].y + UPPER_OFFSET, 
                appModes.mode2[1].x + appModes.mode2[1].width - RIGHT_OFFSET, 
                appModes.mode2[1].y + appModes.mode2[1].height - DOWN_OFFSET);
              Window_Change(
                2, 
                appModes.mode2[1].x, 
                appModes.mode2[1].y, 
                appModes.mode2[1].width, 
                appModes.mode2[1].height);
              Window_Clear(2);
              Window_Draw_Raretorch(2, 40);

              Window_TransformObject(
                3, 
                1, 
                0, 
                appModes.mode1[2].x + LEFT_OFFSET, 
                appModes.mode1[2].y + UPPER_OFFSET, 
                appModes.mode1[2].x + appModes.mode1[2].width - RIGHT_OFFSET, 
                appModes.mode1[2].y + appModes.mode1[2].height - DOWN_OFFSET, 
                appModes.mode2[2].x - (appModes.mode1[2].width - appModes.mode2[2].width) + LEFT_OFFSET, 
                appModes.mode2[2].y + UPPER_OFFSET, 
                appModes.mode2[2].x + appModes.mode2[2].width - RIGHT_OFFSET, 
                appModes.mode2[2].y + appModes.mode2[2].height - DOWN_OFFSET);
              Window_Change(
                3, 
                appModes.mode2[2].x, 
                appModes.mode2[2].y, 
                appModes.mode2[2].width, 
                appModes.mode2[2].height);
              Graph_label_init(3, 100 - curResist, 1);
          } else if (SYSTEM_MODE == 2){
              Window_TransformObject(
                1, 
                1, 
                0, 
                appModes.mode2[0].x + LEFT_OFFSET, 
                appModes.mode2[0].y + UPPER_OFFSET, 
                appModes.mode2[0].x + appModes.mode2[0].width - RIGHT_OFFSET, 
                appModes.mode2[0].y + appModes.mode2[0].height - DOWN_OFFSET, 
                appModes.mode3[0].x - (appModes.mode2[0].width - appModes.mode3[0].width) + LEFT_OFFSET, 
                appModes.mode3[0].y + UPPER_OFFSET, 
                appModes.mode3[0].x + appModes.mode3[0].width - RIGHT_OFFSET, 
                appModes.mode3[0].y + appModes.mode3[0].height - DOWN_OFFSET);
              Window_Change(
                1, 
                appModes.mode3[0].x, 
                appModes.mode3[0].y, 
                appModes.mode3[0].width, 
                appModes.mode3[0].height);
              Graph_label_init(1, curResist, 0);

              Window_TransformObject(
                2, 
                1, 
                0, 
                appModes.mode2[1].x + LEFT_OFFSET, 
                appModes.mode2[1].y + UPPER_OFFSET, 
                appModes.mode2[1].x + appModes.mode2[1].width - RIGHT_OFFSET, 
                appModes.mode2[1].y + appModes.mode2[1].height - DOWN_OFFSET, 
                appModes.mode3[1].x - (appModes.mode2[1].width - appModes.mode3[1].width) + LEFT_OFFSET, 
                appModes.mode3[1].y + UPPER_OFFSET, 
                appModes.mode3[1].x + appModes.mode3[1].width - RIGHT_OFFSET, 
                appModes.mode3[1].y + appModes.mode3[1].height - DOWN_OFFSET);
              Window_Change(
                2, 
                appModes.mode3[1].x, 
                appModes.mode3[1].y, 
                appModes.mode3[1].width, 
                appModes.mode3[1].height);
              Window_Clear(2);
              Window_Draw_Raretorch(2, 40);

              Window_TransformObject(
                3, 
                1, 
                0, 
                appModes.mode2[2].x + LEFT_OFFSET, 
                appModes.mode2[2].y + UPPER_OFFSET, 
                appModes.mode2[2].x + appModes.mode2[2].width - RIGHT_OFFSET, 
                appModes.mode2[2].y + appModes.mode2[2].height - DOWN_OFFSET, 
                appModes.mode3[2].x - (appModes.mode2[2].width - appModes.mode3[2].width) + LEFT_OFFSET, 
                appModes.mode3[2].y + UPPER_OFFSET, 
                appModes.mode3[2].x + appModes.mode3[2].width - RIGHT_OFFSET, 
                appModes.mode3[2].y + appModes.mode3[2].height - DOWN_OFFSET);
              Window_Change(
                3, 
                appModes.mode3[2].x, 
                appModes.mode3[2].y, 
                appModes.mode3[2].width, 
                appModes.mode3[2].height);
              Graph_label_init(3, 100 - curResist, 1);
          } else if (SYSTEM_MODE == 0){
              Window_TransformObject(
                1, 
                1, 
                0, 
                appModes.mode3[0].x + LEFT_OFFSET, 
                appModes.mode3[0].y + UPPER_OFFSET, 
                appModes.mode3[0].x + appModes.mode3[0].width - RIGHT_OFFSET, 
                appModes.mode3[0].y + appModes.mode3[0].height - DOWN_OFFSET, 
                appModes.mode1[0].x - (appModes.mode3[0].width - appModes.mode1[0].width) + LEFT_OFFSET, 
                appModes.mode1[0].y + UPPER_OFFSET, 
                appModes.mode1[0].x + appModes.mode1[0].width - RIGHT_OFFSET, 
                appModes.mode1[0].y + appModes.mode1[0].height - DOWN_OFFSET);
              Window_Change(
                1, 
                appModes.mode1[0].x, 
                appModes.mode1[0].y, 
                appModes.mode1[0].width, 
                appModes.mode1[0].height);
              Graph_label_init(1, curResist, 0);

              Window_TransformObject(
                2, 
                1, 
                0, 
                appModes.mode3[1].x + LEFT_OFFSET, 
                appModes.mode3[1].y + UPPER_OFFSET, 
                appModes.mode3[1].x + appModes.mode3[1].width - RIGHT_OFFSET, 
                appModes.mode3[1].y + appModes.mode3[1].height - DOWN_OFFSET, 
                appModes.mode1[1].x - (appModes.mode3[1].width - appModes.mode1[1].width) + LEFT_OFFSET, 
                appModes.mode1[1].y + UPPER_OFFSET, 
                appModes.mode1[1].x + appModes.mode1[1].width - RIGHT_OFFSET, 
                appModes.mode1[1].y + appModes.mode1[1].height - DOWN_OFFSET);
              Window_Change(
                2, 
                appModes.mode1[1].x, 
                appModes.mode1[1].y, 
                appModes.mode1[1].width, 
                appModes.mode1[1].height);
              Window_Clear(2);
              Window_Draw_Raretorch(2, 2);

              Window_TransformObject(
                3, 
                1, 
                0, 
                appModes.mode3[2].x + LEFT_OFFSET, 
                appModes.mode3[2].y + UPPER_OFFSET, 
                appModes.mode3[2].x + appModes.mode3[2].width - RIGHT_OFFSET, 
                appModes.mode3[2].y + appModes.mode3[2].height - DOWN_OFFSET, 
                appModes.mode1[2].x - (appModes.mode3[2].width - appModes.mode1[2].width) + LEFT_OFFSET, 
                appModes.mode1[2].y + UPPER_OFFSET, 
                appModes.mode1[2].x + appModes.mode1[2].width - RIGHT_OFFSET, 
                appModes.mode1[2].y + appModes.mode1[2].height - DOWN_OFFSET);
              Window_Change(
                3, 
                appModes.mode1[2].x, 
                appModes.mode1[2].y, 
                appModes.mode1[2].width, 
                appModes.mode1[2].height);
              Graph_label_init(3, 100 - curResist, 1);
          }
          SYSTEM_MODE_INTERRUPT = 0;
        }
        Graph_Update(1, curResist);
        Graph_Update(3, 100 - curResist);
        Window_Render(1);
        Window_Render(2);
        Window_Render(3);
        SSD1306_UpdateScreen();
    }
  }
}

void EXTI15_10_IRQHandler(void){
  if (EXTI->PR & EXTI_PR_PR15) {
    EXTI->PR |= EXTI_PR_PR15;
    if (debouncer == 10){
      if (SYSTEM_MODE > 2) {
        SYSTEM_MODE = 0;
        SYSTEM_MODE_INTERRUPT = 1;
        debouncer = 0;
      } else {
        SYSTEM_MODE++;
        SYSTEM_MODE_INTERRUPT = 1;
        debouncer = 0;
      }
    }
  }
}