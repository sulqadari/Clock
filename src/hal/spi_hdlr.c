#include "spi_hdlr.h"
#include "rcc_hdlr.h"

void
spi_dma_init_master(void)
{
	rcc_periph_clock_enable(RCC_SPI1);
	rcc_periph_clock_enable(RCC_DMA1);
	
	CLEAR_REG(GPIOA->CRL);

	gpio_set_mode(GPIOA_BASE,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		GPIO5 | GPIO7);	// clock and MOSI, alternate function in push-pull mode
	
	gpio_set_mode(GPIOA_BASE,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		GPIO4);	// Slave Select, output in push-pull mode
	
	SET_BIT(SPI1->CR1, SPI_CR1_MSTR);	// Set SPI1 as master
	CLEAR_BIT(SPI1->CR1, SPI_CR1_BR_2);	// Speed rate: F_PCLK / 32
	SET_BIT(SPI1->CR1, SPI_CR1_SSM);	// Software-handled NSS
	SET_BIT(SPI1->CR1, SPI_CR1_SSI);	// Set NSS to high

	SET_BIT(SPI1->CR2, SPI_CR2_TXDMAEN);	// Enable DMA requests.
	SET_BIT(SPI1->CR1, SPI_CR1_SPE);	// Enable SPI
}

void
spi_trasmit(uint8_t* data, uint16_t len)
{
	CLEAR_BIT(DMA1_Channel3->CCR, DMA_CCR_EN);	// Turn DMA off
	DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR;	// Get address of SPI1->DR and store it in DMA
	DMA1_Channel3->CMAR = (uint32_t)data;		// Get address of data to be sent and store it in DMA
	DMA1_Channel3->CNDTR = len;

	CLEAR_BIT(DMA1_Channel3->CCR, DMA_CCR_MEM2MEM);	// Disable M2M mode
	CLEAR_BIT(DMA1_Channel3->CCR, DMA_CCR_PL);		// lowest priority
	CLEAR_BIT(DMA1_Channel3->CCR, DMA_CCR_MSIZE);	// 8-bit memory size
	SET_BIT(DMA1_Channel3->CCR, DMA_CCR_PSIZE_0);	// Peripheral size equals 16 bit
	SET_BIT(DMA1_Channel3->CCR, DMA_CCR_MINC);		// Enable memory increment mode
	CLEAR_BIT(DMA1_Channel3->CCR, DMA_CCR_PINC);	// Disable peripheral increment mode
	SET_BIT(DMA1_Channel3->CCR, DMA_CCR_DIR);		// Direction: from DMA to peripheral
	SET_BIT(DMA1_Channel3->CCR, DMA_CCR_EN);		// Enable DMA
}

void
spi_write(uint32_t spi, uint16_t data)
{
	/* Write data (8 or 16 bits, depending on DFF) into DR. */
	SPI_DR(spi) = data;
}

uint16_t
spi_xfer(uint32_t spi, uint16_t data)
{
	spi_write(spi, data);

	/* Wait for transfer finished. */
	while (!(SPI_SR(spi) & SPI_SR_RXNE));

	/* Read the data (8 or 16 bits, depending on DFF bit) from DR. */
	return SPI_DR(spi);
}

void
spi_enable(uint32_t spi)
{
	SPI_CR1(spi) |= SPI_CR1_SPE; /* Enable SPI. */
}

void spi_disable(uint32_t spi)
{
	uint32_t reg32;

	reg32 = SPI_CR1(spi);
	reg32 &= ~(SPI_CR1_SPE); /* Disable SPI. */
	SPI_CR1(spi) = reg32;
}