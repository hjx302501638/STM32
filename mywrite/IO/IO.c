#include "IO.h"



//初始化输出IO
void Iinit_Output_Io(void)
{
	RCC->APB2ENR|=0x7<<2;//开启GPIOA B C时钟
	
	GPIOA->CRL  &= 0x0000ffff;//A 4 5 6 7
	GPIOA->CRL  |= 0x33330000;//A 4 5 6 7
	
	GPIOA->CRH  &= 0xfff00ff0;//A 8 11 12
	GPIOA->CRH  |= 0x00033003;//A 8 11 12
	
	GPIOB->CRH  &= 0xffffff0f;//B 9
	GPIOB->CRH  |= 0x00000030;//B 9
	
	GPIOC->CRH  &= 0x000000ff;//C 10 11 12 13 14 15
	GPIOC->CRH  |= 0x33333300;//C 10 11 12 13 14 15
	
	Pump_1_1_IO = OFF;
	Pump_1_2_IO = OFF;
	Pump_2_1_IO = OFF;
	Pump_2_2_IO = OFF;
	JS_IO				= OFF;
	Mllt_IO			= OFF;
	CX_IO				= OFF;
	RO_IO				= OFF;
	UP_IO				= OFF;
	JK_IO				= OFF;
	LED_IO			= OFF;
	Cri_IO			= OFF;
	EDI_IO			= OFF;
}

//初始化 输入IO
void Init_In_IO(void)
{
		GPIOB->CRL &= 0x000fff00;//0 1 5 6 7
		GPIOB->CRL |= 0x88800088;//0 1 5 6 7	
}

void IO_INIT(void)
{
		Iinit_Output_Io();
		Init_In_IO();
}


void Close_ALL_IO(void)
{
	Pump_1_1_IO = OFF;
	Pump_1_2_IO = OFF;
	Pump_2_1_IO = OFF;
	Pump_2_2_IO = OFF;
	JS_IO				= OFF;
	Mllt_IO			= OFF;
	CX_IO				= OFF;
	RO_IO				= OFF;
	UP_IO				= OFF;
	JK_IO				= OFF;
	LED_IO			= OFF;
	Cri_IO			= OFF;
	EDI_IO			= OFF;
}










