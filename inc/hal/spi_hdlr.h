#ifndef SPI_HDLR_H
#define SPI_HDLR_H

#include <stdint.h>
#include "stm32f1xx.h"

void spi_init_with_dma(void);
void spi_trasmit(uint8_t* data, uint16_t len);

#endif /* SPI_HDLR_H */