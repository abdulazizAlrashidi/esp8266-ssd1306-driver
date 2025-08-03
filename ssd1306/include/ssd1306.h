/**
 * @file ssd1306.h
 * @author Abdulaziz Alrashidi
 * @brief A compact driver for SSD1306 OLED display via I2C, tailored for Ideaspark NodeMCU ESP8266 with pre-installed SSD1306.
 * @version 0.1
 * @date 2025-08-02
 * @copyright Copyright (c) 2025
 * @license MIT
 * @see https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */

#ifndef SSD1306_H
#define SSD1306_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"
#include "driver/i2c.h"

// --- Pin Configuration ---
#define I2C_SCL_IO      14                                  //!< gpio number for I2C clock
#define I2C_SDA_IO      12                                  //!< gpio number for I2C data
#define I2C_NUM         I2C_NUM_0                           //!< I2C port number

// --- SSD1306 Constants ---
#define SSD1306_ADDR    0x3C                                //!< SSD1306 device address
#define SSD1306_CMD     0x00                                //!< command register address
#define SSD1306_DATA    0x40                                //!< data register address

#define WRITE_BIT       I2C_MASTER_WRITE                    //!< I2C write
#define ACK_CHECK_EN    0x1                                 //!< I2C master will check ack from slave
#define ACK_CHECK_DIS   0x0                                 //!< I2C master will not check ack from slave

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define BUFFER_SIZE     (SCREEN_WIDTH * SCREEN_HEIGHT / 8)  // 128 columns × 8 pages

#define COLOR_WHITE     1                                   // pixel on
#define COLOR_BLACK     0                                   // pixel off

// --- Font ---
extern const char font5x7[];

// --- Framebuffer ---
extern uint8_t buffer[BUFFER_SIZE];

// --- Function Prototypes ---
/**
 * @brief Initializes the I2C protocol (config and installation)
 * 
 * @return esp_err_t ESP_OK if initialized successfully
 */
esp_err_t i2c_init(void);


/**
 * @brief A wrapper for i2c_master_write specifically designed for ssd1306
 * 
 * Writes data_len bytes targeting specific ssd1306 register
 * 
 * @param i2c_num I2C port number to be used
 * @param reg_address the target register to be written into
 * @param data the data to be written
 * @param data_len the length of the data to be written
 */
void ssd1306_write(i2c_port_t i2c_num, uint8_t reg_address, uint8_t* data, size_t data_len);


/**
 * @brief Initializes the SSD1306 device with the proper initialization commands.
 * 
 * This must be called once in the beginning of every program that uses this driver.
 * 
 */
void ssd1306_init(void);


/**
 * @brief Sends a command to the SSD1306 device.
 * 
 * @param cmd The command to be sent.
 */
void ssd1306_cmd(uint8_t cmd);


/**
 * @brief Sends data to the SSD1306 device.
 * 
 * @param data An array of the data to be sent.
 * @param len The length of the array of the data to be sent.
 */
void ssd1306_data(uint8_t* data, size_t len);


/**
 * @brief Clears the internal framebuffer.
 * 
 * Sets all bytes in the framebuffer to zero, effectively clearing the display content in memory.
 * 
 * @note Call ssd1306_display() afterward to reflect the changes on the actual screen.
 */
void ssd1306_clear(void);


/**
 * @brief Displays the framebuffer.
 * 
 * Renders whatever in the framebuffer on the screen.
 * 
 */
void ssd1306_display(void);



/**
 * @brief Draws a single pixel on the screen.
 * 
 * @note Call ssd1306_display() afterward to render the pixel on the actual screen.
 * 
 * @param x 
 * @param y 
 * @param color 
 */
void ssd1306_draw_pixel(uint8_t x, uint8_t y, bool color);


/**
 * @brief Draws a single ASCII character on the screen.
 * 
 * @note Call ssd1306_display() afterward to render the char on the actual screen.
 * 
 * @param x X-coordinate of the top-left corner.
 * @param y Y-coordinate of the top-left corner.
 * @param c Character to draw.
 * @param size_x Horizontal scaling factor.
 * @param size_y Vertical scaling factor.
 * @param color Pixel color (COLOR_WHITE = on, COLOR_BLACK = off).
 */
void ssd1306_draw_char(uint8_t x, uint8_t y, char c, uint8_t size_x, uint8_t size_y, bool color);


/**
 * @brief Draws a string starting at a specified position.
 * 
 * @note Call ssd1306_display() afterward to render the string on the actual screen.
 * 
 * @param x X-coordinate of the starting position.
 * @param y Y-coordinate of the starting position.
 * @param str Null-terminated string to draw.
 * @param size_x Horizontal scaling factor.
 * @param size_y Vertical scaling factor.
 * @param color Pixel color (COLOR_WHITE = on, COLOR_BLACK = off).
 */
