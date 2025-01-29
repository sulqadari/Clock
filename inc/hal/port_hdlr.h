#ifndef PORT_HDLR_H
#define PORT_HDLR_H

#include <stdint.h>
#include "stm32f1xx.h"

void hal_enablePort(void);
void hal_setPortMode(void);
void hal_PinHigh(void);
void hal_PinLow(void);

#endif /* PORT_HDLR_H */