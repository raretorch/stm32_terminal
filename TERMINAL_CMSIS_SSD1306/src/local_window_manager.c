/**
  ******************************************************************************
  * @file    local_window_manager.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Window Manager implementation for STM32F10x microcontroller.
	******************************************************************************
**/

#include "window_handler.h"
#include "ssd1306.h"
#include "string.h"

Window Windows[4]; // Initialize the array to hold multiple windows

Modes appModes = {
    .mode1 = {
        {0,0,63,64},
        {65,0,63,16},
        {65,18,63,46},
        {0,0,0,0}
    },
    .mode2 = {
        {65,18,63,46},
        {0,0,128,16},
        {0,18,63,46},
        {0,0,0,0}
    },
    .mode3 = {
        {0,18,128,22},
        {0,0,128,16},
        {0,42,128,22},
        {0,0,0,0}
    }
};

uint8_t Window_Init(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  Window new_window;

  new_window.id = id;
  new_window.x = x;
  new_window.y = y;
  new_window.width = width;
  new_window.height = height;

  // Clear the mask and buffer for the new window
  for (int i = 0; i < 64; i++) {
    new_window.mask[i].low = UINT64_MAX;
    new_window.mask[i].high = UINT64_MAX;
    new_window.buffer[i].low = 0;
    new_window.buffer[i].high = 0;
  }

  for (int i = 0; i < 64; i++) {
    if ((i < y) || (i >= y + height)) {
        new_window.mask[i].high = 0;
        new_window.mask[i].low = 0;
    } else {
        if (x > 64) {
            new_window.mask[i].high = 0;
            new_window.mask[i].low &= (UINT64_MAX >> (x-64));
        } else {
            new_window.mask[i].high &= (UINT64_MAX >> x);
        }

        if (x + width < 64){
            new_window.mask[i].high &= (UINT64_MAX << (64 - (x + width)));
            new_window.mask[i].low = 0;
        } else {
            new_window.mask[i].low &= (UINT64_MAX << (128 - (x + width)));
        }
    }
  }

  Windows[id - 1] = new_window;
  return new_window.id;
}

uint8_t Window_Change(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  Window new_window;

  new_window.id = id;
  new_window.x = x;
  new_window.y = y;
  new_window.width = width;
  new_window.height = height;

  // Clear the mask for the new window
  for (int i = 0; i < 64; i++) {
    new_window.mask[i].low = UINT64_MAX;
    new_window.mask[i].high = UINT64_MAX;
    new_window.buffer[i].high = Windows[id - 1].buffer[i].high;
    new_window.buffer[i].low = Windows[id - 1].buffer[i].low;
  }

  for (int i = 0; i < 64; i++) {
    if ((i < y) || (i >= y + height)) {
        new_window.mask[i].high = 0;
        new_window.mask[i].low = 0;
    } else {
        if (x > 64) {
            new_window.mask[i].high = 0;
            new_window.mask[i].low &= (UINT64_MAX >> (x-64));
        } else {
            new_window.mask[i].high &= (UINT64_MAX >> x);
        }

        if (x + width < 64){
            new_window.mask[i].high &= (UINT64_MAX << (64 - (x + width)));
            new_window.mask[i].low = 0;
        } else {
            new_window.mask[i].low &= (UINT64_MAX << (128 - (x + width)));
        }
    }
  }

  Windows[id - 1] = new_window;
  return  new_window.id;
}

void Window_Clear(uint8_t window_id){
    Window *temp = &Windows[window_id - 1];
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        temp->buffer[i].high = 0;
        temp->buffer[i].low = 0;
    }
}

uint8_t Window_Change_Fullscreen(uint8_t id){
    Window_Change(id, 0, 0, 128, 64);
    return id;
}

