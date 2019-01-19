#include "getwarte.h"
#include "Dis12864.h"
#include "Timer.h"
#include "keyboard.h"
#include "flashmanege.h"
#include "my_adc.h"
#include "stmflash.h"
#include "Voice.h"
#include "IO.h"
#include "APP.h"


extern const unsigned char Char_GetWater[4][10];
extern const unsigned char Char_Conductance[5] ;
extern const unsigned char Char_GetRO[14];
extern const unsigned char Char_GetWaterClose[14];
extern const unsigned char Char_SensorBreak_SC[12];
extern const unsigned char Char_Resistivity[6] ;
extern const unsigned char Char_GetUP[14];
extern const unsigned char Char_GetWaterQuantify[7];
extern const unsigned char Char_Number[16];
extern const unsigned char Char_GetROStatus[2][12];
extern const unsigned char Char_GetUPStatus[2][12];
extern const unsigned char Char_GetUPStatus[2][12]  ;
extern const unsigned char Char_GetEDI[14];
extern const unsigned char Char_GetEDIStatus[2][12];

/*******************************************************************************
* �� �� ��: u8 GET_Warte_Judeg(void)
* ��    ��: ȡˮ�ж�  
* ˵    ��: ������ȡˮ����1  ���򷵻�0
*******************************************************************************/
u8 GET_Warte_Judeg(void)
{
	u8 ret1 =0,ret2 =0;
	
	//C�汾�ж�
	if(AdminPra.Soft_Version == 2)
	{
		if(!L_Level)ret1 = 0; //�ж��Ƿ��ڵ�Һλ����
		else ret1 = 1 ;
	}
	if(Machine_State==CX_State){
		ret2 = 1; 
		VoiceOut(VOICE_WASH);//��ʾϵͳ��ϴ
	}//�ڳ�ϴ״̬��ֹȡˮ
	else ret2 =0;
	if(ret2 || ret1)
		return 1;
	else return 0;
  
}

/*******************************************************************************
* �� �� ��: void Change_Warn(void)
* ��    ��: �Ĳĸ�����ʾ
* ˵    ��: 
*******************************************************************************/
void Change_Warn(u8 water,f32 sample,u16 quantify)
{
   u16 samplebuf;
  samplebuf  = (u16) (sample*100+0.5);
  if(quantify>=300)
  {
//    if(water==UP_WATER)
//    {
//      if((samplebuf<High_Parameter.Water_Resistivity)
//         &&(User_Parameter.UP_Change_Number<10)
//           &&!Result.gbUP_Breaken)
//      {
//        User_Parameter.UP_Change_Number++;
//        Save_InformationSegB( );
//      }
//      else if((samplebuf>(High_Parameter.Water_Resistivity))&&(User_Parameter.UP_Change_Number>=5))
//      {
//        User_Parameter.UP_Change_Number=0;
//        Save_InformationSegB( );
//      }
//      if(User_Parameter.UP_Change_Number==10)
//      {
//        VoiceOut(VOICE_CHANGE_UP);
//      }
//    }
//    //20140314����ROȡˮʧЧ��ʾ
//    else if(water==RO_WATER)
//    {
//      if(User_Parameter.UP_Change_Number==10)
//      {
//        VoiceOut(VOICE_CHANGE_UP);
//      }
    }
  
}




