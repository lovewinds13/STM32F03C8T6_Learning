#include "drvadc.h"
#include "bsp_SysTick.h"


void delayadc(uint32_t tim)
{
    for(;tim>0;tim--);
}
/*******************************************************************************
	函数名：ADC_Configuration
	输  入:
	输  出:
	功能说明：配置ADC, PA1
*/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

    /* 使能 ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);              //8M/2=4,ADC最大时间不能超过14M

	/* 配置PA1为模拟输入 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置ADC1, 不用DMA, 用软件自己触发 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		 //ADC1工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			 //单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode =  DISABLE;		 //单次转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //转换由软件触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 //ADC1数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	   //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);		   //根据ADC_InitStruct中指定的参数，初始化外设ADC1的寄存器


	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);	//ADC1,ADC通道0,规则采样顺序值为1,采样时间为239.5周期


	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);						//重置ADC1的校准寄存器
	/* Check the end of ADC1 reset calibration register */
	//while(ADC_GetResetCalibrationStatus(ADC1)); //获取ADC1重置校准寄存器的状态,设置状态则等待
    
    delayadc(10000);
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);					//开始ADC1的校准状态
	/* Check the end of ADC1 calibration */
    delayadc(10000);
	//while(ADC_GetCalibrationStatus(ADC1));		//等待校准完成
    /* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);		  //使能ADC1
	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能ADC1的软件转换启动功能
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Adc_GpioConfig
//	功能说明: ADC输入采集引脚配置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-11
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void Adc_GpioConfig(void)
{	
	GPIO_InitTypeDef gpio_init;
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_ADC_GPIO, ENABLE );	  //使能ADC1通道时钟
	
	//PA1 作为模拟通道输入引脚                         
	gpio_init.GPIO_Pin = ADC_IN_PIN;	//ADC输入引脚
	gpio_init.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(PORT_ADC_IN, &gpio_init);	
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Adc_Config
//	功能说明: ADC输入采集功能配置
//	形    参: 无
//	返 回 值: 无
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void  Adc_Config(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_ADC_CHL, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	ADC_DeInit(ADC_CHL);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode 					= ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode 			= DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign 			= ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel			= 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC_CHL, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_Cmd(ADC_CHL, ENABLE);	//使能指定的ADC1
	#if 1
	ADC_ResetCalibration(ADC_CHL);	//使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC_CHL));	//等待复位校准结束
	
	ADC_StartCalibration(ADC_CHL);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC_CHL));	 //等待校准结束
	#endif
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Adc_Init
//	功能说明: ADC初始化
//	形    参: 无
//	返 回 值: 无
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void Adc_Init(void)
{
	Adc_GpioConfig();	//IO初始化
	Adc_Config();	//ADC功能配置
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Get_AdcConvertVal
//	功能说明: 输出ADC转换结果
//	形    参: 	_ucChl：ADC 采集通道
//	返 回 值: 采集数据结果
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint16_t Get_AdcConvertVal(uint8_t _ucChl)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC_CHL, _ucChl, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC_CHL, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC_CHL, ADC_FLAG_EOC));	//等待转换结束

	return ADC_GetConversionValue(ADC_CHL);		//返回最近一次ADC1规则组的转换结果
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Get_Adc_Average
//	功能说明: 转换出 ADC 采集数据
//	形    参: 	_ucChl：ADC 通道号
//				_ucTimes：采集次数
//	返 回 值: 转换数据
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint16_t Get_AdcAverage(uint8_t _ucChl, uint8_t _ucTimes)
{
	uint32_t ulTmpVal = 0;
	uint8_t i = 0;
	
	for(i = 0; i < _ucTimes; i++)
	{
		ulTmpVal += Get_AdcConvertVal(_ucChl);
//		SysTick_Delay_Ms(5);
	}
	return ulTmpVal / _ucTimes;
} 

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: App_AdcTest
//	功能说明: ADC单通道采样测试
//	形    参: 无
//	返 回 值: 无
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void App_AdcTest(void)
{
	uint16_t usAdcVal = 0;
	float fTmpVal = 0.0;
	
	usAdcVal = Get_AdcAverage(ADC_Channel_1, 10);
//	LCD_ShowxNum(156,130,adcx,4,16,0);//显示ADC的值
	
	//INPUT VOLTAGE = (ADC Value / ADC Resolution) * Reference Voltage
	fTmpVal = (float)usAdcVal * (3.3 / 4096);	//12位ADC
//	printf("voltage is %04fv.\r\n", fTmpVal);
	
//	usAdcVal = fTmpVal;	//整数部分
//	printf("Voltage integer of usAdcVal is :%d \r\n", usAdcVal);
//	LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
	
//	fTmpVal -= usAdcVal;	//小数部分
//	printf("Voltage float of is :%f \r\n",fTmpVal);
//	fTmpVal *= 1000;
//	LCD_ShowxNum(172, 150, fTmpVal, 3, 16, 0X80);
	
	SysTick_Delay_Ms(250);	
}
