/*********************************************************
*FileName��  Voice.c
*Author:     LGQ
*Date:       2013.07.05
*Function(s):
 

*Description: ����оƬ�������� WTV040-16S

*Note: 
*
*************************************************************
*Modification History

*************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Voice.h"
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Externed variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* �� �� ��: void Voice_Init(void)
* ��    ��: ��ʼ������оƬ����IO��ʼ��
* ˵    ��: 
*******************************************************************************/
void VoiceIO_Init(void)
{
	OpenPBclk;//����ʱ��
  WTV040_DI_OUT;
  WTV040_CLK_OUT;
  WTV040_REST_OUT;
  WTV040_REST_H;
  delay_us(200);
  WTV040_CLK_H;
  delay_us(200);
	
	//VoiceOut(VOICE_WELCOME);
}


/*******************************************************************************
* �� �� ��: void VoiceOut(u8 addr)
* ��    ��: ����������ַ
* ˵    ��: 
*******************************************************************************/
void VoiceOut(u8 addr)
{
  u8 i;	  
  WTV040_REST_L;
  delay_ms(5);
  WTV040_REST_H;
  delay_ms(50);
  WTV040_CLK_L;
  delay_ms(5);
	
  for(i=0;i<8;i++)
  {
    WTV040_CLK_L;
    delay_us(200);  //��ʱ200us
    if((addr&0x01)==0x01)
    {
      WTV040_DI_H;
    }
    else
    {
      WTV040_DI_L;
    }
    addr>>=1;
    delay_us(200);  //��ʱ200us
    WTV040_CLK_H;
    delay_us(200);  //��ʱ200us
  }
}



/* Copyright (C) 2013 WT.Co., Ltd. **** END OF FILE ****/