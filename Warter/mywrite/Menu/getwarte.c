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
* 函 数 名: u8 GET_Warte_Judeg(void)
* 功    能: 取水判断  
* 说    明: 不可以取水返回1  否则返回0
*******************************************************************************/
u8 GET_Warte_Judeg(void)
{
	u8 ret1 =0,ret2 =0;
	
	//C版本判断
	if(AdminPra.Soft_Version == 2)
	{
		if(!L_Level)ret1 = 0; //判断是否在低液位以上
		else ret1 = 1 ;
	}
	if(Machine_State==CX_State){
		ret2 = 1; 
		VoiceOut(VOICE_WASH);//提示系统冲洗
	}//在冲洗状态禁止取水
	else ret2 =0;
	if(ret2 || ret1)
		return 1;
	else return 0;
  
}

/*******************************************************************************
* 函 数 名: void Change_Warn(void)
* 功    能: 耗材更换提示
* 说    明: 
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
//    //20140314增加RO取水失效提示
//    else if(water==RO_WATER)
//    {
//      if(User_Parameter.UP_Change_Number==10)
//      {
//        VoiceOut(VOICE_CHANGE_UP);
//      }
    }
  
}




//保存RO取水信息
void  Save_RO_GETWarte(u16  ML)
{
	u32 ALL=0;
	u16 ALL_Warte[2]={0};
	u16 temp=0;
	u16 quantify;
	quantify = GET_ADVAL.GetRO_AD();
		STMFLASH_Read(ALL_RO_Warte_Addr,ALL_Warte,2);//读出累计取水量
		/*判断数据是否合法*/
		if((ALL_Warte[0] == 0xffff)&&(ALL_Warte[1] == 0xffff))
		{ALL_Warte[0] =0;ALL_Warte[1]=0;}
		/*累加数据*/
		ALL =(ALL_Warte[0]<<16)|ALL_Warte[1];
		if(ML)temp = ML;
		else
		temp = GET_Warte_Timer*flow.RO_Flow;
		ALL += temp;
		ALL_Warte[0] = (u16)(ALL>>16);
		ALL_Warte[1] = (u16)(ALL);
		STMFLASH_Write(ALL_RO_Warte_Addr,ALL_Warte,2);//写入数据到flash 中保存		
		/*保存取水记录*/		
		if(GET_ADVAL.State1)//如果存水质检测模块
		{
		Writer_GetWater_Data(RO_Data,temp,quantify,0);
		}else 	Writer_GetWater_Data(RO_Data,temp,0xffee,0);//没有监测模块值为0xffee
}
/*******************************************************************************
* 函 数 名: void GetRO(void)
* 功    能: 取RO水
* 说    明: 
*******************************************************************************/
void GetRO(u8 now)
{
  u8 i;
	u16 GET_COUNT =0;
  u16 quantify;
	if(GET_Warte_Judeg())return ;//进行是否可以取水判断
	
	quantify = GET_ADVAL.GetRO_AD();
	
	
	/****************禁止取水**********************/
	if(quantify>AdminPra.ROthres)//超出设定门限
	{
		if(AdminPra.Material_falg)//禁止取水
		{
			VoiceOut(VOICE_CHANGE_RO);
			return ;
		}else //仅仅提示
		{	
				VoiceOut(VOICE_CHANGE_RO);			
		}
	}else VoiceOut(VOICE_GET_RO);//输出语音
	/****************禁止取水**********************/
	
	
	key_event_lesure();//退出忙碌标记
	DisplayBuf_Clr();//清除显示缓存
	Clr_Pictrue(); 
  for(i=0;i<14;i++)
  {
    Display_Buf[0][i]   =    Char_GetRO[i];      
    Display_Buf[2][i]   =    Char_GetWaterClose[i];
  }  
   
  Display_RefreshAll();  
   /*取RO水操作*/
	 GET_Warte_Timer=0;//清除流量监测
	 RO_IO = ON;//打开ROF
   while(1)
  {  
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 	
			/*计算水质*/
    if(GET_ADVAL.State1)//如果存水质检测模块
    {
      DisplayBuf_ClrRow(1);
      DisplayFloat( 1,1,quantify); //显示当前水质
      for(i=0;i<5;i++)
      {
        Display_Buf[1][i+9]   =    Char_Conductance[i];//显示单位
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
			if(GET_COUNT == 50) ///5S更新一次
			{
				quantify = GET_ADVAL.GetRO_AD();
				GET_COUNT=0;
			}
    }
		
		if(key.KEY_Falg)
				{		
						key_event_buys();//进入忙碌标记
						/*这段区域内按键无效*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														 RO_IO = OFF;//打开ROF
														 Save_RO_GETWarte(0);//保存取水信息													
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
						key_event_lesure();//退出忙碌标记
				}
		
   
  }
	/*保存取水信息*/
	/*提示是否需要更换耗材*/
}

//保存EDI取水信息  ML  0 保存计算到的数据   否则保存设置的数据
void  Save_EDI_GETWarte(u16  ML)
{
	u16 quantify;
	u32 ALL=0;
	u16 ALL_Warte[2]={0};
	u16 temp=0;
	quantify = GET_ADVAL.GetEDI_AD();
	STMFLASH_Read(ALL_EDI_Warte_Addr,ALL_Warte,2);//读出累计取水量
	/*判断数据是否合法*/
	if((ALL_Warte[0] == 0xffff)&&(ALL_Warte[1] == 0xffff))
	{ALL_Warte[0] =0;ALL_Warte[1]=0;}
	/*累加数据*/
	ALL =ALL_Warte[0]<<16|ALL_Warte[1];
	if(ML) temp = ML;
	else
	temp = GET_Warte_Timer*flow.EDI_Flow;
	ALL += temp;
	ALL_Warte[0] = ALL>>16;
	ALL_Warte[1] = (u16)ALL;
	STMFLASH_Write(ALL_EDI_Warte_Addr,ALL_Warte,2);//写入数据到flash 中保存
	if(GET_ADVAL.State2)//如果存水质检测模块
	{
	Writer_GetWater_Data(EDI_Data,temp,quantify,0);
	}else 	Writer_GetWater_Data(EDI_Data,temp,0xffee,0);//没有监测模块值为0xffee
}
/*******************************************************************************
* 函 数 名: void GetEDI(void)
* 功    能: 取EDI水
* 说    明: 
*******************************************************************************/
void GetEDI(u8 now)
{
  u8 i;
  u16 quantify;u16 GET_COUNT=0;
	if(GET_Warte_Judeg())return ;//进行是否可以取水判断
	quantify = GET_ADVAL.GetEDI_AD();
	
	/****************禁止取水**********************/
	if(quantify>AdminPra.EDIthres)//超出设定门限
	{
		if(AdminPra.Material_falg)//禁止取水
		{
			//输出语音
			return ;
		}else //仅仅提示
		{		
				//输出语音
		}
	}else ;//输出语音
	/****************禁止取水**********************/
	
	key_event_lesure();//退出忙碌标记
	DisplayBuf_Clr();//清除显示缓存
	 Clr_Pictrue(); 
  for(i=0;i<14;i++)
  {
    Display_Buf[0][i]   =    Char_GetEDI[i];      
    Display_Buf[2][i]   =    Char_GetWaterClose[i];
  }  
   
  Display_RefreshAll();  
  /*取EDI水操作*/
	 GET_Warte_Timer=0;//清除流量监测
	 EDI_IO = ON;//打开EDIF
   while(1)
  {  
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 	
			/*计算水质*/
    if(GET_ADVAL.State2)//如果存水质检测模块
    {
      DisplayBuf_ClrRow(1);
      DisplayFloat( 1,1,quantify ); //显示当前水质
      for(i=0;i<5;i++)
      {
        Display_Buf[1][i+9]   =    Char_Conductance[i];//显示单位
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
			if(GET_COUNT == 50) ///5S更新一次
			{
				quantify = GET_ADVAL.GetEDI_AD();
				GET_COUNT=0;
			}
    }
		
		if(key.KEY_Falg)
				{		
						key_event_buys();//进入忙碌标记
						/*这段区域内按键无效*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														EDI_IO = OFF;//打开EDIF														
															/*设置参数退出*/
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
						key_event_lesure();//退出忙碌标记
				}
		
   
  }
	/*保存取水信息*/
	/*提示是否需要更换耗材*/
}


//保存RO取水信息
void  Save_UP_GETWarte(u16  ML)
{
	
	u32 ALL=0;
	u16 temp=0;
	u16 ALL_Warte[2]={0};
	float quantify;
	quantify = GET_ADVAL.GetUP_AD();
	STMFLASH_Read(ALL_UP_Warte_Addr,ALL_Warte,2);//读出累计取水量
	/*判断数据是否合法*/
	if((ALL_Warte[0] == 0xffff)&&(ALL_Warte[1] == 0xffff))
	{ALL_Warte[0] =0;ALL_Warte[1]=0;}
	/*累加数据*/
	ALL =ALL_Warte[0]<<16|ALL_Warte[1];
	if(ML) temp = ML;
	else
	temp = GET_Warte_Timer*flow.UP_Flow;
	ALL += temp;
	ALL_Warte[0] = ALL>>16;
	ALL_Warte[1] = (u16)ALL;
	STMFLASH_Write(ALL_UP_Warte_Addr,ALL_Warte,2);//写入数据到flash 中保存
	if(GET_ADVAL.State)//如果存水质检测模块
	{															
	Writer_GetWater_Data(UP_Data,temp,(u16)(quantify*100),0);
	}else 	Writer_GetWater_Data(UP_Data,temp,0xffee,0);//没有监测模块值为0xffee
}

/*******************************************************************************
* 函 数 名: void GetUP(void)
* 功    能: 取UP水
* 说    明: 
*******************************************************************************/
void GetUP(u8 now)
{
	u8 flag =0;
  u8 i;
  float quantify;
	f32 toc;
	u16 GET_COUNT=0;
	float Now_Warte=0;//当前水质
	quantify = GET_ADVAL.GetUP_AD();
	if(GET_Warte_Judeg())return ;//进行是否可以取水判断
	
	quantify = GET_ADVAL.GetUP_AD();
	
	
	DisplayBuf_Clr();
  Clr_Pictrue();
		key_event_lesure();//退出忙碌标记
    for(i=0;i<14;i++)
  {
    Display_Buf[0][i]   =    Char_GetUP[i];      
    Display_Buf[3][i]   =    Char_GetWaterClose[i];
  }  
  
  Display_RefreshAll();
	//取水操作
  //VoiceOut(VOICE_GET_UP);//语音提示
	 /****循环功能部分*****/
	 if((AdminPra.XH_Flag)&&(GET_ADVAL.State))//判断是否打开循环功能
	 {
			Now_Warte = GET_ADVAL.GetUP_AD();//检测水质
		  if((Now_Warte*100) >AdminPra.XHthres)
			{
				GET_Warte_Timer=0;//清除流量监测
				UP_IO = ON;//打开UPF
				JK_IO = ON;//打开继电器
				VoiceOut(VOICE_GET_UP);//输出语音
				XH_OUT_TIMER=0;
				/*关闭循环泵和循环阀*/
				Cri_IO  = OFF;
				Pump_2_2_IO = OFF;
			}
			else {//打开循环泵和循环阀
						Cri_IO  = ON;
						Pump_2_2_IO = ON;
						XH_OUT_TIMER=0;
						flag = 1;
						}
	 }else 
	 {
		 GET_Warte_Timer=0;//清除流量监测
		 UP_IO = ON;//打开UPF
		 JK_IO = ON;//打开继电器
	 }
	 if(AdminPra.XH_Flag);//如果开启了循环功能
	 else
	 {
			 /****************禁止取水**********************/
			if((quantify*100)<AdminPra.UPthres)
			{
				if(AdminPra.Material_falg)//禁止取水
				{
					VoiceOut(0x16);
					return ;
				}else //仅仅提示
				{		
					VoiceOut(0x16);			
				}
			}else VoiceOut(VOICE_GET_UP);//输出语音
	}
	/****************禁止取水**********************/
	 
   while(1)
  { 
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 	
    /*获取水质参数*/
    if(GET_ADVAL.State)  //判断模块是否存在
    {
			Now_Warte = GET_ADVAL.GetUP_AD();//检测水质
			
      DisplayBuf_ClrRow(1);
      DisplayFloat( 1,1,quantify);
			if((AdminPra.XH_Flag)&&(flag == 1))
			{
				/********************循环功能**************************/
				if(((Now_Warte*100) > AdminPra.XHthres)&&(flag == 1))
				{
					GET_Warte_Timer=0;//清除流量监测
					UP_IO = ON;//打开UPF
					JK_IO = ON;//打开继电器
					VoiceOut(VOICE_GET_UP);//输出语音
					/*关闭循环泵和循环阀*/
					Cri_IO  = OFF;
					Pump_2_2_IO = OFF;
					flag =0;
				}
				if((XH_OUT_TIMER > AdminPra.XH_Timer)&&(flag == 1))//时间溢出
				{
					 /****************禁止取水**********************/
						if((quantify*100)<AdminPra.UPthres)
						{
							if(AdminPra.Material_falg)//禁止取水
							{
								VoiceOut(0x16);
								return ;
							}else //仅仅提示
							{		
								VoiceOut(0x16);			
							}
						}else VoiceOut(VOICE_GET_UP);//输出语音
						flag =0;
						GET_Warte_Timer=0;//清除流量监测
						UP_IO = ON;//打开UPF
						JK_IO = ON;//打开继电器
						VoiceOut(VOICE_GET_UP);//输出语音
						/*关闭循环泵和循环阀*/
						Cri_IO  = OFF;
						Pump_2_2_IO = OFF;
				}
			}
			/*********************禁止取水*************************/
			
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
			if(GET_COUNT == 10) ///5S更新一次
			{
				quantify = GET_ADVAL.GetUP_AD();
				GET_COUNT=0;
			}
    }
    if(key.KEY_Falg)
				{		
						key_event_buys();//进入忙碌标记
						/*这段区域内按键无效*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														UP_IO = OFF;//关闭UPF
														/*设置参数退出*/
														Save_UP_GETWarte(0);//保存UP取水记录															
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
						key_event_lesure();//退出忙碌标记
				}
  /*保存取水信息*/
	/*提示是否需要更换耗材*/
}
}


/*******************************************************************************
* 函 数 名: void GetRO_Quantify(void)
* 功    能: 定量取RO水
* 说    明: 
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
	if(GET_Warte_Judeg())return ;//进行是否可以取水判断
	
	quantify = GET_ADVAL.GetRO_AD();//获取RO水质参数
	key_event_lesure();//退出忙碌标记
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
		Sotf_Versions_Controller();//版本控制器生效
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
				RO_IO = OFF;//关闭ROF			
				Save_RO_GETWarte(Quantify);//保存取水信息
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
			if(GET_COUNT == 50) ///5S更新一次
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
							key_event_buys();//进入忙碌标记
							/*这段区域内按键无效*/
							switch(key.KEY_Val)
							{
								case KEY_OFF_ON:break;
								case KEY_ENTER:
															if(Certain)
																{
																	if(StartGetWater)
																	{
																		StartGetWater=FALSE;
																		RO_IO = OFF;//关闭ROF																		
																		Save_RO_GETWarte(0);//保存取水信息																
																	}
																	else
																	{
																		  /****************禁止取水**********************/
																			if(quantify>AdminPra.ROthres)//超出设定门限
																			{
																				if(AdminPra.Material_falg)//禁止取水
																				{
																					VoiceOut(VOICE_CHANGE_RO);
																					OppositeColor(1,now,10);		
																					return ;
																				}else //仅仅提示
																				{	
																						VoiceOut(VOICE_CHANGE_RO);			
																				}
																			}else VoiceOut(VOICE_GET_RO);//输出语音
																			/****************禁止取水**********************/
																			
																		StartGetWater=TRUE;
																		GET_Warte_Timer=0;//清除流量监测
																		RO_IO = ON;//打开ROF
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
														/*设置参数退出*/
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
						key_event_lesure();//退出忙碌标记
				}
}
}

/*******************************************************************************
* 函 数 名: void GetEDI_Quantify(void)
* 功    能: 定量取EDI水
* 说    明: 
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
	if(GET_Warte_Judeg())return ;//进行是否可以取水判断
	quantify = GET_ADVAL.GetEDI_AD();
	key_event_lesure();//退出忙碌标记
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
		Sotf_Versions_Controller();//版本控制器生效
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
				EDI_IO = OFF;//打开EDIF
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
			if(GET_COUNT == 50) ///5S更新一次
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
							key_event_buys();//进入忙碌标记
							/*这段区域内按键无效*/
							switch(key.KEY_Val)
							{
								case KEY_OFF_ON:break;
								case KEY_ENTER:
															if(Certain)
																{
																	if(StartGetWater)
																	{
																		StartGetWater=FALSE;
																		EDI_IO = OFF;//打开EDIF
																		Save_EDI_GETWarte(0);
																		/*保存信息*/
																	}
																	else
																	{
																		/****************禁止取水**********************/
																		if(quantify>AdminPra.EDIthres)//超出设定门限
																		{
																			if(AdminPra.Material_falg)//禁止取水
																			{
																				//输出语音
																				OppositeColor(1,now,10);		
																				return ;
																			}else //仅仅提示
																			{		
																					//输出语音
																			}
																		}else ;//输出语音
																		/****************禁止取水**********************/
																		StartGetWater=TRUE;
																		GET_Warte_Timer=0;//清除流量监测
																		EDI_IO = ON;//打开EDIF
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
														/*设置参数退出*/
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
						key_event_lesure();//退出忙碌标记
				}
}
}


/*******************************************************************************
* 函 数 名: void GetUP_Quantify(void)
* 功    能: 定量取UP水
* 说    明: 
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
	float Now_Warte=0;//当前水质
	key_event_lesure();//退出忙碌标记
  Quantify   =  UserPra.RetionUP;
  Quantify1  =   Quantify/100;
  Quantify2  =   Quantify%100;
  quantify = GET_ADVAL.GetUP_AD();
  if(GET_Warte_Judeg())return ;//进行是否可以取水判断
	
  DisplayBuf_Clr();
  OppositeColor(4,0,1);
  for(i=0;i<7;i++)
    Display_Buf[0][i]=Char_GetWaterQuantify[i];
  Display_Buf[0][12]   =  'm';
  Display_Buf[0][13]   =  'L';
  
  
  
  
		while(1)
		{
#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 	
			if(GET_ADVAL.State)//判断是装有模块
			{
				Now_Warte = GET_ADVAL.GetUP_AD();//检测水质
				DisplayBuf_ClrRow(2);
				/*循环功能*/
				if((AdminPra.XH_Flag)&&(flag == 1))
				{
					if(((Now_Warte*100) > AdminPra.XHthres)&&(flag == 1))//水质达标
					{
						GET_Warte_Timer=0;//清除流量监测
						UP_IO = ON;//打开UPF
						JK_IO = ON;//打开继电器
						VoiceOut(VOICE_GET_UP);//输出语音
						/*关闭循环泵和循环阀*/
						Cri_IO  = OFF;
						Pump_2_2_IO = OFF;
						flag =2;
					}
					if((XH_OUT_TIMER > AdminPra.XH_Timer)&&(flag == 1))//时间溢出
						{
							 /****************禁止取水**********************/
								if((quantify*100)<AdminPra.UPthres)
								{
									if(AdminPra.Material_falg)//禁止取水
									{
										VoiceOut(0x16);
										return ;
									}else //仅仅提示
									{		
										VoiceOut(0x16);			
									}
								}else VoiceOut(VOICE_GET_UP);//输出语音
								flag =2;
								GET_Warte_Timer=0;//清除流量监测
								UP_IO = ON;//打开UPF
								JK_IO = ON;//打开继电器
								/*关闭循环泵和循环阀*/
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
				
				if(AdminPra.XH_Flag)//如果开启循环功能
				{
					if(((GET_Warte_Timer*flow.UP_Flow) >= Quantify) &&(flag == 2))
					{
						UP_IO = OFF;//关闭UPF
						JK_IO = OFF;//关闭继电器
						Save_UP_GETWarte(Quantify);//保存UP取水记录		
						StartGetWater =0;
						flag = 3;
					}
				}
				else 
				{
					if((GET_Warte_Timer*flow.UP_Flow) >= Quantify)
					{
						UP_IO = OFF;//关闭UPF
						JK_IO = OFF;//关闭继电器
						Save_UP_GETWarte(Quantify);//保存UP取水记录		
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
				if(GET_COUNT == 10) ///5S更新一次
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
							key_event_buys();//进入忙碌标记
							/*这段区域内按键无效*/
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
																				Cri_IO  = OFF;//关闭循环阀
																			  Pump_2_2_IO = OFF;//关闭循环泵
																				UP_IO = OFF;//关闭UPF
																				JK_IO = OFF;//关闭继电器
																			}
																			else 
																			{
																				UP_IO = OFF;//关闭UPF
																				JK_IO = OFF;//关闭继电器
																			}
																			
																			Save_UP_GETWarte(0);//保存UP取水记录			
																			StartGetWater=FALSE;																			
																		}
																		else
																		{
																																				
																				/******************循环功能部分***********************/
																			 if((AdminPra.XH_Flag)&&(GET_ADVAL.State))//判断是否打开循环功能
																				 {
																						Now_Warte = GET_ADVAL.GetUP_AD();//检测水质
																						if((Now_Warte*100) >AdminPra.XHthres)
																						{
																							GET_Warte_Timer=0;//清除流量监测
																							UP_IO = ON;//打开UPF
																							JK_IO = ON;//打开继电器
																							GET_Warte_Timer=0;//清除流量监测
																							VoiceOut(VOICE_GET_UP);//输出语音
																							/*关闭循环泵和循环阀*/
																							Cri_IO  = OFF;
																							Pump_2_2_IO = OFF;
																							flag = 2;
																						}
																						else {//打开循环泵和循环阀
																									Cri_IO  = ON;
																									Pump_2_2_IO = ON;
																									flag = 1;
																										//VoiceOut(VOICE_GET_UP);//输出语音
																										XH_OUT_TIMER=0;
																									}
																				 }
																				 else 
																				 {
																							 UP_IO = ON;//关闭UPF
																							 JK_IO = ON;//打开继电器
																							 GET_Warte_Timer=0;//清除流量监测
																							VoiceOut(VOICE_GET_UP);//输出语音
																				 }
																				 
																				 if(AdminPra.XH_Flag);
																				 else 
																				 {
																							 /****************禁止取水**********************/
																					if((quantify*100)<AdminPra.UPthres)//超出设定门限
																					{
																						if(AdminPra.Material_falg)//禁止取水
																						{
																							VoiceOut(0x16);
																							OppositeColor(1,now,10);		
																							return ;
																						}else //仅仅提示
																						{		
																							VoiceOut(0x16);			
																						}
																					}else VoiceOut(VOICE_GET_UP);//输出语音
																					/****************禁止取水**********************/		
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
															/*设置参数退出*/
															 
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
							key_event_lesure();//退出忙碌标记
					}
	/*保存取水信息*/
	/*提示是否需要更换耗材*/
	}
	
}


/*******************************************************************************
* 函 数 名:void GetWater(void)
* 功    能: 取水菜单
* 说    明: 
*******************************************************************************/
void GetWater(void)
{
  u8 i;
  u8 MenuIndex;
  u8 CursorIndex;
  u8 DisplayIndex;
  u8 CursorIndex_per;
	key_event_lesure();//退出忙碌标记
  DisplayBuf_Clr();
  Clr_Pictrue(); 
	CursorIndex_per=10;
  MenuIndex=0;     //对应菜单号0-4
  CursorIndex=0;   //对应光标号0-3
  DisplayIndex=0;  //对应显示号0-1
  for(i=0;i<15;i++)
  {
    //ComputResult ( );
  }  
  
  while(1)
  { 
		#if GET_WT_VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
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
				key_event_buys();//进入忙碌标记
				/*这段区域内按键无效*/
				switch(key.KEY_Val)
				{
					
					case KEY_OFF_ON:break;
					case KEY_ENTER:
								 switch(MenuIndex)
								{
									case 0:GetRO(CursorIndex);//取RO水
													DisplayBuf_Clr();break;
									case 1: GetUP(CursorIndex);//取UP水
													DisplayBuf_Clr();break;
									case 2:GetEDI(CursorIndex);//取EDI水									
													DisplayBuf_Clr();break;
									case 3:GetRO_Quantify(CursorIndex);//定量取RO水			
												  DisplayBuf_Clr();break;
									case 4:GetUP_Quantify(CursorIndex);//定量取UP水			
												  DisplayBuf_Clr();break;
									case 5:GetEDI_Quantify(CursorIndex);//定量取EDI水			
												  DisplayBuf_Clr();break;
									
								}
								break;
					case KEY_UP:										
									 if(CursorIndex == 0)
										{
											CursorIndex=0;
											MenuIndex--;          //当前选项
											if((DisplayIndex ==0 )&&(CursorIndex == 0))
												{
													CursorIndex = 3;  //回到最底层	
													DisplayIndex=2;	
													MenuIndex		= 5;//选择了第3条目录											
												}else DisplayIndex--; 
										}
										else 
										{
											CursorIndex--;	//光标减 对应光标号0-3	
											MenuIndex--;
										}											
										
									break;
					case Sleep_TM:Clr_Pictrue(); return ;	//睡眠返回等待界面
					case KEY_DOWM:
										CursorIndex++;	//光标加 对应光标号0-3	
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
				key_event_lesure();//退出忙碌标记
		}
  }
}






