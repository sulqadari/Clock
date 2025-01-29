#include "stm32f1xx.h"
#include "clock_src.h"
#include "port_hdlr.h"

int
main(void)
{
	hal_clockInit();
	while (0) {
		hal_enablePort();
		hal_setPortMode();
		hal_PinHigh();
		hal_PinLow();
	}
}