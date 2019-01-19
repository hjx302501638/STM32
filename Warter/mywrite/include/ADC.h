/* ********** Copyright (C) 2008 Chengdu Reliance Eltctric Co., Ltd. ***********
* FileName     : TLC1543.h
* Author       : LGQ
* Version      : 1.0
* Date         : 2011.07.27
* Description  : Head file of SHT menu
********************************************************************************
* History :
* 
********************************************************************************
* 
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TLC1543_DRIVER_H_
#define _TLC1543_DRIVER_H_

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/

typedef struct
{
  f32   ResultAverage[3];
  
  f32   fResult_Store[3][10];   //存储近5次采样计算结果
  f32   fResult_Normal[3];      //乘系数校准，真实测量值大小，即变比为1时值
  f32   fResult_Normal_per[3];  //预计算
  BOOL  gbRO_Breaken;
  BOOL  gbUP_Breaken;
  BOOL  gbSC_Breaken;
  u8    RO_Break_Times;
  u8    UP_Break_Times;
  u8    SC_Break_Times;
}ResultStruct;

extern ResultStruct  Result;
/*这个地方需要加 stm32操作TLC1543 的操作*/
//#define TLC_EOC_OUT     P6DIR|=BIT3
//#define TLC_EOC_IN      P6DIR&=~BIT3
//#define TLC_EOC_SET     P6OUT|=BIT3
//#define TLC_EOC_CLR     P6OUT&=~BIT3
//#define TLC_EOC_BIT     (P6IN&BIT3)

//#define TLC_CLK_OUT     P6DIR|=BIT4
//#define TLC_CLK_IN      P6DIR&=~BIT4
//#define TLC_CLK_SET     P6OUT|=BIT4
//#define TLC_CLK_CLR     P6OUT&=~BIT4
//#define TLC_CLK_BIT     (P6IN&BIT4)

//#define TLC_ADDR_OUT    P6DIR|=BIT5
//#define TLC_ADDR_IN     P6DIR&=~BIT5
//#define TLC_ADDR_SET    P6OUT|=BIT5
//#define TLC_ADDR_CLR    P6OUT&=~BIT5
//#define TLC_ADDR_BIT    (P6IN&BIT5)

//#define TLC_DATA_OUT    P6DIR|=BIT6
//#define TLC_DATA_IN     P6DIR&=~BIT6
//#define TLC_DATA_SET    P6OUT|=BIT6
//#define TLC_DATA_CLR    P6OUT&=~BIT6
//#define TLC_DATA_BIT    (P6IN&BIT6)

//#define TLC_CS_OUT      P6DIR|=BIT7
//#define TLC_CS_IN       P6DIR&=~BIT7
//#define TLC_CS_SET      P6OUT|=BIT7
//#define TLC_CS_CLR      P6OUT&=~BIT7
//#define TLC_CS_BIT      (P6IN&BIT7)




#define TLC_EOC_OUT     1
#define TLC_EOC_IN     1
#define TLC_EOC_SET     1
#define TLC_EOC_CLR     1
#define TLC_EOC_BIT     1

#define TLC_CLK_OUT     1
#define TLC_CLK_IN     1
#define TLC_CLK_SET    1
#define TLC_CLK_CLR    1
#define TLC_CLK_BIT     1

#define TLC_ADDR_OUT    1
#define TLC_ADDR_IN     1
#define TLC_ADDR_SET   1
#define TLC_ADDR_CLR   1
#define TLC_ADDR_BIT    1

#define TLC_DATA_OUT    1
#define TLC_DATA_IN     1
#define TLC_DATA_SET   1
#define TLC_DATA_CLR    1
#define TLC_DATA_BIT    1

#define TLC_CS_OUT      1
#define TLC_CS_IN     1
#define TLC_CS_SET     1
#define TLC_CS_CLR    1
#define TLC_CS_BIT      1

#define CONDUCTANCE_C       0.048875855327468230694037145650049   //电导率系数
#define RESISTIVITY_C       0.04459921798631476050830889540567  //电阻率系数

#define BREAKTIMES                        10
#define CONSTANT_SEG                      163.68
#define CONSTANT_SEG1                     100
#define CONSTANT_CONDUCTANCE_A_SC         3.0547409579667644183773216031281
#define CONSTANT_CONDUCTANCE_B_SC         -500
#define CONSTANT_CONDUCTANCE_A            0.030547409579667644183773216031281
#define CONSTANT_CONDUCTANCE_B            -5
#define CONSTANT_RESISTIVITY_A            0.027874511241446725317693059628543
#define CONSTANT_RESISTIVITY_B            -4.5625

#define VOLTAGE_OFFSET                    1.05      //电压系数设定，模块输出与5V之间偏差
/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void TLC_Init(void);
u16 GetADValue (u8 chan);
void GetAllADValue (void);
void ComputResult (void);
#endif   /*_TLC1543_DRIVER_H_*/

/* Copyright (C) 2009 Chengdu Reliance Eltctric Co., Ltd. **** END OF FILE ****/

