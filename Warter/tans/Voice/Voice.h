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
#ifndef _VOICE_H
#define _VOICE_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/




/*����ط���Ҫ��stm32��������оƬ�Ķ���*/

//#define   WTV040_DI_OUT       P5DIR|=BIT2
//#define   WTV040_CLK_OUT      P5DIR|=BIT1
//#define   WTV040_REST_OUT      P5DIR|=BIT0

//#define   WTV040_REST_H       P5OUT|=BIT0
//#define   WTV040_REST_L       P5OUT&=~BIT0
//#define   WTV040_CLK_H        P5OUT|=BIT1
//#define   WTV040_CLK_L        P5OUT&=~BIT1    
//#define   WTV040_DI_H         P5OUT|=BIT2
//#define   WTV040_DI_L         P5OUT&=~BIT2

#define OpenPBclk   RCC->APB2ENR |= 1<<3    //��ʱ��PB

#define   WTV040_DI_OUT      {GPIOB->CRH &= ~(0xf<<(4*4));GPIOB->CRH |= (0x3<<(4*4));}//����PB12Ϊ���ģʽ
#define   WTV040_CLK_OUT     {GPIOB->CRH &= ~(0xf<<(5*4));GPIOB->CRH |= (0x3<<(5*4));}//����PB13Ϊ���ģʽ
#define   WTV040_REST_OUT    {GPIOB->CRH &= ~(0xf<<(3*4));GPIOB->CRH |= (0x3<<(3*4));}//����PB11Ϊ���ģʽ

#define   WTV040_REST_H      {GPIOB->BSRR |= 1<<11;}//PB11����Ϊ�ߵ�ƽ
#define   WTV040_REST_L      {GPIOB->BRR |= 1<<11;}//PB11����Ϊ�͵�ƽ
#define   WTV040_CLK_H       {GPIOB->BSRR |= 1<<13;}//PB13����Ϊ�ߵ�ƽ
#define   WTV040_CLK_L       {GPIOB->BRR |= 1<<13;}//PB13����Ϊ�͵�ƽ
#define   WTV040_DI_H        {GPIOB->BSRR |= 1<<12;}//PB12����Ϊ�ߵ�ƽ
#define   WTV040_DI_L        {GPIOB->BRR |= 1<<12;}//PB12����Ϊ�͵�ƽ



/********�������ݵ�ַ����***************/
#define   VOICE_WELCOME       0x00
#define   VOICE_FALSE         0x01
#define   VOICE_GET_UP        0x02
#define   VOICE_GET_RO        0x03
#define   VOICE_CHANGE_UP     0x04
#define   VOICE_CHANGE_RO     0x05
#define   VOICE_QUANTIFY_500  0x06
#define   VOICE_QUANTIFY_1000 0x07
#define   VOICE_QUANTIFY_2000 0x08
#define   VOICE_QUANTIFY_5000 0x09
#define   VOICE_WASH          0x0A
#define   VOICE_WATERLESS     0x0B
#define   VOICE_PRODUCEWATER  0x0C
#define   VOICE_WAIT          0x0D
#define   VOICE_RO_500        0x0E
#define   VOICE_RO_1000       0x0F
#define   VOICE_RO_2000       0x10
#define   VOICE_RO_5000       0x11
#define   VOICE_UP_500        0x12
#define   VOICE_UP_1000       0x13
#define   VOICE_UP_2000       0x14
#define   VOICE_UP_5000       0x15
  

/********�������������***************/
#define   VoiceVolumeE0  0xE0  //E0H������С��EFH�������
#define   VoiceVolumeE1  0xE1
#define   VoiceVolumeE2  0xE2
#define   VoiceVolumeE3  0xE3
#define   VoiceVolumeE4  0xE4
#define   VoiceVolumeE5  0xE5
#define   VoiceVolumeE6  0xE6
#define   VoiceVolumeE7  0xE7
#define   VoiceVolumeE8  0xE8
#define   VoiceVolumeE9  0xE9
#define   VoiceVolumeEA  0xEA
#define   VoiceVolumeEB  0xEB
#define   VoiceVolumeEC  0xEC
#define   VoiceVolumeED  0xED
#define   VoiceVolumeEE  0xEE
#define   VoiceVolumeEF  0xEF

#define   VoiceOff                   0xF0
#define   VoiceOn                    0xF1
#define   VoiceTheSameOneCyclic      0xF2 //ѭ�����ŵ�ǰ����
#define   VoiceTheNextOneContinuous  0xF3 //���벥��
#define   VoiceInsertSilence         0xF8 //���뾲��
#define   VoiceStopAtOnece           0xFE //ֹͣ����

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void VoiceIO_Init(void);
void VoiceOut(u8 addr);



#endif   /*_TLC1543_DRIVER_H_*/



/* Copyright (C) 2009 Chengdu Reliance Eltctric Co., Ltd. **** END OF FILE ****/

