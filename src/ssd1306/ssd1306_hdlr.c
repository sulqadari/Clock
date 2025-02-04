#include "ssd1306_hdlr.h"
#include "spi_hdlr.h"

static uint8_t displayBuff[BUFFER_SIZE];

static void
trasmit_command(uint8_t command)
{
	COMMAND;
	spi_trasmit(command);
	DATA;
}

void
ssd1306_init(void)
{
	GPIOA->CRL |= GPIO_CRL_MODE2 |GPIO_CRL_MODE1 | GPIO_CRL_MODE3;
	GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3);	// Configure ports PA1,PA2,PA3 as output
	
	RESET_RES;									// Reset the screen and 
	for(uint16_t i = 0; i < BUFFER_SIZE; i++)	// flush the buffer
		displayBuff[i] = 0;

	RESET_SET;
	CS_SET;		// Select the slave

	trasmit_command(0xAE); //display off
	trasmit_command(0xD5); //Set Memory Addressing Mode
	trasmit_command(0x80); //00,Horizontal Addressing Mode;01,Vertical
	trasmit_command(0xA8); //Set Page Start Address for Page Addressing
	trasmit_command(0x3F); //Set COM Output Scan Direction
	trasmit_command(0xD3); //set low column address
	trasmit_command(0x00); //set high column address
	trasmit_command(0x40); //set start line address
	trasmit_command(0x8D); //set contrast control register
	trasmit_command(0x14);
	trasmit_command(0x20); //set segment re-map 0 to 127
	trasmit_command(0x00); //set normal display
	trasmit_command(0xA1); //set multiplex ratio(1 to 64)
	trasmit_command(0xC8); //
	trasmit_command(0xDA); //0xa4,Output follows RAM
	trasmit_command(0x12); //set display offset
	trasmit_command(0x81); //not offset
	trasmit_command(0x8F); //set display clock divide ratio/oscillator frequency
	trasmit_command(0xD9); //set divide ratio
	trasmit_command(0xF1); //set pre-charge period
	trasmit_command(0xDB); 
	trasmit_command(0x40); //set com pins hardware configuration
	trasmit_command(0xA4);
	trasmit_command(0xA6); //set vcomh
	trasmit_command(0xAF); //0x20,0.77xVcc
}

void
ssd1306_enable_dma(void)
{
	DATA;
	DMA1_Channel3->CCR  &= ~(DMA_CCR_EN);			// Provide the DMA with the clock
	DMA1_Channel3->CPAR  = (uint32_t)(&SPI1->DR);	// Store in DMA the SPI data register
	DMA1_Channel3->CMAR  = (uint32_t)&displayBuff;	// Data address
	DMA1_Channel3->CNDTR = sizeof(displayBuff);		// Data size
	DMA1->IFCR          &= ~(DMA_IFCR_CGIF3);
	
	CS_RES; //Select the slave
	DMA1_Channel3->CCR |= DMA_CCR_CIRC;	// Set cyclic mode of the DMA
	DMA1_Channel3->CCR |= DMA_CCR_EN;	// Enable DMA
}

void
ssd1306_fill_display(Color color)
{
	for (uint16_t i = 0; i < SSD1306_WIDTH; ++i) {
		displayBuff[i] = color;
	}
}