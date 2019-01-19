#ifndef __IO_H
#define __IO_H
#include "sys.h"

#define ON   0
#define OFF  1

void IO_INIT(void);
void Close_ALL_IO(void);//�ر����е����

/*
	��1.1  		-----------PC10	
	��1.2		-----------PC11
	��2			-----------PC12
	��ˮ��JS		-----------PC13
	����Mllt	-----------PC14
	��ϴCX		-----------PC15
	ROȡˮ		-----------PA4
	UPȡˮ		-----------PA5
	�̵���		-----------PA6
	ָʾ��		-----------PA7
	
	ѭ����ŷ�	-----------PA8
	ѭ����		-----------PA11
	EDI��ŷ�    ----------PA12 
	LED         ------------PB9
*/
#define Pump_1_1_IO   PCout(13) //��1.1
#define Pump_1_2_IO   PAout(8) //��1.2 
#define Pump_2_1_IO   PCout(10) //��2.0
#define Pump_2_2_IO   PCout(15) //ѭ����	
#define JS_IO  				PCout(12) //��ˮ��JS
#define Mllt_IO 			PCout(14) //����Mllt
#define CX_IO 		    PAout(11) //��ϴCX
#define RO_IO 		    PAout(4)	//ROȡˮ
#define UP_IO 		    PAout(5)	//UPȡˮ
#define JK_IO 		    PAout(6)	//�̵���	
#define LED_IO 		    PAout(7)	//ָʾ��	
#define Cri_IO 		    PCout(11)	//ѭ����ŷ�
#define EDI_IO 		    PAout(12)	//EDI��ŷ�



/*
	��ѹ��		-----------PB0
	��ѹ��		-----------PB1
	��Һλ		-----------PB5   
	��Һλ		-----------PB6
	��Һλ		-----------PB7
*/
#define L_Level     PBin(5)
#define M_Level     PBin(6)
#define H_Level     PBin(7)
#define H_F         PBin(1)
#define L_F         PBin(0)

#define F_ON  1
#define F_OFF 0

#endif 
