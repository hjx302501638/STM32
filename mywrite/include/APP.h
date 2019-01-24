#ifndef __APP_H
#define __APP_H
#include "sys.h"

#define CX_State   1  //��ϴ״̬
#define QS_State   2  //ȱˮ
#define ZS_State   3  //��ˮ״̬
#define BY_State   4  //����״̬
#define ZS1_State  5  //һ����ˮ״̬
#define ZS1and2_State 6 //һ����ͬʱ��ˮ
#define ZS2_State  7  //������ˮ״̬


//�����ź�����
typedef struct Input_Signal
{
	u32 L_levl;
	u32 M_levl;
	u32 H_levl;
	u32 H_f;
	u32 L_f;
}Input_Signal,*pInput_Signal;

#define Delay_L_Level      30000
#define Delay_M_Level      30000
#define Delay_H_Level      30000
#define Delay_L_F          30000
#define Delay_H_F          30000



void Machine_A_Routine(void);//A�汾����
void Machine_B_Routine(void);//B�汾����
void Machine_C_Routine(void);//C�汾����
void Sotf_Versions_Controller(void);

void A_CX_Step1(void);
void B_CX_Step1(void);
extern u8 Machine_State ;//����ָʾ����״̬
extern u8 NO_Warter_Flag;//����ָʾϵͳȱˮ
#endif



