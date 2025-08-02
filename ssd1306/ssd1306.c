/**
 * @file ssd1306.c
 * @author Abdulaziz Alrashidi
 * @brief Implementation of the SSD1306 OLED display driver for Ideaspark NodeMCU ESP8266 with pre-installed SSD1306 via I2C.
 * @version 0.1
 * @date 2025-08-02
 * @copyright Copyright (c) 2025
 * @license MIT
 * 
 * @details
 * This file contains the internal logic and implementation of functions declared in ssd1306.h.
 * All drawing operations are performed on a framebuffer, which must be flushed to the screen 
 * using ssd1306_display().
 */

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ssd1306.h"

uint8_t buffer[BUFFER_SIZE];

const char font5x7[] = {
	0x00, 0x00, 0x00, 0x00, 0x00,// (space)
	0x00, 0x00, 0x5F, 0x00, 0x00,// !
	0x00, 0x07, 0x00, 0x07, 0x00,// "
	0x14, 0x7F, 0x14, 0x7F, 0x14,// #
	0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
	0x23, 0x13, 0x08, 0x64, 0x62,// %
	0x36, 0x49, 0x55, 0x22, 0x50,// &
	0x00, 0x05, 0x03, 0x00, 0x00,// '
	0x00, 0x1C, 0x22, 0x41, 0x00,// (
	0x00, 0x41, 0x22, 0x1C, 0x00,// )
	0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
	0x08, 0x08, 0x3E, 0x08, 0x08,// +
	0x00, 0x50, 0x30, 0x00, 0x00,// ,
	0x08, 0x08, 0x08, 0x08, 0x08,// -
	0x00, 0x60, 0x60, 0x00, 0x00,// .
	0x20, 0x10, 0x08, 0x04, 0x02,// /
	0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
	0x00, 0x42, 0x7F, 0x40, 0x00,// 1
	0x42, 0x61, 0x51, 0x49, 0x46,// 2
	0x21, 0x41, 0x45, 0x4B, 0x31,// 3
	0x18, 0x14, 0x12, 0x7F, 0x10,// 4
	0x27, 0x45, 0x45, 0x45, 0x39,// 5
	0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
	0x01, 0x71, 0x09, 0x05, 0x03,// 7
	0x36, 0x49, 0x49, 0x49, 0x36,// 8
	0x06, 0x49, 0x49, 0x29, 0x1E,// 9
	0x00, 0x36, 0x36, 0x00, 0x00,// :
	0x00, 0x56, 0x36, 0x00, 0x00,// ;
	0x00, 0x08, 0x14, 0x22, 0x41,// <
	0x14, 0x14, 0x14, 0x14, 0x14,// =
	0x41, 0x22, 0x14, 0x08, 0x00,// >
	0x02, 0x01, 0x51, 0x09, 0x06,// ?
	0x32, 0x49, 0x79, 0x41, 0x3E,// @
	0x7E, 0x11, 0x11, 0x11, 0x7E,// A
	0x7F, 0x49, 0x49, 0x49, 0x36,// B
	0x3E, 0x41, 0x41, 0x41, 0x22,// C
	0x7F, 0x41, 0x41, 0x22, 0x1C,// D
	0x7F, 0x49, 0x49, 0x49, 0x41,// E
	0x7F, 0x09, 0x09, 0x01, 0x01,// F
	0x3E, 0x41, 0x41, 0x51, 0x32,// G
	0x7F, 0x08, 0x08, 0x08, 0x7F,// H
	0x00, 0x41, 0x7F, 0x41, 0x00,// I
	0x20, 0x40, 0x41, 0x3F, 0x01,// J
	0x7F, 0x08, 0x14, 0x22, 0x41,// K
	0x7F, 0x40, 0x40, 0x40, 0x40,// L
	0x7F, 0x02, 0x04, 0x02, 0x7F,// M
	0x7F, 0x04, 0x08, 0x10, 0x7F,// N
	0x3E, 0x41, 0x41, 0x41, 0x3E,// O
	0x7F, 0x09, 0x09, 0x09, 0x06,// P
	0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
	0x7F, 0x09, 0x19, 0x29, 0x46,// R
	0x46, 0x49, 0x49, 0x49, 0x31,// S
	0x01, 0x01, 0x7F, 0x01, 0x01,// T
	0x3F, 0x40, 0x40, 0x40, 0x3F,// U
	0x1F, 0x20, 0x40, 0x20, 0x1F,// V
	0x7F, 0x20, 0x18, 0x20, 0x7F,// W
	0x63, 0x14, 0x08, 0x14, 0x63,// X
	0x03, 0x04, 0x78, 0x04, 0x03,// Y
	0x61, 0x51, 0x49, 0x45, 0x43,// Z
	0x00, 0x00, 0x7F, 0x41, 0x41,// [
	0x02, 0x04, 0x08, 0x10, 0x20,// "\"
	0x41, 0x41, 0x7F, 0x00, 0x00,// ]
	0x04, 0x02, 0x01, 0x02, 0x04,// ^
	0x40, 0x40, 0x40, 0x40, 0x40,// _
	0x00, 0x01, 0x02, 0x04, 0x00,// `
	0x20, 0x54, 0x54, 0x54, 0x78,// a
	0x7F, 0x48, 0x44, 0x44, 0x38,// b
	0x38, 0x44, 0x44, 0x44, 0x20,// c
	0x38, 0x44, 0x44, 0x48, 0x7F,// d
	0x38, 0x54, 0x54, 0x54, 0x18,// e
	0x08, 0x7E, 0x09, 0x01, 0x02,// f
	0x08, 0x14, 0x54, 0x54, 0x3C,// g
	0x7F, 0x08, 0x04, 0x04, 0x78,// h
	0x00, 0x44, 0x7D, 0x40, 0x00,// i
	0x20, 0x40, 0x44, 0x3D, 0x00,// j
	0x00, 0x7F, 0x10, 0x28, 0x44,// k
	0x00, 0x41, 0x7F, 0x40, 0x00,// l
	0x7C, 0x04, 0x18, 0x04, 0x78,// m
	0x7C, 0x08, 0x04, 0x04, 0x78,// n
	0x38, 0x44, 0x44, 0x44, 0x38,// o
	0x7C, 0x14, 0x14, 0x14, 0x08,// p
	0x08, 0x14, 0x14, 0x18, 0x7C,// q
	0x7C, 0x08, 0x04, 0x04, 0x08,// r
	0x48, 0x54, 0x54, 0x54, 0x20,// s
	0x04, 0x3F, 0x44, 0x40, 0x20,// t
	0x3C, 0x40, 0x40, 0x20, 0x7C,// u
	0x1C, 0x20, 0x40, 0x20, 0x1C,// v
	0x3C, 0x40, 0x30, 0x40, 0x3C,// w
	0x44, 0x28, 0x10, 0x28, 0x44,// x
	0x0C, 0x50, 0x50, 0x50, 0x3C,// y
	0x44, 0x64, 0x54, 0x4C, 0x44,// z
	0x00, 0x08, 0x36, 0x41, 0x00,// {
	0x00, 0x00, 0x7F, 0x00, 0x00,// |
	0x00, 0x41, 0x36, 0x08, 0x00,// }
	0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
	0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
};

