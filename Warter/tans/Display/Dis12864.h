/* ********** Copyright (C) 2010 Chengdu Reliance Eltctric Co., Ltd. ***********
* FileName     : Keyboard.h
* Author       : LGQ
* Version      : 1.0
* Date         : 2010.11.1
* Description  : Head file of Display.c
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIS12864__H__
#define __DIS12864__H__

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/



  
/**************3.3VҺ����******************/
#define OpenPAclk      RCC->APB2ENR |= 1<<2    //��ʱ��PA
/*����CS����*/
#define CYCSOUT       {GPIOA->CRL &= ~(0xf<<0); GPIOA->CRL |= (0x3<<0);} //���ģʽ
#define CYCSSET       {GPIOA->BSRR |= 1<<0;}   //����ߵ�ƽ
#define CYCSCLR       {GPIOA->BRR  |= 1<<0;}   //����͵�ƽ
/*����SID����*/
#define CYSIDOUT      {GPIOA->CRL &= ~(0xf<<4); GPIOA->CRL |= (0x3<<4);} //���ģʽ	
#define CYSIDSET      {GPIOA->BSRR |= 1<<1;}   //����ߵ�ƽ
#define CYSIDCLR      {GPIOA->BRR  |= 1<<1;}   //����͵�ƽ
/*����SCLK����*/
#define CYCLKOUT      {GPIOA->CRL &= ~(0xf<<8); GPIOA->CRL |= (0x3<<8);} //���ģʽ	
#define CYCLKSET      {GPIOA->BSRR |= 1<<2;}   //����ߵ�ƽ
#define CYCLKCLR      {GPIOA->BRR  |= 1<<2;}   //����͵�ƽ
/************3.3VҺ����END******************/

#define LCD_LightInit {GPIOA->CRL &= ~(0xf<<12); GPIOA->CRL |= (0x3<<12);} //���ģʽ	
#define LCD_LightOn   {GPIOA->BSRR |= 1<<3;}   //����ߵ�ƽ
#define LCD_LightOff  {GPIOA->BRR  |= 1<<3;}   //����͵�ƽ
    


#define CELSIUS       0x0002
#define CGROM1        0x40
#define CGROM2        0x50
#define CGROM3        0x60
#define CGROM4        0x70
#define BASIC_CMD     0x30
/* Exported macro ------------------------------------------------------------*/
extern unsigned char Display_Buf[4][16];
/* Exported functions ------------------------------------------------------- */
void Ini_Lcd(void);
void Send(unsigned char type,unsigned char transdata);
void Draw_CGROM(unsigned char a,const unsigned char *ptr);
void Clear_GDRAM(void);
void Disp_HZ(unsigned char addr,const unsigned char * pt,unsigned char num);
void Draw_PM(const unsigned char *ptr);
void Draw_TX(unsigned char Yaddr,unsigned char Xaddr,const unsigned char * dp);
void Clr_Pictrue(void);
void Clr_PictruePre(unsigned char x,unsigned char y,unsigned char width);
void OppositeColor(unsigned char x,unsigned char y,unsigned char width);
void Display_Refresh( unsigned char i );
void DisplayBuf_Clr( void );
void Display_RefreshAll( void );
void DisplayTemp( unsigned char x,unsigned char y,signed int temp );
void DisplayHum( unsigned char x,unsigned char y,signed int hum );
void DisplayBuf_ClrRow(u8 row );
void DisplayFloat( unsigned char x,unsigned char y,float result );
void DisplayInt( unsigned char x,unsigned char y,unsigned int result );
void DisplayFloat1( unsigned char x,unsigned char y,u16 result );
#endif /**/
/* Copyright (C) 2010 Chengdu Reliance Eltctric Co., Ltd. **** END OF FILE ****/