//����ROȡˮ��Ϣ
void  Save_RO_GETWarte(u16  ML)
{
	u32 ALL=0;
	u16 ALL_Warte[2]={0};
	u16 temp=0;
	u16 quantify;
	quantify = GET_ADVAL.GetRO_AD();
		STMFLASH_Read(ALL_RO_Warte_Addr,ALL_Warte,2);//�����ۼ�ȡˮ��
		/*�ж������Ƿ�Ϸ�*/
		if((ALL_Warte[0] == 0xffff)&&(ALL_Warte[1] == 0xffff))
		{ALL_Warte[0] =0;ALL_Warte[1]=0;}
		/*�ۼ�����*/
		ALL =(ALL_Warte[0]<<16)|ALL_Warte[1];
		if(ML)temp = ML;
		else
		temp = GET_Warte_Timer*flow.RO_Flow;
		ALL += temp;
		ALL_Warte[0] = (u16)(ALL>>16);
		ALL_Warte[1] = (u16)(ALL);
		STMFLASH_Write(ALL_RO_Warte_Addr,ALL_Warte,2);//д�����ݵ�flash �б���		
		/*����ȡˮ��¼*/		
		if(GET_ADVAL.State1)//�����ˮ�ʼ��ģ��
		{
		Writer_GetWater_Data(RO_Data,temp,quantify,0);
		}else 	Writer_GetWater_Data(RO_Data,temp,0xffee,0);//û�м��ģ��ֵΪ0xffee
}
/*******************************************************************************
* �� �� ��: void GetRO(void)
* ��    ��: ȡROˮ
* ˵    ��: 
*******************************************************************************/
void GetRO(u8 now)
{
  u8 i;
	u16 GET_COUNT =0;
  u16 quantify;
	if(GET_Warte_Judeg())return ;//�����Ƿ����ȡˮ�ж�
	
	quantify = GET_ADVAL.GetRO_AD();
	
	
	/****************��ֹȡˮ**********************/
	if(quantify>AdminPra.ROthres)//�����趨����
	{
		if(AdminPra.Material_falg)//��ֹȡˮ
		{
			VoiceOut(VOICE_CHANGE_RO);
			return ;
		}else //������ʾ
		{	
				VoiceOut(VOICE_CHANGE_RO);			
		}
	}else VoiceOut(VOICE_GET_RO);//�������
	/****************��ֹȡˮ**********************/
	
	
	key_event_lesure();//�˳�æµ���
	DisplayBuf_Clr();//�����ʾ����
	Clr_Pictrue(); 
  for(i=0;i<14;i++)
  {
    Display_Buf[0][i]   =    Char_GetRO[i];      
    Display_Buf[2][i]   =    Char_GetWaterClose[i];
  }  
   
  Display_RefreshAll();  
   /*ȡROˮ����*/
	 GET_Warte_Timer=0;//����������
	 RO_IO = ON;//��ROF
   while(1)
  {  
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
			/*����ˮ��*/
    if(GET_ADVAL.State1)//�����ˮ�ʼ��ģ��
    {
      DisplayBuf_ClrRow(1);
      DisplayFloat( 1,1,quantify); //��ʾ��ǰˮ��
      for(i=0;i<5;i++)
      {
        Display_Buf[1][i+9]   =    Char_Conductance[i];//��ʾ��λ
      } 
    }
    else
    {
      DisplayBuf_ClrRow(1);
      for(i=0;i<12;i++)
      Display_Buf[1][i+2]   = Char_SensorBreak_SC[i];
    }
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
			GET_COUNT++;
			if(GET_COUNT == 50) ///5S����һ��
			{
				quantify = GET_ADVAL.GetRO_AD();
				GET_COUNT=0;
			}
    }
		
		if(key.KEY_Falg)
				{		
						key_event_buys();//����æµ���
						/*��������ڰ�����Ч*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														 RO_IO = OFF;//��ROF
														 Save_RO_GETWarte(0);//����ȡˮ��Ϣ													
														if(now == 0xff);
														else
														 OppositeColor(0,now,10);														
														 return;															
							case KEY_UP: 										
													 break;
							case KEY_DOWM:
													break;
							case KEY_BANK:break;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}
		
   
  }
	/*����ȡˮ��Ϣ*/
	/*��ʾ�Ƿ���Ҫ�����Ĳ�*/
}

//����EDIȡˮ��Ϣ  ML  0 ������㵽������   ���򱣴����õ�����
void  Save_EDI_GETWarte(u16  ML)
{
	u16 quantify;
	u32 ALL=0;
	u16 ALL_Warte[2]={0};
	u16 temp=0;
	quantify = GET_ADVAL.GetEDI_AD();
	STMFLASH_Read(ALL_EDI_Warte_Addr,ALL_Warte,2);//�����ۼ�ȡˮ��
	/*�ж������Ƿ�Ϸ�*/
	if((ALL_Warte[0] == 0xffff)&&(ALL_Warte[1] == 0xffff))
	{ALL_Warte[0] =0;ALL_Warte[1]=0;}
	/*�ۼ�����*/
	ALL =ALL_Warte[0]<<16|ALL_Warte[1];
	if(ML) temp = ML;
	else
	temp = GET_Warte_Timer*flow.EDI_Flow;
	ALL += temp;
	ALL_Warte[0] = ALL>>16;
	ALL_Warte[1] = (u16)ALL;
	STMFLASH_Write(ALL_EDI_Warte_Addr,ALL_Warte,2);//д�����ݵ�flash �б���
	if(GET_ADVAL.State2)//�����ˮ�ʼ��ģ��
	{
	Writer_GetWater_Data(EDI_Data,temp,quantify,0);
	}else 	Writer_GetWater_Data(EDI_Data,temp,0xffee,0);//û�м��ģ��ֵΪ0xffee
}
/*******************************************************************************
* �� �� ��: void GetEDI(void)
* ��    ��: ȡEDIˮ
* ˵    ��: 
*******************************************************************************/
void GetEDI(u8 now)
{
  u8 i;
  u16 quantify;u16 GET_COUNT=0;
	if(GET_Warte_Judeg())return ;//�����Ƿ����ȡˮ�ж�
	quantify = GET_ADVAL.GetEDI_AD();
	
	/****************��ֹȡˮ**********************/
	if(quantify>AdminPra.EDIthres)//�����趨����
	{
		if(AdminPra.Material_falg)//��ֹȡˮ
		{
			//�������
			return ;
		}else //������ʾ
		{		
				//�������
		}
	}else ;//�������
	/****************��ֹȡˮ**********************/
	
	key_event_lesure();//�˳�æµ���
	DisplayBuf_Clr();//�����ʾ����
	 Clr_Pictrue(); 
  for(i=0;i<14;i++)
  {
    Display_Buf[0][i]   =    Char_GetEDI[i];      
    Display_Buf[2][i]   =    Char_GetWaterClose[i];
  }  
   
  Display_RefreshAll();  
  /*ȡEDIˮ����*/
	 GET_Warte_Timer=0;//����������
	 EDI_IO = ON;//��EDIF
   while(1)
  {  
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
			/*����ˮ��*/
    if(GET_ADVAL.State2)//�����ˮ�ʼ��ģ��
    {
      DisplayBuf_ClrRow(1);
      DisplayFloat( 1,1,quantify ); //��ʾ��ǰˮ��
      for(i=0;i<5;i++)
      {
        Display_Buf[1][i+9]   =    Char_Conductance[i];//��ʾ��λ
      } 
    }
    else
    {
      DisplayBuf_ClrRow(1);
      for(i=0;i<12;i++)
      Display_Buf[1][i+2]   = Char_SensorBreak_SC[i];
    }
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
			GET_COUNT++;
			if(GET_COUNT == 50) ///5S����һ��
			{
				quantify = GET_ADVAL.GetEDI_AD();
				GET_COUNT=0;
			}
    }
		
		if(key.KEY_Falg)
				{		
						key_event_buys();//����æµ���
						/*��������ڰ�����Ч*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														EDI_IO = OFF;//��EDIF														
															/*���ò����˳�*/
														Save_EDI_GETWarte(0);
														if(now == 0xff);
														else
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 										
													 break;
							case KEY_DOWM:
													break;
							case KEY_BANK: break;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}
		
   
  }
	/*����ȡˮ��Ϣ*/
	/*��ʾ�Ƿ���Ҫ�����Ĳ�*/
}


