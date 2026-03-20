/**
  ******************************************************************************
  * @file    graph_label_handler.h
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   Window handler functions for managing graph label.
	******************************************************************************
**/

#ifndef __GRAPH_LABEL_HANDLER_H
#define __GRAPH_LABEL_HANDLER_H

#include "window_handler.h"
#include <stdint.h>

typedef struct {
    Window* parent_window;
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t width;
    uint8_t heigth;
    uint8_t last_value;
    uint8_t value;
    uint8_t filling_mode;
} Graph;

extern Graph graphs[4];

void Graph_label_init(uint8_t window_id, uint8_t start_value, uint8_t filling_mode);
void Graph_label_change(uint8_t window_id);
void Graph_Update(uint8_t id, uint8_t value);


#endif /*__GRAPH_LABEL_HANDLER_H*/