#ifndef SSD1306_HDLR_H
#define SSD1306_HDLR_H

#include <stdint.h>
#include "stm32f1xx.h"

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64
#define BUFFER_SIZE    1024

typedef enum Color
{
	clr_black,
	clr_white = 0xFF
} Color;

void ssd1306_init(void);
void ssd1306_fill_display(Color color);
void ssd1306_enable_dma(void);

#endif /* SSD1306_HDLR_H */