#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "myiic.h"
#include "myrc522.h"
#include "key.h"             //矩阵键盘模块

//////////////////////////////////////////////////////////
//M1卡分為16個扇區，每個扇區由4塊（塊0、塊1、塊2、塊3）組成
//我們也將16個扇區的64個塊按絕對地址編號0~63
//第0扇區的塊0（即絕對地址0塊），他用於存放廠商代碼，已經固化，不可更改
//每個扇區的塊0、塊1、塊2為數據塊，可用於存放數據
//每個扇區的塊3為控制塊（絕對地址塊3、7、11....），包括了密碼A，存取控制、密碼B。


/*******************************
*连线说明：
*IRQ<------>悬空
*SDA<----->PB11
*SCL<----->PB10
*RST <----->PB1
*GND <----->GND
*VCC <----->VCC
************************************/
#define LED0 PBout(5)
#define BEEP PBout(8)

void IO()
{
    RCC->APB2ENR|=1<<3;//先使能外设IO PORTB时钟 	
    
	GPIOB->CRL&=0XFF0FFF0F;//PB1、5 推挽输出        //PB1 == rc522复位
	GPIOB->CRL|=0X00300030;	   

    GPIOB->ODR|=1<<1; 
    GPIOB->ODR|=1<<5; 
    
	GPIOB->CRH&=0XFFFFFFF0;//PB8 推挽输出
	GPIOB->CRH|=0X00000003;	   
	GPIOB->ODR|=0<<8;     //PB8 输出低
}



/*全局变量*/


u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};

/*函数声明*/
void ShowID(u16 x,u16 y, u8 *p);	 //显示卡的卡号，以十六进制显示
void PutNum(u16 x,u16 y, u32 n1,u8 n0, u16 charColor, u16 bkColor);	//显示余额函数
void Store(u8 *p,u8 store,u8 cash);//最重要的一个函数	


int main()
{
    
	unsigned char status;
	unsigned char s=0x08;
// 	u8 t,key;
	u8 j; 

    unsigned char CT[2];//卡类型
    unsigned char SN[4]; //卡号
    unsigned char RFID[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};			//存放RFID 
    
	Stm32_Clock_Init(9);	//系统时钟设置
    uart_init(72,9600);	 	//串口初始化为9600
	delay_init(72);	   	 	//延时初始化
// 	KEY_Init();
    
    LED_Init();
	LCD_Init();
    POINT_COLOR=RED;
	LCD_Clear(WHITE);
	IIC_Init();
    IO();

    delay_ms(10);
    
    
	InitRc522();				//初始化射频卡模块    

    POINT_COLOR=GREEN;	    
  	while(1) 
	{	

            POINT_COLOR=RED;	
//             key=KEY_Scan(0);
// 	       	if(key==1){s++;LCD_ShowNum(50,0,s,3,16);}
// 			else if(key==3) {s--;LCD_ShowNum(50,0,s,3,16);}


			status = PcdRequest(PICC_REQALL,CT);/*尋卡*/
			if(status==MI_OK)//尋卡成功
            {
                LCD_ShowString(0,30,200,16,16,"FIND_A_CARD");
				for(j=0;j<2;j++)
				{
					 LCD_ShowNum(120+j*32,30,CT[j],3,16);
				}
                status=MI_ERR;
                status = PcdAnticoll(SN);/*防冲撞*/
            }


			if (status==MI_OK)//防衝撞成功
			{   
                POINT_COLOR=BLUE;
				LCD_ShowString(0,50,200,16,16,"Anticoll__OK");

                POINT_COLOR=RED;
				ShowID(0,80,SN); //在液晶屏上显示卡的ID号
				//printf("ID:%02x %02x %02x %02x\n",SN[0],SN[1],SN[2],SN[3]);//发送卡号


                LCD_ShowString(0,120,200,16,16,"The Card ID is:");
                for(j=0;j<4;j++)
                {
                     LCD_ShowNum(0,136+j*16,SN[j],3,16);
                }

				status=MI_ERR;	
				status =PcdSelect(SN);
				//Reset_RC522();
		
			}
			else
			{
// 				LEDA=0;//只点亮led0
// 				LEDB=1;
			}
			if(status==MI_OK)//選卡成功
            {
                LCD_ShowString(0,200,200,16,16,"PcdSelect_MI_OK  ");
                status=MI_ERR;
                status =PcdAuthState(0x60,0x09,KEY,SN);
            }
            if(status==MI_OK)//驗證成功
            {
                LCD_ShowString(0,220,200,16,16,"PcdAuthState_MI_OK  ");
                status=MI_ERR;             

//                 status=PcdWrite(s,RFID1);
                status=PcdRead(s,RFID);
            }

			if(status==MI_OK)//讀卡成功
			 {
    			for(j=4;j<8;j++)
				{
					 LCD_ShowNum(0+(j-4)*36,260,RFID[j],4,16);
				}
			  LCD_ShowString(0,240,200,16,16,"READ_MI_OK");
			  status=MI_ERR;
			  delay_ms(100);
			 }
	}		
}
			



/*************************************
*函数功能：显示卡的卡号，以十六进制显示
*参数：x，y 坐标
*		p 卡号的地址
*		charcolor 字符的颜色
*		bkcolor   背景的颜色
***************************************/
void ShowID(u16 x,u16 y, u8 *p)	 //显示卡的卡号，以十六进制显示
{
	u8 num[9];
	u8 i;

	for(i=0;i<4;i++)
	{
		num[i*2]=p[i]/16;
		num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');
		num[i*2+1]=p[i]%16;
		num[i*2+1]>9?(num[i*2+1]+='7'):(num[i*2+1]+='0');
	}
	num[8]=0;  
	LCD_ShowString(x,y,200,16,16,"The Card ID is:");	
	LCD_ShowString(x,y+16,200,16,16,num);	
}
/********************************
*函数功能：求p的n次幂
*/
int power(u8 p,u8 n)
{
	int pow=1;
	u8 i;
	for(i=0;i<n;i++)
	{
		pow*=p;	
	}
	return pow;
}
 
u8 ReadData(u8   addr,u8 *pKey,u8 *pSnr,u8 *dataout)
{
	u8 status,k;
	status=0x02;//
	k=5;
	do
    {
		status=PcdAuthState(PICC_AUTHENT1A,addr,pKey,pSnr);
		k--;
		//printf("AuthState is wrong\n");						      
    }while(status!=MI_OK && k>0);

	status=0x02;//
	k=5;
	do
    {
		status=PcdRead(addr,dataout);
		k--;
		//printf("ReadData is wrong\n");							      
    }while(status!=MI_OK && k>0);
	return status;
}
u8 WriteData(u8   addr,u8 *pKey,u8 *pSnr,u8 *datain)
{
	u8 status,k;
	status=0x02;//
	k=5;
	do
    {
		status=PcdAuthState(PICC_AUTHENT1A,addr,pKey,pSnr);
		k--;
		//printf("AuthState is wrong\n");						      
    }while(status!=MI_OK && k>0);

	status=0x02;//
	k=5;
	do
    {
		status=PcdWrite(addr,datain);
		k--;
		//printf("ReadData is wrong\n");							      
    }while(status!=MI_OK && k>0);
	return status;
}
void PutNum(u16 x,u16 y, u32 n1,u8 n0, u16 charColor, u16 bkColor)
{
	
}
void Store(u8 *p,u8 store,u8 cash)
{

}

