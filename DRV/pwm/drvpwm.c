#include "drvpwm.h"

/////////////////////////////////////////////////////////////////////////////////////////
// ��������:	pwm_init_timer1
// ��������:	��ʱ����ʼ������
// ��ڲ���:	Ԥ��Ƶֵ���Զ���װ��ֵ
// �������:	tim_psr: Ԥ��Ƶֵ, tim_arr: �Զ���װ��ֵ
// �������:	��
// ��    ע:

// ����T = tmi_arr * (1/CK_CNT)
// ��������PWM���� T = 7200 *(1/72M) = 0.1ms
// PWM��Ƶ�� = ʱ��Ƶ��  / (�Զ���װ��ֵ + 1) * (Ԥ��Ƶֵ + 1)
// ��������PWMƵ�� f = 72M / [( 7199+1) * (0+1) ] = 10KHz

//TIM1���ʱ��Ƶ��Ϊ72MHZ = 72 000 000 HZ 
// Ƶ��=Clock/((Prescaler+1)*(Period+1))=144M/(1440 * 100)=1K
// ռ�ձ�=Pulse/(Period+1)=30/100=30% �������������ƽ�������
//    PWM_Init_TIM1(0,7199); 
//    PWMƵ�� f = 72000000/(7199+1)*(0+1) = 10KHZ
//    PWM���� T = 1/f = 1/10000 = 0.0001S = 0.1ms

//    TIM_SetCompare1(TIM1,3599);//����TIMx����Ƚ�1�Ĵ���ֵ  �˴�ΪTIM1ͨ��1 ռ�ձ�Ϊ50%
//    TIM_SetCompare4(TIM1,3599);//����TIMx����Ƚ�4�Ĵ���ֵ  �˴�ΪTIM1ͨ��4 ռ�ձ�Ϊ50%

// PA8		TIM1(CH1)
// PA11 	TIM1(CH4)

//STM32�ʼ�֮PWM���������Ƶ�ʼ�ռ�ձ� (https://blog.csdn.net/weixin_51636477/article/details/121088952)
//TIM���߼���ʱ�����PWM (https://www.ws-dc.com/jishu_2339776_1_1.html)
/////////////////////////////////////////////////////////////////////////////////////////
void pwm_init_timer1(uint16_t tim_psc, uint16_t tim_arr) 
{
	GPIO_InitTypeDef gpiox_init;                    //GPIO��ʼ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //��ʱ����ʼ��
	TIM_OCInitTypeDef TIM_OCInitStruct;				    //��ʱ��ͨ����ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);//����ʱ��
	
	gpiox_init.GPIO_Mode	= GPIO_Mode_AF_PP;      // ��ʼ��GPIO--PA8��PA11Ϊ�����������
	gpiox_init.GPIO_Pin		= GPIO_Pin_8 | GPIO_Pin_11;
	gpiox_init.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpiox_init);                          //GPIO��ʼ��
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);            //����Ĭ��ֵ���ÿ��TIM_TimeBaseInitStruct��Ա
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;      //��Ƶ����, ����ʱ�ӷ�Ƶ����Ϊ0����ѡ��1��2��4��Ƶ����������ʱ�õ����ʴ˴�����Ϊ
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;  //���ϼ���ģʽ
	//����CNT��������ʱ��CK_CNT = CK_INT /��TIM_Prescaler+1)
	//������������ CK_CNT = 72M/��0+1) = 72M
	TIM_TimeBaseInitStruct.TIM_Prescaler		= tim_psc;			//Ԥ��Ƶֵ
	//���ö�ʱ������Ϊ0xFFFF,��65535�������÷�ΧΪ0x00000~oxFFFFF
	//�Զ���װ�ؼĴ���(ARR)��ֵ�ۼ�TIM_Period��Ƶ�ʺ����һ�����»��ж�
	TIM_TimeBaseInitStruct.TIM_Period			= tim_arr;          //�Զ���װ��ֵ	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);             //��ɶ�ʱ����ʼ��
	
	TIM_OCInitStruct.TIM_OCMode			= TIM_OCMode_PWM1;     			 // ��ʼ������Ƚ�
	TIM_OCInitStruct.TIM_OCPolarity		= TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState	= TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse			= 0;	//�Ƚ�ֵ��ʼ��, TIM_SetCompare ����
	
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);//��ʱ��ͨ��1��ʼ��
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);//��ʱ��ͨ��4��ʼ��
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC1Ԥװ�ؼĴ���ʹ��//CH1 TIM1
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC4Ԥװ�ؼĴ���ʹ��//CH4 TIM1
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //������ֹ�ڶ�ʱ������ʱ��ARR���Զ���װ��ֵ���Ļ�������д����ֵ���Ա��ڸ����¼�����ʱ���븲����ǰ��ֵ,�˴�Ϊ����
	TIM_CtrlPWMOutputs(TIM1, ENABLE); //�߼���ʱ��ר��!!!--MOE�����ʹ�� //�߼���ʱ��TIM1����������������������PWM: TIM_CtrlPWMOutputs(TIM1��ENABL);
	
	TIM_Cmd(TIM1, ENABLE);           //��ʱ��TIM1ʹ��
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: pwm_set_duty
//	����˵��: duty_val1: CH1��ռ�ձ�; duty_val2:CH4��ռ�ձ�; 
//	��    ��: ��
//	�� �� ֵ: ��
//	��    ��: 
//	��    ��: 
//  ��    ע: 1799:25%; 3599:50%; 5399:75%; 7199:100% (������װ��ֵ����)
//---------------------------------------------------------------------------------------------------------------------------------------------
void pwm_set_duty(uint16_t duty_val1, uint16_t duty_val2)
{
//	TIM_SetCompare1(TIM1, 3599);//����TIMx����Ƚ�1�Ĵ���ֵ  �˴�ΪTIM1ͨ��1 ռ�ձ�Ϊ50%
//	TIM_SetCompare4(TIM1, 3599);//����TIMx����Ƚ�4�Ĵ���ֵ  �˴�ΪTIM1ͨ��4 ռ�ձ�Ϊ50%
	
	TIM_SetCompare1(TIM1, duty_val1);//����TIMx����Ƚ�1�Ĵ���ֵ  �˴�ΪTIM1ͨ��1 ռ�ձ�Ϊ50%
	TIM_SetCompare4(TIM1, duty_val2);//����TIMx����Ƚ�4�Ĵ���ֵ  �˴�ΪTIM1ͨ��4 ռ�ձ�Ϊ50%
}


