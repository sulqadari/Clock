#include "stm32f1xx.h"
#include "rcc_hdlr.h"
#include "gpio_hdlr.h"
#include "spi_hdlr.h"
#include "ugui.h"
#include <string.h>

typedef struct {
	int value;
} Canvas;

static UG_GUI gui;
static uint8_t dummy;
uint8_t pixmap[128 * 64 / 8];

static uint8_t *
to_pixel(short x,short y,unsigned *bitno) {
	*bitno = 7 - y % 8;	// Inverted

	if ( x < 0 || x >= 128
	  || y < 0 || y >= 64 )
	  	return &dummy;

	unsigned inv_y = 63 - y;
	unsigned pageno = inv_y / 8;
	unsigned colno = x % 128;

	return &pixmap[pageno * 128 + colno];
}

static void
draw_point(short x,short y,short pen) {

	if ( x < 0 || x >= 128 || y < 0 || y >= 64 )
		return;

	unsigned bitno;
	uint8_t *byte = to_pixel(x,y,&bitno);
	uint8_t mask = 1 << bitno;
	
	switch ( pen ) {
	case 0:
		*byte &= ~mask;
		break;
	case 1:
		*byte |= mask;
		break;
	default:
		*byte ^= mask;
	}
}

static int
ug_to_pen(UG_COLOR c) {

	switch ( c ) {
	case C_BLACK:
		return 0;
	case C_RED:
		return 2;
	default:
		return 1;
	}
}

static UG_COLOR
pen_to_ug(int pen) {

	switch ( pen ) {
	case 0:
		return C_BLACK;
	case 2:
		return C_RED;
	default:
		return C_WHITE;
	}	
}

static void
local_draw_point(UG_S16 x,UG_S16 y,UG_COLOR c) {
	draw_point(x,y,ug_to_pen(c));
}

void
canvas_init(Canvas* canvas)
{
	canvas->value = 0;
	memset(pixmap, 0, 128 * 64 / 8);

	UG_Init(&gui, local_draw_point, 128, 64);
	UG_SetBackcolor(pen_to_ug(1));
	UG_SetForecolor(pen_to_ug(0));
}

static void
oled_init(void)
{
	static uint8_t cmds[] = {
		0xAE, 0x00, 0x10, 0x40, 0x81, 0xCF, 0xA1, 0xA6, 
		0xA8, 0x3F, 0xD3, 0x00, 0xD5, 0x80, 0xD9, 0xF1, 
		0xDA, 0x12, 0xDB, 0x40, 0x8D, 0x14, 0xAF, 0xFF };

	gpio_clear(GPIOC_BASE,GPIO13);

	gpio_clear(GPIOB_BASE,GPIO10);
	spi_enable(SPI1);
	for ( unsigned ux=0; cmds[ux] != 0xFF; ++ux )
		spi_xfer(SPI1,cmds[ux]);
	spi_disable(SPI1);

	gpio_set(GPIOC_BASE,GPIO13);
}

int
main(void)
{
	Canvas canvas;

	rcc_set_hse72();
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC_BASE,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);
	// PB10 -> D/C, PB11 -> RES
	gpio_set_mode(GPIOB_BASE,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO10 | GPIO11);

	// Activate OLED Reset line
	gpio_clear(GPIOB_BASE,GPIO11); 

	spi_dma_init_master();

	oled_init();
	canvas_init(&canvas);

	while (1) {
		gpio_toggle(GPIOC_BASE, GPIO13);
		for(volatile int i = 0; i < 0x200000; i++) ;
	}
}