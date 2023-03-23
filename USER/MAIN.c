#include "stm32f10x.h"
#include "stdint.h"

#include "drvlcd1602.h"
#include "bsp_SysTick.h"
#include "drvpwm.h"



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
		lcd_dis_str(0, 1, 7, (unsigned char *)"LCD1602");	//��ʾ��ʾ��Ϣ
		lcd_dis_str(1, 0, 16, (unsigned char *)"1111222233334444");
	}
}

//PWM
int main_pwm(void)
{
	SystemCoreClockUpdate();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	
	pwm_init_timer1(0, 7199);	//10K
	
	while (1) 
	{
		pwm_set_duty(3599, 3599);
	}
}

int main(void)	
{
	main_pwm();
}
