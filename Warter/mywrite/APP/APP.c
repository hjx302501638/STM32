#include "APP.h"
#include "IO.h"
#include "Voice.h"
#include "Timer.h"
#include "flashmanege.h"


u8 Machine_State =0;//用来指示机器状态
u8 Last_State  =0;
u8 Machine_Sw  =1;//开关状态  1 开启  0 关闭
u8 Voice_flag =0;
u8 NO_Warter_Flag =0;
void A_CX_Step2(void);
void A_CX_Step3(void);

//冲洗步骤1
void A_CX_Step1(void)
{
	VoiceOut(VOICE_WASH);//语言提示
	JS_IO = ON;//打开进水阀
	Mllt_IO = ON;//超滤阀打开
	TmRegsiter("CX_Step2",A_CX_Step2,3000);
}

//冲洗步骤2
void A_CX_Step2(void)
{
	Mllt_IO = OFF;//超滤阀关闭
	Pump_1_1_IO = ON;//启动泵P1.1
	Pump_1_2_IO = ON;//启动泵P1.2
	CX_IO = ON;//冲洗电磁阀打开
	TmRegsiter("CX_Step2",A_CX_Step3,3000);
}

//冲洗步骤3
void A_CX_Step3(void)
{
	CX_IO = OFF;//冲洗电磁阀关闭
	Machine_State = ZS_State;//标记为制水状态
	if(Machine_State != Last_State)
	VoiceOut(VOICE_PRODUCEWATER);//语言提示制水
	Last_State = Machine_State;
	
}



//A版本程序
void Machine_A_Routine(void)
{
	//检测LF是否正常（检测水压是否正常）
	if(L_F == F_ON)
	{
		NO_Warter_Flag =0;
		Voice_flag = 0;
		if(((Machine_State  == 0)||(Machine_State == QS_State)) && Machine_Sw)
		{
			Machine_Sw =0;
			Machine_State = CX_State;//系统进入冲洗状态
			A_CX_Step1();//开始冲洗
		}
		if(Machine_State == ZS_State)
		{
			if(H_F)//到达满水备用状态
			{
				JS_IO = OFF;//关闭进水阀
				Pump_1_1_IO = OFF;//关闭泵P1.1
				Pump_1_2_IO = OFF;//关闭泵P1.2
				
				Machine_State = BY_State;
				if(Machine_State != Last_State)
				VoiceOut(VOICE_WAIT);//语言提示备水
				Last_State = Machine_State;
			}else 
			{
				JS_IO = ON;//打开进水阀
				Pump_1_1_IO = ON;//打开泵P1.1
				Pump_1_2_IO = ON;//打开泵P1.2
				 Machine_State = ZS_State;
					if(Machine_State != Last_State)
					VoiceOut(VOICE_PRODUCEWATER);//语言提示制水
					Last_State = Machine_State;
				
			}
		}
		if(Machine_State == BY_State)
		{
			if(!H_F)//在备用状态高压阀断开
			{
				JS_IO = ON;//打开进水阀
				Pump_1_1_IO = ON;//打开泵P1.1
				Pump_1_2_IO = ON;//打开泵P1.2
				
				
				Machine_State = ZS_State;//机器进入制水模式
				if(Machine_State != Last_State)
				VoiceOut(VOICE_PRODUCEWATER);//语言提示制水
				Last_State = Machine_State;
			}else 
			{
				JS_IO = OFF;//打开进水阀
				Pump_1_1_IO = OFF;//打开泵P1.1
				Pump_1_2_IO = OFF;//打开泵P1.2	
				
				Machine_State = BY_State;//机器进入制水模式
				if(Machine_State != Last_State)
				VoiceOut(VOICE_WAIT);//语言提示备水
				Last_State = Machine_State;
			}
		}
	}else 
	{
		if((Machine_State != BY_State)&&((Machine_State != ZS_State)))
		Machine_State = QS_State;//系统进入缺水状态
		else if(Voice_flag == 0)
		{
			VoiceOut(VOICE_WATERLESS);//语言提示	
			Voice_flag =1;
			NO_Warter_Flag =1;
		}else if(Machine_State == ZS_State)
		{
			if(Voice_flag ==1)
				VoiceOut(VOICE_PRODUCEWATER);//语言提示制水
		
		}else if(Machine_State == BY_State)
		{
			if(Voice_flag ==1)
				VoiceOut(VOICE_WAIT);//语言提示备水
		}
		
		if(Last_State != Machine_State)
			VoiceOut(VOICE_WATERLESS);//语言提示	
		
		/*保护状态*/
		JS_IO = OFF;//关闭进水阀
		Pump_1_1_IO = OFF;//关闭泵P1.1
		Pump_1_2_IO = OFF;//关闭泵P1.2
		Last_State = Machine_State;
		Voice_flag =2;
	}

}



