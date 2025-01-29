#include "spi_hdlr.h"

void
hal_initSPI1(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;					// Enable port A
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;					// Enable SPI1
	GPIOA->CRL    = 0;
	GPIOA->CRL   |= GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5_1;	// SCK as ALTF push-pull, 2 MHz
	GPIOA->CRL   |= GPIO_CRL_CNF6_1; 					// MISO as input floating/pull-up
	GPIOA->CRL   |= GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7_1; // MOSI as ALTF push-pull, 2 MHz
}