/*********************************************************
*FileName��  Display.c
*Author:     LGQ
*Date:       2011.07.25
*Function(s):
 

*Description: * 12864Һ����ʾ���������ֿ⣬���У�

*Note: 
*
*************************************************************
*Modification History

*************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "Dis12864.h"
#include "DisplayChar.h"
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
unsigned char Display_Buf[4][16];          //��ʾ����
unsigned char per_x=0,per_y=0,per_w=0;
/* Exported variables --------------------------------------------------------*/

/* Externed variables --------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/*******************************************
�������ƣ�Ini_Lcd
��    �ܣ���ʼ��Һ��ģ��
��    ������
����ֵ  ����
********************************************/
void Ini_Lcd(void)
{	
	OpenPAclk;
	
	
  CYCSOUT;//rs
  CYSIDOUT;//data
  CYCLKOUT;//clk
  delay_ms(100);                 //��ʱ�ȴ�Һ����ɸ�λ
  Send(0,0x30);  /*��������:һ����8λ����,����ָ�*/
  delay_ms(72);
  Send(0,0x02);  /*DDRAM��ַ��λ*/
  delay_us(72);
  Send(0,0x0c);  /*��ʾ�趨:����ʾ,����ʾ���,������ǰ��ʾλ��������*/
  delay_us(72);
  Send(0,0x01);  /*��������DDRAM��λַ����������Ϊ��00H��*/
  delay_us(72);
  Send(0,0x06);  /*�������ã����趨:��ʾ�ַ�/����������λ,DDRAM��ַ��1*/
  delay_us(72);	
  LCD_LightInit;
  LCD_LightOff;
  Draw_CGROM(CGROM1,Char_Space);
  Draw_CGROM(CGROM2,Char_Degree);
  Clear_GDRAM();
  DisplayBuf_Clr( );
  Display_RefreshAll(  );
}  
/*******************************************
�������ƣ�Send
��    �ܣ�MCU��Һ��ģ�鷢��1һ���ֽڵ�����
��    ����type--�������ͣ�0--�������1--��ʾ����
          transdata--���͵�����
����ֵ  ����
********************************************/
void Send(unsigned char type,unsigned char transdata)
{
	unsigned char firstbyte = 0xf8;
	unsigned char temp;
	unsigned char i,j = 3;

	if(type) firstbyte |= 0x02;
	
	CYCSSET;		 	
	CYCLKCLR;		
	while(j > 0)
	{
		if(j == 3) temp = firstbyte;
		else if(j == 2) temp = transdata&0xf0;
		else  temp = (transdata << 4) & 0xf0;
		
		for(i = 8;i > 0;i--)
		{
			if(temp & 0x80)	{CYSIDSET;} 
			else	{	CYSIDCLR;}
                        __nop();
                        __nop();
                        //DELAY_uS(1);
			CYCLKSET;				
			temp <<= 1;
                        delay_us(2);
			CYCLKCLR;
                        __nop();
                        __nop();
                        //DELAY_uS(1);
		}
		//�����ֽ�֮��һ��Ҫ���㹻����ʱ�������׳���ʱ������
		if(j == 3)  delay_us(200); 
		else        delay_us(40);
		
		j--;
	} 
	
	CYSIDCLR;       
	CYCSCLR;		
}
/*******************************************
��������:Draw_CGROM
��    ��:��CGROM�ж���һ�������ַ�
��    ��:a:0x40,0X50,0x60,0x70��ַ��
����ֵ  :��
********************************************/
void Draw_CGROM(unsigned char a,const unsigned char *ptr)
{
  unsigned char j;
  Send(0,0x30);        //�ص�����ָ�
  //Send(0,0x34);        //����չָ�
  Send(0,a);
  for(j = 0;j < 32;j++)
  {
    Send(1,*ptr++);  	
  }
  //Send(0,0x30);        //�ص�����ָ�
}
/*******************************************
�������ƣ�Clear_GDRAM
��    �ܣ����Һ��GDRAM�ڲ����������
��    ������
����ֵ  ����
********************************************/
void Clear_GDRAM(void)
{
    unsigned char i,j,k;
    
	Send(0,0x34);        //����չָ�
	i = 0x80;            
	for(j = 0;j < 32;j++)
	{
 	 	Send(0,i++);  	   
 		Send(0,0x80);  
  		for(k = 0;k < 16;k++)
  		{
  		    Send(1,0x00); 
  		}
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	    Send(0,i++);	   
  		Send(0,0x88); 	   
  		for(k = 0;k < 16;k++)
  		{
   		    Send(1,0x00); 
   		} 
	}   
	Send(0,0x30);        //�ص�����ָ�	
}
/*******************************************
�������ƣ�Disp_HZ
��    �ܣ���ʾ���ֳ���
��    ����addr--��ʾλ�õ��׵�ַ
          pt--ָ����ʾ���ݵ�ָ��
          num--��ʾ���ݵĸ���
����ֵ  ����
********************************************/
void Disp_HZ(unsigned char addr,const unsigned char * pt,unsigned char num)
{
    unsigned char i;
		
    Send(0,addr);          
   	for(i = 0;i < (num*2);i++) 
	   Send(1,*(pt++)); 
} 
/*******************************************
�������ƣ�Draw_PM
��    �ܣ���������Ļ�ϻ�һ��ͼƬ
��    ����ptr--ָ�򱣴�ͼƬλ�õ�ָ��
����ֵ  ����
********************************************/
void Draw_PM(const unsigned char *ptr)
{
    unsigned char i,j,k;
    
	Send(0,0x34);        //����չָ�
	i = 0x80;            
	for(j = 0;j < 32;j++)
	{
 	 	Send(0,i++);  	   
 		Send(0,0x80);  
  		for(k = 0;k < 16;k++)
  		{
  		    Send(1,*ptr++); 
  		}
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	    Send(0,i++);	   
  		Send(0,0x88); 	   
  		for(k = 0;k < 16;k++)
  		{
   		    Send(1,*ptr++); 
   		} 
	}
    Send(0,0x36);		        //�򿪻�ͼ��ʾ
	Send(0,0x30);               //�ص�����ָ�	
}
/*******************************************
�������ƣ�Draw_TX
��    �ܣ���Һ�������һ��16*16��ͼ��
��    ����Yaddr--Y��ַ��
          Xaddr--X��ַ
          dp--ָ�򱣴�ͼ�����ݵ�ָ��
����ֵ  ����
********************************************/
void Draw_TX(unsigned char Yaddr,unsigned char Xaddr,const unsigned char * dp) 
{
    unsigned char j;
    unsigned char k = 0;

    Send(0,0x34);           //ʹ����չָ����رջ�ͼ��ʾ
    for(j = 0;j < 16;j++)
    {
        Send(0,Yaddr++);     //Y��ַ  
        Send(0,Xaddr);       //X��ַ
        Send(1,dp[k++]);     //�������ֽڵ���ʾ���� 
        Send(1,dp[k++]);    			
    }
	Send(0,0x36);		    //�򿪻�ͼ��ʾ
	Send(0,0x30);		    //�ص�����ָ�ģʽ
}


