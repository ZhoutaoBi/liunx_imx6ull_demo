/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_delay.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : 延时文件。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
***************************************************************/
#include "bsp_delay.h"

void delay_init(void)
{
	GPT1->CR = 0;
	GPT1->IR = 0;
	GPT1->CR |= (1<<15);/*enter software restart*/
	while((GPT1->CR >> 15) & 0x01);/*Wait for the software restart to complete*/
	GPT1->CR |= (1<<6);/*GPT clock source ipg_clk=66Mhz*/
	GPT1->PR = 65;/*Frequency division register 66/(65+1) = 1Mhz = 1us*/

	GPT1->OCR[0] = 0XFFFFFFFF;/*We set the comparison value to the maximum 0XFFFFFFFF*/



	GPT1->CR |= (1<<0);/*enable GPT1*/

}
void delayus(unsigned int usdelay)
{
	unsigned long oldcnt,newcnt;/*32 bits so choose the long type*/
	unsigned long tcntvalue = 0;
	GPT1->CNT = oldcnt;
	while (1)
	{
		newcnt = GPT1->CNT;
		if(newcnt!=oldcnt)
		{
			if(newcnt>oldcnt)
			{
				tcntvalue += (newcnt-oldcnt);
			}
			else
			{
				tcntvalue += (0XFFFFFFFF-oldcnt + newcnt);
			}
			oldcnt = newcnt;
		}
		if(tcntvalue >= usdelay)break;
	}	
}
void delayms(unsigned int msdelay)
{
	unsigned int i=0;
	for(i=0;i<msdelay;i++)
	{
		delayus(1000);
	}
}




/*
 * @description	: 短时间延时函数
 * @param - n	: 要延时循环次数(空操作循环次数，模式延时)
 * @return 		: 无
 */
void delay_short(volatile unsigned int n)
{
	while(n--){}
}

/*
 * @description	: 延时函数,在396Mhz的主频下
 * 			  	  延时时间大约为1ms
 * @param - n	: 要延时的ms数
 * @return 		: 无
 */
void delay(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}


