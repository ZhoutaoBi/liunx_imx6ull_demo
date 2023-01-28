#ifndef _BSP_EPITTIMER_H
#define _BSP_EPITTIMER_H

#include "imx6ul.h"
#include "bsp_led.h"
#include "bsp_int.h"

void epit1_init(unsigned int frac,unsigned int value);
void epit1_irqhandler(void);



#endif