void Window_DrawBorder(uint8_t window_id){
    Window *temp = &Windows[window_id - 1]; // take link to a window
    Window_DrawHorizontalLine(window_id, 0, 0, temp->width - 1);
    Window_DrawHorizontalLine(window_id, 0, 1, temp->width - 1);
    Window_DrawHorizontalLine(window_id, 0, 2, temp->width - 1);
    Window_DrawHorizontalLine(window_id, 0, 3, temp->width - 1);
    Window_DrawHorizontalLine(window_id, 0, temp->height - 1, temp->width - 1);
    Window_DrawVerticalLine(window_id, 0, 0, temp->height - 1);
    Window_DrawVerticalLine(window_id, temp->width - 1, 0, temp->height - 1);
    
    if (temp->id == 1) {
        Window_ErasePixel(window_id, temp->width - 5, 1);
        Window_ErasePixel(window_id, temp->width - 6,  1);
        Window_ErasePixel(window_id, temp->width - 5,  2);
        Window_ErasePixel(window_id, temp->width - 6,  2);
    }

    if (temp->id == 2) {
        Window_ErasePixel(window_id, temp->width - 5, 1);
        Window_ErasePixel(window_id, temp->width - 6,  1);
        Window_ErasePixel(window_id, temp->width - 5,  2);
        Window_ErasePixel(window_id, temp->width - 6,  2);
        Window_ErasePixel(window_id, temp->width - 9,  1);
        Window_ErasePixel(window_id, temp->width - 10,  1);
        Window_ErasePixel(window_id, temp->width - 9,  2);
        Window_ErasePixel(window_id, temp->width - 10,  2);
    }

    if (temp->id == 3) {
        Window_ErasePixel(window_id, temp->width - 5, 1);
        Window_ErasePixel(window_id, temp->width - 6,  1);
        Window_ErasePixel(window_id, temp->width - 5,  2);
        Window_ErasePixel(window_id, temp->width - 6,  2);
        Window_ErasePixel(window_id, temp->width - 9,  1);
        Window_ErasePixel(window_id, temp->width - 10,  1);
        Window_ErasePixel(window_id, temp->width - 9,  2);
        Window_ErasePixel(window_id, temp->width - 10,  2);
        Window_ErasePixel(window_id, temp->width - 13,  1);
        Window_ErasePixel(window_id, temp->width - 14,  1);
        Window_ErasePixel(window_id, temp->width - 13,  2);
        Window_ErasePixel(window_id, temp->width - 14,  2);
    }

    if (temp ->id == 4) {
        Window_ErasePixel(window_id, temp->width - 5, 1);
        Window_ErasePixel(window_id, temp->width - 6,  1);
        Window_ErasePixel(window_id, temp->width - 5,  2);
        Window_ErasePixel(window_id, temp->width - 6,  2);
        Window_ErasePixel(window_id, temp->width - 9,  1);
        Window_ErasePixel(window_id, temp->width - 10,  1);
        Window_ErasePixel(window_id, temp->width - 9,  2);
        Window_ErasePixel(window_id, temp->width - 10,  2);
        Window_ErasePixel(window_id, temp->width - 13,  1);
        Window_ErasePixel(window_id, temp->width - 14,  1);
        Window_ErasePixel(window_id, temp->width - 13,  2);
        Window_ErasePixel(window_id, temp->width - 14,  2);
        Window_ErasePixel(window_id, temp->width - 17,  1);
        Window_ErasePixel(window_id, temp->width - 18,  1);
        Window_ErasePixel(window_id, temp->width - 17,  2);
        Window_ErasePixel(window_id, temp->width - 18,  2);
    }

    Window_ErasePixel(window_id, 0, 0);
    Window_ErasePixel(window_id,  temp->width - 1, 0);
    Window_ErasePixel(window_id, 0, temp->height - 1);
    Window_ErasePixel(window_id, temp->width - 1, temp->height - 1);

    Window_DrawPixel(window_id, 1, 4);
    Window_DrawPixel(window_id, temp->width - 2, 4);
}

void Window_DrawPixel(uint8_t window_id, uint8_t x_pos, uint8_t y_pos){
    Window *temp = &Windows[window_id - 1]; // take link to a window
    if (x_pos + temp->x < 64) {
        temp->buffer[temp->y + y_pos].high |= (uint64_t) 1 << (63 - temp->x - x_pos);  
    } else {
        temp->buffer[temp->y + y_pos].low |= (uint64_t) 1 << (127 - temp->x - x_pos);  
    }
}

