/**
  ******************************************************************************
  * @file    window_handler.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Window handler functions for managing display windows.
	******************************************************************************
**/

#ifndef __WINDOW_HANDLER_H
#define __WINDOW_HANDLER_H

#define UPPER_OFFSET    6
#define DOWN_OFFSET     2
#define LEFT_OFFSET     2
#define RIGHT_OFFSET     2

#include <stdint.h>

typedef struct {
  uint64_t low;
  uint64_t high;
} uint128_t;

typedef struct {
  uint8_t id;   // Unique identifier for the window
  uint16_t x;   // X position of the window
  uint16_t y;   // Y position of the window
  uint16_t width;  // Width of the window
  uint16_t height; // Height of the window
  uint128_t mask[64]; // 128x64 pixel matrix for the window content
  uint128_t buffer[64]; // Buffer for rendering the window content
} Window;



typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t width;
  uint8_t height;
} WindowCoordinates;

typedef  struct {
   WindowCoordinates mode1[4];
   WindowCoordinates mode2[4];
   WindowCoordinates mode3[4];
} Modes;

extern Modes appModes;

extern Window Windows[4]; // Array to hold multiple windows

uint8_t Window_Init(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

uint8_t Window_Change(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
uint8_t Window_Change_Fullscreen(uint8_t id);

void ConvertBufToSSD1306 (uint128_t input_buf[], uint8_t output_buf[]);

void Window_Render(uint8_t window_id);
void Window_Clear(uint8_t window_id);
void Window_DrawPixel(
  uint8_t window_id, 
  uint8_t x_pos, 
  uint8_t y_pos);
void Window_ErasePixel(
  uint8_t window_id, 
  uint8_t x_pos, 
  uint8_t y_pos);
void Window_DrawHorizontalLine(
  uint8_t window_id,
  uint8_t x_pos, 
  uint8_t y_pos, 
  uint8_t length);
void Window_DrawVerticalLine(
  uint8_t window_id, 
  uint8_t x_pos, 
  uint8_t y_pos, 
  uint8_t length);
void Window_DrawBorder(uint8_t window_id);
void Window_TransformObject(
  uint8_t window_id,
    _Bool resize_vertical, 
    _Bool resize_horizontal, 
    uint8_t instance_x_global_pos, 
    uint8_t instance_y_global_pos, 
    uint8_t instance_x_end_global_pos,
    uint8_t instance_y_end_global_pos,
    uint8_t target_x_global_pos, 
    uint8_t target_y_global_pos, 
    uint8_t target_x_end_global_pos,
    uint8_t target_y_end_global_pos);
void Window_Draw_Raretorch(uint8_t window_id, uint8_t offset);

#endif /* __WINDOW_HANDLER_H */