//����ROȡˮ��Ϣ
void  Save_UP_GETWarte(u16  ML)
{
	
	u32 ALL=0;
	u16 temp=0;
	u16 ALL_Warte[2]={0};
	float quantify;
	quantify = GET_ADVAL.GetUP_AD();
	STMFLASH_Read(ALL_UP_Warte_Addr,ALL_Warte,2);//�����ۼ�ȡˮ��
	/*�ж������Ƿ�Ϸ�*/
	if((ALL_Warte[0] == 0xffff)&&(ALL_Warte[1] == 0xffff))
	{ALL_Warte[0] =0;ALL_Warte[1]=0;}
	/*�ۼ�����*/
	ALL =ALL_Warte[0]<<16|ALL_Warte[1];
	if(ML) temp = ML;
	else
	temp = GET_Warte_Timer*flow.UP_Flow;
	ALL += temp;
	ALL_Warte[0] = ALL>>16;
	ALL_Warte[1] = (u16)ALL;
	STMFLASH_Write(ALL_UP_Warte_Addr,ALL_Warte,2);//д�����ݵ�flash �б���
	if(GET_ADVAL.State)//�����ˮ�ʼ��ģ��
	{															
	Writer_GetWater_Data(UP_Data,temp,(u16)(quantify*100),0);
	}else 	Writer_GetWater_Data(UP_Data,temp,0xffee,0);//û�м��ģ��ֵΪ0xffee
}

/*******************************************************************************
* �� �� ��: void GetUP(void)
* ��    ��: ȡUPˮ
* ˵    ��: 
*******************************************************************************/
void GetUP(u8 now)
{
	u8 flag =0;
  u8 i;
  float quantify;
	f32 toc;
	u16 GET_COUNT=0;
	float Now_Warte=0;//��ǰˮ��
	quantify = GET_ADVAL.GetUP_AD();
	if(GET_Warte_Judeg())return ;//�����Ƿ����ȡˮ�ж�
	
	quantify = GET_ADVAL.GetUP_AD();
	
	
	DisplayBuf_Clr();
  Clr_Pictrue();
		key_event_lesure();//�˳�æµ���
    for(i=0;i<14;i++)
  {
    Display_Buf[0][i]   =    Char_GetUP[i];      
    Display_Buf[3][i]   =    Char_GetWaterClose[i];
  }  
  
  Display_RefreshAll();
	//ȡˮ����
  //VoiceOut(VOICE_GET_UP);//������ʾ
	 /****ѭ�����ܲ���*****/
	 if((AdminPra.XH_Flag)&&(GET_ADVAL.State))//�ж��Ƿ��ѭ������
	 {
			Now_Warte = GET_ADVAL.GetUP_AD();//���ˮ��
		  if((Now_Warte*100) >AdminPra.XHthres)
			{
				GET_Warte_Timer=0;//����������
				UP_IO = ON;//��UPF
				JK_IO = ON;//�򿪼̵���
				VoiceOut(VOICE_GET_UP);//�������
				XH_OUT_TIMER=0;
				/*�ر�ѭ���ú�ѭ����*/
				Cri_IO  = OFF;
				Pump_2_2_IO = OFF;
			}
			else {//��ѭ���ú�ѭ����
						Cri_IO  = ON;
						Pump_2_2_IO = ON;
						XH_OUT_TIMER=0;
						flag = 1;
						}
	 }else 
	 {
		 GET_Warte_Timer=0;//����������
		 UP_IO = ON;//��UPF
		 JK_IO = ON;//�򿪼̵���
	 }
	 if(AdminPra.XH_Flag);//���������ѭ������
	 else
	 {
			 /****************��ֹȡˮ**********************/
			if((quantify*100)<AdminPra.UPthres)
			{
				if(AdminPra.Material_falg)//��ֹȡˮ
				{
					VoiceOut(0x16);
					return ;
				}else //������ʾ
				{		
					VoiceOut(0x16);			
				}
			}else VoiceOut(VOICE_GET_UP);//�������
	}
	/****************��ֹȡˮ**********************/
	 
   while(1)
  { 
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
    /*��ȡˮ�ʲ���*/
    if(GET_ADVAL.State)  //�ж�ģ���Ƿ����
    {
			Now_Warte = GET_ADVAL.GetUP_AD();//���ˮ��
			
      DisplayBuf_ClrRow(1);
      DisplayFloat( 1,1,quantify);
			if((AdminPra.XH_Flag)&&(flag == 1))
			{
				/********************ѭ������**************************/
				if(((Now_Warte*100) > AdminPra.XHthres)&&(flag == 1))
				{
					GET_Warte_Timer=0;//����������
					UP_IO = ON;//��UPF
					JK_IO = ON;//�򿪼̵���
					VoiceOut(VOICE_GET_UP);//�������
					/*�ر�ѭ���ú�ѭ����*/
					Cri_IO  = OFF;
					Pump_2_2_IO = OFF;
					flag =0;
				}
				if((XH_OUT_TIMER > AdminPra.XH_Timer)&&(flag == 1))//ʱ�����
				{
					 /****************��ֹȡˮ**********************/
						if((quantify*100)<AdminPra.UPthres)
						{
							if(AdminPra.Material_falg)//��ֹȡˮ
							{
								VoiceOut(0x16);
								return ;
							}else //������ʾ
							{		
								VoiceOut(0x16);			
							}
						}else VoiceOut(VOICE_GET_UP);//�������
						flag =0;
						GET_Warte_Timer=0;//����������
						UP_IO = ON;//��UPF
						JK_IO = ON;//�򿪼̵���
						VoiceOut(VOICE_GET_UP);//�������
						/*�ر�ѭ���ú�ѭ����*/
						Cri_IO  = OFF;
						Pump_2_2_IO = OFF;
				}
			}
			/*********************��ֹȡˮ*************************/
			
      for(i=0;i<6;i++)
      {
        Display_Buf[1][i+9]   =    Char_Resistivity[i]; 
      }
      toc = (-4.0)*quantify+74.5;
      if(toc<=30.0)
        DisplayFloat( 3,2,toc );
      else
      {
        Display_Buf[2][4]=' ';
        Display_Buf[2][5]='-';
        Display_Buf[2][6]='-';
        Display_Buf[2][7]='.';
        Display_Buf[2][8]='-';
        Display_Buf[2][9]='-';
        Display_Buf[2][10]=' ';
      }
      Display_Buf[2][0]='T';
      Display_Buf[2][1]='O';
      Display_Buf[2][2]='C';
      Display_Buf[2][3]=':';
      Display_Buf[2][11]='p';
      Display_Buf[2][12]='p';
      Display_Buf[2][13]='b';
    }
    else
    {
      DisplayBuf_ClrRow(1);
      DisplayBuf_ClrRow(2);
      for(i=0;i<12;i++)
      Display_Buf[1][i+2]   = Char_SensorBreak_SC[i];
    }
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
			GET_COUNT++;
			if(GET_COUNT == 10) ///5S����һ��
			{
				quantify = GET_ADVAL.GetUP_AD();
				GET_COUNT=0;
			}
    }
    if(key.KEY_Falg)
				{		
						key_event_buys();//����æµ���
						/*��������ڰ�����Ч*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														UP_IO = OFF;//�ر�UPF
														/*���ò����˳�*/
														Save_UP_GETWarte(0);//����UPȡˮ��¼															
														if(now == 0xff);
														else
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 										
													 break;
							case KEY_DOWM:
													break;
							case KEY_BANK: 
										break;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}
  /*����ȡˮ��Ϣ*/
	/*��ʾ�Ƿ���Ҫ�����Ĳ�*/
}
}


