#ifndef PORT_HDLR_H
#define PORT_HDLR_H

#include <stdint.h>
#include "stm32f1xx.h"

void hal_enablePort(void);
void hal_setPortMode(void);
void hal_pinHigh(void);
void hal_pinLow(void);

#endif /* PORT_HDLR_H */