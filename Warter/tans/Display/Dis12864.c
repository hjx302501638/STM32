/*********************************************************
*FileName：  Display.c
*Author:     LGQ
*Date:       2011.07.25
*Function(s):
 

*Description: * 12864液晶显示，带中文字库，串行；

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
unsigned char Display_Buf[4][16];          //显示缓存
unsigned char per_x=0,per_y=0,per_w=0;
/* Exported variables --------------------------------------------------------*/

/* Externed variables --------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/*******************************************
函数名称：Ini_Lcd
功    能：初始化液晶模块
参    数：无
返回值  ：无
********************************************/
void Ini_Lcd(void)
{	
	OpenPAclk;
	
	
  CYCSOUT;//rs
  CYSIDOUT;//data
  CYCLKOUT;//clk
  delay_ms(100);                 //延时等待液晶完成复位
  Send(0,0x30);  /*功能设置:一次送8位数据,基本指令集*/
  delay_ms(72);
  Send(0,0x02);  /*DDRAM地址归位*/
  delay_us(72);
  Send(0,0x0c);  /*显示设定:开显示,不显示光标,不做当前显示位反白闪动*/
  delay_us(72);
  Send(0,0x01);  /*清屏，将DDRAM的位址计数器调整为“00H”*/
  delay_us(72);
  Send(0,0x06);  /*功能设置，点设定:显示字符/光标从左到右移位,DDRAM地址加1*/
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
函数名称：Send
功    能：MCU向液晶模块发送1一个字节的数据
参    数：type--数据类型，0--控制命令，1--显示数据
          transdata--发送的数据
返回值  ：无
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
		//三个字节之间一定要有足够的延时，否则易出现时序问题
		if(j == 3)  delay_us(200); 
		else        delay_us(40);
		
		j--;
	} 
	
	CYSIDCLR;       
	CYCSCLR;		
}
/*******************************************
函数名称:Draw_CGROM
功    能:在CGROM中定义一个特殊字符
参    数:a:0x40,0X50,0x60,0x70地址；
返回值  :无
********************************************/
void Draw_CGROM(unsigned char a,const unsigned char *ptr)
{
  unsigned char j;
  Send(0,0x30);        //回到基本指令集
  //Send(0,0x34);        //打开扩展指令集
  Send(0,a);
  for(j = 0;j < 32;j++)
  {
    Send(1,*ptr++);  	
  }
  //Send(0,0x30);        //回到基本指令集
}
/*******************************************
函数名称：Clear_GDRAM
功    能：清除液晶GDRAM内部的随机数据
参    数：无
返回值  ：无
********************************************/
void Clear_GDRAM(void)
{
    unsigned char i,j,k;
    
	Send(0,0x34);        //打开扩展指令集
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
	Send(0,0x30);        //回到基本指令集	
}
/*******************************************
函数名称：Disp_HZ
功    能：显示汉字程序
参    数：addr--显示位置的首地址
          pt--指向显示数据的指针
          num--显示数据的个数
返回值  ：无
********************************************/
void Disp_HZ(unsigned char addr,const unsigned char * pt,unsigned char num)
{
    unsigned char i;
		
    Send(0,addr);          
   	for(i = 0;i < (num*2);i++) 
	   Send(1,*(pt++)); 
} 
/*******************************************
函数名称：Draw_PM
功    能：在整个屏幕上画一个图片
参    数：ptr--指向保存图片位置的指针
返回值  ：无
********************************************/
void Draw_PM(const unsigned char *ptr)
{
    unsigned char i,j,k;
    
	Send(0,0x34);        //打开扩展指令集
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
    Send(0,0x36);		        //打开绘图显示
	Send(0,0x30);               //回到基本指令集	
}
/*******************************************
函数名称：Draw_TX
功    能：在液晶上描绘一个16*16的图形
参    数：Yaddr--Y地址，
          Xaddr--X地址
          dp--指向保存图形数据的指针
返回值  ：无
********************************************/
void Draw_TX(unsigned char Yaddr,unsigned char Xaddr,const unsigned char * dp) 
{
    unsigned char j;
    unsigned char k = 0;

    Send(0,0x34);           //使用扩展指令集，关闭绘图显示
    for(j = 0;j < 16;j++)
    {
        Send(0,Yaddr++);     //Y地址  
        Send(0,Xaddr);       //X地址
        Send(1,dp[k++]);     //送两个字节的显示数据 
        Send(1,dp[k++]);    			
    }
	Send(0,0x36);		    //打开绘图显示
	Send(0,0x30);		    //回到基本指令集模式
}


/*******************************************
函数名称：void Clr_Pictrue()
功    能：清图形显示
参    数：
返回值  ：无
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
  Send(0,0x34);        //写数据时,关闭图形显示,且打开扩展指令集
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
函数名称：void OppositeColor(u8 x,u8 y,u8 width)
功    能：反白
参    数：X值为0－7，Y值为0－3，width为行反白格数
返回值  ：无
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
  Send(0,0x34);        //写数据时,关闭图形显示,且打开扩展指令集
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
	Send(0,0x36);       //写完数据,开图形显示 
	Send(0,0x30);       //从扩展指令到基本指令
}
/*******************************************
函数名称:void Display_Refresh( void )
功    能:更新显示文字
参    数:i:行数
返回值  :无
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
函数名称:void Display_RefreshAll( void )
功    能:全屏更新
参    数:i:行数
返回值  :无
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
函数名称:void DisplayBuf_Clr( void )
功    能:显示缓存清零
参    数:
返回值  :无
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
函数名称:void DisplayBuf_ClrRow(u8 row )
功    能:清零1行显示
参    数:
返回值  :无
********************************************/
void DisplayBuf_ClrRow(u8 row )
{
  unsigned char i;  
  for(i=0;i<16;i++)
    Display_Buf[row][i]=' ';
}
/*******************************************
函数名称:void DisplayTemp( unsigned char x，unsigned char y，signed int temp )
功    能:更新温度显示
参    数:x横坐标起始位0-9奇数位，y纵坐标0-3，temp温度-999-9999,显示值-99.9-999.9
返回值  :无
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
函数名称:void DisplayHum( unsigned char x，unsigned char y，signed int hum )
功    能:更新温度显示
参    数:x横坐标起始位0-13，y纵坐标0-3，hum湿度
返回值  :无
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
函数名称:void DisplayFloat( unsigned char x，unsigned char y，float result)
功    能:更新温度显示
参    数:x横坐标起始位0-9奇数位，y纵坐标0-3，显示值0.00-9999.99,显示长度占7个数字位。
返回值  :无
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
函数名称:void DisplayInt( unsigned char x,unsigned char y,float result )
功    能:更新温度显示
参    数:x横坐标起始位0-9奇数位，y纵坐标0-3，显示值0-65535,显示长度占5个数字位。
返回值  :无
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
