#include "APP.h"
#include "IO.h"
#include "Voice.h"
#include "Timer.h"
#include "flashmanege.h"


u8 Machine_State =0;//����ָʾ����״̬
u8 Last_State  =0;
u8 Machine_Sw  =1;//����״̬  1 ����  0 �ر�
u8 Voice_flag =0;
u8 NO_Warter_Flag =0;
void A_CX_Step2(void);
void A_CX_Step3(void);

//��ϴ����1
void A_CX_Step1(void)
{
	VoiceOut(VOICE_WASH);//������ʾ
	JS_IO = ON;//�򿪽�ˮ��
	Mllt_IO = ON;//���˷���
	TmRegsiter("CX_Step2",A_CX_Step2,3000);
}

//��ϴ����2
void A_CX_Step2(void)
{
	Mllt_IO = OFF;//���˷��ر�
	Pump_1_1_IO = ON;//������P1.1
	Pump_1_2_IO = ON;//������P1.2
	CX_IO = ON;//��ϴ��ŷ���
	TmRegsiter("CX_Step2",A_CX_Step3,3000);
}

//��ϴ����3
void A_CX_Step3(void)
{
	CX_IO = OFF;//��ϴ��ŷ��ر�
	Machine_State = ZS_State;//���Ϊ��ˮ״̬
	if(Machine_State != Last_State)
	VoiceOut(VOICE_PRODUCEWATER);//������ʾ��ˮ
	Last_State = Machine_State;
	
}



//A�汾����
void Machine_A_Routine(void)
{
	//���LF�Ƿ����������ˮѹ�Ƿ�������
	if(L_F == F_ON)
	{
		NO_Warter_Flag =0;
		Voice_flag = 0;
		if(((Machine_State  == 0)||(Machine_State == QS_State)) && Machine_Sw)
		{
			Machine_Sw =0;
			Machine_State = CX_State;//ϵͳ�����ϴ״̬
			A_CX_Step1();//��ʼ��ϴ
		}
		if(Machine_State == ZS_State)
		{
			if(H_F)//������ˮ����״̬
			{
				JS_IO = OFF;//�رս�ˮ��
				Pump_1_1_IO = OFF;//�رձ�P1.1
				Pump_1_2_IO = OFF;//�رձ�P1.2
				
				Machine_State = BY_State;
				if(Machine_State != Last_State)
				VoiceOut(VOICE_WAIT);//������ʾ��ˮ
				Last_State = Machine_State;
			}else 
			{
				JS_IO = ON;//�򿪽�ˮ��
				Pump_1_1_IO = ON;//�򿪱�P1.1
				Pump_1_2_IO = ON;//�򿪱�P1.2
				 Machine_State = ZS_State;
					if(Machine_State != Last_State)
					VoiceOut(VOICE_PRODUCEWATER);//������ʾ��ˮ
					Last_State = Machine_State;
				
			}
		}
		if(Machine_State == BY_State)
		{
			if(!H_F)//�ڱ���״̬��ѹ���Ͽ�
			{
				JS_IO = ON;//�򿪽�ˮ��
				Pump_1_1_IO = ON;//�򿪱�P1.1
				Pump_1_2_IO = ON;//�򿪱�P1.2
				
				
				Machine_State = ZS_State;//����������ˮģʽ
				if(Machine_State != Last_State)
				VoiceOut(VOICE_PRODUCEWATER);//������ʾ��ˮ
				Last_State = Machine_State;
			}else 
			{
				JS_IO = OFF;//�򿪽�ˮ��
				Pump_1_1_IO = OFF;//�򿪱�P1.1
				Pump_1_2_IO = OFF;//�򿪱�P1.2	
				
				Machine_State = BY_State;//����������ˮģʽ
				if(Machine_State != Last_State)
				VoiceOut(VOICE_WAIT);//������ʾ��ˮ
				Last_State = Machine_State;
			}
		}
	}else 
	{
		if((Machine_State != BY_State)&&((Machine_State != ZS_State)))
		Machine_State = QS_State;//ϵͳ����ȱˮ״̬
		else if(Voice_flag == 0)
		{
			VoiceOut(VOICE_WATERLESS);//������ʾ	
			Voice_flag =1;
			NO_Warter_Flag =1;
		}else if(Machine_State == ZS_State)
		{
			if(Voice_flag ==1)
				VoiceOut(VOICE_PRODUCEWATER);//������ʾ��ˮ
		
		}else if(Machine_State == BY_State)
		{
			if(Voice_flag ==1)
				VoiceOut(VOICE_WAIT);//������ʾ��ˮ
		}
		
		if(Last_State != Machine_State)
			VoiceOut(VOICE_WATERLESS);//������ʾ	
		
		/*����״̬*/
		JS_IO = OFF;//�رս�ˮ��
		Pump_1_1_IO = OFF;//�رձ�P1.1
		Pump_1_2_IO = OFF;//�رձ�P1.2
		Last_State = Machine_State;
		Voice_flag =2;
	}

}



