/**
  ******************************************************************************
  * @file    local_display.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Graphicals implementations for local display using SSD1306 OLED.
	******************************************************************************
**/
 
#include <stdint.h>
#include <string.h>
#include "i2c.h"
#include "ssd1306.h"
#include "sys_tick.h"

uint8_t SSD1306_BUFFER_A[SSD1306_LENGTH * SSD1306_WIDTH / 8];
uint8_t SSD1306_BUFFER_B[SSD1306_LENGTH * SSD1306_WIDTH / 8];

void SSD1306_WriteCmd(uint8_t cmd) {
    uint8_t buf[2] = {SSD1306_CMD, cmd};
    I2CWrite(SSD1306_I2C_ADDR, buf, 2);
}

void SSD1306_WriteData(uint8_t *data, uint16_t length) {
    uint8_t buf[length + 1];
    buf[0] = SSD1306_DATA;
        for(uint16_t i = 0; i < length; i++) {
            buf[i + 1] = data[i];
        }
        I2CWrite(SSD1306_I2C_ADDR, buf, length + 1);
}

void SSD1306_Init(void) {
    Delay(10);

    memset(SSD1306_BUFFER_A, 0, SSD1306_LENGTH * SSD1306_WIDTH / 8);
    memset(SSD1306_BUFFER_B, 0, SSD1306_LENGTH * SSD1306_WIDTH / 8);

    SSD1306_WriteCmd(0xAE);  // Display OFF
    
    SSD1306_WriteCmd(0x8D);  // Charge pump
    SSD1306_WriteCmd(0x14);  // Enable charge pump
    
    SSD1306_WriteCmd(0x20);  // Memory addressing mode
    SSD1306_WriteCmd(0x00);  // Horizontal addressing mode
    
    SSD1306_WriteCmd(0xA1);  // Segment remap (column 127 mapped to SEG0)
    
    SSD1306_WriteCmd(0xC8);  // COM output scan direction (remapped mode)
    
    SSD1306_WriteCmd(0xDA);  // COM pins hardware configuration
    SSD1306_WriteCmd(0x12);  // Alternative pin configuration
    
    SSD1306_WriteCmd(0x81);  // Contrast control
    SSD1306_WriteCmd(0xFF);  // Maximum contrast
    
    SSD1306_WriteCmd(0xD9);  // Pre-charge period
    SSD1306_WriteCmd(0xF1);  // Phase1=15, Phase2=1
    
    SSD1306_WriteCmd(0xAF);  // Display ON
}

void SSD1306_UpdateScreen(){
    for (uint16_t i = 0; i < SSD1306_LENGTH * SSD1306_WIDTH/8; i++) {
        SSD1306_BUFFER_A[i] = SSD1306_BUFFER_B[i];
    }
    memset(SSD1306_BUFFER_B, 0, SSD1306_LENGTH * SSD1306_WIDTH/8);
    SSD1306_WriteData(SSD1306_BUFFER_A, SSD1306_LENGTH * SSD1306_WIDTH/8);
}