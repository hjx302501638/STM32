/* ********** Copyright (C) 2010 Chengdu Reliance Eltctric Co., Ltd. ***********
* FileName     : DS1302.h
* Author       : LGQ
* Version      : 1.0
* Date         : 2010.11.6
* Description  : Head file of Display.c
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DS1302__H__
#define __DS1302__H__

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/

#define OpenPBclk      RCC->APB2ENR |= 1<<3    //打开时钟PB
#define DS_RST_OUT            {GPIOB->CRH &= ~(0xf<<8);GPIOB->CRH |= (0x3<<8);} //设置 PB10 为输出模式
#define DS_RST0               {GPIOB->BRR |= 1<<10;}//PB10设置为低电平
#define DS_RST1               {GPIOB->BSRR |= 1<<10;}//PB10设置为高电平


#define DS_SCL_OUT            {GPIOB->CRL &= ~(0xf<<12);GPIOB->CRL |= (0x3<<12);}//设置PB3为输出模式
#define DS_SCL0               {GPIOB->BRR |= 1<<3;}//PB3设置为低电平
#define DS_SCL1               {GPIOB->BSRR |= 1<<3;}//PB3设置为高电平

#define DS_SDA_IN             {GPIOB->CRL &= ~(0xf<<16);GPIOB->CRL |= (0x8<<16);}//设置PB4为输入模式
#define DS_SDA_OUT            {GPIOB->CRL &= ~(0xf<<16);GPIOB->CRL |= (0x3<<16);}//设置PB4为输出模式
#define DS_SDA0()               {GPIOB->BRR |= 1<<4;}//PB4设置为低电平
#define DS_SDA1()               {GPIOB->BSRR |= 1<<4;}//PB4设置为高电平
#define DS_SDA_BIT             PBin(4)
extern u8  gbCellFault;

void DS1302delay(u8 time);
void Reset_DS1302(void);
void Write1Byte(unsigned char wdata);
unsigned char Read1Byte(void);
void W_Data(unsigned char addr, unsigned char wdata);
unsigned char R_Data(unsigned char addr);
void BurstWrite1302(unsigned char *ptr);
void BurstRead1302(unsigned char *ptr);
void BurstWriteRAM(unsigned char *ptr);
void BurstReadRAM(unsigned char *ptr);
void Set_DS1302(unsigned char *ptr);
void Get_DS1302(unsigned char *ptr);
void Timer_DisChar( void );
void DS1302_POWER( void );
void DS1302_Datato10( void );
void DS1302_10toData( void );

#endif /**/
/* Copyright (C) 2010 Chengdu Reliance Eltctric Co., Ltd. **** END OF FILE ****/




