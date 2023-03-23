#include "stm32f10x.h"
#include "stdint.h"

#include "drvlcd1602.h"
#include "bsp_SysTick.h"

#include <stdio.h>
#include "math.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 

//LCD1602 测试
int main_lcd1602(void)
{
	SystemCoreClockUpdate();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	SysTick_Init();
	LCD1602_Init();	//LCD1602	初始化
	
	while (1) 
	{
		lcd_dis_str(0, 1, 7, (unsigned char *)"LCD1602");	//显示提示信息
		lcd_dis_str(1, 0, 16, (unsigned char *)"1111222233334444");
	}
}

int main(void)	
{
	main_lcd1602();
}
