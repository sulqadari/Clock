#include "spi_hdlr.h"

void
spi_init_with_dma(void)
{
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN); // Enable port A and SPI1
	RCC->AHBENR  |= RCC_AHBENR_DMA1EN; // Enable DMA 1
	
	GPIOA->CRL |= (GPIO_CRL_MODE5  | GPIO_CRL_MODE7);	// Configure ports No A4, A5 and A7 as output with 50MHz clock rate.
	GPIOA->CRL &= (GPIO_CRL_CNF5   | GPIO_CRL_CNF7);	// Clean the state of ports No A5 and A7
	GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1);	// Refigure ports No A5 and A7 as output alternate functions in push-pull mode,
														// while A4 is left in default state (ordinary output in push-pull mode)
	SPI1->CR1 |= SPI_CR1_MSTR;	// Set SPI1 as master
	SPI1->CR1 &= ~SPI_CR1_BR;	// Divider 2
	SPI1->CR1 |= SPI_CR1_SSM;	// Software-handled NSS
	SPI1->CR1 |= SPI_CR1_SSI;	// Set NSS to high
	SPI1->CR2 |= SPI_CR2_TXDMAEN;	// Enable DMA requests.
	SPI1->CR1 |= SPI_CR1_SPE;	// Enable SPI

	DMA1_Channel3->CCR |= DMA_CCR_PSIZE_0;	// The size of peripheral equals 1 byte
	DMA1_Channel3->CCR |= DMA_CCR_DIR;		// Direction: from DMA to peripheral
	DMA1_Channel3->CCR |= DMA_CCR_MINC;		// Enable memory increment
	DMA1_Channel3->CCR |= DMA_CCR_PL;		// DMA hash highest priority
}

void
spi_trasmit(uint8_t byte)
{
	CS_RES;
	
	while (!(SPI1->SR & SPI_SR_TXE)) { }
	SPI1->DR = byte;

	CS_SET;
}