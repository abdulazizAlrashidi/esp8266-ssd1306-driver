/**
 * @file ssd1306_example.c
 * @author Abdulaziz Alrashidi
 * @brief Example using ssd1306 driver
 * @version 0.1
 * @date 2025-08-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "ssd1306.h"

void app_main()
{
  ssd1306_init();
  ssd1306_draw_string(0, 0, "Hello OLED!", 1, 1, COLOR_WHITE);
  ssd1306_display();
}
