#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#define DELAY_DISPLAY 10
#define DELAY_SAMPLE   10

extern u32 Display_Delay;
extern u32  XH_OUT_TIMER;
extern u32 CX_TIMER;
#define TIMER_NUM  10//�¼��������Ϊ10��
#define Message_Len 100 
#define NameMax       20
#define Sleep_MAX    2000//20s����˯��ģʽ
/*�¼�����*/
typedef void (*timer4_fun)(void);




typedef struct Timer4_Event_Des
{
	char *name;
	u8  Messag[Message_Len];//��Ϣ�������ڴ��ݲ���
	u8  MesLen;
	u16 delay_Timer;
	u16 Now_Timer;
	timer4_fun  fp;	
}Timer4_Event_Des,*pTimer4_Event_Des;



/*�����¼��ṹ��*/
typedef struct timer4_desc
{
	char *name;//������
  timer4_fun fp;//����
}timer4_desc,*ptimer4_desc;




//��������    �����������   1ML:MS����1MLˮ��Ҫ���ٺ��룩
typedef struct  Flow
{
	float UP_Flow;
	float RO_Flow;
	float EDI_Flow;
}Flow,*pFlow;
extern Flow  flow;
extern u32  GET_Warte_Timer;
void  GET_FLOW(void);//�����������
void GET_UP_TIMER(u16 ML);//����ȡUPˮ
void GET_RO_TIMER(u16 ML);//����ȡROˮ
void GET_EDI_TIMER(u16 ML);//����ȡEDIˮ

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
u8  TmRegsiter(char *name,timer4_fun fp,u16 TM);//ע�ắ��
u8  Recevie_MessagetoQueue(char *name,u8 *dataout);//��ȡ��Ϣ�����е�����

#endif 