esp_err_t i2c_init(void)
{
    int i2c_port = I2C_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .scl_io_num = I2C_SCL_IO,
        .scl_pullup_en = 1,
        .sda_io_num = I2C_SDA_IO,
        .sda_pullup_en = 1,
        .clk_stretch_tick = 300
    };

    ESP_ERROR_CHECK(i2c_driver_install(i2c_port, conf.mode));
    ESP_ERROR_CHECK(i2c_param_config(i2c_port, &conf));

    return ESP_OK;
}

void ssd1306_write(i2c_port_t i2c_num, uint8_t reg_address, uint8_t* data, size_t data_len)
{
    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();

    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, SSD1306_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd_handle, reg_address, ACK_CHECK_EN);
    i2c_master_write(cmd_handle, data, data_len, ACK_CHECK_EN);
    i2c_master_stop(cmd_handle);
    i2c_master_cmd_begin(i2c_num, cmd_handle, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd_handle);
}

void ssd1306_init(void)
{
    uint8_t init_cmds[] = {
        0xAE,                // Display OFF
        0xD5, 0x80,          // Set display clock divide ratio/oscillator frequency
        0xA8, 0x3F,          // Set multiplex ratio (0x3F = 64)
        0xD3, 0x00,          // Set display offset to 0
        0x40,                // Set start line to 0
        0x8D, 0x14,          // Enable charge pump
        0x20, 0x00,          // Memory addressing mode: Horizontal
        0xA1,                // Set segment re-map (column address 127 mapped to SEG0)
        0xC8,                // COM output scan direction: remapped mode (scan bottom-to-top)
        0xDA, 0x12,          // COM pins hardware config
        0x81, 0x7F,          // Contrast control
        0xD9, 0xF1,          // Pre-charge period
        0xDB, 0x40,          // VCOMH deselect level
        0xA4,                // Entire display ON (resume to RAM content)
        0xA6,                // Normal display (not inverted)
        0xAF                 // Display ON
    };

    i2c_init();
    
    for (int i = 0; i < sizeof(init_cmds); i++) {
        ssd1306_cmd(init_cmds[i]);
    }

    ssd1306_clear();
    ssd1306_display();
}