void B_CX_Step2(void)
{
	CX_IO = OFF;//��ϴ��ŷ��ر�
	Machine_State = ZS1_State;//���Ϊ1����ˮ״̬
}
void B_CX_Step1(void)
{
	VoiceOut(VOICE_WASH);//������ʾ
	JS_IO = ON;//�򿪽�ˮ��
	Pump_1_1_IO = ON;//������P1.1
	Pump_1_2_IO = ON;//������P1.2
	CX_IO = ON;//��ϴ��ŷ���
	TmRegsiter("CX_Step2",B_CX_Step2,3000);
}

//B�汾����
void Machine_B_Routine(void)
{
	if(L_F == F_ON)
	{
		NO_Warter_Flag =0;
		Voice_flag =0;
		if(((Machine_State  == 0)||(Machine_State == QS_State)) && Machine_Sw)//�����ϴ
		{
			Machine_Sw =0;
			Machine_State = CX_State;//ϵͳ�����ϴ״̬
			B_CX_Step1();//��ʼ��ϴ
		}
		 if(Machine_State == ZS1_State)
		 {
			 
			 if(!M_Level)//��Һλ��ͨ
			 {
				  JS_IO = ON;//�򿪽�ˮ��
				  Pump_1_1_IO = ON;//������P1.1
					Pump_1_2_IO = ON;//������P1.2
				  //Machine_State = ZS1_State;//һ����ˮ״̬
			 }
			 if(L_Level);//��Һλδ��ͨ
				 else 
				 {
					 if(!H_F)//��ѹ���պ�
					 {
						 //��Һλ��ͨ
						 Pump_2_1_IO = ON;//����������
						 Machine_State = ZS1and2_State;//һ����ͬʱ��ˮ״̬
					 }
				 }
				 if(H_Level)//��Һλ�Ͽ�
				 {
					  JS_IO = OFF;//�رս�ˮ��
						Pump_1_1_IO = OFF;//�رձ�P1.1
						Pump_1_2_IO = OFF;//�رձ�P1.2						
				 }
				 if(H_F)//��ѹ���Ͽ�
				 {
					 Pump_2_1_IO = OFF;//�رն�����					
				 }
				 if(H_Level &&H_F)
					 Machine_State = BY_State;//����״̬
		 }
		 if(Machine_State == ZS1and2_State)//һ����ͬʱ��ˮ״̬
		 {
			 if(Last_State != Machine_State)
			 VoiceOut(VOICE_PRODUCEWATER);//������ʾ��ˮ
			 Last_State = Machine_State;
			 if(H_F)//��ѹ����HF�Ͽ�
			 {
					Pump_2_1_IO = OFF;//�رն�����				 
			 }else 
			 {
				 Pump_2_1_IO = ON;//�رն�����				 
			 }
			 if(H_Level)
			 {
				  JS_IO = OFF;//�رս�ˮ��
					Pump_1_1_IO = OFF;//�رձ�P1.1
					Pump_1_2_IO = OFF;//�رձ�P1.2				  
			 }
			 if(!M_Level)//��Һλ��ͨ
			 {
				  JS_IO = ON;//�򿪽�ˮ��
				  Pump_1_1_IO = ON;//������P1.1
					Pump_1_2_IO = ON;//������P1.2
				  Machine_State = ZS1_State;//һ����ˮ״̬
			 }
			 if(H_Level &&H_F)
			 Machine_State = BY_State;//����״̬
		 }
		 if(Machine_State == BY_State)//�ڱ�ˮ״̬���ж�
		 {
			 if(Last_State != Machine_State)
			 VoiceOut(VOICE_WAIT);//������ʾ��ˮ
			 Last_State = Machine_State;
			 if(!H_F)//��ѹ����ͨ
			 {
				  if(!L_Level)//���ֵ�Һλ��ͨ�ſ�ʼ������ˮ
					 {
							//��Һλ��ͨ
							Pump_2_1_IO = ON;//����������
							Machine_State = ZS2_State;//������ˮ״̬
					 }
			 }else 
			 {
				 Pump_2_1_IO = OFF;//�رն�����
			 }
			 if(!M_Level)//��Һλ��ͨ
			 {
				  JS_IO = ON;//�򿪽�ˮ��
				  Pump_1_1_IO = ON;//������P1.1
					Pump_1_2_IO = ON;//������P1.2
				  Machine_State = ZS1_State;//һ����ˮ״̬
			 }
			 
		 }
		 if(Machine_State == ZS2_State)
		 {
			 if(M_Level);//��Һλû�н�ͨ
			 else 
			 {
				  JS_IO = ON;//�򿪽�ˮ��
				  Pump_1_1_IO = ON;//������P1.1
					Pump_1_2_IO = ON;//������P1.2
				  Machine_State = ZS1_State;//һ����ˮ״̬
			 }
			if(H_Level)//��Һλ�Ͽ�
			 {
					JS_IO = OFF;//�رս�ˮ��
					Pump_1_1_IO = OFF;//�رձ�P1.1
					Pump_1_2_IO = OFF;//�رձ�P1.2						
			 }
			 if(!L_Level)//���ֵ�Һλ��ͨ�ſ�ʼ������ˮ
			 {
				 if(H_F)//HF�Ͽ�
				 {
					 Pump_2_1_IO = OFF;//�رն�����
				 }
		   }
			 if(H_Level &&H_F)
			 Machine_State = BY_State;//����״̬
		 }
	}
	else 
	{
		if((Machine_State != BY_State)&&(Machine_State != ZS1_State)&&(Machine_State != ZS2_State)&&(Machine_State != ZS1and2_State))
		{
			Machine_State = QS_State;//ϵͳ����ȱˮ״̬
		}
	  if(Voice_flag == 0)
		{
			VoiceOut(VOICE_WATERLESS);//������ʾ	
			Voice_flag =1;
			NO_Warter_Flag =1;
		}
		if(Last_State != Machine_State)
			VoiceOut(VOICE_WATERLESS);//������ʾ	
		Last_State = Machine_State;
		/*����״̬*/
		JS_IO = OFF;//�رս�ˮ��
		Pump_1_1_IO = OFF;//�رձ�P1.1
		Pump_1_2_IO = OFF;//�رձ�P1.2
		Pump_2_1_IO = OFF;//�رն�����
		Voice_flag =2;
	}
}




