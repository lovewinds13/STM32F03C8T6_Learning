#include "stm32f10x.h"
#include "stdint.h"

#include "drvlcd1602.h"
#include "bsp_SysTick.h"
#include "drvpwm.h"
#include "drvadc.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

//PWM
int main_pwm(void)
{
	SystemCoreClockUpdate();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	
	pwm_init_timer1(0, 7199);	//10K
	
	while (1) 
	{
		pwm_set_duty(3599, 3599);
	}
}

void adc_delay(u32 tim)
{
    for(;tim>0;tim--);
}
//ADC TEST
int main_adc(void)
{
	uint8_t adc_buf[6] = {0};
	uint16_t advalue,temp0, temp1;
	float voltage;
	
	SystemCoreClockUpdate();
//	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI); 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
//	SysTick_Init();
	LCD1602_Init();	//LCD1602	初始化
	ADC_Configuration();
//	Adc_Init();
	lcd_dis_str(0, 1, 4, (unsigned char *)"ADC:");
	
	while (1)
	{
		#if 1 
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);	        //软件启动ADC转换 
        //while(ADC_GetITStatus(ADC1,ADC_IT_EOC) == RESET);
//        adc_delay(100000);
        advalue = ADC_GetConversionValue(ADC1);
        advalue &=0x0FFF;
		LCD1602_SetCursor(0, 6);
		lcd_show_num(advalue, 4);	//显示ADC读取数值0-4095
//        voltage =(float)advalue*(5000/4096.0);
		voltage =(float)advalue*(3300/4096.0);	//0-3.3V
		advalue = (uint16_t)voltage;
		temp0 = advalue/1000;
//        printf("ADC Result:%d",temp);
		temp1 = advalue%1000;
//        printf(".%d V\r\n",temp);
		sprintf((char *)adc_buf, "%d.%d", temp0, temp1);
		lcd_dis_str(1, 1, 5, (unsigned char *)adc_buf);	//显示电压值
		lcd_dis_str(1, 6, 1, (unsigned char *)"V");
		#endif
		
		#if 0
		uint16_t usAdcVal = 0;
		float fTmpVal = 0.0;
		
		usAdcVal = Get_AdcAverage(ADC_Channel_1, 10);
		fTmpVal = (float)usAdcVal * (3.3 / 4096);	//12位ADC
		sprintf((char *)adc_buf, "%04f", fTmpVal);
		lcd_dis_str(1, 1, 5, (unsigned char *)adc_buf);
		#endif
	}
}

int main(void)	
{
//	main_lcd1602();
	main_adc();
}