void ssd1306_draw_string(uint8_t x, uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, bool color);


/**
 * @brief Draws a wrapped string, breaking lines if the text exceeds the screen width.
 * 
 * @note Call ssd1306_display() afterward to render the string on the actual screen.
 * 
 * @param x Starting X-coordinate.
 * @param y Starting Y-coordinate.
 * @param str String to draw.
 * @param size_x Horizontal scaling factor.
 * @param size_y Vertical scaling factor.
 * @param color Pixel color (COLOR_WHITE = on, COLOR_BLACK = off).
 */
void ssd1306_draw_string_wrapped(uint8_t x, uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, bool color);


/**
 * @brief Draws a string character-by-character with a delay between each character.
 * 
 * @note No need to call ssd1306_display() afterward.
 * 
 * @param x Starting X-coordinate.
 * @param y Starting Y-coordinate.
 * @param str String to draw.
 * @param size_x Horizontal scaling factor.
 * @param size_y Vertical scaling factor.
 * @param tick_delay_ms Delay in milliseconds between characters.
 * @param color Pixel color (COLOR_WHITE = on, COLOR_BLACK = off).
 */
void ssd1306_draw_string_char_by_char(uint8_t x, uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms, bool color);


/**
 * @brief Draws a wrapped string character-by-character with a delay.
 * 
 * @note No need to call ssd1306_display() afterward.
 * 
 * @param x Starting X-coordinate.
 * @param y Starting Y-coordinate.
 * @param str String to draw.
 * @param size_x Horizontal scaling factor.
 * @param size_y Vertical scaling factor.
 * @param tick_delay_ms Delay between each character in milliseconds.
 * @param color Pixel color (COLOR_WHITE = on, COLOR_BLACK = off).
 */
void ssd1306_draw_string_wrapped_char_by_char(uint8_t x, uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms, bool color);


/**
 * @brief Draws a horizontally centered string at a specific vertical position.
 * 
 * @note Call ssd1306_display() afterward to render the string on the actual screen.
 * 
 * @param y Y-coordinate for vertical position.
 * @param str String to draw.
 * @param size_x Horizontal scaling factor.
 * @param size_y Vertical scaling factor.
 * @param color Pixel color (COLOR_WHITE = on, COLOR_BLACK = off).
 */
void ssd1306_draw_string_centered(uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, bool color);


/**
 * @brief Draws a horizontally centered string character-by-character with a delay.
 * 
 * @note No need to call ssd1306_display() afterward.
 * 
 * @param y Y-coordinate for vertical position.
 * @param str String to draw.
 * @param size_x Horizontal scaling factor.
 * @param size_y Vertical scaling factor.
 * @param tick_delay_ms Delay between each character in milliseconds.
 * @param Pixel color (COLOR_WHITE = on, COLOR_BLACK = off).
 */
void ssd1306_draw_string_centered_char_by_char(uint8_t y, const char* str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms, bool color);


/**
 * @brief Calculates the width in pixels of a string at a given scale.
 * 
 * @param str String to measure.
 * @param size_x Horizontal scaling factor.
 * @return Width of the string in pixels.
 */
uint8_t ssd1306_get_string_width(const char* str, uint8_t size_x);



/**
 * @brief Draws a filled rectangle.
 * 
 * @note Call ssd1306_display() afterward to render the rectangle on the actual screen.
 * 
 * @param x X-coordinate of the top-left corner.
 * @param y Y-coordinate of the top-left corner.
 * @param w Width of the rectangle.
 * @param h Height of the rectangle.
 * @param color Pixel color.
 */
void ssd1306_draw_full_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color);


/**
 * @brief Draws an empty (outlined) rectangle.
 * 
 * @note Call ssd1306_display() afterward to render the rectangle on the actual screen.
 * 
 * @param x X-coordinate of the top-left corner.
 * @param y Y-coordinate of the top-left corner.
 * @param w Width of the rectangle.
 * @param h Height of the rectangle.
 * @param color Pixel color.
 */
void ssd1306_draw_empty_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color);


/**
 * @brief Draws a filled circle.
 * 
 * @note Call ssd1306_display() afterward to render the circle on the actual screen.
 * 
 * @param x0 Center X-coordinate.
 * @param y0 Center Y-coordinate.
 * @param radius Radius of the circle.
 * @param color Pixel color.
 */
void ssd1306_draw_full_circle(uint8_t x0, uint8_t y0, uint8_t radius, bool color);