void B_CX_Step2(void)
{
	CX_IO = OFF;//冲洗电磁阀关闭
	Machine_State = ZS1_State;//标记为1级制水状态
}
void B_CX_Step1(void)
{
	VoiceOut(VOICE_WASH);//语言提示
	JS_IO = ON;//打开进水阀
	Pump_1_1_IO = ON;//启动泵P1.1
	Pump_1_2_IO = ON;//启动泵P1.2
	CX_IO = ON;//冲洗电磁阀打开
	TmRegsiter("CX_Step2",B_CX_Step2,3000);
}

//B版本程序
void Machine_B_Routine(void)
{
	if(L_F == F_ON)
	{
		NO_Warter_Flag =0;
		Voice_flag =0;
		if(((Machine_State  == 0)||(Machine_State == QS_State)) && Machine_Sw)//进入冲洗
		{
			Machine_Sw =0;
			Machine_State = CX_State;//系统进入冲洗状态
			B_CX_Step1();//开始冲洗
		}
		 if(Machine_State == ZS1_State)
		 {
			 
			 if(!M_Level)//中液位接通
			 {
				  JS_IO = ON;//打开进水阀
				  Pump_1_1_IO = ON;//启动泵P1.1
					Pump_1_2_IO = ON;//启动泵P1.2
				  //Machine_State = ZS1_State;//一级制水状态
			 }
			 if(L_Level);//低液位未接通
				 else 
				 {
					 if(!H_F)//高压阀闭合
					 {
						 //低液位接通
						 Pump_2_1_IO = ON;//启动二级泵
						 Machine_State = ZS1and2_State;//一二级同时制水状态
					 }
				 }
				 if(H_Level)//高液位断开
				 {
					  JS_IO = OFF;//关闭进水阀
						Pump_1_1_IO = OFF;//关闭泵P1.1
						Pump_1_2_IO = OFF;//关闭泵P1.2						
				 }
				 if(H_F)//高压阀断开
				 {
					 Pump_2_1_IO = OFF;//关闭二级泵					
				 }
				 if(H_Level &&H_F)
					 Machine_State = BY_State;//备用状态
		 }
		 if(Machine_State == ZS1and2_State)//一二级同时制水状态
		 {
			 if(Last_State != Machine_State)
			 VoiceOut(VOICE_PRODUCEWATER);//语言提示制水
			 Last_State = Machine_State;
			 if(H_F)//高压开关HF断开
			 {
					Pump_2_1_IO = OFF;//关闭二级泵				 
			 }else 
			 {
				 Pump_2_1_IO = ON;//关闭二级泵				 
			 }
			 if(H_Level)
			 {
				  JS_IO = OFF;//关闭进水阀
					Pump_1_1_IO = OFF;//关闭泵P1.1
					Pump_1_2_IO = OFF;//关闭泵P1.2				  
			 }
			 if(!M_Level)//中液位接通
			 {
				  JS_IO = ON;//打开进水阀
				  Pump_1_1_IO = ON;//启动泵P1.1
					Pump_1_2_IO = ON;//启动泵P1.2
				  Machine_State = ZS1_State;//一级制水状态
			 }
			 if(H_Level &&H_F)
			 Machine_State = BY_State;//备用状态
		 }
		 if(Machine_State == BY_State)//在备水状态下判断
		 {
			 if(Last_State != Machine_State)
			 VoiceOut(VOICE_WAIT);//语言提示备水
			 Last_State = Machine_State;
			 if(!H_F)//高压阀接通
			 {
				  if(!L_Level)//保持低液位接通才开始二级制水
					 {
							//中液位接通
							Pump_2_1_IO = ON;//启动二级泵
							Machine_State = ZS2_State;//二级制水状态
					 }
			 }else 
			 {
				 Pump_2_1_IO = OFF;//关闭二级泵
			 }
			 if(!M_Level)//中液位接通
			 {
				  JS_IO = ON;//打开进水阀
				  Pump_1_1_IO = ON;//启动泵P1.1
					Pump_1_2_IO = ON;//启动泵P1.2
				  Machine_State = ZS1_State;//一级制水状态
			 }
			 
		 }
		 if(Machine_State == ZS2_State)
		 {
			 if(M_Level);//中液位没有接通
			 else 
			 {
				  JS_IO = ON;//打开进水阀
				  Pump_1_1_IO = ON;//启动泵P1.1
					Pump_1_2_IO = ON;//启动泵P1.2
				  Machine_State = ZS1_State;//一级制水状态
			 }
			if(H_Level)//高液位断开
			 {
					JS_IO = OFF;//关闭进水阀
					Pump_1_1_IO = OFF;//关闭泵P1.1
					Pump_1_2_IO = OFF;//关闭泵P1.2						
			 }
			 if(!L_Level)//保持低液位接通才开始二级制水
			 {
				 if(H_F)//HF断开
				 {
					 Pump_2_1_IO = OFF;//关闭二级泵
				 }
		   }
			 if(H_Level &&H_F)
			 Machine_State = BY_State;//备用状态
		 }
	}
	else 
	{
		if((Machine_State != BY_State)&&(Machine_State != ZS1_State)&&(Machine_State != ZS2_State)&&(Machine_State != ZS1and2_State))
		{
			Machine_State = QS_State;//系统进入缺水状态
		}
	  if(Voice_flag == 0)
		{
			VoiceOut(VOICE_WATERLESS);//语言提示	
			Voice_flag =1;
			NO_Warter_Flag =1;
		}
		if(Last_State != Machine_State)
			VoiceOut(VOICE_WATERLESS);//语言提示	
		Last_State = Machine_State;
		/*保护状态*/
		JS_IO = OFF;//关闭进水阀
		Pump_1_1_IO = OFF;//关闭泵P1.1
		Pump_1_2_IO = OFF;//关闭泵P1.2
		Pump_2_1_IO = OFF;//关闭二级泵
		Voice_flag =2;
	}
}