void ssd1306_cmd(uint8_t cmd)
{
    i2c_port_t i2c_port = I2C_NUM;
    uint8_t cmd_arr[] = { cmd };
    ssd1306_write(i2c_port, SSD1306_CMD, cmd_arr, sizeof(cmd_arr));
}

void ssd1306_display(void)
{
    // Set column address range: 0 to 127
    ssd1306_cmd(0x21);
    ssd1306_cmd(0x00);
    ssd1306_cmd(0x7F);

    // Set page address range: 0 to 7
    ssd1306_cmd(0x22);
    ssd1306_cmd(0x00);
    ssd1306_cmd(0x07);

    // Send entire buffer
    ssd1306_data(buffer, BUFFER_SIZE);
}

void ssd1306_clear(void)
{
    memset(buffer, 0x00, sizeof(buffer)); // clears buffer
}

void ssd1306_data(uint8_t* data, size_t len)
{
    int i2c_port = I2C_NUM;
    ssd1306_write(i2c_port, SSD1306_DATA, data, len);
}

void ssd1306_draw_pixel(uint8_t x, uint8_t y, bool color)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    if (color)
        buffer[x + (y / 8) * SCREEN_WIDTH] |= (1 << (y % 8));
    else
        buffer[x + (y / 8) * SCREEN_WIDTH] &= ~(1 << (y % 8));
}

void ssd1306_draw_char(uint8_t x, uint8_t y, char c, uint8_t size_x, uint8_t size_y, bool color)
{
    if (c < 32 || c > 126) return; // unsupported char

    uint16_t index = (c - 32) * 5;

    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = font5x7[index + i];
        for (uint8_t j = 0; j < 8; j++) {
            if (line & 0x01) {
                // Scale by size_x and size_y
                for (uint8_t dx = 0; dx < size_x; dx++) {
                    for (uint8_t dy = 0; dy < size_y; dy++) {
                        ssd1306_draw_pixel(x + i * size_x + dx, y + j * size_y + dy, color);
                    }
                }
            }
            line >>= 1;
        }
    }

    // Add spacing column (blank)
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t dx = 0; dx < size_x; dx++) {
            for (uint8_t dy = 0; dy < size_y; dy++) {
                ssd1306_draw_pixel(x + 5 * size_x + dx, y + j * size_y + dy, COLOR_BLACK);
            }
        }
    }
}

void ssd1306_draw_string(uint8_t x, uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, bool color)
{
    while (*str) {
        ssd1306_draw_char(x, y, *str++, size_x, size_y, color);
        x += (6 * size_x); // 5 pixels + 1 space, scaled
    }
}

void ssd1306_draw_string_wrapped(uint8_t x, uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, bool color)
{
    uint8_t start_x = x;
    while (*str) {
        if (x + (6 * size_x) >= SCREEN_WIDTH) {
            x = start_x;
            y += (8 * size_y); // next line
            if (y >= SCREEN_HEIGHT) break; // stop if bottom reached
        }
        ssd1306_draw_char(x, y, *str++, size_x, size_y, color);
        x += (6 * size_x);
    }
}

void ssd1306_draw_string_char_by_char(uint8_t x, uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms, bool color)
{
    while (*str) {
        ssd1306_draw_char(x, y, *str++, size_x, size_y, color);
        x += (6 * size_x); // 5 pixels + 1 space, scaled
		vTaskDelay(tick_delay_ms / portTICK_PERIOD_MS);
		ssd1306_display();
    }
}