/*******************************************************************************
* �� �� ��: void GetRO_Quantify(void)
* ��    ��: ����ȡROˮ
* ˵    ��: 
*******************************************************************************/
void GetRO_Quantify(u8 now)
{
  u8   i,rl=0,rl1=1;
  u8   Quantify1=0,Quantify2=0;
  u16  Quantify=0;
  u16  quantify=0;	
	u8 StartGetWater  =   FALSE;
  u8 Certain        =   FALSE;
	u8 GET_COUNT = 0;
	if(GET_Warte_Judeg())return ;//�����Ƿ����ȡˮ�ж�
	
	quantify = GET_ADVAL.GetRO_AD();//��ȡROˮ�ʲ���
	key_event_lesure();//�˳�æµ���
  Quantify   =   UserPra.RetionRO;
  Quantify1  =   Quantify/100;
  Quantify2  =   Quantify%100;
 
  DisplayBuf_Clr();
  OppositeColor(3,1,1);
  for(i=0;i<7;i++)
    Display_Buf[0][i]=Char_GetWaterQuantify[i];
  Display_Buf[1][10]   =  'm';
  Display_Buf[1][11]   =  'L';
  
  
  
  while(1)
  {
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
    if(GET_ADVAL.State1)
    {
      DisplayBuf_ClrRow(3);
      DisplayFloat( 1,3,quantify );
      for(i=0;i<5;i++)
      {
        Display_Buf[3][i+9]   =    Char_Conductance[i]; 
      }
    }
    else
    {
      DisplayBuf_ClrRow(3);
      for(i=0;i<12;i++)
        Display_Buf[3][i+2]   = Char_SensorBreak_SC[i];
    }
    Display_Buf[1][6]= Char_Number[(u8)(Quantify1/10)];
    Display_Buf[1][7]= Char_Number[(u8)(Quantify1%10)];
    Display_Buf[1][8]= Char_Number[(u8)(Quantify2/10)];
    Display_Buf[1][9]= Char_Number[(u8)(Quantify2%10)];
    if(StartGetWater)
    {
			if((GET_Warte_Timer*flow.RO_Flow) >=Quantify)
			{
				StartGetWater = FALSE;
				RO_IO = OFF;//�ر�ROF			
				Save_RO_GETWarte(Quantify);//����ȡˮ��Ϣ
			}
      for(i=0;i<12;i++)
        Display_Buf[2][i]  =  Char_GetROStatus[1][i];
    }
    else
    {
      for(i=0;i<12;i++)
        Display_Buf[2][i]  =  Char_GetROStatus[0][i];
    }
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
			if(GET_COUNT == 50) ///5S����һ��
			{
				quantify = GET_ADVAL.GetRO_AD();
				GET_COUNT=0;
			}
    }
    if(Certain)
    {      
      if(rl1!=rl)
      {
        rl1=rl;
        OppositeColor(4,2,4);
        Display_RefreshAll();
      }
    }
    else
    {
      if(rl1!=rl)
      {
        rl1=rl;      
        OppositeColor(rl+3,1,2);
        Display_RefreshAll();
      }
    }
   
		
	
    if(key.KEY_Falg)
				{		
							key_event_buys();//����æµ���
							/*��������ڰ�����Ч*/
							switch(key.KEY_Val)
							{
								case KEY_OFF_ON:break;
								case KEY_ENTER:
															if(Certain)
																{
																	if(StartGetWater)
																	{
																		StartGetWater=FALSE;
																		RO_IO = OFF;//�ر�ROF																		
																		Save_RO_GETWarte(0);//����ȡˮ��Ϣ																
																	}
																	else
																	{
																		  /****************��ֹȡˮ**********************/
																			if(quantify>AdminPra.ROthres)//�����趨����
																			{
																				if(AdminPra.Material_falg)//��ֹȡˮ
																				{
																					VoiceOut(VOICE_CHANGE_RO);
																					OppositeColor(1,now,10);		
																					return ;
																				}else //������ʾ
																				{	
																						VoiceOut(VOICE_CHANGE_RO);			
																				}
																			}else VoiceOut(VOICE_GET_RO);//�������
																			/****************��ֹȡˮ**********************/
																			
																		StartGetWater=TRUE;
																		GET_Warte_Timer=0;//����������
																		RO_IO = ON;//��ROF
																	}           
																}
																else
																{
																	if(rl==0)
																	{
																		rl=1;
																	}
																	else
																	{
																		Quantify=(u16)Quantify1*100;
																		Quantify+=Quantify2;
																		//User_Parameter.RO_Quantify = Quantify;
																		Certain  =  TRUE;
																		rl1=10;
																		//Save_InformationSegB( );
																	}
																}
														/*���ò����˳�*/
														 //OppositeColor(1,now,10);								
														 break;															
							case KEY_UP: 
												if(Certain)
																{
																	if(!StartGetWater)
																	{
																		Certain = FALSE;
																		rl=0;
																	}
																}
																else
																{
																	if(rl==1)
																	{
																		if(Quantify2<99)
																			Quantify2++;
																		else
																			Quantify2=0;                    
																	}
																	else
																	{
																		if(Quantify1<99)
																			Quantify1++;
																		else
																			Quantify1=0;
																	}
																}								
													 break;
							case KEY_DOWM:
															if(Certain)
															{
																if(!StartGetWater)
																{
																	Certain = FALSE;
																	rl=0;
																}
															}
															else
															{
																if(rl==1)
																{
																	if(Quantify2>0)
																		Quantify2--;
																	else
																		Quantify2=99;                    
																}
																else
																{
																	if(Quantify1>0)
																		Quantify1--;
																	else
																		Quantify1=99;
																}
															}
													break;
							case KEY_BANK:
														UserPra.RetionRO = (Quantify1*100)+Quantify2;
														Seve_Userdata();
														OppositeColor(1,now,10);								
														return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}
}
}

