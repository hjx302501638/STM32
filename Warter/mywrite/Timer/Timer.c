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
u32 Timer_Sleep  = 0;//睡眠用
u8 admin_sleep_flag=1; //管理员界面不需要休眠
Flow  flow=
{
	0,0,0
};



void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM4_Int_Init(7200,100);//初始化定时器4  10ms进入一次中断

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			Display_Delay++;
			GET_Warte_Timer++;
			XH_OUT_TIMER++;
			CX_TIMER++;
			key_Event();
		  TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
	
		}
}


//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
 	TIM4->ARR=arr;  	//设定计数器自动重装值 
	TIM4->PSC=psc;  	//预分频器	  
	TIM4->DIER|=1<<0;   //允许更新中断	  
	TIM4->CR1|=0x01;    //使能定时器4
	
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、								 
}



//注册函数
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

//卸载函数
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

//事件管理器
void Event_Manage(void)
{
	u8 i;
	for(i=0;i<TIMER_NUM;i++)
	{
		if(Timer_Delay[i].fp&&Timer_Delay[i].name)
		{	
			Timer_Delay[i].Now_Timer+=1;//现在时间加			
			if(Timer_Delay[i].Now_Timer == Timer_Delay[i].delay_Timer)
			{				
				 Timer_Delay[i].fp();//执行函数							 
					/*卸载当前函数*/
					Timer_Delay[i].name = NULL;
					Timer_Delay[i].fp = NULL;
					Timer_Delay[i].delay_Timer = 0;
					Timer_Delay[i].Now_Timer = 0;					
			}	
		}
	}
}

//传递信息到消息队列
void  Send_MessagetoQueue(char *name,u8 *data,u8 datalen)
{
	u8 i;
	for(i=0;i<TIMER_NUM;i++)//找到位置
	{
		if(!strcmp(Timer_Delay[i].name,name))
		{
			memcpy(Timer_Delay[i].Messag,data,datalen);
			Timer_Delay[i].MesLen = datalen;
			return ;
		}
	}
}
//获取消息队列中的数据
u8  Recevie_MessagetoQueue(char *name,u8 *dataout)
{
	u8 i;
	for(i=0;i<TIMER_NUM;i++)//找到位置
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
//定时器4中断服务程序	 
void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X0001)//溢出中断
	{
		__disable_irq() ; //关闭总中断
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
				LCD_LightOff;//关闭背光灯
			}
		}
	  __enable_irq() ; //打开总中断
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}




/*定量取水调用*/
//获得流量参数
void  GET_FLOW(void)
{
	flow.UP_Flow =  (float) AdminPra.FlowUPSet/600;
	flow.RO_Flow =  (float) AdminPra.FlowROSet/600;
	flow.EDI_Flow = (float) AdminPra.FlowEDISet/600;
}

/*******************************************************************************
* 函 数 名: void GET_UP_TIMER(u16 ML)
* 功    能: 定量取UP水
* 说    明: 
*******************************************************************************/
void OFF_UPF(void)
{
	UP_IO = OFF;
}
void GET_UP_TIMER(u16 ML)
{
	u16 delay_timer=0;
	/*打开UPF*/
	UP_IO = ON;
	delay_timer = (u16)ML*flow.UP_Flow;
	TmRegsiter("UPF",OFF_UPF,delay_timer);//注册函数
}
/*******************************************************************************
* 函 数 名: void GET_RO_TIMER(u16 ML)
* 功    能: 定量取RO水
* 说    明: 
*******************************************************************************/
void OFF_ROF(void)
{
	RO_IO = OFF;
}
void GET_RO_TIMER(u16 ML)
{
	u16 delay_timer=0;
	/*打开UPF*/
	UP_IO = ON;
	delay_timer = (u16)ML*flow.RO_Flow;
	TmRegsiter("ROF",OFF_ROF,delay_timer);//注册函数
}



/*******************************************************************************
* 函 数 名: void GET_EDI_TIMER(u16 ML)
* 功    能: 定量取EDI水
* 说    明: 
*******************************************************************************/
void OFF_EDIF(void)
{
	EDI_IO = OFF;
}
void GET_EDI_TIMER(u16 ML)
{
	u16 delay_timer=0;
	/*打开UPF*/
	UP_IO = ON;
	delay_timer = (u16)ML*flow.EDI_Flow;
	TmRegsiter("ROF",OFF_EDIF,delay_timer);//注册函数
}