void ssd1306_draw_string_wrapped_char_by_char(uint8_t x, uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms, bool color)
{
    uint8_t start_x = x;
    while (*str) {
        if (x + (6 * size_x) >= SCREEN_WIDTH) {
            x = start_x;
            y += (8 * size_y); // next line
            if (y >= SCREEN_HEIGHT) break; // stop if bottom reached
        }
        ssd1306_draw_char(x, y, *str++, size_x, size_y, color);
        x += (6 * size_x);
		vTaskDelay(tick_delay_ms / portTICK_PERIOD_MS);
		ssd1306_display();
    }
}

uint8_t ssd1306_get_string_width(const char* str, uint8_t size_x)
{
    return strlen(str) * 6 * size_x;
}

void ssd1306_draw_string_centered(uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, bool color)
{
    uint8_t str_w = ssd1306_get_string_width(str, size_x);
    uint8_t x = (SCREEN_WIDTH - str_w) / 2;
    ssd1306_draw_string(x, y, str, size_x, size_y, color);
}

void ssd1306_draw_string_centered_char_by_char(uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms, bool color)
{
    uint8_t str_w = ssd1306_get_string_width(str, size_x);
    uint8_t x = (SCREEN_WIDTH - str_w) / 2;
    ssd1306_draw_string_char_by_char(x, y, str, size_x, size_y, tick_delay_ms, color);
}

void ssd1306_draw_full_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color)
{
    for (uint8_t i = x; i < x + w && i < SCREEN_WIDTH; i++) {
        for (uint8_t j = y; j < y + h && j < SCREEN_HEIGHT; j++) {
            ssd1306_draw_pixel(i, j, color);
        }
    }
}

void ssd1306_draw_empty_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color)
{
    for (uint8_t i = x; i < x + w && i < SCREEN_WIDTH; i++) {
        ssd1306_draw_pixel(i, y, color);             // Top
        ssd1306_draw_pixel(i, y + h - 1, color);      // Bottom
    }
    for (uint8_t j = y; j < y + h && j < SCREEN_HEIGHT; j++) {
        ssd1306_draw_pixel(x, j, color);             // Left
        ssd1306_draw_pixel(x + w - 1, j, color);     // Right
    }
}

void ssd1306_draw_full_circle(uint8_t x0, uint8_t y0, uint8_t radius, bool color)
{
    int x = 0;
    int y = radius;
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;

    // Draw center vertical line
    for (int i = y0 - radius; i <= y0 + radius; i++) {
        ssd1306_draw_pixel(x0, i, color);
    }

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        for (int i = y0 - y; i <= y0 + y; i++) {
            ssd1306_draw_pixel(x0 + x, i, color);
            ssd1306_draw_pixel(x0 - x, i, color);
        }
        for (int i = y0 - x; i <= y0 + x; i++) {
            ssd1306_draw_pixel(x0 + y, i, color);
            ssd1306_draw_pixel(x0 - y, i, color);
        }
    }
}

void ssd1306_draw_empty_circle(uint8_t x0, uint8_t y0, uint8_t radius, bool color)
{
    int x = 0;
    int y = radius;
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;

    ssd1306_draw_pixel(x0, y0 + radius, color);
    ssd1306_draw_pixel(x0, y0 - radius, color);
    ssd1306_draw_pixel(x0 + radius, y0, color);
    ssd1306_draw_pixel(x0 - radius, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ssd1306_draw_pixel(x0 + x, y0 + y, color);
        ssd1306_draw_pixel(x0 - x, y0 + y, color);
        ssd1306_draw_pixel(x0 + x, y0 - y, color);
        ssd1306_draw_pixel(x0 - x, y0 - y, color);
        ssd1306_draw_pixel(x0 + y, y0 + x, color);
        ssd1306_draw_pixel(x0 - y, y0 + x, color);
        ssd1306_draw_pixel(x0 + y, y0 - x, color);
        ssd1306_draw_pixel(x0 - y, y0 - x, color);
    }
}

void ssd1306_draw_line(int x0, int y0, int x1, int y1, bool color)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        ssd1306_draw_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void ssd1306_draw_horizontal_line(int x_start, int x_end, int y, bool color)
{
    if (y < 0 || y >= SCREEN_HEIGHT) return;
    if (x_start > x_end) {
        int temp = x_start;
        x_start = x_end;
        x_end = temp;
    }
    if (x_start < 0) x_start = 0;
    if (x_end >= SCREEN_WIDTH) x_end = SCREEN_WIDTH - 1;
    for (int x = x_start; x <= x_end; x++) {
        ssd1306_draw_pixel(x, y, color);
    }
}