/*******************************************
�������ƣ�void Clr_Pictrue()
��    �ܣ���ͼ����ʾ
��    ����
����ֵ  ����
********************************************/
void Clr_Pictrue(void)
{ 
  unsigned char i,j;
  Send(0,0x34);
  for(i=0;i<32;i++)
  {
     Send(0,0x80+i);
     Send(0,0x80); 
    for(j=0;j<16;j++)   
     Send(1,0x00);     
    delay_us(100);
  }

  for(i=0;i<32;i++)
  { 
    Send(0,0x80+i);
    Send(0,0x88);
    for(j=0;j<16;j++)         
	Send(1,0x00);    
    delay_us(100);
  }
  Send(0,0x30); 
}

void Clr_PictruePre(unsigned char x,unsigned char y,unsigned char width)
{ 
  unsigned char i,j,flag=0x00;
  if(y>1)
  {
   flag=0x08;
   y=y-2;
  }
  Send(0,0x34);        //д����ʱ,�ر�ͼ����ʾ,�Ҵ���չָ�
	for(i=0;i<16;i++)
	{
		Send(0,(0x80+(y<<4)+i));
		Send(0,(0x80+flag+x));
		for(j=0;j<width;j++)
		{
		Send(1,0x00);
		}delay_us(100);
	}
  Send(0,0x30); 
}

