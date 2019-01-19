#include "Timer.h"
#include "keyboard.h"
#include "string.h"
#include "IO.h"
#include "flashmanege.h"
#include "Dis12864.h"

u32 Display_Delay;
u32 CX_TIMER;
Timer4_Event_Des  Timer_Delay[TIMER_NUM];
timer4_desc  timer4_array[TIMER_NUM];
u32  GET_Warte_Timer;
u32  XH_OUT_TIMER=0;
u8  Sleep_Flag = 0;
u32 Timer_Sleep  = 0;//˯����
u8 admin_sleep_flag=1; //����Ա���治��Ҫ����
Flow  flow=
{
	0,0,0
};



void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM4_Int_Init(7200,100);//��ʼ����ʱ��4  10ms����һ���ж�

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			Display_Delay++;
			GET_Warte_Timer++;
			XH_OUT_TIMER++;
			CX_TIMER++;
			key_Event();
		  TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	
		}
}


//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM4->PSC=psc;  	//Ԥ��Ƶ��	  
	TIM4->DIER|=1<<0;   //��������ж�	  
	TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��4
	
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����								 
}



//ע�ắ��
u8  TmRegsiter(char *name,timer4_fun fp,u16 TM)
{
	u8 i;
	for(i=0;i<TIMER_NUM;i++)
	{
		if(Timer_Delay[i].fp);
		else 
		{
			Timer_Delay[i].name = name;
			Timer_Delay[i].fp = fp;
			Timer_Delay[i].delay_Timer = TM;
			Timer_Delay[i].Now_Timer = 0;
			return 0;
		}
	}	return 1;
}

//ж�غ���
void UnTMRegsiter(char *name)
{
	u8 i;
	for(i=0;i<TIMER_NUM;i++)
	{
		if(!strcmp((const char *)Timer_Delay[i].name,(const char *)name))
		{
			Timer_Delay[i].name = NULL;
			Timer_Delay[i].fp = NULL;
			Timer_Delay[i].delay_Timer = 0;
			Timer_Delay[i].Now_Timer = 0;			
			return ;
		}
	}
}

//�¼�������
void Event_Manage(void)
{
	u8 i;
	for(i=0;i<TIMER_NUM;i++)
	{
		if(Timer_Delay[i].fp&&Timer_Delay[i].name)
		{	
			Timer_Delay[i].Now_Timer+=1;//����ʱ���			
			if(Timer_Delay[i].Now_Timer == Timer_Delay[i].delay_Timer)
			{				
				 Timer_Delay[i].fp();//ִ�к���							 
					/*ж�ص�ǰ����*/
					Timer_Delay[i].name = NULL;
					Timer_Delay[i].fp = NULL;
					Timer_Delay[i].delay_Timer = 0;
					Timer_Delay[i].Now_Timer = 0;					
			}	
		}
	}
}

//������Ϣ����Ϣ����
void  Send_MessagetoQueue(char *name,u8 *data,u8 datalen)
{
	u8 i;
	for(i=0;i<TIMER_NUM;i++)//�ҵ�λ��
	{
		if(!strcmp(Timer_Delay[i].name,name))
		{
			memcpy(Timer_Delay[i].Messag,data,datalen);
			Timer_Delay[i].MesLen = datalen;
			return ;
		}
	}
}
//��ȡ��Ϣ�����е�����
u8  Recevie_MessagetoQueue(char *name,u8 *dataout)
{
	u8 i;
	for(i=0;i<TIMER_NUM;i++)//�ҵ�λ��
	{
		if(!strcmp(Timer_Delay[i].name,name))
		{
			memcpy(dataout,Timer_Delay[i].Messag,Timer_Delay[i].MesLen);			
			return Timer_Delay[i].MesLen;
		}
	}
	return 0;
}

extern u8 wait_disp;
//��ʱ��4�жϷ������	 
void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X0001)//����ж�
	{
		__disable_irq() ; //�ر����ж�
		Event_Manage();
		Timer_Sleep++;
		if(Sleep_Flag &&admin_sleep_flag )
		{
			if(Timer_Sleep > Sleep_MAX)
			{
				key.KEY_Falg++;
				key.KEY_Val=0x03;
				Timer_Sleep =0;
				wait_disp = 0;
				Sleep_Flag=0;
				LCD_LightOff;//�رձ����
			}
		}
	  __enable_irq() ; //�����ж�
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	    
}




/*����ȡˮ����*/
//�����������
void  GET_FLOW(void)
{
	flow.UP_Flow =  (float) AdminPra.FlowUPSet/600;
	flow.RO_Flow =  (float) AdminPra.FlowROSet/600;
	flow.EDI_Flow = (float) AdminPra.FlowEDISet/600;
}

/*******************************************************************************
* �� �� ��: void GET_UP_TIMER(u16 ML)
* ��    ��: ����ȡUPˮ
* ˵    ��: 
*******************************************************************************/
void OFF_UPF(void)
{
	UP_IO = OFF;
}
void GET_UP_TIMER(u16 ML)
{
	u16 delay_timer=0;
	/*��UPF*/
	UP_IO = ON;
	delay_timer = (u16)ML*flow.UP_Flow;
	TmRegsiter("UPF",OFF_UPF,delay_timer);//ע�ắ��
}
/*******************************************************************************
* �� �� ��: void GET_RO_TIMER(u16 ML)
* ��    ��: ����ȡROˮ
* ˵    ��: 
*******************************************************************************/
void OFF_ROF(void)
{
	RO_IO = OFF;
}
void GET_RO_TIMER(u16 ML)
{
	u16 delay_timer=0;
	/*��UPF*/
	UP_IO = ON;
	delay_timer = (u16)ML*flow.RO_Flow;
	TmRegsiter("ROF",OFF_ROF,delay_timer);//ע�ắ��
}



/*******************************************************************************
* �� �� ��: void GET_EDI_TIMER(u16 ML)
* ��    ��: ����ȡEDIˮ
* ˵    ��: 
*******************************************************************************/
void OFF_EDIF(void)
{
	EDI_IO = OFF;
}
void GET_EDI_TIMER(u16 ML)
{
	u16 delay_timer=0;
	/*��UPF*/
	UP_IO = ON;
	delay_timer = (u16)ML*flow.EDI_Flow;
	TmRegsiter("ROF",OFF_EDIF,delay_timer);//ע�ắ��
}





