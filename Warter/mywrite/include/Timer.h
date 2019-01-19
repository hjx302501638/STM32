#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#define DELAY_DISPLAY 10
#define DELAY_SAMPLE   10

extern u32 Display_Delay;
extern u32  XH_OUT_TIMER;
extern u32 CX_TIMER;
#define TIMER_NUM  10//事件个数最大为10个
#define Message_Len 100 
#define NameMax       20
#define Sleep_MAX    2000//20s进入睡眠模式
/*事件函数*/
typedef void (*timer4_fun)(void);




typedef struct Timer4_Event_Des
{
	char *name;
	u8  Messag[Message_Len];//消息队列用于传递参数
	u8  MesLen;
	u16 delay_Timer;
	u16 Now_Timer;
	timer4_fun  fp;	
}Timer4_Event_Des,*pTimer4_Event_Des;



/*函数事件结构体*/
typedef struct timer4_desc
{
	char *name;//函数名
  timer4_fun fp;//函数
}timer4_desc,*ptimer4_desc;




//流量参数    存放流量比例   1ML:MS（放1ML水需要多少毫秒）
typedef struct  Flow
{
	float UP_Flow;
	float RO_Flow;
	float EDI_Flow;
}Flow,*pFlow;
extern Flow  flow;
extern u32  GET_Warte_Timer;
void  GET_FLOW(void);//获得流量参数
void GET_UP_TIMER(u16 ML);//定量取UP水
void GET_RO_TIMER(u16 ML);//定量取RO水
void GET_EDI_TIMER(u16 ML);//定量取EDI水

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
u8  TmRegsiter(char *name,timer4_fun fp,u16 TM);//注册函数
u8  Recevie_MessagetoQueue(char *name,u8 *dataout);//获取消息队列中的数据

#endif 