void ssd1306_draw_empty_triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool color)
{
    ssd1306_draw_line(x0, y0, x1, y1, color);
    ssd1306_draw_line(x1, y1, x2, y2, color);
    ssd1306_draw_line(x2, y2, x0, y0, color);
}

void ssd1306_draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool color)
{
    // Sort vertices by y-coordinate (ascending)
    if (y0 > y1) { int t = y0; y0 = y1; y1 = t; t = x0; x0 = x1; x1 = t; }
    if (y1 > y2) { int t = y1; y1 = y2; y2 = t; t = x1; x1 = x2; x2 = t; }
    if (y0 > y1) { int t = y0; y0 = y1; y1 = t; t = x0; x0 = x1; x1 = t; }

    if (y0 == y2) {
        // Degenerate triangle (all on one line)
        int min_x = x0 < x1 ? (x0 < x2 ? x0 : x2) : (x1 < x2 ? x1 : x2);
        int max_x = x0 > x1 ? (x0 > x2 ? x0 : x2) : (x1 > x2 ? x1 : x2);
        ssd1306_draw_horizontal_line(min_x, max_x, y0, color);
        return;
    }

    int dx01 = x1 - x0, dy01 = y1 - y0;
    int dx02 = x2 - x0, dy02 = y2 - y0;
    int dx12 = x2 - x1, dy12 = y2 - y1;

    int sa = 0, sb = 0;
    int y;

    int last = (y1 == y2) ? y1 : y1 - 1;

    for (y = y0; y <= last; y++) {
        int a = x0 + sa / (dy01 == 0 ? 1 : dy01);
        int b = x0 + sb / (dy02 == 0 ? 1 : dy02);
        sa += dx01;
        sb += dx02;
        ssd1306_draw_horizontal_line(a, b, y, color);
    }

    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);

    for (; y <= y2; y++) {
        int a = x1 + sa / (dy12 == 0 ? 1 : dy12);
        int b = x0 + sb / (dy02 == 0 ? 1 : dy02);
        sa += dx12;
        sb += dx02;
        ssd1306_draw_horizontal_line(a, b, y, color);
    }
}

void ssd1306_overwrite_char(uint8_t x, uint8_t y, char old, char new, uint8_t old_size_x, uint8_t old_size_y, uint8_t new_size_x, uint8_t new_size_y)
{
	ssd1306_draw_char(x, y, old, old_size_x, old_size_y, COLOR_BLACK);
	ssd1306_draw_char(x, y, new, new_size_x, new_size_y, COLOR_WHITE);
}

void ssd1306_overwrite_string_char_by_char(uint8_t x, uint8_t y, const char* old_str, const char* new_str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms)
{
    while (*old_str || *new_str) {
		if (*old_str && *new_str) {
			ssd1306_overwrite_char(x, y, *old_str++, *new_str++, size_x, size_y, size_x, size_y);
		} else if (!(*old_str) && *new_str) {
			ssd1306_draw_char(x, y, *new_str++, size_x, size_y, COLOR_WHITE);
		} else if (*old_str && !(*new_str)) {
			ssd1306_draw_char(x, y, *old_str++, size_x, size_y, COLOR_BLACK);
		}
		x += (6 * size_x); // 5 pixels + 1 space, scaled
		ssd1306_display();
		vTaskDelay(tick_delay_ms / portTICK_PERIOD_MS);
    }
}

void ssd1306_overwrite_string_wrapped_char_by_char(uint8_t x, uint8_t y, const char* old_str, const char* new_str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms)
{
    uint8_t start_x = x;
    while (*old_str || *new_str) {
        // Wrap to next line if needed
        if (x + (6 * size_x) > SCREEN_WIDTH) {
            x = start_x;
            y += (8 * size_y);
            if (y >= SCREEN_HEIGHT) break;
        }

        if (*old_str && *new_str) {
            ssd1306_overwrite_char(x, y, *old_str++, *new_str++, size_x, size_y, size_x, size_y);
        } else if (!(*old_str) && *new_str) {
            ssd1306_draw_char(x, y, *new_str++, size_x, size_y, COLOR_WHITE);
        } else if (*old_str && !(*new_str)) {
            ssd1306_draw_char(x, y, *old_str++, size_x, size_y, COLOR_BLACK);
        }

        x += (6 * size_x);
        ssd1306_display();
        vTaskDelay(tick_delay_ms / portTICK_PERIOD_MS);
    }
}
