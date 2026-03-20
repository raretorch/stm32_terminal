/**
  ******************************************************************************
  * @file    local_graph.c
  * @author  Raretorch
  * @version V1.0.0
  * @date    16-March-2026
  * @brief   graph implementation
	******************************************************************************
**/

#include "graph_label_handler.h"
#include "ssd1306.h"

Graph graphs[4];

void Graph_label_init(uint8_t window_id, uint8_t start_value, uint8_t filling_mode){
    Window *temp = &Windows[window_id - 1]; // take link to a window
    Graph new_graph;
    new_graph.parent_window = temp;
    new_graph.x_pos = temp->x + LEFT_OFFSET;
    new_graph.y_pos = temp->y + UPPER_OFFSET;
    new_graph.width = temp->width - LEFT_OFFSET - RIGHT_OFFSET;
    new_graph.heigth = temp->height - UPPER_OFFSET - DOWN_OFFSET;
    new_graph.value = start_value;
    new_graph.last_value = start_value;
    new_graph.filling_mode = filling_mode;
    graphs[window_id - 1] = new_graph;
}

void Graph_Update(uint8_t id, uint8_t value){
    Graph *graph = &graphs[id - 1];
    graph->last_value = graph->value;
    graph->value = value;

    uint128_t graph_mask[SSD1306_WIDTH];
    for (int i = 0; i < 64; i++) {
    graph_mask[i].low = UINT64_MAX;
    graph_mask[i].high = UINT64_MAX;
    }

    for (int i = 0; i < 64; i++) {
        if ((i < graph->y_pos) || (i >= graph->y_pos + graph->heigth)) {
            graph_mask[i].high = 0;
            graph_mask[i].low = 0;
        } else {
            if (graph->x_pos > 64) {
                graph_mask[i].high = 0;
                graph_mask[i].low &= (UINT64_MAX >> (graph->x_pos - 64));
            } else {
                graph_mask[i].high &= (UINT64_MAX >> graph->x_pos);
            }

            if (graph->x_pos + graph->width < 64){
                graph_mask[i].high &= (UINT64_MAX << (64 - (graph->x_pos + graph->width)));
                graph_mask[i].low = 0;
            } else {
                graph_mask[i].low &= (UINT64_MAX << (128 - (graph->x_pos + graph->width)));
            }
        }
    }

    for (uint8_t i = graph->y_pos; i < graph->y_pos + graph->heigth; i++) {
        graph->parent_window->buffer[i].high &= graph_mask[i].high;
        graph->parent_window->buffer[i].low &= graph_mask[i].low;
    }   

    for (uint8_t i = graph->y_pos; i < graph->y_pos + graph->heigth; i++) {
        graph->parent_window->buffer[i].high <<= 1;
        if (graph->parent_window->buffer[i].low >> 63) {
            graph->parent_window->buffer[i].high |= 1;
        }
        graph->parent_window->buffer[i].low <<= 1;
        Window_ErasePixel(graph->parent_window->id, (graph->x_pos - graph->parent_window->x) + 1, i - graph->parent_window->y);
    }

    uint8_t procents_per_pixel = (100 / graph->heigth) + 1;
    uint8_t value_in_pixels = graph->heigth - (graph->value / procents_per_pixel);
    uint8_t last_value_in_pixels = graph->heigth - (graph->last_value / procents_per_pixel);

    uint8_t value_x = (graph->x_pos - graph->parent_window->x) + graph->width - 1;
    uint8_t value_y = (graph->y_pos - graph->parent_window->y) + value_in_pixels - 1;
    uint8_t last_value_y = (graph->y_pos - graph->parent_window->y) + last_value_in_pixels - 1;

    if (!graph->filling_mode) {
        if (value_in_pixels < last_value_in_pixels) {
            Window_DrawVerticalLine(
            graph->parent_window->id, 
            value_x, 
            value_y,
            last_value_in_pixels - value_in_pixels);
         } else if (value_in_pixels == last_value_in_pixels){
            Window_DrawPixel(
            graph->parent_window->id, 
            value_x, 
            value_y);
        } else {
            Window_DrawVerticalLine(
            graph->parent_window->id, 
            value_x, 
            last_value_y,
            value_in_pixels - last_value_in_pixels);
        }
    } else {
        Window_DrawVerticalLine(
            graph->parent_window->id, 
            value_x, 
            value_y,
            graph->heigth - value_in_pixels);
    }
}