#include "drvpwm.h"

/////////////////////////////////////////////////////////////////////////////////////////
// 函数名称:	pwm_init_timer1
// 功能描述:	定时器初始化函数
// 入口参数:	预分频值，自动重装载值
// 输入参数:	tim_psr: 预分频值, tim_arr: 自动重装载值
// 输出参数:	无
// 备    注:

// 周期T = tmi_arr * (1/CK_CNT)
// 本例程中PWM周期 T = 7200 *(1/72M) = 0.1ms
// PWM的频率 = 时钟频率  / (自动重装载值 + 1) * (预分频值 + 1)
// 本例程中PWM频率 f = 72M / [( 7199+1) * (0+1) ] = 10KHz

//TIM1最大时钟频率为72MHZ = 72 000 000 HZ 
// 频率=Clock/((Prescaler+1)*(Period+1))=144M/(1440 * 100)=1K
// 占空比=Pulse/(Period+1)=30/100=30% 后三个和输出电平极性相关
//    PWM_Init_TIM1(0,7199); 
//    PWM频率 f = 72000000/(7199+1)*(0+1) = 10KHZ
//    PWM周期 T = 1/f = 1/10000 = 0.0001S = 0.1ms

//    TIM_SetCompare1(TIM1,3599);//设置TIMx捕获比较1寄存器值  此处为TIM1通道1 占空比为50%
//    TIM_SetCompare4(TIM1,3599);//设置TIMx捕获比较4寄存器值  此处为TIM1通道4 占空比为50%

// PA8		TIM1(CH1)
// PA11 	TIM1(CH4)

//STM32笔记之PWM输出，调节频率及占空比 (https://blog.csdn.net/weixin_51636477/article/details/121088952)
//TIM—高级定时器输出PWM (https://www.ws-dc.com/jishu_2339776_1_1.html)
/////////////////////////////////////////////////////////////////////////////////////////
void pwm_init_timer1(uint16_t tim_psc, uint16_t tim_arr) 
{
	GPIO_InitTypeDef gpiox_init;                    //GPIO初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //定时器初始化
	TIM_OCInitTypeDef TIM_OCInitStruct;				    //定时器通道初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);//开启时钟
	
	gpiox_init.GPIO_Mode	= GPIO_Mode_AF_PP;      // 初始化GPIO--PA8、PA11为复用推挽输出
	gpiox_init.GPIO_Pin		= GPIO_Pin_8 | GPIO_Pin_11;
	gpiox_init.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpiox_init);                          //GPIO初始化
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);            //用其默认值填充每个TIM_TimeBaseInitStruct成员
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;      //分频因子, 配置时钟分频因子为0，可选择1、2、4分频。配置死区时用到，故此处设置为
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;  //向上计数模式
	//驱动CNT计数器的时钟CK_CNT = CK_INT /（TIM_Prescaler+1)
	//例：本例程中 CK_CNT = 72M/（0+1) = 72M
	TIM_TimeBaseInitStruct.TIM_Prescaler		= tim_psc;			//预分频值
	//配置定时器周期为0xFFFF,即65535，可设置范围为0x00000~oxFFFFF
	//自动重装载寄存器(ARR)的值累计TIM_Period个频率后产生一个更新或中断
	TIM_TimeBaseInitStruct.TIM_Period			= tim_arr;          //自动重装载值	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);             //完成定时器初始化
	
	TIM_OCInitStruct.TIM_OCMode			= TIM_OCMode_PWM1;     			 // 初始化输出比较
	TIM_OCInitStruct.TIM_OCPolarity		= TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState	= TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse			= 0;	//比较值初始化, TIM_SetCompare 设置
	
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);//定时器通道1初始化
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);//定时器通道4初始化
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC1预装载寄存器使能//CH1 TIM1
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC4预装载寄存器使能//CH4 TIM1
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //允许或禁止在定时器工作时向ARR（自动重装载值）的缓冲器中写入新值，以便在更新事件发生时载入覆盖以前的值,此处为允许
	TIM_CtrlPWMOutputs(TIM1, ENABLE); //高级定时器专属!!!--MOE主输出使能 //高级定时器TIM1：必须有这个函数才能输出PWM: TIM_CtrlPWMOutputs(TIM1，ENABL);
	
	TIM_Cmd(TIM1, ENABLE);           //定时器TIM1使能
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: pwm_set_duty
//	功能说明: duty_val1: CH1的占空比; duty_val2:CH4的占空比; 
//	形    参: 无
//	返 回 值: 无
//	日    期: 
//	作    者: 
//  备    注: 1799:25%; 3599:50%; 5399:75%; 7199:100% (根据重装数值设置)
//---------------------------------------------------------------------------------------------------------------------------------------------
void pwm_set_duty(uint16_t duty_val1, uint16_t duty_val2)
{
//	TIM_SetCompare1(TIM1, 3599);//设置TIMx捕获比较1寄存器值  此处为TIM1通道1 占空比为50%
//	TIM_SetCompare4(TIM1, 3599);//设置TIMx捕获比较4寄存器值  此处为TIM1通道4 占空比为50%
	
	TIM_SetCompare1(TIM1, duty_val1);//设置TIMx捕获比较1寄存器值  此处为TIM1通道1 占空比为50%
	TIM_SetCompare4(TIM1, duty_val2);//设置TIMx捕获比较4寄存器值  此处为TIM1通道4 占空比为50%
}


