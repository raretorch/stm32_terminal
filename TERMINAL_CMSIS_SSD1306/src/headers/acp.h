/**
  ******************************************************************************
  * @file    acp.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   acp handler
	******************************************************************************
**/

#ifndef _ACP_H
#define _ACP_H

    #include "stm32f10x.h"
    #include "gpio.h"
    #include <stdint.h>

    static uint16_t ADCDelay = 100;
    static uint16_t curResist;

    void ADC_Init(){
        ConfigureADCPin();
        ADC1->CR2 |= ADC_CR2_ADON;
        ADC1->CR2 |= ADC_CR2_RSTCAL;
        while(ADC1->CR2 & ADC_CR2_RSTCAL);
        ADC1->CR2 |= ADC_CR2_CAL;
        while(ADC1->CR2 & ADC_CR2_CAL);
        
        ADC1->CR1 = 0;
        ADC1->CR2 &= ~(ADC_CR2_ALIGN | ADC_CR2_CONT | ADC_CR2_EXTTRIG);
        ADC1->SMPR2 |= ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_2;
        
        ADC1->CR2 |= ADC_CR2_ADON;
        
        ADC1->SQR3 = 0;
        ADC1->SQR2 = 0;
        ADC1->SQR1 = 0;
        ADC1->SQR3 |= (5 << 0);   // SQ1 = 5
        ADC1->CR1 &= ~ADC_CR1_SCAN;
    }

    uint32_t ADCValue(void){
	    ADC1->CR2 |= ADC_CR2_ADON;
	    while(!(ADC1->SR & ADC_SR_EOC));
	    return ADC1->DR;
    }

#endif //_ACP_H