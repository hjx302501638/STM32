#ifndef __IO_H
#define __IO_H
#include "sys.h"

#define ON   0
#define OFF  1

void IO_INIT(void);
void Close_ALL_IO(void);//关闭所有的输出

/*
	泵1.1  		-----------PC10	
	泵1.2		-----------PC11
	泵2			-----------PC12
	进水阀JS		-----------PC13
	超滤Mllt	-----------PC14
	冲洗CX		-----------PC15
	RO取水		-----------PA4
	UP取水		-----------PA5
	继电器		-----------PA6
	指示灯		-----------PA7
	
	循环电磁阀	-----------PA8
	循环泵		-----------PA11
	EDI电磁阀    ----------PA12 
	LED         ------------PB9
*/
#define Pump_1_1_IO   PCout(13) //泵1.1
#define Pump_1_2_IO   PAout(8) //泵1.2 
#define Pump_2_1_IO   PCout(10) //泵2.0
#define Pump_2_2_IO   PCout(15) //循环泵	
#define JS_IO  				PCout(12) //进水阀JS
#define Mllt_IO 			PCout(14) //超滤Mllt
#define CX_IO 		    PAout(11) //冲洗CX
#define RO_IO 		    PAout(4)	//RO取水
#define UP_IO 		    PAout(5)	//UP取水
#define JK_IO 		    PAout(6)	//继电器	
#define LED_IO 		    PAout(7)	//指示灯	
#define Cri_IO 		    PCout(11)	//循环电磁阀
#define EDI_IO 		    PAout(12)	//EDI电磁阀



/*
	低压阀		-----------PB0
	高压阀		-----------PB1
	低液位		-----------PB5   
	中液位		-----------PB6
	高液位		-----------PB7
*/
#define L_Level     PBin(5)
#define M_Level     PBin(6)
#define H_Level     PBin(7)
#define H_F         PBin(1)
#define L_F         PBin(0)

#define F_ON  1
#define F_OFF 0

#endif 
