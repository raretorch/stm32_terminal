/**
  ******************************************************************************
  * @file    local_usart.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   USART configuration and communication functions for STM32F10x microcontroller.
	******************************************************************************
 **/

#include "stm32f10x.h"
#include "usart.h"
#include "gpio.h"
#include "rcc.h"
#include "sys_tick.h"

void USART1_Init(uint32_t baudrate) {
    ConfigureUART1Pins();
    ConfigureGPIOC13Pin();

    USART1->CR1 = USART_CR1_UE;

    USART1->BRR = baudrate; // Set baud rate (assuming PCLK2 is 72 MHz)
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART1->CR2 = 0;
    USART1->CR3 = 0;
    USART1_DebugSendString("USART is initialized\n\n");
}

void USART1_DebugSend(uint8_t ch){
	while ((USART1->SR & USART_SR_TXE) == 0) {}
	USART1->DR = ch;
}

uint8_t USART1_GetData(void) {
    while ((USART1->SR & USART_SR_RXNE) == 0) {}
    USART1_RECIEVED_DATA = USART1->DR;
    return USART1_RECIEVED_DATA;
}

void USART1_DebugSendString(char* str){
	while (*str) {
        USART1_DebugSend(*str++);
    }
}

void USART1_DebugSendInt(int32_t num) {
    char str[12];
    int i = 0;
    if (num == 0) { USART1_DebugSend('0'); return; }
    if (num < 0) { USART1_DebugSend('-'); num = -num; }
    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    while (--i >= 0) {
        USART1_DebugSend(str[i]);
    }
}

void USART1_DebugSendByteInt(uint8_t byte) {
    char bits[9];
    for (int i = 7; i >= 0; i--) {
        // Проверяем i-й бит (i=7..0)
        bits[7 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    bits[8] = '\0'; // Завершаем строку
    USART1_DebugSendString(bits);
}

void USART1_DebugSendHex(uint8_t byte) {
    uint8_t hexChars[] = "0123456789ABCDEF";
    uint8_t highNibble = (byte >> 4) & 0x0F;
    uint8_t lowNibble = byte & 0x0F;

	USART1_DebugSendString("0x");

    // Wait until transmit data register is empty
    while (!(USART1->SR & USART_SR_TXE)); 
    USART1->DR = hexChars[highNibble]; // Send High Nibble

    // Wait until transmit data register is empty
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = hexChars[lowNibble]; // Send Low Nibble
}

void USART1_SystemStartup(void){
    USART1_DebugSendString("System Startup...\n");
    USART1_DebugSendString("RCC Configuration:\n");
    USART1_DebugSendString("Clock Multiplexer: ");
    USART1_DebugSendByteInt(rccConfig.PLLMUL >> 18);
    USART1_DebugSendString(" : ");
    USART1_DebugSendInt((rccConfig.PLLMUL >> 18) + 2); // PLLMUL value is encoded as (MUL - 2) in bits 21:18
    USART1_DebugSendString("\n");
    USART1_DebugSendString("System Clock: ");
    USART1_DebugSendInt(GetSystemCoreClock());
    USART1_DebugSendString(" Hz\n");
    USART1_DebugSendString("SysTick Mode: ");
    USART1_DebugSendString(GetSysTickMode() == MS ? "Milliseconds" : (GetSysTickMode() == SEC ? "Seconds" : "Nanoseconds"));
    USART1_DebugSendString(" \n\n");
 }

 void Enable_PC13(void){
    EnableGPIOC13Pin();
 }

 void Disable_PC13(void){
    DisableGPIOC13Pin();
 }
