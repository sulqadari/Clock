#ifndef SPI_HDLR_H
#define SPI_HDLR_H

#include <stdint.h>
#include "stm32f1xx.h"

#define BUFFER_SIZE 1024

#define CS_SET		GPIOA->BSRR |= GPIO_BSRR_BS2
#define CS_RES		GPIOA->BSRR |= GPIO_BSRR_BR2
#define RESET_SET	GPIOA->BSRR |= GPIO_BSRR_BS1
#define RESET_RES	GPIOA->BSRR |= GPIO_BSRR_BR1
#define DATA		GPIOA->BSRR |= GPIO_BSRR_BS3
#define COMMAND		GPIOA->BSRR |= GPIO_BSRR_BR3

void spi_init_with_dma(void);
void spi_trasmit(uint8_t* data, uint16_t len);

#endif /* SPI_HDLR_H */