/*******************************************************************************
* �� �� ��: void GetEDI_Quantify(void)
* ��    ��: ����ȡEDIˮ
* ˵    ��: 
*******************************************************************************/
void GetEDI_Quantify(u8 now)
{
  u8   i,rl=0,rl1=1;
  u8   Quantify1=0,Quantify2=0;
  u16  Quantify;
  u16  quantify;
	u8 StartGetWater  =   FALSE;
  u8 Certain        =   FALSE;
	u8 GET_COUNT=0;
	if(GET_Warte_Judeg())return ;//�����Ƿ����ȡˮ�ж�
	quantify = GET_ADVAL.GetEDI_AD();
	key_event_lesure();//�˳�æµ���
  Quantify   =   UserPra.RetionEDI;
  Quantify1  =   Quantify/100;
  Quantify2  =   Quantify%100;
 
  DisplayBuf_Clr();
  OppositeColor(3,1,1);
  for(i=0;i<7;i++)
    Display_Buf[0][i]=Char_GetWaterQuantify[i];
  Display_Buf[1][10]   =  'm';
  Display_Buf[1][11]   =  'L';
  
  
  
  while(1)
  {
#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
    if(GET_ADVAL.State2)
    {
      DisplayBuf_ClrRow(3);
      DisplayFloat( 1,3,quantify );
      for(i=0;i<5;i++)
      {
        Display_Buf[3][i+9]   =    Char_Conductance[i]; 
      }
    }
    else
    {
      DisplayBuf_ClrRow(3);
      for(i=0;i<12;i++)
        Display_Buf[3][i+2]   = Char_SensorBreak_SC[i];
    }
    Display_Buf[1][6]= Char_Number[(u8)(Quantify1/10)];
    Display_Buf[1][7]= Char_Number[(u8)(Quantify1%10)];
    Display_Buf[1][8]= Char_Number[(u8)(Quantify2/10)];
    Display_Buf[1][9]= Char_Number[(u8)(Quantify2%10)];
    if(StartGetWater)
    {
      for(i=0;i<12;i++)
        Display_Buf[2][i]  =  Char_GetEDIStatus[1][i];
			if((GET_Warte_Timer*flow.EDI_Flow)  >= Quantify)
			{
				EDI_IO = OFF;//��EDIF
				Save_EDI_GETWarte(Quantify);
				StartGetWater=0;
			}
    }
    else
    {
      for(i=0;i<12;i++)
        Display_Buf[2][i]  =  Char_GetEDIStatus[0][i];
    }
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
			if(GET_COUNT == 50) ///5S����һ��
			{
				quantify = GET_ADVAL.GetEDI_AD();
				GET_COUNT=0;
			}
    }
    if(Certain)
    {      
      if(rl1!=rl)
      {
        rl1=rl;
        OppositeColor(4,2,4);
        Display_RefreshAll();
      }
    }
    else
    {
      if(rl1!=rl)
      {
        rl1=rl;      
        OppositeColor(rl+3,1,2);
        Display_RefreshAll();
      }
    }
   
    if(key.KEY_Falg)
				{		
							key_event_buys();//����æµ���
							/*��������ڰ�����Ч*/
							switch(key.KEY_Val)
							{
								case KEY_OFF_ON:break;
								case KEY_ENTER:
															if(Certain)
																{
																	if(StartGetWater)
																	{
																		StartGetWater=FALSE;
																		EDI_IO = OFF;//��EDIF
																		Save_EDI_GETWarte(0);
																		/*������Ϣ*/
																	}
																	else
																	{
																		/****************��ֹȡˮ**********************/
																		if(quantify>AdminPra.EDIthres)//�����趨����
																		{
																			if(AdminPra.Material_falg)//��ֹȡˮ
																			{
																				//�������
																				OppositeColor(1,now,10);		
																				return ;
																			}else //������ʾ
																			{		
																					//�������
																			}
																		}else ;//�������
																		/****************��ֹȡˮ**********************/
																		StartGetWater=TRUE;
																		GET_Warte_Timer=0;//����������
																		EDI_IO = ON;//��EDIF
																	}           
																}
																else
																{
																	if(rl==0)
																	{
																		rl=1;
																	}
																	else
																	{
																		Quantify=(u16)Quantify1*100;
																		Quantify+=Quantify2;
																		//User_Parameter.RO_Quantify = Quantify;
																		Certain  =  TRUE;
																		rl1=10;
																		//Save_InformationSegB( );
																	}
																}
														/*���ò����˳�*/
														 //OppositeColor(1,now,10);								
														 break;															
							case KEY_UP: 
												if(Certain)
																{
																	if(!StartGetWater)
																	{
																		Certain = FALSE;
																		rl=0;
																	}
																}
																else
																{
																	if(rl==1)
																	{
																		if(Quantify2<99)
																			Quantify2++;
																		else
																			Quantify2=0;                    
																	}
																	else
																	{
																		if(Quantify1<99)
																			Quantify1++;
																		else
																			Quantify1=0;
																	}
																}								
													 break;
							case KEY_DOWM:
															if(Certain)
															{
																if(!StartGetWater)
																{
																	Certain = FALSE;
																	rl=0;
																}
															}
															else
															{
																if(rl==1)
																{
																	if(Quantify2>0)
																		Quantify2--;
																	else
																		Quantify2=99;                    
																}
																else
																{
																	if(Quantify1>0)
																		Quantify1--;
																	else
																		Quantify1=99;
																}
															}
													break;
							case KEY_BANK:
														UserPra.RetionEDI = (Quantify1*100)+Quantify2;
														Seve_Userdata(); 
														OppositeColor(1,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}
}
}


/*******************************************************************************
* �� �� ��: void GetUP_Quantify(void)
* ��    ��: ����ȡUPˮ
* ˵    ��: 
*******************************************************************************/
void GetUP_Quantify(u8 now)
{
  u8   i,rl=0,rl1=1;
  u8   Quantify1=0,Quantify2=0;
  u16  Quantify=0;
  float  quantify=0;
	u8   StartGetWater  =   FALSE;
	u8   Certain        =   FALSE;
	u16 GET_COUNT=0;
  f32  toc;
	u8   flag =0;
	float Now_Warte=0;//��ǰˮ��
	key_event_lesure();//�˳�æµ���
  Quantify   =  UserPra.RetionUP;
  Quantify1  =   Quantify/100;
  Quantify2  =   Quantify%100;
  quantify = GET_ADVAL.GetUP_AD();
  if(GET_Warte_Judeg())return ;//�����Ƿ����ȡˮ�ж�
	
  DisplayBuf_Clr();
  OppositeColor(4,0,1);
  for(i=0;i<7;i++)
    Display_Buf[0][i]=Char_GetWaterQuantify[i];
  Display_Buf[0][12]   =  'm';
  Display_Buf[0][13]   =  'L';
  
  
  
  
		while(1)
		{
#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
			if(GET_ADVAL.State)//�ж���װ��ģ��
			{
				Now_Warte = GET_ADVAL.GetUP_AD();//���ˮ��
				DisplayBuf_ClrRow(2);
				/*ѭ������*/
				if((AdminPra.XH_Flag)&&(flag == 1))
				{
					if(((Now_Warte*100) > AdminPra.XHthres)&&(flag == 1))//ˮ�ʴ��
					{
						GET_Warte_Timer=0;//����������
						UP_IO = ON;//��UPF
						JK_IO = ON;//�򿪼̵���
						VoiceOut(VOICE_GET_UP);//�������
						/*�ر�ѭ���ú�ѭ����*/
						Cri_IO  = OFF;
						Pump_2_2_IO = OFF;
						flag =2;
					}
					if((XH_OUT_TIMER > AdminPra.XH_Timer)&&(flag == 1))//ʱ�����
						{
							 /****************��ֹȡˮ**********************/
								if((quantify*100)<AdminPra.UPthres)
								{
									if(AdminPra.Material_falg)//��ֹȡˮ
									{
										VoiceOut(0x16);
										return ;
									}else //������ʾ
									{		
										VoiceOut(0x16);			
									}
								}else VoiceOut(VOICE_GET_UP);//�������
								flag =2;
								GET_Warte_Timer=0;//����������
								UP_IO = ON;//��UPF
								JK_IO = ON;//�򿪼̵���
								/*�ر�ѭ���ú�ѭ����*/
								Cri_IO  = OFF;
								Pump_2_2_IO = OFF;
						}
					
				}
				DisplayFloat( 1,3,quantify );
				for(i=0;i<6;i++)
				{
					Display_Buf[3][i+9]   =    Char_Resistivity[i]; 
				}
				
				toc = (-4.0)*quantify+74.5;
				if(toc<30.0)
					DisplayFloat( 3,2,toc );
				else
				{
					Display_Buf[2][4]=' ';
					Display_Buf[2][5]='-';
					Display_Buf[2][6]='-';
					Display_Buf[2][7]='.';
					Display_Buf[2][8]='-';
					Display_Buf[2][9]='-';
					Display_Buf[2][10]=' ';
				}
				Display_Buf[2][0]='T';
				Display_Buf[2][1]='O';
				Display_Buf[2][2]='C';
				Display_Buf[2][3]=':';
				Display_Buf[2][11]='p';
				Display_Buf[2][12]='p';
				Display_Buf[2][13]='b';
			}
			else
			{
				DisplayBuf_ClrRow(2);
				DisplayBuf_ClrRow(3);
				for(i=0;i<12;i++)
					Display_Buf[2][i+2]   = Char_SensorBreak_SC[i];
			}    
			Display_Buf[0][8]= Char_Number[(u8)(Quantify1/10)];
			Display_Buf[0][9]= Char_Number[(u8)(Quantify1%10)];
			Display_Buf[0][10]= Char_Number[(u8)(Quantify2/10)];
			Display_Buf[0][11]= Char_Number[(u8)(Quantify2%10)];
			if(StartGetWater)
			{
				for(i=0;i<12;i++)
					Display_Buf[1][i]  =  Char_GetUPStatus[1][i];
				
				if(AdminPra.XH_Flag)//�������ѭ������
				{
					if(((GET_Warte_Timer*flow.UP_Flow) >= Quantify) &&(flag == 2))
					{
						UP_IO = OFF;//�ر�UPF
						JK_IO = OFF;//�رռ̵���
						Save_UP_GETWarte(Quantify);//����UPȡˮ��¼		
						StartGetWater =0;
						flag = 3;
					}
				}
				else 
				{
					if((GET_Warte_Timer*flow.UP_Flow) >= Quantify)
					{
						UP_IO = OFF;//�ر�UPF
						JK_IO = OFF;//�رռ̵���
						Save_UP_GETWarte(Quantify);//����UPȡˮ��¼		
						StartGetWater =0;
					}
				}
			}
			else
			{
				for(i=0;i<12;i++)
					Display_Buf[1][i]  =  Char_GetUPStatus[0][i];
			}
			if(Display_Delay>=DELAY_DISPLAY)
			{
				GET_COUNT++;
				if(GET_COUNT == 10) ///5S����һ��
				{
					quantify = GET_ADVAL.GetUP_AD();
					GET_COUNT=0;
				}
				Display_Delay   =   0;
				Display_RefreshAll();
			}
			if(Certain)
			{      
				if(rl1!=rl)
				{
					rl1=rl;
					OppositeColor(4,1,4);
					Display_RefreshAll();
				}
			}
			else
			{
				if(rl1!=rl)
				{
					rl1=rl;      
					OppositeColor(rl+4,0,2);
					Display_RefreshAll();
				}
			}
			
		 if(key.KEY_Falg)
					{		
							key_event_buys();//����æµ���
							/*��������ڰ�����Ч*/
							switch(key.KEY_Val)
							{
								case KEY_OFF_ON:break;
								case KEY_ENTER:
																if(Certain)
																	{
																		if(StartGetWater)
																		{
																			if(AdminPra.XH_Flag)
																			{
																				flag =0;
																				Cri_IO  = OFF;//�ر�ѭ����
																			  Pump_2_2_IO = OFF;//�ر�ѭ����
																				UP_IO = OFF;//�ر�UPF
																				JK_IO = OFF;//�رռ̵���
																			}
																			else 
																			{
																				UP_IO = OFF;//�ر�UPF
																				JK_IO = OFF;//�رռ̵���
																			}
																			
																			Save_UP_GETWarte(0);//����UPȡˮ��¼			
																			StartGetWater=FALSE;																			
																		}
																		else
																		{
																																				
																				/******************ѭ�����ܲ���***********************/
																			 if((AdminPra.XH_Flag)&&(GET_ADVAL.State))//�ж��Ƿ��ѭ������
																				 {
																						Now_Warte = GET_ADVAL.GetUP_AD();//���ˮ��
																						if((Now_Warte*100) >AdminPra.XHthres)
																						{
																							GET_Warte_Timer=0;//����������
																							UP_IO = ON;//��UPF
																							JK_IO = ON;//�򿪼̵���
																							GET_Warte_Timer=0;//����������
																							VoiceOut(VOICE_GET_UP);//�������
																							/*�ر�ѭ���ú�ѭ����*/
																							Cri_IO  = OFF;
																							Pump_2_2_IO = OFF;
																							flag = 2;
																						}
																						else {//��ѭ���ú�ѭ����
																									Cri_IO  = ON;
																									Pump_2_2_IO = ON;
																									flag = 1;
																										//VoiceOut(VOICE_GET_UP);//�������
																										XH_OUT_TIMER=0;
																									}
																				 }
																				 else 
																				 {
																							 UP_IO = ON;//�ر�UPF
																							 JK_IO = ON;//�򿪼̵���
																							 GET_Warte_Timer=0;//����������
																							VoiceOut(VOICE_GET_UP);//�������
																				 }
																				 
																				 if(AdminPra.XH_Flag);
																				 else 
																				 {
																							 /****************��ֹȡˮ**********************/
																					if((quantify*100)<AdminPra.UPthres)//�����趨����
																					{
																						if(AdminPra.Material_falg)//��ֹȡˮ
																						{
																							VoiceOut(0x16);
																							OppositeColor(1,now,10);		
																							return ;
																						}else //������ʾ
																						{		
																							VoiceOut(0x16);			
																						}
																					}else VoiceOut(VOICE_GET_UP);//�������
																					/****************��ֹȡˮ**********************/		
																				 }
																			 StartGetWater=TRUE;
																		}           
																	}
																	else
																	{
																		if(rl==0)
																		{
																			rl=1;
																		}
																		else
																		{
																			Quantify=(u16)Quantify1*100;
																			Quantify+=Quantify2;
																			//User_Parameter.UP_Quantify = Quantify;
																			Certain  =  TRUE;
																			rl1=10;
																			//Save_InformationSegB( );
																		}
																	}
															/*���ò����˳�*/
															 
															 break;															
								case KEY_UP: 
												 if(Certain)
																{
																	if(!StartGetWater)
																	{
																		Certain = FALSE;
																		rl=0;
																	}
																}
																else
																{
																	if(rl==1)
																	{
																		if(Quantify2<99)
																			Quantify2++;
																		else
																			Quantify2=0;                    
																	}
																	else
																	{
																		if(Quantify1<99)
																			Quantify1++;
																		else
																			Quantify1=0;
																	}
																}									
														 break;
								case KEY_DOWM:
															if(Certain)
																{
																	if(!StartGetWater)
																	{
																		Certain = FALSE;
																		rl=0;
																	}
																}
																else
																{
																	if(rl==1)
																	{
																		if(Quantify2>0)
																			Quantify2--;
																		else
																			Quantify2=99;                    
																	}
																	else
																	{
																		if(Quantify1>0)
																			Quantify1--;
																		else
																			Quantify1=99;
																	}
																}
														break;
								case KEY_BANK:
															UserPra.RetionUP = (Quantify1*100)+Quantify2;
														  Seve_Userdata();  
															OppositeColor(1,now,10);								
											return;
								case KEY_EDI:   break;
								default:break;
							}
							key_event_lesure();//�˳�æµ���
					}
	/*����ȡˮ��Ϣ*/
	/*��ʾ�Ƿ���Ҫ�����Ĳ�*/
	}
	
}


/*******************************************************************************
* �� �� ��:void GetWater(void)
* ��    ��: ȡˮ�˵�
* ˵    ��: 
*******************************************************************************/
void GetWater(void)
{
  u8 i;
  u8 MenuIndex;
  u8 CursorIndex;
  u8 DisplayIndex;
  u8 CursorIndex_per;
	key_event_lesure();//�˳�æµ���
  DisplayBuf_Clr();
  Clr_Pictrue(); 
	CursorIndex_per=10;
  MenuIndex=0;     //��Ӧ�˵���0-4
  CursorIndex=0;   //��Ӧ����0-3
  DisplayIndex=0;  //��Ӧ��ʾ��0-1
  for(i=0;i<15;i++)
  {
    //ComputResult ( );
  }  
  
  while(1)
  { 
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
    if(Display_Delay>=125)
    {
//      ComputResult ( );
//      Semple_Delay=0;
    }
    for(i=0;i<10;i++)
    {
      Display_Buf[0][i+2]   =    Char_GetWater[DisplayIndex][i];
      Display_Buf[1][i+2]   =    Char_GetWater[DisplayIndex+1][i];
      Display_Buf[2][i+2]   =    Char_GetWater[DisplayIndex+2][i];
      Display_Buf[3][i+2]   =    Char_GetWater[DisplayIndex+3][i];
    }		
		if(CursorIndex_per!=CursorIndex)
    {
      CursorIndex_per=CursorIndex;
      OppositeColor(1,CursorIndex,10);      
    }
		if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }
    if(key.KEY_Falg)
		{
				key_event_buys();//����æµ���
				/*��������ڰ�����Ч*/
				switch(key.KEY_Val)
				{
					
					case KEY_OFF_ON:break;
					case KEY_ENTER:
								 switch(MenuIndex)
								{
									case 0:GetRO(CursorIndex);//ȡROˮ
													DisplayBuf_Clr();break;
									case 1: GetUP(CursorIndex);//ȡUPˮ
													DisplayBuf_Clr();break;
									case 2:GetEDI(CursorIndex);//ȡEDIˮ									
													DisplayBuf_Clr();break;
									case 3:GetRO_Quantify(CursorIndex);//����ȡROˮ			
												  DisplayBuf_Clr();break;
									case 4:GetUP_Quantify(CursorIndex);//����ȡUPˮ			
												  DisplayBuf_Clr();break;
									case 5:GetEDI_Quantify(CursorIndex);//����ȡEDIˮ			
												  DisplayBuf_Clr();break;
									
								}
								break;
					case KEY_UP:										
									 if(CursorIndex == 0)
										{
											CursorIndex=0;
											MenuIndex--;          //��ǰѡ��
											if((DisplayIndex ==0 )&&(CursorIndex == 0))
												{
													CursorIndex = 3;  //�ص���ײ�	
													DisplayIndex=2;	
													MenuIndex		= 5;//ѡ���˵�3��Ŀ¼											
												}else DisplayIndex--; 
										}
										else 
										{
											CursorIndex--;	//���� ��Ӧ����0-3	
											MenuIndex--;
										}											
										
									break;
					case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
					case KEY_DOWM:
										CursorIndex++;	//���� ��Ӧ����0-3	
										MenuIndex++;
										if(CursorIndex>=4)
										{
											DisplayIndex++; 
											CursorIndex = 3;
										}										
										if(DisplayIndex>2) 
										{
											DisplayIndex=0;
											CursorIndex=0;
											MenuIndex=0;
										}	break;
					case KEY_BANK:
												DisplayBuf_Clr();
												Clr_Pictrue();
												OppositeColor((0*3),3,4);
												return ;
					case KEY_EDI:break;
					default:break;
				}
				key_event_lesure();//�˳�æµ���
		}
  }
}