/*******************************************
�������ƣ�void OppositeColor(u8 x,u8 y,u8 width)
��    �ܣ�����
��    ����XֵΪ0��7��YֵΪ0��3��widthΪ�з��׸���
����ֵ  ����
********************************************/
void OppositeColor(unsigned char x,unsigned char y,unsigned char width)
{ 
  unsigned char i,j,flag=0x00;
  Clr_PictruePre(per_x,per_y,per_w);
  per_x=x;
  per_y=y;
  per_w=width;
  if(y>1)
  {
   flag=0x08;
   y=y-2;
  }
  Send(0,0x34);        //д����ʱ,�ر�ͼ����ʾ,�Ҵ���չָ�
	for(i=0;i<16;i++)
	{
		Send(0,(0x80+(y<<4)+i));
		Send(0,(0x80+flag+x));
		for(j=0;j<width;j++)
		{
		Send(1,0xff);
		}
		//delay_Nms(1);
                delay_us(100);
	}
	Send(0,0x36);       //д������,��ͼ����ʾ 
	Send(0,0x30);       //����չָ�����ָ��
}
/*******************************************
��������:void Display_Refresh( void )
��    ��:������ʾ����
��    ��:i:����
����ֵ  :��
********************************************/
void Display_Refresh( unsigned char i )
{
  switch(i)
  {
    case 1:
      Disp_HZ(0x80,Display_Buf[0], 8);
      break;
    case 2:
      Disp_HZ(0x90,Display_Buf[1], 8);
      break;
    case 3:
      Disp_HZ(0x88,Display_Buf[2], 8);
      break;
    case 4:
      Disp_HZ(0x98,Display_Buf[3], 8);
      break;
    default :       
     break; 
  }
    
}
/*******************************************
��������:void Display_RefreshAll( void )
��    ��:ȫ������
��    ��:i:����
����ֵ  :��
********************************************/
void Display_RefreshAll( void )
{
  //FedDOG( );
  Display_Refresh(1);
  Display_Refresh(2);
  Display_Refresh(3);
  Display_Refresh(4);
}
/*******************************************
��������:void DisplayBuf_Clr( void )
��    ��:��ʾ��������
��    ��:
����ֵ  :��
********************************************/
void DisplayBuf_Clr( void )
{
  unsigned char i,j;
  for(i=0;i<4;i++)
  {
    for(j=0;j<16;j++)
      Display_Buf[i][j]=' ';
  }
}
/*******************************************
��������:void DisplayBuf_ClrRow(u8 row )
��    ��:����1����ʾ
��    ��:
����ֵ  :��
********************************************/
void DisplayBuf_ClrRow(u8 row )
{
  unsigned char i;  
  for(i=0;i<16;i++)
    Display_Buf[row][i]=' ';
}
/*******************************************
��������:void DisplayTemp( unsigned char x��unsigned char y��signed int temp )
��    ��:�����¶���ʾ
��    ��:x��������ʼλ0-9����λ��y������0-3��temp�¶�-999-9999,��ʾֵ-99.9-999.9
����ֵ  :��
********************************************/
void DisplayTemp( unsigned char x,unsigned char y,signed int temp )
{
  unsigned char i;
  if(temp==-210)
  {
    Display_Buf[y][x]=' ';
    for(i=1;i<5;i++)
    {
      Display_Buf[y][x+i]='-';
    }
  }
  else if(temp<0)
  {
    temp=-temp;
    Display_Buf[y][x]='-';
    if(temp>=100)
      Display_Buf[y][x+1]=Char_Number[temp % 1000 / 100 ];
    else
      Display_Buf[y][x+1]=' ';    
    Display_Buf[y][x+2]=Char_Number[temp % 100 / 10 ];
    Display_Buf[y][x+3]='.';
    Display_Buf[y][x+4]=Char_Number[temp % 10 ];
  }
  else
  {
    if(temp >= 1000)
      Display_Buf[y][x]=Char_Number[temp / 1000 ]; 
    else
      Display_Buf[y][x]=' ';
    if(temp>=100)
      Display_Buf[y][x+1]=Char_Number[temp % 1000 / 100 ];
    else
      Display_Buf[y][x+1]=' ';
    
    Display_Buf[y][x+2]=Char_Number[temp % 100 / 10 ];
    Display_Buf[y][x+3]='.';
    Display_Buf[y][x+4]=Char_Number[temp % 10 ];
  }
  Display_Buf[y][x+5]=0x00;
  Display_Buf[y][x+6]=0x02;
}
/*******************************************
��������:void DisplayHum( unsigned char x��unsigned char y��signed int hum )
��    ��:�����¶���ʾ
��    ��:x��������ʼλ0-13��y������0-3��humʪ��
����ֵ  :��
********************************************/
void DisplayHum( unsigned char x,unsigned char y,signed int hum )
{
  if((hum>99)||(hum==0))
  {
    Display_Buf[y][x]='-';
    Display_Buf[y][x+1]='-';
  }
  else
  {
    if(hum>=10)
    {
      Display_Buf[y][x]=Char_Number[hum / 10 ];
      Display_Buf[y][x+1]=Char_Number[hum % 10 ];
    }
    else
    {
      Display_Buf[y][x]=' ';
      Display_Buf[y][x+1]=Char_Number[hum % 10 ];
    }
  }
  Display_Buf[y][x+2]='%';
}


