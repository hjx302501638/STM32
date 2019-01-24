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

/*初始化*/
void  Init_ALL(void)
{
		/****************初始化**************************************/
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	 delay_init();	    	 //延时函数初始化	 
	 /*关闭JTAG*/
	 RCC->APB2ENR|=1<<0;
	 AFIO->MAPR |= 0X2<<24;	 
	 Reset_DS1302();	
	 Ini_Lcd();	 
	 InitKeyboard();
	 IO_INIT();
	 VoiceIO_Init();
	 uart_init(9600);	 	//串口初始化为115200
	 My_Adc_Init();//初始化ADC
		delay_ms(1000);
   My_Adc_Init();
	 TIM3_Int_Init(1000,720);
	 
	 /****************初始化**************************************/
}


/*开机处理*/
void Open_UP(void)
{

		
		/****************开机处理************************************/
	 Check_AD_Device();//检测表是否存在
	
	 Read_Userdata();//读取用户参数
	 Read_Admindata();//读取管理员参数
	 GET_FLOW();      //获得流量参数
	 while(1)
	 {
		
		 if(UserPra.OnOffFlag)
		 {
			 XH_OUT_TIMER = 0;
			 Timer_Sleep=0;
			 //读取当前时间
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
	 
	 VoiceOut(VOICE_WELCOME);//开机提示
	 Draw_PM(WTLogo);   //显示LOGO
	 Display_RefreshAll();
	 delay_ms(1000);   //等待进入界面 
	 delay_ms(1000);   //等待进入界面 
	 delay_ms(1000);   //等待进入界面 
	 /*       清除显示         */
	 DisplayBuf_Clr();
	 Clr_Pictrue(); 
	 Display_RefreshAll();
	 /*检测 所有表是否存在*/
	 Check_AD_Device();//检测表是否存在
	 Sleep_Flag =1;//默认开启睡眠
	/****************开机处理************************************/
}




int main(void)
 { 	
	 u32 temp=0;
	 Init_ALL();
	 Open_UP();
	while(1)
	{
		 if(Sleep_Flag) LCD_LightOn;//睡眠控制
	   Sotf_Versions_Controller();//软件版本控制器
		 menu();//菜单控制
		/********************自动关机控制*******************/
		 temp++;
			if(temp == 0x000fffff)
			{		temp =0;				 
				 if(UserPra.OnOffFlag)
				 {
					 //读取当前时间
					 BurstRead1302(Ds1302DataBuf);
					 DS1302_Datato10();					 
						 if((Ds1302DataBuf10[3]==UserPra.OffTimer[0])&&(Ds1302DataBuf10[4]==UserPra.OffTimer[1]))
						 { 					
							 __set_FAULTMASK(1); // 关闭所有中断
								NVIC_SystemReset(); // 复位
						 }					
				 }
		 }
		/***************自动关机控制*********************/
	}   	   
}











