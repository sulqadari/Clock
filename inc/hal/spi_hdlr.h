#ifndef SPI_HDLR_H
#define SPI_HDLR_H

#include <stdint.h>
#include "stm32f1xx.h"
#include "gpio_hdlr.h"

#define SPI_DR(spi_base)		MMIO32((spi_base) + 0x0c)
#define SPI_SR(spi_base)		MMIO32((spi_base) + 0x08)
#define SPI_CR1(spi_base)		MMIO32((spi_base) + 0x00)

void spi_dma_init_master(void);
void spi_trasmit(uint8_t* data, uint16_t len);
void spi_write(uint32_t spi, uint16_t data);
uint16_t spi_xfer(uint32_t spi, uint16_t data);
void spi_enable(uint32_t spi);
void spi_disable(uint32_t spi);

#endif /* SPI_HDLR_H */