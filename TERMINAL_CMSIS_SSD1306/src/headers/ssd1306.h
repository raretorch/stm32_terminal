/**
  ******************************************************************************
  * @file    ssd1306.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   SSD1306 OLED display driver functions.
	******************************************************************************
 **/

#ifndef __SSD1306_H
#define __SSD1306_H

#include <stdint.h>

#define SSD1306_I2C_ADDR    0x3C

#define SSD1306_LENGTH      128
#define SSD1306_WIDTH       64

#define SSD1306_CMD         0x00
#define SSD1306_DATA        0x40

extern uint8_t SSD1306_BUFFER_A[SSD1306_LENGTH * SSD1306_WIDTH / 8];
extern uint8_t SSD1306_BUFFER_B[SSD1306_LENGTH * SSD1306_WIDTH / 8];

void SSD1306_Init(void);

void SSD1306_WriteCmd(uint8_t cmd);
void SSD1306_WriteData(uint8_t *data, uint16_t len);

void SSD1306_UpdateScreen(void);

#endif /* __SSD1306_H */