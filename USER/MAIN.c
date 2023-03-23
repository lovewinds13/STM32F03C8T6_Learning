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

//LCD1602 ����
int main_lcd1602(void)
{
	SystemCoreClockUpdate();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	SysTick_Init();
	LCD1602_Init();	//LCD1602	��ʼ��
	
	while (1) 
	{
		lcd_dis_str(0, 1, 3, (unsigned char *)"CAR");	//��ʾ��ʾ��Ϣ
	}
}

int main(void)	
{
	main_lcd1602();
}