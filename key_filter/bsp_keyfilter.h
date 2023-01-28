#ifndef _BSP_KEYFILTER_H_
#define _BSP_KEYFILTER_H_

#include "bsp_exit.h"
#include "bsp_epittimer.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_beep.h"

void filterkey_init(void);
void filtertimer_init(unsigned int value);
void filtertimer_stop(void);
void filtertimer_restart(unsigned int value);
void filtertimer_irqhandler(void);
void gpio1_16_31_irqhandler(void);
#endif