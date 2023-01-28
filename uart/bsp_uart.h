#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "imx6ul.h"
#include "bsp_gpio.h"
void uart_init(void);
void uart_io_init(void);
void uart_setbaudrate(UART_Type *base,unsigned int baudrate,unsigned int srcclock_hz);
void uart_disable(UART_Type *base);
void uart_enable(UART_Type *base);
void uart_softreseset(UART_Type *base);
void putc(unsigned char c);
unsigned char getc(void);
void puts(char *str);
#endif 