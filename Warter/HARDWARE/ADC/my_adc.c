 #include "my_adc.h"
 #include "delay.h"
 #include "flashmanege.h"



u16 AD_Value[10][4] ;		   
//初始化ADC通道 使用DMA方式传递ad数据
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
	//RCC->AHBENR |=1<<0;//开启DMA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
	


	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PC1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
	//PC2 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	//PC1 作为模拟通道输入引脚  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
	//PC3 作为模拟通道输入引脚  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
 
	
	DMA_DeInit(DMA1_Channel1);	   								//将DMA通道1寄存器重设为缺省值

	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)( &(ADC1->DR)); 	//DMA外设ADC1基地址
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&AD_Value;		//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;			//内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = 40;	//DMA通道的缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		//内存地址寄存器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;				//工作在循环缓存模式
	DMA_InitStructure.DMA_Priority =  DMA_Priority_High;		 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
	
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure); 
	

	
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //设置为独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//adc工作位扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发关闭	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//转换数据右对齐 	
	ADC_InitStructure.ADC_NbrOfChannel = 4;				//4个AD转换通道

 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5 );
 	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5 );
	
	
  ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_DMACmd(ADC1, ENABLE);							//开启ADC的DMA支持
	ADC_ResetCalibration(ADC1);	//使能复位校准 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	DMA_Cmd(DMA1_Channel1, ENABLE);      //启动DMA通道
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	

}	




u8 times = 10;
//获取adc0通道ad值  电阻通道
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
	ret += AdminPra.UPpra;//加上偏移
	if(ret >18.25)ret = 18.25;
	return ret;
} 	 

//获取adc1通道ad值   Ro通道
 float Get_Adc2_Average(void)
{
	u32 temp_val=0;
	u8 t,t1;
	u16 temp;
	float ret;
	for(t=0;t<times;t++)
		temp_val+=AD_Value[t][1];
	temp=temp_val/10;
	/*获得量程*/
	if(AdminPra.ROrange == 0){t = 100;t1 = 1;}
	else if(AdminPra.ROrange == 1) {t=10;t1 = 10;}
	else if(AdminPra.ROrange == 2) {t=1;t1 = 100;}
	ret = (temp-AD_offset_Res-10)*AD_Ratio_Tivi;
	ret /=t;
	ret += (AdminPra.ROpra/100*t1);
	if(ret <0) ret =0;
	return ret;
}

//获取adc2通道ad值
 float Get_Adc3_Average(void)
{
	int temp_val=0;
	u8 t,t1;
	u16 temp=0;
	float ret =0;
	for(t=0;t<times;t++)
		temp_val+=AD_Value[t][2];
	temp = temp_val/10;
	/*获得量程*/
	if(AdminPra.EDIrange == 0){t = 100;t1 = 1;}
	else if(AdminPra.EDIrange == 1) {t=10;t1 =10;}
	else if(AdminPra.EDIrange == 2) {t=1;t1 = 100;}
	ret = (temp-AD_offset_Res-10)*AD_Ratio_Tivi;
	ret /=t;
	ret += (AdminPra.EDIpra/100*t1);
	if(ret <0) ret =0;
	return ret;
}


//获取adc3通道ad值  SR  源水  默认使用电阻表测量
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
	ret = AdminPra.UPpra;//加上偏移
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








