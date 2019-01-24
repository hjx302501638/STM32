#ifndef __APP_H
#define __APP_H
#include "sys.h"

#define CX_State   1  //冲洗状态
#define QS_State   2  //缺水
#define ZS_State   3  //制水状态
#define BY_State   4  //备用状态
#define ZS1_State  5  //一级制水状态
#define ZS1and2_State 6 //一二级同时制水
#define ZS2_State  7  //二级制水状态


//输入信号消抖
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



void Machine_A_Routine(void);//A版本程序
void Machine_B_Routine(void);//B版本程序
void Machine_C_Routine(void);//C版本程序
void Sotf_Versions_Controller(void);

void A_CX_Step1(void);
void B_CX_Step1(void);
extern u8 Machine_State ;//用来指示机器状态
extern u8 NO_Warter_Flag;//用来指示系统缺水
#endif