//C�汾����
void Machine_C_Routine(void)
{
	if(L_F == F_ON)
	{
			NO_Warter_Flag =0;
			Voice_flag =0;
			if(((Machine_State  == 0)||(Machine_State == QS_State)) && Machine_Sw)//�����ϴ
			{
				Machine_Sw =0;
				Machine_State = CX_State;//ϵͳ�����ϴ״̬
				A_CX_Step1();//��ʼ��ϴ   ��ϴ�����A�汾��ͬ
			}
		 if(Machine_State == ZS_State)
		 {
			 if(Last_State != Machine_State)
			 VoiceOut(VOICE_PRODUCEWATER);//������ʾ��ˮ
			 Last_State = Machine_State;
			 if((!L_Level)&&(!H_F))//����Һλ��ͨ
			 {
				 Pump_2_1_IO = ON;//������2
			 }
			 if(H_F)//����ѹ���ضϿ�
			 {
				 Pump_2_1_IO = OFF;//�رձ�2
			 }
			 if(H_Level)//��Һλ�Ͽ�
			 {
					JS_IO = OFF;//�رս�ˮ��
					Pump_1_1_IO = OFF;//�رձ�P1.1
					Pump_1_2_IO = OFF;//�رձ�P1.2
					Machine_State = BY_State;//�������ڱ���״̬
			 }
			 if(!M_Level)
			 {
				 JS_IO = ON;//�򿪽�ˮ��
				 Pump_1_1_IO = ON;//�򿪱�P1.1
				 Pump_1_2_IO = ON;//�򿪱�P1.2
				 Machine_State = ZS_State;//�������ڱ���״̬
			 }
		 }
		 if(Machine_State == BY_State)
		 {
			 if(Last_State != Machine_State)
			 VoiceOut(VOICE_WAIT);//������ʾ��ˮ
			 Last_State = Machine_State;
			 if(!H_F)//��ѹ���ؽ�ͨ
			 {
				 if(!L_Level)//��Һλ��ͨ  
				 Pump_2_1_IO = ON;//������2
			 }else
			 {
				 Pump_2_1_IO = OFF;//�رձ�2
			 }
			 if((!M_Level)&&(!H_Level))//��Һλ��ͨ
			 {
				  JS_IO = ON;//�򿪽�ˮ��
					Pump_1_1_IO = ON;//�򿪱�P1.1
					Pump_1_2_IO = ON;//�򿪱�P1.2
				  Machine_State = ZS_State;//������ˮ״̬
				 	VoiceOut(VOICE_PRODUCEWATER);//������ʾ��ˮ
			 }
		 }
	}else 
	{
		if((Machine_State != BY_State)&&((Machine_State != ZS_State)))
		Machine_State = QS_State;//ϵͳ����ȱˮ״̬
		 if(Voice_flag == 0)
		{
			VoiceOut(VOICE_WATERLESS);//������ʾ	
			Voice_flag =1;
			NO_Warter_Flag = 1;
		}
		
		if(Last_State != Machine_State)
			VoiceOut(VOICE_WATERLESS);//������ʾ	
		
		/*����״̬*/
		JS_IO = OFF;//�رս�ˮ��
		Pump_1_1_IO = OFF;//�رձ�P1.1
		Pump_1_2_IO = OFF;//�رձ�P1.2
		Last_State = Machine_State;
	}
}


//�汾������
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






