#include "bsp_keyfilter.h"

void filterkey_init(void)
{
    /*Configure GPIO multiplexing*/
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);
    /*Configure GPIO electrical properties*/
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0xF080);
    
    /*Configure GPIO interrupt, input and output mode*/
    gpio_pin_config_t key_config;
    key_config.direction = kGPIO_DigitalInput;
    key_config.outputLogic = 1;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    gpio_init(GPIO1,18,&key_config);
    /*enable GIC*/
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    /*register irqhandler*/
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn,(system_irq_handler_t)gpio1_16_31_irqhandler,NULL);
    /*enable interrupt*/
    gpio_enableint(GPIO1, 18);
    /*EPIT init 66MHz   10ms*/
    filtertimer_init(66000000/100);
}

void filtertimer_init(unsigned int value)
{
    EPIT1->CR = 0;/*Clear the EPIT1 control register*/
    /*
     *[25:24] = 0 1  Select clock source.
     *[3] = 1        Counter reload control.
     *[2] = 1        Output compare interrupt enable. 
     *[1] = 1        EPIT enable mode.
     */
    EPIT1->CR = (1<<24 | 1<<3 | 1<<2 | 1<<1);

    /*Load register entering stored values*/
    EPIT1->LR = value;
    /*The EPIT compare register (EPIT_CMPR) holds the value that determines when a compare event is generated*/
    EPIT1->CMPR = 0;
    /*enable GIC*/
    GIC_EnableIRQ(EPIT1_IRQn);
    /*register irqhandler*/
    system_register_irqhandler(EPIT1_IRQn,(system_irq_handler_t)filtertimer_irqhandler,NULL);
    /*enable gpio interrupt*/
    gpio_enableint(GPIO1, 18);
}

/**/
void filtertimer_stop(void)
{
EPIT1->CR &= ~(1<<0); /* disable the EPIT1 timer*/
}

void filtertimer_restart(unsigned int value)/* enable the EPIT1 timer*/
{
    EPIT1->CR &= ~(1<<0); 
    EPIT1->LR = value;
    EPIT1->CR |= (1<<0);
}


/*
Normally, EPIT1 timer interrupts run at the same time as external keypress interrupts. For EPIT1 timer interrupts,
It stops after only one run. Because EPIT1->SR becomes 1, the filtertimer_stop function stops EPIT1;
When the keypress is interrupted, the press is restarted because of the filtertimer_restart function.
When the button is pressed, the EPIT1 timer starts to tick. After 10ms, the EPIT1 timer interrupt is triggered
If the button is still pressed, the shaking is successfully eliminated.
*/
void filtertimer_irqhandler(void)/*EPIT1 timer interrupt handler*/
{
    
    static unsigned char state = OFF;
    /*determines whether the comparison event has occurred*/
    /*Only the bit0 bits are kept*/
    if(EPIT1->SR&(1<<0)) 
    {
        filtertimer_stop();
        if(gpio_pinread(GPIO1,18) == 0)
        {
            state = !state;
            beep_switch(state);
        }
    }
    EPIT1->SR |= 1<<0;  /*Force the comparison event to complete*/

}

void gpio1_16_31_irqhandler(void)/*External interrupt triggers, press the falling edge*/
{
    filtertimer_restart(66000000/100);/*Once triggered, reset the EPIT timer*/
    gpio_clearintflags(GPIO1, 18);
}

