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
		 RCC->APB2ENR |= 1<<4;    //��ʱ��PC
		 GPIOC->CRL   &= 0X0000FFFF;
		 GPIOC->CRH   &= 0XFFFFFF00;
		 GPIOC->CRL   |= 0X88880000;
		 GPIOC->CRH   |= 0X00000088;
	   GPIOC->ODR   |= 0X3F<<4;//����IO		 	
}



//������ʼ��
void InitKeyboard(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_C,4,FTIR); 	//�����ش���
	Ex_NVIC_Config(GPIO_C,5,FTIR); 	//�½��ش���
	Ex_NVIC_Config(GPIO_C,6,FTIR); 	//�½��ش���
 	Ex_NVIC_Config(GPIO_C,7,FTIR); 	//�½��ش���
	Ex_NVIC_Config(GPIO_C,8,FTIR); 	//�½��ش���
	Ex_NVIC_Config(GPIO_C,9,FTIR); 	//�½��ش���
	MY_NVIC_Init(2,3,EXTI4_IRQn,2);	//��ռ2�������ȼ�3����2
	MY_NVIC_Init(2,2,EXTI9_5_IRQn,2);	//��ռ2�������ȼ�2����2	   
 	
}
u32  long_Sw =0;
extern u8 OPEN_FLAG;
void EXTI9_5_IRQHandler(void)
{
		if(key_Enter);//ȷ�ϼ�
		else 
		{
			if(key.Event_Busy);//���æµ�Ͳ����밴���ж��¼�
			else 
				{		
					delay_ms(10);					
					key.KEY_Falg++;
					key.KEY_Val|=1<<1;
					EXTI->PR |= 1<<5;	
				}
		}
		if(key_Up);//��
		else
		{
			if(key.Event_Busy);//���æµ�Ͳ����밴���ж��¼�
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
		if(key_Dowm);//��
		else 
		{
			if(key.Event_Busy);//���æµ�Ͳ����밴���ж��¼�
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
		if(key_Bank);//���ؼ�
		else 
		{
			if(key.Event_Busy);//���æµ�Ͳ����밴���ж��¼�
			else 
				{		
					delay_ms(10);					
					key.KEY_Falg++;
					key.KEY_Val|=1<<4;
					EXTI->PR |= 1<<8;	
				}
		}
		
		if(key_Edi);//EDI��
		else 
		{
			if(key.Event_Busy);//���æµ�Ͳ����밴���ж��¼�
			else 
				{		
					delay_ms(10);					
					key.KEY_Falg++;
					key.KEY_Val|=1<<0;
					if(OPEN_FLAG)
					{
						 LED_IO = OFF;
						 __set_FAULTMASK(1); // �ر������ж�
						  NVIC_SystemReset(); // ��λ
					}
					EXTI->PR |= 1<<9;	
				}
		}
		Timer_Sleep = 0;
		if((Sleep_Flag == 0)&&OPEN_FLAG)
		{
			LCD_LightOn;//�򿪱����
			Sleep_Flag =1;//������һ�� ˯��
		}
		
}



void EXTI4_IRQHandler(void )
{
	
	Timer_Sleep = 0;
	if(key_Off_On);//���ػ���
	else 
	{	
		if(key.Event_Busy);//���æµ�Ͳ����밴���ж��¼�
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
//������ʱ���¼�
//�жϰ�����������ģʽ  �������¼���Ӧ�� ��key.KEY_Falg=0;key.KEY_Mode=0;key.KEY_Count=0;key.KEY_Shadow=0xff;
void key_Event(void)
{	
	  key.Event_Busy = 0;	
		if(key.Event_Busy)
			return ;//æµֱ�ӷ���
			if(timer_event_Off)return ;
			if((key_Up==0)&&(key_Dowm==0))
			{
				timer_event_Off=1;
			}else timer_event_Off=0;
}

//�����¼�������ɵ���
static void key_Event_Over(void)
{		
		if(key.KEY_Val == 0x03);
		else{ 
			
		key.KEY_Val =0;
		key.KEY_Falg=0; //�������λ==0
		}
		key.KEY_Mode=0; //�ָ�Ϊ����ģʽ
		key.KEY_Count=0;//��������ֵ  ==0
		//key.KEY_Shadow=0xff;//Ӱ�ӱ��� == 0xff
}

//�¼�û�д�����Ͳ����а��� ״̬�ж�
void key_event_buys(void)
{
	key.Event_Busy = 1;	
}

void key_event_lesure(void)
{
	key.Event_Busy = 0;	
	key_Event_Over();
}