/**
 * @brief Draws an empty (outlined) circle.
 * 
 * @note Call ssd1306_display() afterward to render the circle on the actual screen.
 * 
 * @param x0 Center X-coordinate.
 * @param y0 Center Y-coordinate.
 * @param radius Radius of the circle.
 * @param color Pixel color.
 */
void ssd1306_draw_empty_circle(uint8_t x0, uint8_t y0, uint8_t radius, bool color);


/**
 * @brief Draws a line between two points using Bresenham's algorithm.
 * 
 * @note Call ssd1306_display() afterward to render the line on the actual screen.
 * 
 * @param x0 Starting X-coordinate.
 * @param y0 Starting Y-coordinate.
 * @param x1 Ending X-coordinate.
 * @param y1 Ending Y-coordinate.
 * @param color Pixel color.
 */
void ssd1306_draw_line(int x0, int y0, int x1, int y1, bool color);


/**
 * @brief Draws a horizontal line.
 * 
 * @note Call ssd1306_display() afterward to render the line on the actual screen.
 * 
 * @param x_start Starting X-coordinate.
 * @param x_end Ending X-coordinate.
 * @param y Y-coordinate for the line.
 * @param color Pixel color.
 */
void ssd1306_draw_horizontal_line(int x_start, int x_end, int y, bool color);


/**
 * @brief Draws an empty (outlined) triangle.
 * 
 * @note Call ssd1306_display() afterward to render the triangle on the actual screen.
 * 
 * @param x0 X of point 1.
 * @param y0 Y of point 1.
 * @param x1 X of point 2.
 * @param y1 Y of point 2.
 * @param x2 X of point 3.
 * @param y2 Y of point 3.
 * @param color Pixel color.
 */
void ssd1306_draw_empty_triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool color);


/**
 * @brief Draws a filled triangle.
 * 
 * @note Call ssd1306_display() afterward to render the triangle on the actual screen.
 * 
 * @param x0 X of point 1.
 * @param y0 Y of point 1.
 * @param x1 X of point 2.
 * @param y1 Y of point 2.
 * @param x2 X of point 3.
 * @param y2 Y of point 3.
 * @param color Pixel color.
 */
void ssd1306_draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool color);



/**
 * @brief Overwrites a character on screen with a new one.
 * 
 * @note Call ssd1306_display() afterward to render overwritten char on the actual screen.
 * 
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param old Character to be replaced.
 * @param new New character.
 * @param old_size_x Old character horizontal size.
 * @param old_size_y Old character vertical size.
 * @param new_size_x New character horizontal size.
 * @param new_size_y New character vertical size.
 */
void ssd1306_overwrite_char(uint8_t x, uint8_t y, char old, char new, uint8_t old_size_x, uint8_t old_size_y, uint8_t new_size_x, uint8_t new_size_y);


/**
 * @brief Overwrites one string with another character-by-character.
 * 
 * @note No need to call ssd1306_display() afterward.
 * 
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param old_str Old string to be erased.
 * @param new_str New string to be drawn.
 * @param size_x Scaling factor in x-direction.
 * @param size_y Scaling factor in y-direction.
 * @param tick_delay_ms Delay between characters in milliseconds.
 */
void ssd1306_overwrite_string_char_by_char(uint8_t x, uint8_t y, const char* old_str, const char* new_str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms);


/**
 * @brief Overwrites a wrapped string character-by-character.
 * 
 * @note No need to call ssd1306_display() afterward.
 * 
 * @param x Starting X-coordinate.
 * @param y Starting Y-coordinate.
 * @param old_str Old string to be erased.
 * @param new_str New string to be drawn.
 * @param size_x Horizontal scale factor.
 * @param size_y Vertical scale factor.
 * @param tick_delay_ms Delay between each character in milliseconds.
 */
void ssd1306_overwrite_string_wrapped_char_by_char(uint8_t x, uint8_t y, const char* old_str, const char* new_str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms);


/**
 * @brief Overwrites a centered string character-by-character
 * 
 * @note No need to call ssd1306_display afterward.
 * 
 * @param y Starting Y-coordinate.
 * @param old_str Old string to be erased
 * @param new_str New string to be drawn.
 * @param size_x Horizontal scale factor.
 * @param size_y Vertical scale factor.
 * @param tick_delay_ms Delay between each character in milliseconds.
 */
void ssd1306_overwrite_string_centered_char_by_char(uint8_t y, const char* old_str, const char* new_str, uint8_t size_x, uint8_t size_y, uint32_t tick_delay_ms);

#endif // SSD1306_H
