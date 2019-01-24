#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "myiic.h"
#include "myrc522.h"
#include "key.h"             //�������ģ��

//////////////////////////////////////////////////////////
//M1���֞�16���ȅ^��ÿ���ȅ^��4�K���K0���K1���K2���K3���M��
//�҂�Ҳ��16���ȅ^��64���K���^����ַ��̖0~63
//��0�ȅ^�ĉK0�����^����ַ0�K��������춴�ŏS�̴��a���ѽ��̻������ɸ���
//ÿ���ȅ^�ĉK0���K1���K2�锵���K������춴�Ŕ���
//ÿ���ȅ^�ĉK3����ƉK���^����ַ�K3��7��11....�����������ܴaA����ȡ���ơ��ܴaB��


/*******************************
*����˵����
*IRQ<------>����
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
    RCC->APB2ENR|=1<<3;//��ʹ������IO PORTBʱ�� 	
    
	GPIOB->CRL&=0XFF0FFF0F;//PB1��5 �������        //PB1 == rc522��λ
	GPIOB->CRL|=0X00300030;	   

    GPIOB->ODR|=1<<1; 
    GPIOB->ODR|=1<<5; 
    
	GPIOB->CRH&=0XFFFFFFF0;//PB8 �������
	GPIOB->CRH|=0X00000003;	   
	GPIOB->ODR|=0<<8;     //PB8 �����
}



/*ȫ�ֱ���*/


u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};

/*��������*/
void ShowID(u16 x,u16 y, u8 *p);	 //��ʾ���Ŀ��ţ���ʮ��������ʾ
void PutNum(u16 x,u16 y, u32 n1,u8 n0, u16 charColor, u16 bkColor);	//��ʾ����
void Store(u8 *p,u8 store,u8 cash);//����Ҫ��һ������	


int main()
{
    
	unsigned char status;
	unsigned char s=0x08;
// 	u8 t,key;
	u8 j; 

    unsigned char CT[2];//������
    unsigned char SN[4]; //����
    unsigned char RFID[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};			//���RFID 
    
	Stm32_Clock_Init(9);	//ϵͳʱ������
    uart_init(72,9600);	 	//���ڳ�ʼ��Ϊ9600
	delay_init(72);	   	 	//��ʱ��ʼ��
// 	KEY_Init();
    
    LED_Init();
	LCD_Init();
    POINT_COLOR=RED;
	LCD_Clear(WHITE);
	IIC_Init();
    IO();

    delay_ms(10);
    
    
	InitRc522();				//��ʼ����Ƶ��ģ��    

    POINT_COLOR=GREEN;	    
  	while(1) 
	{	

            POINT_COLOR=RED;	
//             key=KEY_Scan(0);
// 	       	if(key==1){s++;LCD_ShowNum(50,0,s,3,16);}
// 			else if(key==3) {s--;LCD_ShowNum(50,0,s,3,16);}


			status = PcdRequest(PICC_REQALL,CT);/*����*/
			if(status==MI_OK)//�����ɹ�
            {
                LCD_ShowString(0,30,200,16,16,"FIND_A_CARD");
				for(j=0;j<2;j++)
				{
					 LCD_ShowNum(120+j*32,30,CT[j],3,16);
				}
                status=MI_ERR;
                status = PcdAnticoll(SN);/*����ײ*/
            }


			if (status==MI_OK)//���nײ�ɹ�
			{   
                POINT_COLOR=BLUE;
				LCD_ShowString(0,50,200,16,16,"Anticoll__OK");

                POINT_COLOR=RED;
				ShowID(0,80,SN); //��Һ��������ʾ����ID��
				//printf("ID:%02x %02x %02x %02x\n",SN[0],SN[1],SN[2],SN[3]);//���Ϳ���


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
// 				LEDA=0;//ֻ����led0
// 				LEDB=1;
			}
			if(status==MI_OK)//�x���ɹ�
            {
                LCD_ShowString(0,200,200,16,16,"PcdSelect_MI_OK  ");
                status=MI_ERR;
                status =PcdAuthState(0x60,0x09,KEY,SN);
            }
            if(status==MI_OK)//��C�ɹ�
            {
                LCD_ShowString(0,220,200,16,16,"PcdAuthState_MI_OK  ");
                status=MI_ERR;             

//                 status=PcdWrite(s,RFID1);
                status=PcdRead(s,RFID);
            }

			if(status==MI_OK)//�x���ɹ�
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
*�������ܣ���ʾ���Ŀ��ţ���ʮ��������ʾ
*������x��y ����
*		p ���ŵĵ�ַ
*		charcolor �ַ�����ɫ
*		bkcolor   ��������ɫ
***************************************/
void ShowID(u16 x,u16 y, u8 *p)	 //��ʾ���Ŀ��ţ���ʮ��������ʾ
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
*�������ܣ���p��n����
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