/*******************************************
��������:void DisplayFloat( unsigned char x��unsigned char y��float result)
��    ��:�����¶���ʾ
��    ��:x��������ʼλ0-9����λ��y������0-3����ʾֵ0.00-9999.99,��ʾ����ռ7������λ��
����ֵ  :��
********************************************/
void DisplayFloat( unsigned char x,unsigned char y,float result )
{
  u32 temp;
    temp=(u32)(result*100);
  
    if(temp >= 100000)
      Display_Buf[y][x]=Char_Number[temp / 100000 ]; 
    else
      Display_Buf[y][x]=' ';
    if(temp>=10000)
      Display_Buf[y][x+1]=Char_Number[temp % 100000 / 10000 ];
    else
      Display_Buf[y][x+1]=' ';
    if(temp>=1000)
      Display_Buf[y][x+2]=Char_Number[temp % 10000 / 1000 ];
    else
      Display_Buf[y][x+2]=' ';
    
    Display_Buf[y][x+3]=Char_Number[temp % 1000 / 100 ];
    
    Display_Buf[y][x+4]='.';  
    
    Display_Buf[y][x+5]=Char_Number[temp % 100 / 10 ];    
    Display_Buf[y][x+6]=Char_Number[temp % 10 ];
}

void DisplayFloat1( unsigned char x,unsigned char y,u16 result )
{
    u32 temp;
  
		temp = result;
    if(temp >= 100000)
      Display_Buf[y][x]=Char_Number[temp / 100000 ]; 
    else
      Display_Buf[y][x]=' ';
    if(temp>=10000)
      Display_Buf[y][x+1]=Char_Number[temp % 100000 / 10000 ];
    else
      Display_Buf[y][x+1]=' ';
    if(temp>=1000)
      Display_Buf[y][x+2]=Char_Number[temp % 10000 / 1000 ];
    else
      Display_Buf[y][x+2]=' ';
    
    Display_Buf[y][x+3]=Char_Number[temp % 1000 / 100 ];
    
    Display_Buf[y][x+4]='.';  
    
    Display_Buf[y][x+5]=Char_Number[temp % 100 / 10 ];    
    Display_Buf[y][x+6]=Char_Number[temp % 10 ];
}
/*******************************************
��������:void DisplayInt( unsigned char x,unsigned char y,float result )
��    ��:�����¶���ʾ
��    ��:x��������ʼλ0-9����λ��y������0-3����ʾֵ0-65535,��ʾ����ռ5������λ��
����ֵ  :��
********************************************/
void DisplayInt( unsigned char x,unsigned char y,unsigned int result )
{
  u16 temp;
  temp=result;
  
    if(temp >= 10000)
      Display_Buf[y][x]=Char_Number[temp / 10000 ]; 
    else
      Display_Buf[y][x]=' ';
    if(temp>=1000)
      Display_Buf[y][x+1]=Char_Number[temp % 10000 / 1000 ];
    else
      Display_Buf[y][x+1]=' ';
    if(temp>=100)
      Display_Buf[y][x+2]=Char_Number[temp % 1000 / 100 ];
    else
      Display_Buf[y][x+2]=' ';
    if(temp>=10)
      Display_Buf[y][x+3]=Char_Number[temp % 100 / 10 ];
    else
      Display_Buf[y][x+3]=' ';     
    Display_Buf[y][x+4]=Char_Number[temp % 10 ];
}
/* Copyright (C) 2010 Chengdu Reliance Eltctric Co., Ltd. **** END OF FILE ****/
