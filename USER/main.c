#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stmflash.h"   
#include "Timer.h"
#include "admin.h"
#include "Dis12864.h"
#include "keyboard.h"
#include "query.h"
#include "getwarte.h"
#include "menu.h"
#include "IO.h"
#include "my_adc.h"
#include "flashmanege.h"
#include "DS1302.h"
#include "Voice.h"
#include "APP.h"


extern u8 Ds1302DataBuf[8];
extern u8 Ds1302DataBuf10[7];
extern const unsigned char WTLogo[];
extern u8  Sleep_Flag;
extern  u32 Timer_Sleep;
u8 OPEN_FLAG =0;

/*��ʼ��*/
void  Init_ALL(void)
{
		/****************��ʼ��**************************************/
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	 delay_init();	    	 //��ʱ������ʼ��	 
	 /*�ر�JTAG*/
	 RCC->APB2ENR|=1<<0;
	 AFIO->MAPR |= 0X2<<24;	 
	 Reset_DS1302();	
	 Ini_Lcd();	 
	 InitKeyboard();
	 IO_INIT();
	 VoiceIO_Init();
	 uart_init(9600);	 	//���ڳ�ʼ��Ϊ115200
	 My_Adc_Init();//��ʼ��ADC
		delay_ms(1000);
   My_Adc_Init();
	 TIM3_Int_Init(1000,720);
	 
	 /****************��ʼ��**************************************/
}


/*��������*/
void Open_UP(void)
{

		
		/****************��������************************************/
	 Check_AD_Device();//�����Ƿ����
	
	 Read_Userdata();//��ȡ�û�����
	 Read_Admindata();//��ȡ����Ա����
	 GET_FLOW();      //�����������
	 while(1)
	 {
		
		 if(UserPra.OnOffFlag)
		 {
			 XH_OUT_TIMER = 0;
			 Timer_Sleep=0;
			 //��ȡ��ǰʱ��
			 BurstRead1302(Ds1302DataBuf);
			 DS1302_Datato10();	
				 if((Ds1302DataBuf10[3]==UserPra.OnTimer[0])&&(Ds1302DataBuf10[4]==UserPra.OnTimer[1]))
				 { 					 
					 LCD_LightOn;
					 LED_IO = ON;
					 OPEN_FLAG = 1;
					 break;
				 }
			}
		 if((key.KEY_Val == KEY_OFF_ON)&&(OPEN_FLAG == 0))
		 {
			 LCD_LightOn;
			 LED_IO = ON;
			 OPEN_FLAG = 1;
			 break;
		 }
		 key_event_lesure();
		 
	 }
	 
	 VoiceOut(VOICE_WELCOME);//������ʾ
	 Draw_PM(WTLogo);   //��ʾLOGO
	 Display_RefreshAll();
	 delay_ms(1000);   //�ȴ�������� 
	 delay_ms(1000);   //�ȴ�������� 
	 delay_ms(1000);   //�ȴ�������� 
	 /*       �����ʾ         */
	 DisplayBuf_Clr();
	 Clr_Pictrue(); 
	 Display_RefreshAll();
	 /*��� ���б��Ƿ����*/
	 Check_AD_Device();//�����Ƿ����
	 Sleep_Flag =1;//Ĭ�Ͽ���˯��
	/****************��������************************************/
}




int main(void)
 { 	
	 u32 temp=0;
	 Init_ALL();
	 Open_UP();
	while(1)
	{
		 if(Sleep_Flag) LCD_LightOn;//˯�߿���
	   Sotf_Versions_Controller();//����汾������
		 menu();//�˵�����
		/********************�Զ��ػ�����*******************/
		 temp++;
			if(temp == 0x000fffff)
			{		temp =0;				 
				 if(UserPra.OnOffFlag)
				 {
					 //��ȡ��ǰʱ��
					 BurstRead1302(Ds1302DataBuf);
					 DS1302_Datato10();					 
						 if((Ds1302DataBuf10[3]==UserPra.OffTimer[0])&&(Ds1302DataBuf10[4]==UserPra.OffTimer[1]))
						 { 					
							 __set_FAULTMASK(1); // �ر������ж�
								NVIC_SystemReset(); // ��λ
						 }					
				 }
		 }
		/***************�Զ��ػ�����*********************/
	}   	   
}