//C版本程序
void Machine_C_Routine(void)
{
	if(L_F == F_ON)
	{
			NO_Warter_Flag =0;
			Voice_flag =0;
			if(((Machine_State  == 0)||(Machine_State == QS_State)) && Machine_Sw)//进入冲洗
			{
				Machine_Sw =0;
				Machine_State = CX_State;//系统进入冲洗状态
				A_CX_Step1();//开始冲洗   冲洗步骤和A版本相同
			}
		 if(Machine_State == ZS_State)
		 {
			 if(Last_State != Machine_State)
			 VoiceOut(VOICE_PRODUCEWATER);//语言提示制水
			 Last_State = Machine_State;
			 if((!L_Level)&&(!H_F))//当低液位接通
			 {
				 Pump_2_1_IO = ON;//启动泵2
			 }
			 if(H_F)//当高压开关断开
			 {
				 Pump_2_1_IO = OFF;//关闭泵2
			 }
			 if(H_Level)//高液位断开
			 {
					JS_IO = OFF;//关闭进水阀
					Pump_1_1_IO = OFF;//关闭泵P1.1
					Pump_1_2_IO = OFF;//关闭泵P1.2
					Machine_State = BY_State;//机器处于备用状态
			 }
			 if(!M_Level)
			 {
				 JS_IO = ON;//打开进水阀
				 Pump_1_1_IO = ON;//打开泵P1.1
				 Pump_1_2_IO = ON;//打开泵P1.2
				 Machine_State = ZS_State;//机器处于备用状态
			 }
		 }
		 if(Machine_State == BY_State)
		 {
			 if(Last_State != Machine_State)
			 VoiceOut(VOICE_WAIT);//语言提示备水
			 Last_State = Machine_State;
			 if(!H_F)//高压开关接通
			 {
				 if(!L_Level)//低液位接通  
				 Pump_2_1_IO = ON;//启动泵2
			 }else
			 {
				 Pump_2_1_IO = OFF;//关闭泵2
			 }
			 if((!M_Level)&&(!H_Level))//中液位接通
			 {
				  JS_IO = ON;//打开进水阀
					Pump_1_1_IO = ON;//打开泵P1.1
					Pump_1_2_IO = ON;//打开泵P1.2
				  Machine_State = ZS_State;//进入制水状态
				 	VoiceOut(VOICE_PRODUCEWATER);//语言提示制水
			 }
		 }
	}else 
	{
		if((Machine_State != BY_State)&&((Machine_State != ZS_State)))
		Machine_State = QS_State;//系统进入缺水状态
		 if(Voice_flag == 0)
		{
			VoiceOut(VOICE_WATERLESS);//语言提示	
			Voice_flag =1;
			NO_Warter_Flag = 1;
		}
		
		if(Last_State != Machine_State)
			VoiceOut(VOICE_WATERLESS);//语言提示	
		
		/*保护状态*/
		JS_IO = OFF;//关闭进水阀
		Pump_1_1_IO = OFF;//关闭泵P1.1
		Pump_1_2_IO = OFF;//关闭泵P1.2
		Last_State = Machine_State;
	}
}


//版本控制器
void Sotf_Versions_Controller(void)
{
	switch(AdminPra.Soft_Version)
	{
		case 0:Machine_A_Routine();break;
		case 1:Machine_B_Routine();break;
		case 2:Machine_C_Routine();break;
		default:break;
	}
}






