#ifndef RCC_HDLR_H
#define RCC_HDLR_H

#include <stdint.h>
#include "stm32f1xx.h"
#include "gpio_hdlr.h"

#define _RCC_REG(i)			MMIO32(RCC_BASE + ((i) >> 5))
#define _RCC_BIT(i)			(1 << ((i) & 0x1f))
#define _REG_BIT(base, bit)	(((base) << 5) + (bit))

enum rcc_periph_clken {
  RCC_GPIOA	= _REG_BIT(0x18, 2),/*VNC*/
  RCC_GPIOB	= _REG_BIT(0x18, 3),/*VNC*/
  RCC_GPIOC	= _REG_BIT(0x18, 4),/*VNC*/
  RCC_SPI1	= _REG_BIT(0x18, 12),/*VNC*/
};

int32_t rcc_set_hse72(void);
void rcc_periph_clock_enable(enum rcc_periph_clken clken);

#endif /* RCC_HDLR_H */