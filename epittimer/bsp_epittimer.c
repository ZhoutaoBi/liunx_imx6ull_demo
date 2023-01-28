#include "bsp_epittimer.h"

void epit1_init(unsigned int frac,unsigned int value)
{
    if(frac > 0xfff)
    frac = 0xfff;

    EPIT1->CR = 0;
    
    EPIT1->CR &=~(1<<25);/*Selecting a Clock Source Peripheral clock=66MHz*/
    EPIT1->CR |= (1<<24);

    EPIT1->CR |= (frac<<4);/*Counter clock prescaler value*/

    EPIT1->CR |= (1<<3);/*set-and-forget mode*/

    EPIT1->CR |= (1<<2);/*Output compare interrupt enable.*/
    EPIT1->CR |= (1<<1);/*Counter starts count from load value (RLD=1) or 0xFFFF_FFFF (If RLD=0)*/

    EPIT1->LR = value;
    EPIT1->CMPR = 0;

    /*init interrupt*/
    /*enable CIC*/
    /*register irqhandler*/
    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn,(system_irq_handler_t)epit1_irqhandler,NULL);

    EPIT1->CR |= 1<<0; /* enable EPIT1 */
}

void epit1_irqhandler(void)
{
    static unsigned char state = 0;
    state = !state;
    if(EPIT1->SR & (1<<0)) /* 判断比较事件发生 */
    {
    led_switch(LED0, state); /* 定时器周期到，反转 LED */
    }
    EPIT1->SR |= 1<<0; /* 清除中断标志位 */

}