void Window_DrawHorizontalLine(uint8_t window_id, uint8_t x_pos, uint8_t y_pos, uint8_t length){
    Window *temp = &Windows[window_id - 1]; // take link to a window
    uint8_t global_x = temp->x + x_pos;
    uint8_t global_y = temp->y + y_pos;
    uint128_t line = { .high = UINT64_MAX, .low = UINT64_MAX};

    if (global_x < 64) {
        line.high &= UINT64_MAX >> global_x;
    } else {
        line.low &= UINT64_MAX >> (global_x - 64);
        line.high = 0;
    }

    if(global_x + length < 64){
        line.high &= UINT64_MAX << (63 - (global_x + length));
        line.low = 0;
    } else {
        line.low &= UINT64_MAX << (128 - (global_x + length));
    }

    temp->buffer[global_y].high |= line.high;
    temp->buffer[global_y].low |= line.low;
}

void Window_DrawVerticalLine(uint8_t window_id, uint8_t x_pos, uint8_t y_pos, uint8_t length){
    Window *temp = &Windows[window_id - 1]; // take link to a window
    uint8_t global_x = temp->x + x_pos;
    uint8_t global_y = temp->y + y_pos;
    uint8_t local_length = length;
    if (global_y + local_length > 63) {
        local_length = 63 - global_y;
    }
    for (uint8_t y = global_y; y < global_y + local_length; y++) {
        if (global_x < 64) {
            temp->buffer[y].high |= 1ULL << (63 - global_x);
        } else {
            temp->buffer[y].low |= 1ULL << (127 - global_x);
        }
    }
}

void Window_ErasePixel(uint8_t window_id, uint8_t x_pos, uint8_t y_pos){
    Window *temp = &Windows[window_id - 1]; // take link to a window
    if (x_pos + temp->x < 64) {
        temp->buffer[temp->y + y_pos].high &= ~((uint64_t) 1 << (63 - temp->x - x_pos));  
    } else {
        temp->buffer[temp->y + y_pos].low &= ~((uint64_t) 1 << (127 - temp->x - x_pos));  
    }
}

void Window_Render(uint8_t window_id) {
    Window_DrawBorder(window_id);

    Window *temp = &Windows[window_id - 1]; // take link to a window

    uint8_t output_buf[SSD1306_LENGTH * SSD1306_WIDTH/8];
    uint128_t input_buf[SSD1306_WIDTH];
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        input_buf[i].low = temp->buffer[i].low & temp->mask[i].low;
        input_buf[i].high = temp->buffer[i].high & temp->mask[i].high;
    }

    uint8_t output_mask_buf[SSD1306_LENGTH * SSD1306_WIDTH/8];
    uint128_t input_mask_buf[SSD1306_WIDTH];
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        input_mask_buf[i].low = ~temp->mask[i].low;
        input_mask_buf[i].high = ~temp->mask[i].high;
    }

    ConvertBufToSSD1306(input_buf, output_buf);
    ConvertBufToSSD1306(input_mask_buf, output_mask_buf);

    for (uint16_t i = 0; i < SSD1306_LENGTH * SSD1306_WIDTH/8; i++) {
            SSD1306_BUFFER_B[i] &= output_mask_buf[i];
    }

    for (uint16_t i = 0; i < SSD1306_LENGTH * SSD1306_WIDTH/8; i++) {
            SSD1306_BUFFER_B[i] |= output_buf[i];
    }
}

