# ESP8266 SSD1306 I2C OLED Driver

A compact and efficient C driver for SSD1306 OLED displays using the I2C interface, specifically tailored for the Ideaspark NodeMCU ESP8266 board. This driver allows full control over a 128x64 OLED display, including pixel drawing, text rendering, shape primitives, and animation-friendly features.


## Features

- I2C communication with SSD1306
- Framebuffer-based drawing (fast + flexible)
- Draw pixels, lines, rectangles, circles, triangles
- Text rendering with scalable fonts
- String wrapping, centering, and overwrite effects
- Animation-friendly draw modes (e.g., char-by-char)
- Fully documented, modular, and extendable

## Directory Structure

```

.
├── example               # Example app using the driver
│   └── main              # Contains the example logic
├── ssd1306               # The actual SSD1306 driver
│   ├── include           # Header file (public API)
│   └── ssd1306.c         # Implementation

````

## Getting Started

### Requirements

- [Ideaspark NodeMCU ESP8266 board](https://a.co/d/aJsB1xZ)
- SSD1306 OLED display (128x64, I2C), comes with the MCU!
- [ESP8266 RTOS SDK](https://github.com/espressif/ESP8266_RTOS_SDK) development environment

### Wiring (default pins)

| OLED Pin | ESP8266 Pin |
|----------|-------------|
| SDA      | GPIO12      |
| SCL      | GPIO14      |
| VCC      | 3.3V        |
| GND      | GND         |

### Build & Flash

```bash
cd example
make flash monitor
````

## Usage

Include the header:

```c
#include "ssd1306.h"
```

Initialize I2C and the display:

```c
ssd1306_init();
ssd1306_draw_string(0, 0, "Hello OLED!", 1, 1, COLOR_WHITE);
ssd1306_display();
```

## API Highlights

* `ssd1306_draw_pixel(x, y, color)`
* `ssd1306_draw_string(x, y, str, sx, sy, color)`
* `ssd1306_draw_full_rect(x, y, w, h, color)`
* `ssd1306_draw_full_circle(x, y, r, color)`
* `ssd1306_display()` – Pushes framebuffer to screen

See [`ssd1306.h`](/ssd1306/include/ssd1306.h) for full API reference.


## Resources

* [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

**Author:** Abdulaziz Alrashidi
