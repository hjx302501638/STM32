 #include "my_adc.h"
 #include "delay.h"
 #include "flashmanege.h"



u16 AD_Value[10][4] ;		   
//��ʼ��ADCͨ�� ʹ��DMA��ʽ����ad����
/*PC0  -ch10
*	PC1  -ch11
* PC2  -ch12
* PC3  -ch13
*/
void  My_Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	//RCC->AHBENR |=1<<0;//����DMAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	


	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PC1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
	//PC2 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	//PC1 ��Ϊģ��ͨ����������  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
	//PC3 ��Ϊģ��ͨ����������  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
 
	
	DMA_DeInit(DMA1_Channel1);	   								//��DMAͨ��1�Ĵ�������Ϊȱʡֵ

	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)( &(ADC1->DR)); 	//DMA����ADC1����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&AD_Value;		//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;			//�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = 40;	//DMAͨ���Ļ����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		//�ڴ��ַ�Ĵ�����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ����ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;				//������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority =  DMA_Priority_High;		 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
	
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure); 
	

	
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //����Ϊ��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//adc����λɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�ⲿ�����ر�	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ת�������Ҷ��� 	
	ADC_InitStructure.ADC_NbrOfChannel = 4;				//4��ADת��ͨ��

 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5 );
 	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5 );
	
	
  ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	ADC_DMACmd(ADC1, ENABLE);							//����ADC��DMA֧��
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼ 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
	DMA_Cmd(DMA1_Channel1, ENABLE);      //����DMAͨ��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	

}	




u8 times = 10;
//��ȡadc0ͨ��adֵ  ����ͨ��
 float Get_Adc_Average(void)
{
	int temp_val=0;
	u8 t;
	u16 temp=0;
	float ret;
	for(t=0;t<times;t++)
		temp_val+=AD_Value[t][0];
	temp = temp_val/10; 
	ret = (temp-AD_offset_Res)*AD_Ratio_Res;
	ret = ret/100;
	ret += AdminPra.UPpra;//����ƫ��
	if(ret >18.25)ret = 18.25;
	return ret;
} 	 

//��ȡadc1ͨ��adֵ   Roͨ��
 float Get_Adc2_Average(void)
{
	u32 temp_val=0;
	u8 t,t1;
	u16 temp;
	float ret;
	for(t=0;t<times;t++)
		temp_val+=AD_Value[t][1];
	temp=temp_val/10;
	/*�������*/
	if(AdminPra.ROrange == 0){t = 100;t1 = 1;}
	else if(AdminPra.ROrange == 1) {t=10;t1 = 10;}
	else if(AdminPra.ROrange == 2) {t=1;t1 = 100;}
	ret = (temp-AD_offset_Res-10)*AD_Ratio_Tivi;
	ret /=t;
	ret += (AdminPra.ROpra/100*t1);
	if(ret <0) ret =0;
	return ret;
}

//��ȡadc2ͨ��adֵ
 float Get_Adc3_Average(void)
{
	int temp_val=0;
	u8 t,t1;
	u16 temp=0;
	float ret =0;
	for(t=0;t<times;t++)
		temp_val+=AD_Value[t][2];
	temp = temp_val/10;
	/*�������*/
	if(AdminPra.EDIrange == 0){t = 100;t1 = 1;}
	else if(AdminPra.EDIrange == 1) {t=10;t1 =10;}
	else if(AdminPra.EDIrange == 2) {t=1;t1 = 100;}
	ret = (temp-AD_offset_Res-10)*AD_Ratio_Tivi;
	ret /=t;
	ret += (AdminPra.EDIpra/100*t1);
	if(ret <0) ret =0;
	return ret;
}


//��ȡadc3ͨ��adֵ  SR  Դˮ  Ĭ��ʹ�õ�������
 float Get_Adc4_Average(void)
{
	int temp_val=0;
	u8 t;
	u16 temp=0;
	float ret;
	for(t=0;t<times;t++)
		temp_val+=AD_Value[t][3];
	temp = temp_val/10; 
	ret = (temp-AD_offset_Res)*AD_Ratio_Res;
	ret = ret/100;
	ret = AdminPra.UPpra;//����ƫ��
	if(ret >18.25)ret = 18.25;
	return ret;
}




ADC_Fun GET_ADVAL = 
{
	Get_Adc_Average,
	Get_Adc2_Average,
	Get_Adc3_Average,
	Get_Adc4_Average,
	0,
	0,
	0,
	0
};


void Check_AD_Device(void)
{
//	u8  i;
	
	
	if(AD_Value[0][0]<100)
		GET_ADVAL.State=0;
	else GET_ADVAL.State =1;
	
	
	if(AD_Value[0][1]<100)
		GET_ADVAL.State1 =0;
	else GET_ADVAL.State1 =1;
	
	
	if(AD_Value[0][2]<100)
		GET_ADVAL.State2 =0;
	else GET_ADVAL.State2 =1;
	
	
	if(AD_Value[0][3]<100)
		GET_ADVAL.State3 =0;
	else GET_ADVAL.State3 =1;
}








