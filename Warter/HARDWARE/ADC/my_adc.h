#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"




void  My_Adc_Init(void); 
float Get_Adc_Average(void); 
float Get_Adc2_Average(void);
float Get_Adc3_Average(void);
float Get_Adc4_Average(void);
 
 
 
extern u16 AD_Value[10][4]; 
#define AD_offset_Res  736
#define AD_Ratio_Res   0.6156996587030717
#define AD_Ratio_Tivi  0.675609756097561



typedef  struct ADC_Fun
{
	float (*GetUP_AD) (void);
	float (*GetRO_AD) (void);
	float (*GetEDI_AD)(void);
	float (*GetSR_AD) (void);
	u8     State;//UP
	u8     State1;//RO
	u8     State2;//EDI
	u8     State3;//SR
}ADC_Fun,*pADC_Fun;

extern ADC_Fun GET_ADVAL;
void Check_AD_Device(void);//检测 所有表是否存在


#endif 
