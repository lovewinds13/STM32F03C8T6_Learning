#include "drvadc.h"
#include "bsp_SysTick.h"


void delayadc(uint32_t tim)
{
    for(;tim>0;tim--);
}
/*******************************************************************************
	��������ADC_Configuration
	��  ��:
	��  ��:
	����˵��������ADC, PA1
*/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

    /* ʹ�� ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);              //8M/2=4,ADC���ʱ�䲻�ܳ���14M

	/* ����PA1Ϊģ������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����ADC1, ����DMA, ������Լ����� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		 //ADC1����ģʽ:����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			 //��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode =  DISABLE;		 //����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //ת���������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 //ADC1�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	   //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);		   //����ADC_InitStruct��ָ���Ĳ�������ʼ������ADC1�ļĴ���


	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);	//ADC1,ADCͨ��0,�������˳��ֵΪ1,����ʱ��Ϊ239.5����


	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);						//����ADC1��У׼�Ĵ���
	/* Check the end of ADC1 reset calibration register */
	//while(ADC_GetResetCalibrationStatus(ADC1)); //��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�
    
    delayadc(10000);
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);					//��ʼADC1��У׼״̬
	/* Check the end of ADC1 calibration */
    delayadc(10000);
	//while(ADC_GetCalibrationStatus(ADC1));		//�ȴ�У׼���
    /* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);		  //ʹ��ADC1
	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ADC1�����ת����������
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: Adc_GpioConfig
//	����˵��: ADC����ɼ���������
//	��    ��: ��
//	�� �� ֵ: ��
//	��    ��: 2020-03-11
//  ��    ע: 
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void Adc_GpioConfig(void)
{	
	GPIO_InitTypeDef gpio_init;
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_ADC_GPIO, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	
	//PA1 ��Ϊģ��ͨ����������                         
	gpio_init.GPIO_Pin = ADC_IN_PIN;	//ADC��������
	gpio_init.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(PORT_ADC_IN, &gpio_init);	
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: Adc_Config
//	����˵��: ADC����ɼ���������
//	��    ��: ��
//	�� �� ֵ: ��
//  ��    ע: 
//	��    ��: 2020-03-11
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void  Adc_Config(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_ADC_CHL, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	ADC_DeInit(ADC_CHL);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode 					= ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode 			= DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign 			= ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel			= 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC_CHL, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_Cmd(ADC_CHL, ENABLE);	//ʹ��ָ����ADC1
	#if 1
	ADC_ResetCalibration(ADC_CHL);	//ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC_CHL));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC_CHL);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC_CHL));	 //�ȴ�У׼����
	#endif
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: Adc_Init
//	����˵��: ADC��ʼ��
//	��    ��: ��
//	�� �� ֵ: ��
//  ��    ע: 
//	��    ��: 2020-03-11
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void Adc_Init(void)
{
	Adc_GpioConfig();	//IO��ʼ��
	Adc_Config();	//ADC��������
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: Get_AdcConvertVal
//	����˵��: ���ADCת�����
//	��    ��: 	_ucChl��ADC �ɼ�ͨ��
//	�� �� ֵ: �ɼ����ݽ��
//  ��    ע: 
//	��    ��: 2020-03-11
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint16_t Get_AdcConvertVal(uint8_t _ucChl)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC_CHL, _ucChl, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC_CHL, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC_CHL, ADC_FLAG_EOC));	//�ȴ�ת������

	return ADC_GetConversionValue(ADC_CHL);		//�������һ��ADC1�������ת�����
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: Get_Adc_Average
//	����˵��: ת���� ADC �ɼ�����
//	��    ��: 	_ucChl��ADC ͨ����
//				_ucTimes���ɼ�����
//	�� �� ֵ: ת������
//  ��    ע: 
//	��    ��: 2020-03-11
//	��    ��: by ����AI
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
//	�� �� ��: App_AdcTest
//	����˵��: ADC��ͨ����������
//	��    ��: ��
//	�� �� ֵ: ��
//  ��    ע: 
//	��    ��: 2020-03-11
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void App_AdcTest(void)
{
	uint16_t usAdcVal = 0;
	float fTmpVal = 0.0;
	
	usAdcVal = Get_AdcAverage(ADC_Channel_1, 10);
//	LCD_ShowxNum(156,130,adcx,4,16,0);//��ʾADC��ֵ
	
	//INPUT VOLTAGE = (ADC Value / ADC Resolution) * Reference Voltage
	fTmpVal = (float)usAdcVal * (3.3 / 4096);	//12λADC
//	printf("voltage is %04fv.\r\n", fTmpVal);
	
//	usAdcVal = fTmpVal;	//��������
//	printf("Voltage integer of usAdcVal is :%d \r\n", usAdcVal);
//	LCD_ShowxNum(156,150,adcx,1,16,0);//��ʾ��ѹֵ
	
//	fTmpVal -= usAdcVal;	//С������
//	printf("Voltage float of is :%f \r\n",fTmpVal);
//	fTmpVal *= 1000;
//	LCD_ShowxNum(172, 150, fTmpVal, 3, 16, 0X80);
	
	SysTick_Delay_Ms(250);	
}