void ConvertBufToSSD1306 (uint128_t input_buf[], uint8_t output_buf[]) {

    memset(output_buf, 0, SSD1306_LENGTH * SSD1306_WIDTH/8);

    for (uint8_t y = 0; y < SSD1306_WIDTH; y++) {
        const uint128_t row = input_buf[y];
        int page = y >> 3;      // y / 8
        int bit_in_page = y & 7; // y % 8

        for (uint8_t x = 0; x < SSD1306_LENGTH; x++) {
            int bit_val;
            int mediumLine = SSD1306_LENGTH/2;
            if (x < mediumLine) {
                // Столбцы 0…63 из поля low (младший бит -> x=0)
                bit_val = (row.high >> (mediumLine - x)) & 1;
            } else {
                // Столбцы 64…127 из поля high (младший бит -> x=64)
                bit_val = (row.low >> (SSD1306_LENGTH - x - 1)) & 1;
            }
            if (bit_val) {
                output_buf[page * SSD1306_LENGTH + x] |= (1 << bit_in_page);
            }
        }
    }
}

void Draw_R(uint8_t window_id ,uint8_t offset){
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
}

void Draw_A(uint8_t window_id ,uint8_t offset){
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
}

void Draw_E(uint8_t window_id ,uint8_t offset){
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
}

void Draw_O(uint8_t window_id ,uint8_t offset){
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
}

void Draw_C(uint8_t window_id ,uint8_t offset){
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
}

void Draw_H(uint8_t window_id ,uint8_t offset){
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
}

void Draw_T(uint8_t window_id ,uint8_t offset){
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 1 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 1 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 2 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 3 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 2 + LEFT_OFFSET + 1 + offset, 4 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 3 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
    Window_DrawPixel(window_id, 4 + LEFT_OFFSET + 1 + offset, 5 + UPPER_OFFSET + 1);
}

