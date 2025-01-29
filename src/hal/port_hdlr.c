#include "port_hdlr.h"

void
hal_enablePort(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;	// Provide port B with clock.
}

void
hal_setPortMode(void)
{
	GPIOC->CRL = 0;
	GPIOC->CRH |= GPIO_CRH_MODE13_1;	// MODE: Output 2MHz; CNF: Push-Pull;
}

void
hal_pinHigh(void)
{
	GPIOC->BSRR = GPIO_BSRR_BS13;
}

void
hal_pinLow(void)
{
	GPIOC->BRR = GPIO_BSRR_BS13;
}