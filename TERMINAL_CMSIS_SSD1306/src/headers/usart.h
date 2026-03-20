/**
  ******************************************************************************
  * @file    usart.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   USART configuration and communication functions for STM32F10x microcontroller.
	******************************************************************************
 **/

#ifndef __LOCAL_USART_H
#define __LOCAL_USART_H

#include <stdint.h>

static uint8_t USART1_RECIEVED_DATA;

uint8_t USART1_GetData(void);

void USART1_Init(uint32_t baudrate);
void USART1_DebugSend(uint8_t ch);
void USART1_DebugSendString(char* str);
void USART1_DebugSendInt(int32_t num);
void USART1_DebugSendHex(uint8_t byte);
void USART1_DebugSendByteInt(uint8_t byte);

void USART1_SystemStartup(void);

void Enable_PC13(void);
void Disable_PC13(void);

#endif /* __LOCAL_USART_H */