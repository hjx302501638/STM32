/*********************************************************
*FileName：  DS1302.c
*Author:     LGQ
*Date:       2010.11.6
*Function(s):
 

*Description: DS1302驱动程序 ；

*Note: 
*
*************************************************************
*Modification History

*************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "DS1302.h"
#include "delay.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
u8 Ds1302DataBuf[8];
u8 Ds1302DataBuf10[7];
u8 Ds1302DataBufChar[16];
const unsigned char Char_ChangePower[16]   =   {"    时钟故障    "};
u8  gbCellFault=0;

/* Externed variables --------------------------------------------------------*/
extern const unsigned char Char_Number[16];
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/*******************************************
函数名称：delay
功    能：延时一段时间
参    数：time--延时长度
返回值  ：无
********************************************/
void DS1302delay(u8 time)
{
    u8 i;
    for(i = 0;i < time;i++)     __nop();
}
/*******************************************
函数名称：Reset_DS1302
功    能：对DS1302进行复位操作
参    数：无
返回值  ：无
********************************************/
void Reset_DS1302(void)
{
		OpenPBclk;//初始化 GPIOB时钟
    DS_RST_OUT;  //RST对应的IO设置为输出状态
    DS_SCL_OUT;  //SCLK对应的IO设置为输出状态
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    DS1302delay(10);
    DS_SCL1;    //SCLK=1	

}
/*******************************************
函数名称：Write1Byte
功    能：对DS1302写入1个字节的数据
参    数：wdata--写入的数据
返回值  ：无
********************************************/
void Write1Byte(u8 wdata) 
{ 
    u8 i;
    DS_SDA_OUT;     //SDA对应的IO设置为输出状态
    DS_RST1;        //REST=1;

    for(i = 8; i > 0; i--) 
    {
        if(wdata&0x01)  
				{DS_SDA1();}
        else 
				DS_SDA0();
        DS_SCL0;
        delay_ms(1); 
        DS_SCL1;
        delay_ms(1); 
        wdata >>= 1; 
				
    } 
	
		
}
/*******************************************
函数名称：Read1Byte
功    能：从DS1302读出1个字节的数据
参    数：无
返回值  ：读出的一个字节数据
********************************************/
u8 Read1Byte(void) 
{ 
    u8 i;
	
    u8 rdata = 0x00;

    DS_SDA_IN;  //SDA对应的IO设置为输入状态
    DS_RST1;    //REST=1;

    for(i = 8; i > 0; i--)
    {
        DS_SCL1;
        delay_ms(1); 
			  
        DS_SCL0;
        delay_ms(1); 
        rdata >>= 1;
				
        if(DS_SDA_BIT)  rdata |= 0x80;/////////////////////////////////////////////////////////////////////////
    } 
    
    return(rdata); 
}
/*******************************************
函数名称：W_Data
功    能：向某个寄存器写入一个字节数据
参    数：addr--寄存器地址
          wdata--写入的数据
返回值  ：无
********************************************/
void W_Data(u8 addr, u8 wdata)
{
    DS_RST0; 
    DS_SCL0;   
    delay_us(25);
    DS_RST1;
    Write1Byte(addr);   //写入地址
    Write1Byte(wdata);  //写入数据
    DS_SCL1;
    DS_RST0;
		
} 
/*******************************************
函数名称：R_Data
功    能：从某个寄存器读出一个字节数据
参    数：addr--寄存器地址
返回值  ：读出的数据
********************************************/
u8 R_Data(u8 addr)
{
    u8 rdata;
    
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(addr);    //写入地址
	
    rdata = Read1Byte();  //读出数据 
    DS_SCL1;
    DS_RST0;
    
    return(rdata);
}
/*******************************************
函数名称：BurstWrite1302
功    能：以burst方式向DS1302写入批量时间数据
参    数：ptr--指向时间数据存放地址的指针
返回值  ：读出的数据
说    明：时间数据的存放格式是：
          秒，分，时，日，月，星期，年，控制
            【7个数据（BCD格式）+1个控制】
********************************************/
void BurstWrite1302(u8 *ptr)
{
    u8 i;
    
    W_Data(0x8e,0x00);      //允许写入     
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(0xbe);       // 0xbe:时钟多字节写入命令 
    for (i = 8; i > 0; i--) 
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0;
    W_Data(0x8e,0x80);      // 禁止写入
} 
/*******************************************
函数名称：BurstRead1302
功    能：以burst方式从DS1302读出批量时间数据
参    数：ptr--指向存放时间数据地址的指针
返回值  ：无
说    明：时间数据的存放格式是：
          秒，分，时，日，月，星期，年，控制
            【7个数据（BCD格式）+1个控制】
********************************************/
void BurstRead1302(u8 *ptr)
{
    u8 i;
  
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(0xbf);             //0xbf:时钟多字节读命令
    for (i = 8; i > 0; i--) 
    {
       *ptr++ = Read1Byte(); 
    }
    DS_SCL1;
    DS_RST0;    
}
/*******************************************
函数名称：BurstWriteRAM
功    能：以burst方式向DS1302的RAM中写入批量数据
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：共写入31个字节的数据
********************************************/
void BurstWriteRAM(u8 *ptr)
{
    u8 i;
    
    W_Data(0x8e,0x00);         //允许写入
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(0xfe);          //0xfe:RAM多字节写命令 
    for (i = 31; i>0; i--)     //RAM共有31个字节
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0; 
    W_Data(0x8e,0x80);          //禁止写入
} 
/*******************************************
函数名称：BurstReadRAM
功    能：以burst方式从DS1302的RAM中读出批量数据
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：共读出31个字节的数据
********************************************/
void BurstReadRAM(u8 *ptr)
{
    u8 i;
  
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(0xff);             //0xff:RAM的多字节读命令 
    for (i = 31; i > 0; i--) 
    {
       *ptr++ = Read1Byte();   
    }
    DS_SCL1;
    DS_RST0;
}
/*******************************************
函数名称：Set_DS1302
功    能：设置DS1302内部的时间
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：写入数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void Set_DS1302(u8 *ptr) 
{
    u8 i;
    u8 addr = 0x80; 
    
    W_Data(0x8e,0x00);    //允许写入
    
    for(i = 7;i > 0;i--)
    { 
        W_Data(addr,*ptr++); 
        addr += 2;
    }
    W_Data(0x8e,0x80);      //禁止
}
/******************************************************************** 
*
* 名称: Get_DS1302
* 说明: 
* 功能: 读取DS1302当前时间
* 调用: R_Data(uchar addr) 
* 输入: ucCurtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年 
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
/*******************************************
函数名称：Get_DS1302
功    能：读取DS1302内部的时间
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：读出数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void Get_DS1302(u8 *ptr) 
{
    u8 i;
    u8 addr = 0x81;
    
    for(i = 0;i < 7;i++)
    {
        ptr[i] = R_Data(addr);
        addr += 2;
    }
}
/*******************************************************************************
* 函 数 名: void DS1302_10toData( void )
* 功    能: 十进制数转化为时钟数据,从Ds1302DataBuf10[7]到Ds1302DataBuf[8]；
* 说    明: NOTHING
*******************************************************************************/
void DS1302_10toData( void )
{
  Ds1302DataBuf[0]   =  (Ds1302DataBuf10[5]/10)<<4;
  Ds1302DataBuf[0]  |=  (Ds1302DataBuf10[5]%10);
  Ds1302DataBuf[1]   =  (Ds1302DataBuf10[4]/10)<<4;
  Ds1302DataBuf[1]  |=  (Ds1302DataBuf10[4]%10);
  Ds1302DataBuf[2]   =  (Ds1302DataBuf10[3]/10)<<4;
  Ds1302DataBuf[2]  |=  (Ds1302DataBuf10[3]%10);
  Ds1302DataBuf[3]   =  (Ds1302DataBuf10[2]/10)<<4;
  Ds1302DataBuf[3]  |=  (Ds1302DataBuf10[2]%10);
  Ds1302DataBuf[4]   =  (Ds1302DataBuf10[1]/10)<<4;
  Ds1302DataBuf[4]  |=  (Ds1302DataBuf10[1]%10);
  Ds1302DataBuf[5]   =  Ds1302DataBuf10[6];
  Ds1302DataBuf[6]   =  (Ds1302DataBuf10[0]/10)<<4;
  Ds1302DataBuf[6]  |=  (Ds1302DataBuf10[0]%10);
  Ds1302DataBuf[7]   =  0x00;
}
/*******************************************************************************
* 函 数 名: void DS1302_Datato10( void )
* 功    能: 十进制数转化为时钟数据,从Ds1302DataBuf[8]到Ds1302DataBuf10[7]；
* 说    明: NOTHING
*******************************************************************************/
void DS1302_Datato10( void )
{
  Ds1302DataBuf10[0]   =   (Ds1302DataBuf[6]>>4)*10;
  Ds1302DataBuf10[0]  +=   (Ds1302DataBuf[6]&0x0F);
  Ds1302DataBuf10[1]   =   (Ds1302DataBuf[4]>>4)*10;
  Ds1302DataBuf10[1]  +=   (Ds1302DataBuf[4]&0x0F);
  Ds1302DataBuf10[2]   =   (Ds1302DataBuf[3]>>4)*10;
  Ds1302DataBuf10[2]  +=   (Ds1302DataBuf[3]&0x0F);
  Ds1302DataBuf10[3]   =   (Ds1302DataBuf[2]>>4)*10;
  Ds1302DataBuf10[3]  +=   (Ds1302DataBuf[2]&0x0F);
  Ds1302DataBuf10[4]   =   (Ds1302DataBuf[1]>>4)*10;
  Ds1302DataBuf10[4]  +=   (Ds1302DataBuf[1]&0x0F);
  Ds1302DataBuf10[5]   =   (Ds1302DataBuf[0]>>4)*10;
  Ds1302DataBuf10[5]  +=   (Ds1302DataBuf[0]&0x0F);
  Ds1302DataBuf10[6]   =   Ds1302DataBuf[5];
}
/*******************************************************************************
* 函 数 名: void DS1302_POWER( void )
* 功    能: 判断电池是否故障
* 说    明: NOTHING
*******************************************************************************/
void DS1302_POWER( void )
{
  if(Ds1302DataBuf[0]==0x80)
    gbCellFault   =    1;
  else
    gbCellFault   =    0;
}
/*******************************************************************************
* 函 数 名: void Timer_DisChar( void )
* 功    能: 时钟显示转换待显示数据存在Ds1302DataBufChar中；
* 说    明: NOTHING
*******************************************************************************/
void Timer_DisChar( void )
{
  u8 i;
  if(gbCellFault)
  {
    for(i=0;i<16;i++)
    Ds1302DataBufChar[i]=Char_ChangePower[i];
  }
  else
  {
    Ds1302DataBufChar[0] = '2';
    Ds1302DataBufChar[1] = '0';
    Ds1302DataBufChar[2] = Char_Number[Ds1302DataBuf10[0]/10];
    Ds1302DataBufChar[3] = Char_Number[Ds1302DataBuf10[0]%10];
    Ds1302DataBufChar[4] = '.';
    Ds1302DataBufChar[5] = Char_Number[Ds1302DataBuf10[1]/10];
    Ds1302DataBufChar[6] = Char_Number[Ds1302DataBuf10[1]%10];
    Ds1302DataBufChar[7] = '.';
    Ds1302DataBufChar[8] = Char_Number[Ds1302DataBuf10[2]/10];
    Ds1302DataBufChar[9] = Char_Number[Ds1302DataBuf10[2]%10];
    Ds1302DataBufChar[10]= ' ';
    Ds1302DataBufChar[11]= Char_Number[Ds1302DataBuf10[3]/10];
    Ds1302DataBufChar[12]= Char_Number[Ds1302DataBuf10[3]%10];
    Ds1302DataBufChar[13]= ':';
    Ds1302DataBufChar[14]= Char_Number[Ds1302DataBuf10[4]/10];
    Ds1302DataBufChar[15]= Char_Number[Ds1302DataBuf10[4]%10];    
  }  
}
void Timer_DisChar_EX( u8 *OUT,u16 *IN )
{
	u8 i;
	u8 temp_buff[7]={0};
	if(gbCellFault)
  {
    for(i=0;i<16;i++)
    Ds1302DataBufChar[i]=Char_ChangePower[i];
  }
  else
  {
		temp_buff[0]   =   (IN[6]>>4)*10;
		temp_buff[0]  +=   (IN[6]&0x0F);
		temp_buff[1]   =   (IN[4]>>4)*10;
		temp_buff[1]  +=   (IN[4]&0x0F);
		temp_buff[2]   =   (IN[3]>>4)*10;
		temp_buff[2]  +=   (IN[3]&0x0F);
		temp_buff[3]   =   (IN[2]>>4)*10;
		temp_buff[3]  +=   (IN[2]&0x0F);
		temp_buff[4]   =   (IN[1]>>4)*10;
		temp_buff[4]  +=   (IN[1]&0x0F);
		temp_buff[5]   =   (IN[0]>>4)*10;
		temp_buff[5]  +=   (IN[0]&0x0F);
		temp_buff[6]   =    IN[5];
    OUT[0] = '2';
    OUT[1] = '0';
    OUT[2] = Char_Number[temp_buff[0]/10];
    OUT[3] = Char_Number[temp_buff[0]%10];
    OUT[4] = '.';
    OUT[5] = Char_Number[temp_buff[1]/10];
    OUT[6] = Char_Number[temp_buff[1]%10];
    OUT[7] = '.';
    OUT[8] = Char_Number[temp_buff[2]/10];
    OUT[9] = Char_Number[temp_buff[2]%10];
    OUT[10]= ' ';
    OUT[11]= Char_Number[temp_buff[3]/10];
    OUT[12]= Char_Number[temp_buff[3]%10];
    OUT[13]= ':';
    OUT[14]= Char_Number[temp_buff[4]/10];
    OUT[15]= Char_Number[temp_buff[4]%10];    
  }  
}
/**** END OF FILE ****/
