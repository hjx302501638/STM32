#include "keyboard.h"
#include "admin.h"
#include "delay.h"
#include "IO.h"
#include "dis12864.h"
extern u32 Timer_Sleep;
extern u8  Sleep_Flag;
KEY key =
{
	0,
	0,
	0,
	0xff,
	0,
	0
};
static void KEY_Init(void )
{
		 RCC->APB2ENR |= 1<<4;    //打开时钟PC
		 GPIOC->CRL   &= 0X0000FFFF;
		 GPIOC->CRH   &= 0XFFFFFF00;
		 GPIOC->CRL   |= 0X88880000;
		 GPIOC->CRH   |= 0X00000088;
	   GPIOC->ODR   |= 0X3F<<4;//上拉IO		 	
}



//按键初始化
void InitKeyboard(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_C,4,FTIR); 	//上升沿触发
	Ex_NVIC_Config(GPIO_C,5,FTIR); 	//下降沿触发
	Ex_NVIC_Config(GPIO_C,6,FTIR); 	//下降沿触发
 	Ex_NVIC_Config(GPIO_C,7,FTIR); 	//下降沿触发
	Ex_NVIC_Config(GPIO_C,8,FTIR); 	//下降沿触发
	Ex_NVIC_Config(GPIO_C,9,FTIR); 	//下降沿触发
	MY_NVIC_Init(2,3,EXTI4_IRQn,2);	//抢占2，子优先级3，组2
	MY_NVIC_Init(2,2,EXTI9_5_IRQn,2);	//抢占2，子优先级2，组2	   
 	
}
u32  long_Sw =0;
extern u8 OPEN_FLAG;
void EXTI9_5_IRQHandler(void)
{
		if(key_Enter);//确认键
		else 
		{
			if(key.Event_Busy);//如果忙碌就不进入按键中断事件
			else 
				{		
					delay_ms(10);					
					key.KEY_Falg++;
					key.KEY_Val|=1<<1;
					EXTI->PR |= 1<<5;	
				}
		}
		if(key_Up);//上
		else
		{
			if(key.Event_Busy);//如果忙碌就不进入按键中断事件
			else 
				{	
					delay_ms(10);					
					key.KEY_Falg++;
					while(!key_Up)
						{
						  long_Sw++;if(long_Sw>0x0008ffff)break;
						}
					if(long_Sw>0x0008ffff)
						key.KEY_Val|=1<<6;
						else 
					key.KEY_Val|=1<<2;
						long_Sw =0;
					EXTI->PR |= 1<<6;	
				}
		}
		if(key_Dowm);//下
		else 
		{
			if(key.Event_Busy);//如果忙碌就不进入按键中断事件
			else 
				{		
					delay_ms(10);					
					key.KEY_Falg++;
					while(!key_Dowm)
						{
						  long_Sw++;if(long_Sw>0x0008ffff)break;
						}					
						if(long_Sw>0x0008ffff)
						key.KEY_Val|=1<<7;
						else
						key.KEY_Val|=1<<3;
						long_Sw =0;
						EXTI->PR |= 1<<7;	
				}
		}
		if(key_Bank);//返回键
		else 
		{
			if(key.Event_Busy);//如果忙碌就不进入按键中断事件
			else 
				{		
					delay_ms(10);					
					key.KEY_Falg++;
					key.KEY_Val|=1<<4;
					EXTI->PR |= 1<<8;	
				}
		}
		
		if(key_Edi);//EDI键
		else 
		{
			if(key.Event_Busy);//如果忙碌就不进入按键中断事件
			else 
				{		
					delay_ms(10);					
					key.KEY_Falg++;
					key.KEY_Val|=1<<0;
					if(OPEN_FLAG)
					{
						 LED_IO = OFF;
						 __set_FAULTMASK(1); // 关闭所有中断
						  NVIC_SystemReset(); // 复位
					}
					EXTI->PR |= 1<<9;	
				}
		}
		Timer_Sleep = 0;
		if((Sleep_Flag == 0)&&OPEN_FLAG)
		{
			LCD_LightOn;//打开背光灯
			Sleep_Flag =1;//开启下一次 睡眠
		}
		
}



void EXTI4_IRQHandler(void )
{
	
	Timer_Sleep = 0;
	if(key_Off_On);//开关机键
	else 
	{	
		if(key.Event_Busy);//如果忙碌就不进入按键中断事件
		else 
			{	
				delay_ms(10);				
				key.KEY_Falg++;
				key.KEY_Val|=1<<0;
			}
	}
	EXTI->PR |= 1<<4;
}

u8 timer_event_Off=0;
//按键定时器事件
//判断按键处于那种模式  处理完事件后应该 给key.KEY_Falg=0;key.KEY_Mode=0;key.KEY_Count=0;key.KEY_Shadow=0xff;
void key_Event(void)
{	
	  key.Event_Busy = 0;	
		if(key.Event_Busy)
			return ;//忙碌直接返回
			if(timer_event_Off)return ;
			if((key_Up==0)&&(key_Dowm==0))
			{
				timer_event_Off=1;
			}else timer_event_Off=0;
}

//按键事件处理完成调用
static void key_Event_Over(void)
{		
		if(key.KEY_Val == 0x03);
		else{ 
			
		key.KEY_Val =0;
		key.KEY_Falg=0; //按键标记位==0
		}
		key.KEY_Mode=0; //恢复为单按模式
		key.KEY_Count=0;//按键计数值  ==0
		//key.KEY_Shadow=0xff;//影子变量 == 0xff
}

//事件没有处理完就不进行按键 状态判断
void key_event_buys(void)
{
	key.Event_Busy = 1;	
}

void key_event_lesure(void)
{
	key.Event_Busy = 0;	
	key_Event_Over();
}