void Window_Draw_Raretorch(uint8_t window_id, uint8_t offset) {
    Draw_R(window_id, 0 + offset);
    Draw_A(window_id, 5 + offset);
    Draw_R(window_id, 10 + offset);
    Draw_E(window_id, 15 + offset);
    Draw_T(window_id, 20 + offset);
    Draw_O(window_id, 25 + offset);
    Draw_R(window_id, 30 + offset);
    Draw_C(window_id, 35 + offset);
    Draw_H(window_id, 40 + offset);
}

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
    uint8_t target_y_end_global_pos){

    uint8_t instance_width = instance_x_end_global_pos - instance_x_global_pos;
    uint8_t instance_height = instance_y_end_global_pos - instance_y_global_pos;
    uint8_t target_width = target_x_end_global_pos - target_x_global_pos;
    uint8_t target_height = target_y_end_global_pos - target_y_global_pos;

    Window *temp = &Windows[window_id - 1];

    uint128_t local_buffer[SSD1306_WIDTH];
    uint128_t mask_buffer[SSD1306_WIDTH];

    if (!resize_horizontal) {
        target_width = instance_width;
    }
    if (!resize_vertical) {
        target_height = instance_height;
    }

    // copy local buffer
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        local_buffer[i].high = temp->buffer[i].high;
        local_buffer[i].low = temp->buffer[i].low;
    }

    // fill mask
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        mask_buffer[i].high = UINT64_MAX;
        mask_buffer[i].low = UINT64_MAX;
    }

    // create mask buffer from coordinates
    for (int i = 0; i < 64; i++) {
        if ((i < instance_y_global_pos) || (i >= instance_y_global_pos + instance_height)) {
            mask_buffer[i].high = 0;
            mask_buffer[i].low = 0;
        } else {
            if (instance_x_global_pos > 64) {
                mask_buffer[i].high = 0;
                mask_buffer[i].low &= (UINT64_MAX >> (instance_x_global_pos-64));
            } else {
                mask_buffer[i].high &= (UINT64_MAX >> instance_x_global_pos);
            }

            if (instance_x_global_pos + instance_width < 64){
                mask_buffer[i].high &= (UINT64_MAX << (64 - (instance_x_global_pos + instance_width)));
                mask_buffer[i].low = 0;
            } else {
                mask_buffer[i].low &= (UINT64_MAX << (128 - (instance_x_global_pos + instance_width)));
            }
        }
    }

    // apply mask to local_buffer
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        local_buffer[i].high &= mask_buffer[i].high;
        local_buffer[i].low &= mask_buffer[i].low;
    }

    // transform vertical local_buffer to instance_global_y
    for (uint8_t i = 0; i < instance_height; i++) {
        local_buffer[i].high = local_buffer[i + instance_y_global_pos].high;
        local_buffer[i].low = local_buffer[i + instance_y_global_pos].low;
    }

    // transform horizontal local_buffer to instance_global_x
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        for (uint8_t x = 0; x < instance_x_global_pos; x++) {
            local_buffer[i].high <<= 1;
            if (local_buffer[i].low >> 63) {
                local_buffer[i].high |= 1;
            }
            local_buffer[i].low <<= 1;
        }
    }

    // resample horizontal local_buffer
    if (resize_horizontal){
        for (uint8_t y = 0; y < instance_height; y++) {
            _Bool line_buffer[128];
            _Bool exit_line_buffer[128];

            // set line buffer with bits
            for (uint8_t i = 0; i < SSD1306_LENGTH; i++) {
                exit_line_buffer[i] = 0;
                if (i < SSD1306_LENGTH/2){
                    line_buffer[i] = (_Bool) (local_buffer[y].high >> (63 - i) & 1);
                } else {
                    line_buffer[i] = (_Bool) (local_buffer[y].low >> (63 - (i - 64)) & 1);
                }
            }
            // resize line buffer
            for (uint8_t i = 0; i < target_width; i++) {
                uint8_t src_idx = (i * instance_width)/target_width;
                exit_line_buffer[i] = line_buffer[src_idx];
            }

            // apply line_buffer to line
            local_buffer[y].high = 0;
            local_buffer[y].low = 0;
            for (uint8_t i = 0; i < SSD1306_LENGTH; i++) {
                if (exit_line_buffer[i]) {
                    if (i < 64) {
                        local_buffer[y].high |= ((uint64_t)1 << (63 - i));
                    } else {
                        local_buffer[y].low |= ((uint64_t)1 << (63 - (i - 64)));
                    }
                }
            }
        }
    }

    // resize vertical
    if (resize_vertical) {
        uint128_t operation_buffer[SSD1306_WIDTH];
        for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
            operation_buffer[i].high = local_buffer[i].high;
            operation_buffer[i].low = local_buffer[i].low;
        }

        for (uint8_t i = 0; i < target_height; i++) {
            uint8_t src_idx = (i * instance_height)/ target_height;
            operation_buffer[i].high = local_buffer[src_idx].high;
            operation_buffer[i].low = local_buffer[src_idx].low;
        }

        for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
            local_buffer[i].high = operation_buffer[i].high;
            local_buffer[i].low = operation_buffer[i].low;
        }
    }

    // clear trash after resize
    for (uint8_t i = target_height; i < SSD1306_WIDTH; i++) {
        local_buffer[i].high = 0;
        local_buffer[i].low = 0;
    }

    // transform to target coordinates
    // transform horizontal
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        for (uint8_t x = 0; x < target_x_global_pos; x++) {
            uint64_t bit_from_high = local_buffer[i].high & 1; // Просто проверка &
            local_buffer[i].high >>= 1;
            local_buffer[i].low >>= 1;
            if (bit_from_high) {
                local_buffer[i].low |= ((uint64_t)1 << 63);
            }
        }
    }

    // transform vertical
    for (uint8_t i = 0; i < target_y_global_pos; i++) {
        for (uint8_t d = SSD1306_WIDTH - 1; d > 0; d--) {
            local_buffer[d].high = local_buffer[d - 1].high;
            local_buffer[d].low = local_buffer[d - 1].low; 
        }

        local_buffer[0].high = 0;
        local_buffer[0].low = 0;
    }

    // clear a bit massive after height
    uint8_t end_y = target_y_end_global_pos;
    for (uint8_t i = end_y; i < SSD1306_WIDTH; i++) {
        local_buffer[i].high = 0;
        local_buffer[i].low = 0;
    }

    // apply to window buffer a result
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
        temp->buffer[i].high = local_buffer[i].high;
        temp->buffer[i].low = local_buffer[i].low;
    }
}