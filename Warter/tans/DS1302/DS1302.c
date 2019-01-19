/*********************************************************
*FileName��  DS1302.c
*Author:     LGQ
*Date:       2010.11.6
*Function(s):
 

*Description: DS1302�������� ��

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
const unsigned char Char_ChangePower[16]   =   {"    ʱ�ӹ���    "};
u8  gbCellFault=0;

/* Externed variables --------------------------------------------------------*/
extern const unsigned char Char_Number[16];
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/*******************************************
�������ƣ�delay
��    �ܣ���ʱһ��ʱ��
��    ����time--��ʱ����
����ֵ  ����
********************************************/
void DS1302delay(u8 time)
{
    u8 i;
    for(i = 0;i < time;i++)     __nop();
}
/*******************************************
�������ƣ�Reset_DS1302
��    �ܣ���DS1302���и�λ����
��    ������
����ֵ  ����
********************************************/
void Reset_DS1302(void)
{
		OpenPBclk;//��ʼ�� GPIOBʱ��
    DS_RST_OUT;  //RST��Ӧ��IO����Ϊ���״̬
    DS_SCL_OUT;  //SCLK��Ӧ��IO����Ϊ���״̬
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    DS1302delay(10);
    DS_SCL1;    //SCLK=1	

}
/*******************************************
�������ƣ�Write1Byte
��    �ܣ���DS1302д��1���ֽڵ�����
��    ����wdata--д�������
����ֵ  ����
********************************************/
void Write1Byte(u8 wdata) 
{ 
    u8 i;
    DS_SDA_OUT;     //SDA��Ӧ��IO����Ϊ���״̬
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
�������ƣ�Read1Byte
��    �ܣ���DS1302����1���ֽڵ�����
��    ������
����ֵ  ��������һ���ֽ�����
********************************************/
u8 Read1Byte(void) 
{ 
    u8 i;
	
    u8 rdata = 0x00;

    DS_SDA_IN;  //SDA��Ӧ��IO����Ϊ����״̬
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
�������ƣ�W_Data
��    �ܣ���ĳ���Ĵ���д��һ���ֽ�����
��    ����addr--�Ĵ�����ַ
          wdata--д�������
����ֵ  ����
********************************************/
void W_Data(u8 addr, u8 wdata)
{
    DS_RST0; 
    DS_SCL0;   
    delay_us(25);
    DS_RST1;
    Write1Byte(addr);   //д���ַ
    Write1Byte(wdata);  //д������
    DS_SCL1;
    DS_RST0;
		
} 
/*******************************************
�������ƣ�R_Data
��    �ܣ���ĳ���Ĵ�������һ���ֽ�����
��    ����addr--�Ĵ�����ַ
����ֵ  ������������
********************************************/
u8 R_Data(u8 addr)
{
    u8 rdata;
    
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(addr);    //д���ַ
	
    rdata = Read1Byte();  //�������� 
    DS_SCL1;
    DS_RST0;
    
    return(rdata);
}
/*******************************************
�������ƣ�BurstWrite1302
��    �ܣ���burst��ʽ��DS1302д������ʱ������
��    ����ptr--ָ��ʱ�����ݴ�ŵ�ַ��ָ��
����ֵ  ������������
˵    ����ʱ�����ݵĴ�Ÿ�ʽ�ǣ�
          �룬�֣�ʱ���գ��£����ڣ��꣬����
            ��7�����ݣ�BCD��ʽ��+1�����ơ�
********************************************/
void BurstWrite1302(u8 *ptr)
{
    u8 i;
    
    W_Data(0x8e,0x00);      //����д��     
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(0xbe);       // 0xbe:ʱ�Ӷ��ֽ�д������ 
    for (i = 8; i > 0; i--) 
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0;
    W_Data(0x8e,0x80);      // ��ֹд��
} 
/*******************************************
�������ƣ�BurstRead1302
��    �ܣ���burst��ʽ��DS1302��������ʱ������
��    ����ptr--ָ����ʱ�����ݵ�ַ��ָ��
����ֵ  ����
˵    ����ʱ�����ݵĴ�Ÿ�ʽ�ǣ�
          �룬�֣�ʱ���գ��£����ڣ��꣬����
            ��7�����ݣ�BCD��ʽ��+1�����ơ�
********************************************/
void BurstRead1302(u8 *ptr)
{
    u8 i;
  
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(0xbf);             //0xbf:ʱ�Ӷ��ֽڶ�����
    for (i = 8; i > 0; i--) 
    {
       *ptr++ = Read1Byte(); 
    }
    DS_SCL1;
    DS_RST0;    
}
/*******************************************
�������ƣ�BurstWriteRAM
��    �ܣ���burst��ʽ��DS1302��RAM��д����������
��    ����ptr--ָ�������ݵ�ַ��ָ��
����ֵ  ����
˵��    ����д��31���ֽڵ�����
********************************************/
void BurstWriteRAM(u8 *ptr)
{
    u8 i;
    
    W_Data(0x8e,0x00);         //����д��
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(0xfe);          //0xfe:RAM���ֽ�д���� 
    for (i = 31; i>0; i--)     //RAM����31���ֽ�
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0; 
    W_Data(0x8e,0x80);          //��ֹд��
} 
/*******************************************
�������ƣ�BurstReadRAM
��    �ܣ���burst��ʽ��DS1302��RAM�ж�����������
��    ����ptr--ָ�����ݴ�ŵ�ַ��ָ��
����ֵ  ����
˵��    ��������31���ֽڵ�����
********************************************/
void BurstReadRAM(u8 *ptr)
{
    u8 i;
  
    DS_RST0; 
    DS_SCL0; 
    delay_us(25);
    DS_RST1;
    Write1Byte(0xff);             //0xff:RAM�Ķ��ֽڶ����� 
    for (i = 31; i > 0; i--) 
    {
       *ptr++ = Read1Byte();   
    }
    DS_SCL1;
    DS_RST0;
}
/*******************************************
�������ƣ�Set_DS1302
��    �ܣ�����DS1302�ڲ���ʱ��
��    ����ptr--ָ�������ݵ�ַ��ָ��
����ֵ  ����
˵��    ��д�����ݵĸ�ʽ��
            �� �� ʱ �� �� ���� ��  ����7���ֽڡ�
********************************************/
void Set_DS1302(u8 *ptr) 
{
    u8 i;
    u8 addr = 0x80; 
    
    W_Data(0x8e,0x00);    //����д��
    
    for(i = 7;i > 0;i--)
    { 
        W_Data(addr,*ptr++); 
        addr += 2;
    }
    W_Data(0x8e,0x80);      //��ֹ
}
/******************************************************************** 
*
* ����: Get_DS1302
* ˵��: 
* ����: ��ȡDS1302��ǰʱ��
* ����: R_Data(uchar addr) 
* ����: ucCurtime: ���浱ǰʱ���ַ����ǰʱ���ʽΪ: �� �� ʱ �� �� ���� �� 
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
/*******************************************
�������ƣ�Get_DS1302
��    �ܣ���ȡDS1302�ڲ���ʱ��
��    ����ptr--ָ�����ݴ�ŵ�ַ��ָ��
����ֵ  ����
˵��    ���������ݵĸ�ʽ��
            �� �� ʱ �� �� ���� ��  ����7���ֽڡ�
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
* �� �� ��: void DS1302_10toData( void )
* ��    ��: ʮ������ת��Ϊʱ������,��Ds1302DataBuf10[7]��Ds1302DataBuf[8]��
* ˵    ��: NOTHING
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
* �� �� ��: void DS1302_Datato10( void )
* ��    ��: ʮ������ת��Ϊʱ������,��Ds1302DataBuf[8]��Ds1302DataBuf10[7]��
* ˵    ��: NOTHING
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
* �� �� ��: void DS1302_POWER( void )
* ��    ��: �жϵ���Ƿ����
* ˵    ��: NOTHING
*******************************************************************************/
void DS1302_POWER( void )
{
  if(Ds1302DataBuf[0]==0x80)
    gbCellFault   =    1;
  else
    gbCellFault   =    0;
}
/*******************************************************************************
* �� �� ��: void Timer_DisChar( void )
* ��    ��: ʱ����ʾת������ʾ���ݴ���Ds1302DataBufChar�У�
* ˵    ��: NOTHING
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
