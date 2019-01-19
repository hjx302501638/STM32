#include "useset.h"
#include "Dis12864.h"
#include "Timer.h"
#include "keyboard.h"
#include "flashmanege.h"
#include "DS1302.h"
#include "stmflash.h"
#include "APP.h"
const unsigned char Char_Year[2]={"��"};
const unsigned char Char_Month[2]={"��"};
const unsigned char Char_Day[2]={"��"};


extern const unsigned char Char_UserMenu[8][8];
extern const unsigned char Char_DeviceWashTxt1[14];
extern const unsigned char Char_DeviceWashTxt[14];
extern const unsigned char Char_Number[16];
extern const unsigned char Char_Week[7][6] ;
extern const unsigned char Char_SureChange[15];
extern const unsigned char Char_YesNo[2][2];
extern const unsigned char Char_StartPassWord[2][14];
extern const unsigned char Char_InputNewCode[13];
extern const unsigned char Char_InputCode[11];
extern const unsigned char Char_Wrong[4];
extern const unsigned char Char_History[4][12];
extern const unsigned char Char_Recordless[14];
extern const unsigned char Char_SureClearData[13];
extern const unsigned char Char_GetWater[6][10];
extern const unsigned char Char_Resistivity[6];//�����ʵ�λ
extern const unsigned char Char_Conductance[5];//�絼�ʵ�λ
extern const unsigned char Char_SensorBreak_SC[12];
extern const unsigned char Char_XUfunction[2][4];
extern u8 Ds1302DataBuf10[7];
extern u8 Ds1302DataBuf[8];

const unsigned char Char_ADD[2][8]={"�絼�ʣ�","�����ʣ�",};

//�ж����ܹ���ϴ
u8  MY_CX_TIMER_SET(void)
{
	if(CX_TIMER> CX_TIMER_SET)
	{
		CX_TIMER =0;
		return 0;
	}else return 1;
}
/*******************************************************************************
* �� �� ��: void DeviceWash(void)
* ��    ��: �ֶ���ϴ
* ˵    ��: 
*******************************************************************************/
void DeviceWash(u8 now)
{
  u8 i;
	key_event_lesure();//�˳�æµ���
	if(MY_CX_TIMER_SET())return ;
	switch(AdminPra.Soft_Version)
	{
		case 0:A_CX_Step1();break;
		case 1:B_CX_Step1();break;
		case 2:A_CX_Step1();break;
	}
	/*����*/
	DisplayBuf_Clr();
  Clr_Pictrue();
  for(i=0;i<14;i++)
  {
    Display_Buf[0][i]   =    Char_DeviceWashTxt1[i];      
    Display_Buf[2][i]   =    Char_DeviceWashTxt[i];
  }
  Display_RefreshAll();
	
			while(1)
			{
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
				if(key.KEY_Falg)
						{		
								key_event_buys();//����æµ���
								/*��������ڰ�����Ч*/
								switch(key.KEY_Val)
								{
									case KEY_OFF_ON:break;
									case KEY_ENTER:
																
																/*�����ֶ���ϴ*/
																 OppositeColor(2,now,8);								
																 return;															
									case KEY_UP: 										
															 break;
									case KEY_DOWM:
															break;
									case KEY_BANK: OppositeColor(2,now,8);								
												return;
									case KEY_EDI:   break;
									default:break;
								}
								key_event_lesure();//�˳�æµ���
						}
			}
}


/*******************************************************************************
* �� �� ��: void DS1302_TimeSet(void)
* ��    ��: DS1302����-ʱ������
* ˵    ��: 
*******************************************************************************/
void DS1302_TimeSet(u8 now)
{
  u8 pflag=0,pflag1;
	 u8 ChangeTime =0;  //������һ����������
	
  DisplayBuf_Clr();
	key_event_lesure();//�˳�æµ���
  DS1302_POWER();
 
  if(gbCellFault)//�ж�18b20�Ƿ����   �����ڸ�ֵ
  {
    Ds1302DataBuf10[0]=11;
    Ds1302DataBuf10[1]=9;
    Ds1302DataBuf10[2]=4;
    Ds1302DataBuf10[3]=0;
    Ds1302DataBuf10[4]=0;
    Ds1302DataBuf10[5]=0;
    Ds1302DataBuf10[6]=7;    
  }
  else
  {
    BurstRead1302(Ds1302DataBuf);
    DS1302_Datato10();
			/*��ȡʱ��*/
  }  
  pflag1=10;
  while(1)
  {
    #if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
    Display_Buf[1][0]  =  '2';
    Display_Buf[1][1]  =  '0';
    Display_Buf[1][2]  =  Char_Number[Ds1302DataBuf10[0]/10];
    Display_Buf[1][3]  =  Char_Number[Ds1302DataBuf10[0]%10];
    Display_Buf[1][4]  =  Char_Year[0];
    Display_Buf[1][5]  =  Char_Year[1];
    Display_Buf[1][6]  =  Char_Number[Ds1302DataBuf10[1]/10];
    Display_Buf[1][7]  =  Char_Number[Ds1302DataBuf10[1]%10];
    Display_Buf[1][8]  =  Char_Month[0];
    Display_Buf[1][9]  =  Char_Month[1];
    Display_Buf[1][10] =  Char_Number[Ds1302DataBuf10[2]/10];
    Display_Buf[1][11] =  Char_Number[Ds1302DataBuf10[2]%10];
    Display_Buf[1][12] =  Char_Day[0];
    Display_Buf[1][13] =  Char_Day[1];
    
    Display_Buf[2][0]  =  Char_Number[Ds1302DataBuf10[3]/10];
    Display_Buf[2][1]  =  Char_Number[Ds1302DataBuf10[3]%10];
    Display_Buf[2][2]  =  ':';
    Display_Buf[2][3]  =  ' ';
    Display_Buf[2][4]  =  Char_Number[Ds1302DataBuf10[4]/10];
    Display_Buf[2][5]  =  Char_Number[Ds1302DataBuf10[4]%10];
    Display_Buf[2][6]  =  ':';
    Display_Buf[2][7]  =  ' ';
    Display_Buf[2][8]  =  Char_Number[Ds1302DataBuf10[5]/10];
    Display_Buf[2][9]  =  Char_Number[Ds1302DataBuf10[5]%10];
    Display_Buf[2][10] =  Char_Week[Ds1302DataBuf10[6]-1][0];
    Display_Buf[2][11] =  Char_Week[Ds1302DataBuf10[6]-1][1];
    Display_Buf[2][12] =  Char_Week[Ds1302DataBuf10[6]-1][2];
    Display_Buf[2][13] =  Char_Week[Ds1302DataBuf10[6]-1][3];
    Display_Buf[2][14] =  Char_Week[Ds1302DataBuf10[6]-1][4];
    Display_Buf[2][15] =  Char_Week[Ds1302DataBuf10[6]-1][5];
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }
    if(pflag1!=pflag)
    {
      pflag1=pflag;      
				switch(pflag)
				{
				case 0:
					OppositeColor(0,1,4);
					break;
				case 1:
					OppositeColor(3,1,2);
					break;
				case 2:
					OppositeColor(5,1,2);
					break;
				case 3:
					OppositeColor(0,2,2);
					break;
				case 4:
					OppositeColor(2,2,2);
					break;
				case 5:
					OppositeColor(4,2,2);
					break;
				case 6:
					OppositeColor(5,2,6);
					break;
				default:
					
					break;      
				}
    }
    
			if(key.KEY_Falg)
				{		
					key_event_buys();//����æµ���	
				switch(key.KEY_Val)
				{
					case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
					case KEY_UP:
						 switch(pflag)
						{
						case 0:
							if(Ds1302DataBuf10[0]<99)
								Ds1302DataBuf10[0]++;
							else
								Ds1302DataBuf10[0]=0;
							break;
						case 1:
							if(Ds1302DataBuf10[1]<12)
								Ds1302DataBuf10[1]++;
							else
								Ds1302DataBuf10[1]=1;
							break;
						case 2:
							if(Ds1302DataBuf10[2]<31)
								Ds1302DataBuf10[2]++;
							else
								Ds1302DataBuf10[2]=1;
							 break;
						 case 3:
							if(Ds1302DataBuf10[3]<23)
								Ds1302DataBuf10[3]++;
							else
								Ds1302DataBuf10[3]=0;
							break;
						case 4:
							if(Ds1302DataBuf10[4]<59)
								Ds1302DataBuf10[4]++;
							else
								Ds1302DataBuf10[4]=0;
							break;
						case 5:
							if(Ds1302DataBuf10[5]<59)
								Ds1302DataBuf10[5]++;
							else
								Ds1302DataBuf10[5]=0;
							break;
						case 6:
							if(Ds1302DataBuf10[6]<7)
								Ds1302DataBuf10[6]++;
							else
								Ds1302DataBuf10[6]=1;
							break;
						default:
					
							break; 
						}
					break;
				 
					case KEY_DOWM:
						switch(pflag)
						{
						case 0:
							if(Ds1302DataBuf10[0]>0)
								Ds1302DataBuf10[0]--;
							else
								Ds1302DataBuf10[0]=99;
							break;
						case 1:
							if(Ds1302DataBuf10[1]>1)
								Ds1302DataBuf10[1]--;
							else
								Ds1302DataBuf10[1]=12;
							break;
						case 2:
							if(Ds1302DataBuf10[2]>1)
								Ds1302DataBuf10[2]--;
							else
								Ds1302DataBuf10[2]=31;
							 break;
						 case 3:
							if(Ds1302DataBuf10[3]>0)
								Ds1302DataBuf10[3]--;
							else
								Ds1302DataBuf10[3]=23;
							break;
						case 4:
							if(Ds1302DataBuf10[4]>0)
								Ds1302DataBuf10[4]--;
							else
								Ds1302DataBuf10[4]=59;
							break;
						case 5:
							if(Ds1302DataBuf10[5]>0)
								Ds1302DataBuf10[5]--;
							else
								Ds1302DataBuf10[5]=59;
							break;
						case 6:
							if(Ds1302DataBuf10[6]>1)
								Ds1302DataBuf10[6]--;
							else
								Ds1302DataBuf10[6]=7;
							break;
						default:
					
							break; 
						}
					break;
					case KEY_ENTER:
						 if(pflag<6) pflag++;
						 else        ChangeTime  =  1;
					break;
					case KEY_BANK: OppositeColor(2,now,8);								
							 return;     
					default :       
					break; 
				}
				key_event_lesure();//�˳�æµ���
			}
				if(ChangeTime)
				{
				 /*����ʱ��*/
					DS1302_10toData();
					BurstWrite1302(Ds1302DataBuf);
					OppositeColor(2,now,8);
					return;  
				}
  }
  
}



/*******************************************************************************
* �� �� ��: void Change_RO_UP(void)
* ��    ��: ˢ�ºĲ�
* ˵    ��: 
*******************************************************************************/
void Change_RO_UP(u8 now)
{
  u8   rl=1,rl1=0;
  u8  i;
  DisplayBuf_Clr();
	key_event_lesure();//�˳�æµ���
  for(i=0;i<15;i++)
    Display_Buf[1][i]=Char_SureChange[i];
  Display_Buf[2][4] = Char_YesNo[0][0];
  Display_Buf[2][5] = Char_YesNo[0][1];
  Display_Buf[2][10]= Char_YesNo[1][0];
  Display_Buf[2][11]= Char_YesNo[1][1];
   
  while(1)
  {  
#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 		
    if(rl1!=rl)
    {
      rl1=rl;
      OppositeColor(rl*3+2,2,2);
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
							case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														/*���ò����˳�*/
														 OppositeColor(2,now,8);								
														 return;															
							case KEY_UP: 	
													if(rl==1)  rl=0;
													else       rl=1;								
													 break;
							case KEY_DOWM:
												if(rl==1)  rl=0;
													else       rl=1;
													break;
							case KEY_BANK: OppositeColor(2,now,8);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
								if(1)
									{
										//���ºĲ�
									} 
						key_event_lesure();//�˳�æµ���
				}


		}
}
	
	



/*******************************************************************************
* �� �� ��: void UserCodeChange(void);
* ��    ��: �û������޸�
* ˵    ��: 
*******************************************************************************/
void UserCodeChange(u8 now)
{
  u8   i,rl=0,rl1=1;
  u8   password1=0,password2=0;
  u16  password = UserPra.PassWord;
	  u8 Certain = 0; //����һ����������
 // password   =   FALSE;
  password1  =   password/100;
  password2  =   password%100;
 key_event_lesure();//�˳�æµ���
  DisplayBuf_Clr();
  OppositeColor(3,1,1);
  for(i=0;i<13;i++)
    Display_Buf[0][i]=Char_InputNewCode[i];
  
  
  
  while(1)
  { 
#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 		
    Display_Buf[1][6]= Char_Number[(u8)(password1/10)];
    Display_Buf[1][7]= Char_Number[(u8)(password1%10)];
    Display_Buf[1][8]= Char_Number[(u8)(password2/10)];
    Display_Buf[1][9]= Char_Number[(u8)(password2%10)];
    if(UserPra.PassWordFlag)
    {
      for(i=0;i<14;i++)
        Display_Buf[2][i]  =  Char_StartPassWord[0][i];
    }
    else
    {
      for(i=0;i<14;i++)
        Display_Buf[2][i]  =  Char_StartPassWord[1][i];
    }
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }
    if(Certain)
    {      
      if(rl1!=rl)
      {
        rl1=rl;
        OppositeColor(5,2,4);
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
							case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
							case KEY_OFF_ON:break;
							case KEY_ENTER:
															if(Certain)
															{
																if(UserPra.PassWordFlag)
																	UserPra.PassWordFlag  =   0; 
																else
																	UserPra.PassWordFlag  =   1; 
																
																//Save_InformationSegB( );
															}
															else
															{
																if(rl==0)
																{
																	rl=1;
																}
																else
																{
																	password=(u16)password1*100;
																	password+=password2;
																	UserPra.PassWord = password;
																	Certain  =  1;
																	rl1=10;
																	//Save_InformationSegB( );
																	
																}
															}
														/*���ò����˳�*/
																			
														 break;															
							case KEY_UP: 
											if(Certain)
															{
																	Certain = 0;
																	rl=0;
															}
															else
															{
																if(rl==1)
																{
																	if(password2<99)
																		password2++;
																	else
																		password2=0;                    
																}
																else
																{
																	if(password1<99)
																		password1++;
																	else
																		password1=0;
																}
															}								
													 break;
							case KEY_DOWM:
													 if(Certain)
														{          
																Certain = 0;
																rl=0;
														}
														else
														{
															if(rl==1)
															{
																if(password2>0)
																	password2--;
																else
																	password2=99;                    
															}
															else
															{
																if(password1>0)
																	password1--;
																else
																	password1=99;
															}
														}
													break;
							case KEY_BANK: Seve_Userdata();//���������û�����
														OppositeColor(2,now,8);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}

    
  }  
}


/*******************************************************************************
* �� �� ��: void Communication_Set(void);
* ��    ��: ͨ�����ã������ʹ̶�9600��ֻ�������ӻ���ַ
* ˵    ��: 
*******************************************************************************/
void Communication_Set(u8 now)
{
  u16  Flow;
  Flow       =   25;//User_Parameter.Comm_Address;
  DisplayBuf_Clr();
  OppositeColor(3,2,3);
	key_event_lesure();//�˳�æµ���
  while(1)
  {   
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
    Display_Buf[2][6]= Char_Number[(u8)(Flow%1000/100)];    
    Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];    
    Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
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
														/*���ò����˳�*/
														 OppositeColor(2,now,8);								
														 return;															
							case KEY_UP: 
													 if(Flow<247)
														Flow++;
														else
															Flow=1;
													 break;
							case KEY_DOWM:
													 if(Flow>1)
														Flow--;
														else
															Flow=247;
													break;
							case KEY_BANK: OppositeColor(2,now,8);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}

  }
}


/*******************************************************************************
* �� �� ��: void StartTime_Set(void);
* ��    ��: ����ʱ������
* ˵    ��: 
*******************************************************************************/
void StartTime_Set(u8 now)
{
  u8   rl=0,rl1=10;
  u8   password1=0,password2=0;
  u16  password;
  DisplayBuf_Clr();
  OppositeColor(2,2,1);
	key_event_lesure();//�˳�æµ���
  password = (UserPra.OnTimer[0]<<8)|(UserPra.OnTimer[1]);//����ʱ��
  password1 = (u8)(password>>8);
  password2 = (u8)password;
  
  Display_Buf[2][6]= ' ';
  Display_Buf[2][7]= ':';
  while(1)
  {
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(password1/10)];
    Display_Buf[2][5]= Char_Number[(u8)(password1%10)];
    Display_Buf[2][8]= Char_Number[(u8)(password2/10)];
    Display_Buf[2][9]= Char_Number[(u8)(password2%10)];
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }
    if(rl1!=rl)
    {
      rl1=rl;      
      OppositeColor(rl*2+2,2,2);
      Display_RefreshAll();
    }
		if(key.KEY_Falg)
				{		
						key_event_buys();//����æµ���
						/*��������ڰ�����Ч*/
						switch(key.KEY_Val)
						{
							case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														if(rl==0)
															{
																rl=1;
															}
															else
															{
																UserPra.OnTimer[0]=password1;
																UserPra.OnTimer[1]=password2;
																if((UserPra.OnTimer[0] ==UserPra.OffTimer[0]) && (UserPra.OnTimer[1] ==UserPra.OffTimer[1]))
																	UserPra.OnOffFlag = 0;//�رն�ʱ���ػ�����
																else UserPra.OnOffFlag = 1;//�򿪶�ʱ���ػ�����
																Seve_Userdata();//���������û�����
																OppositeColor(2,now,8);								
																return;
															}								
														 break;															
							case KEY_UP: 
													 if(rl==1)
															{
																if(password2<59)
																	password2++;
																else
																	password2=0;                    
															}
															else
															{
																if(password1<23)
																	password1++;
																else
																	password1=0;
															}
															Display_RefreshAll();
													 break;
							case KEY_DOWM:
													  if(rl==1)
															{
																if(password2>0)
																	password2--;
																else
																	password2=59;                    
															}
															else
															{
																if(password1>0)
																	password1--;
																else
																	password1=23;
															}
															Display_RefreshAll();
													break;
							case KEY_BANK: OppositeColor(2,now,8);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}
  }
}



/*******************************************************************************
* �� �� ��: void StopTime_Set(void);
* ��    ��: �ػ�ʱ������
* ˵    ��: 
*******************************************************************************/
void StopTime_Set(u8  now)
{
  u8   rl=0,rl1=10;
  u8   password1=0,password2=0;
  u16  password;
  DisplayBuf_Clr();
  OppositeColor(2,2,1);
	key_event_lesure();//�˳�æµ���
  password = (UserPra.OffTimer[0]<<8)|(UserPra.OffTimer[1]);//�ػ�ʱ��
  password1 = (u8)(password>>8);
  password2 = (u8)password;
  
  Display_Buf[2][6]= ' ';
  Display_Buf[2][7]= ':';
  while(1)
  {
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(password1/10)];
    Display_Buf[2][5]= Char_Number[(u8)(password1%10)];
    Display_Buf[2][8]= Char_Number[(u8)(password2/10)];
    Display_Buf[2][9]= Char_Number[(u8)(password2%10)];
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }
    if(rl1!=rl)
    {
      rl1=rl;      
      OppositeColor(rl*2+2,2,2);
      Display_RefreshAll();
    }
		if(key.KEY_Falg)
				{		
						key_event_buys();//����æµ���
						/*��������ڰ�����Ч*/
						switch(key.KEY_Val)
						{
							case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														if(rl==0)
															{
																rl=1;
															}
															else
															{
																UserPra.OffTimer[0]=password1;
																UserPra.OffTimer[1]=password2;
																if((UserPra.OnTimer[0] ==UserPra.OffTimer[0]) && (UserPra.OnTimer[1] ==UserPra.OffTimer[1]))
																	UserPra.OnOffFlag = 0;//�رն�ʱ���ػ�����
																else UserPra.OnOffFlag = 1;//�򿪶�ʱ���ػ�����
																Seve_Userdata();//���������û�����
																OppositeColor(2,now,8);								
																	return;
															}
																				
														 break;															
							case KEY_UP: 
													 if(rl==1)
															{
																if(password2<59)
																	password2++;
																else
																	password2=0;                    
															}
															else
															{
																if(password1<23)
																	password1++;
																else
																	password1=0;
															}
															Display_RefreshAll();
													 break;
							case KEY_DOWM:
													  if(rl==1)
															{
																if(password2>0)
																	password2--;
																else
																	password2=59;                    
															}
															else
															{
																if(password1>0)
																	password1--;
																else
																	password1=23;
															}
															Display_RefreshAll();
													break;
							case KEY_BANK: OppositeColor(2,now,8);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}
  }
}


/*******************************************************************************
* �� �� ��: void UserCodeJudge(void);
* ��    ��: �û������ж�
* ˵    ��: 
*******************************************************************************/
u8  UserCodeJudge(void)
{
  u8   i,rl=0,rl1=1;
  u8   password1=0,password2=0;
  u16  password;
  key_event_lesure();//�˳�æµ���		
  DisplayBuf_Clr();
  OppositeColor(3,2,1);
  for(i=0;i<11;i++)
    Display_Buf[1][i]=Char_InputCode[i];

  
  while(1)
  {
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
    Display_Buf[2][6]= Char_Number[(u8)(password1/10)];
    Display_Buf[2][7]= Char_Number[(u8)(password1%10)];
    Display_Buf[2][8]= Char_Number[(u8)(password2/10)];
    Display_Buf[2][9]= Char_Number[(u8)(password2%10)];
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }
    if(rl1!=rl)
    {
      rl1=rl;      
      OppositeColor(rl+3,2,2);
      Display_RefreshAll();
    }
		 if(key.KEY_Falg)
		{
			key_event_buys();//����æµ���
					switch(key.KEY_Val)
					{
							
						case KEY_OFF_ON:break;
						case KEY_UP:
							if(rl==1)
							{
								if(password2<99)
									password2++;
								else
									password2=0;                    
							}
							else
							{
								if(password1<99)
									password1++;
								else
									password1=0;
							}
							Display_RefreshAll();
							break;
						case KEY_DOWM:
							
							if(rl==1)
							{
								if(password2>0)
									password2--;
								else
									password2=99;                    
							}
							else
							{
								if(password1>0)
									password1--;
								else
									password1=99;
							}
							Display_RefreshAll();
							break;
						
						case KEY_ENTER:
							if(rl==0)
							{
								rl=1;
							}
							else
							{
								password=(u16)password1*100;
								password+=password2;
								if((password==UserPra.PassWord)||
									 (password==1001))
									return 0;
								else
								{
									Display_Buf[2][12]=Char_Wrong[0];
									Display_Buf[2][13]=Char_Wrong[1];
									Display_Buf[2][14]=Char_Wrong[2];
									Display_Buf[2][15]=Char_Wrong[3];
									rl=0;
								}
							}
						break;
						case KEY_BANK:return 1;
							default:
							break;
					}
						key_event_lesure();//�˳�æµ���		
				}
  }
}


/*******************************************************************************
* �� �� ��: void Dis_HistoryRO_Add(void)
* ��    ��: ��ʾ�豸RO�ۼ�ȡˮ��
* ˵    ��: 
*******************************************************************************/
void Dis_HistoryRO_Add(u8 now)
{
   u8 i;
   u32 ALL;
   DisplayBuf_Clr();
   Clr_Pictrue();
	 //	STMFLASH_Read(ALL_RO_Warte_Addr,temp,2);
   ALL =  (STMFLASH_ReadHalfWord(ALL_RO_Warte_Addr)<<16)|STMFLASH_ReadHalfWord(ALL_RO_Warte_Addr+2);
   key_event_lesure();//�˳�æµ���
  for(i=0;i<12;i++)
  {
    Display_Buf[0][i]   =    Char_History[0][i];
  }
  Display_Buf[0][12]    =    ':';
  
  Display_Buf[2][14]    =    'L';
  
    
    if(ALL >= 1000000000)
      Display_Buf[2][2]=Char_Number[ALL % 10000000000/1000000000 ]; 
    else
      Display_Buf[2][2]=' ';
    if(ALL>=100000000)
      Display_Buf[2][3]=Char_Number[ALL % 1000000000 / 100000000 ];
    else
      Display_Buf[2][3]=' ';
    if(ALL>=10000000)
      Display_Buf[2][4]=Char_Number[ALL % 100000000 / 10000000 ];
    else
      Display_Buf[2][4]=' ';
    if(ALL>=1000000)
      Display_Buf[2][5]=Char_Number[ALL % 10000000 / 1000000 ];
    else
      Display_Buf[2][5]=' ';
    if(ALL>=100000)
      Display_Buf[2][6]=Char_Number[ALL % 1000000 / 100000 ];
    else
      Display_Buf[2][6]=' ';
    if(ALL>=10000)
      Display_Buf[2][7]=Char_Number[ALL % 100000 / 10000 ];
    else
      Display_Buf[2][7]=' ';
    
    Display_Buf[2][8]=Char_Number[ALL % 10000 / 1000 ];    
    
    Display_Buf[2][9]='.';  
    
    Display_Buf[2][10]=Char_Number[ALL % 1000 / 100 ];    
    Display_Buf[2][11]=Char_Number[ALL % 100/10 ];
    Display_Buf[2][12]=Char_Number[ALL % 10 ];
  Display_RefreshAll();
  while(1)
  {
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
		if(key.KEY_Falg)
			{		
					key_event_buys();//����æµ���
					/*��������ڰ�����Ч*/
					switch(key.KEY_Val)
					{
						case KEY_OFF_ON:break;
						case KEY_ENTER:
													/*���ò����˳�*/
													 OppositeColor(1,now,12);								
													 return;															
						case KEY_UP: 										
												 break;
						case KEY_DOWM:
												break;
						case KEY_BANK: OppositeColor(1,now,12);								
									return;
						case KEY_EDI:   break;
						default:break;
					}
					key_event_lesure();//�˳�æµ���
			}

  }
}


/*******************************************************************************
* �� �� ��: void Dis_HistoryUP_Add(void)
* ��    ��: ��ʾ�豸UP�ۼ�ȡˮ��
* ˵    ��: 
*******************************************************************************/
void Dis_HistoryUP_Add(u8 now)
{
   u8 i;
   u32 temp;
   DisplayBuf_Clr();
   Clr_Pictrue();
   temp =(STMFLASH_ReadHalfWord(ALL_UP_Warte_Addr)<<16)|STMFLASH_ReadHalfWord(ALL_UP_Warte_Addr+2);
   key_event_lesure();//�˳�æµ���
  for(i=0;i<12;i++)
  {
    Display_Buf[0][i]   =    Char_History[1][i];
  }
  Display_Buf[0][12]    =    ':';
  
  Display_Buf[2][14]    =    'L';
    
  
   
    if(temp >= 1000000000)
      Display_Buf[2][2]=Char_Number[temp % 10000000000/1000000000 ]; 
    else
      Display_Buf[2][2]=' ';
    if(temp>=100000000)
      Display_Buf[2][3]=Char_Number[temp % 1000000000 / 100000000 ];
    else
      Display_Buf[2][3]=' ';
    if(temp>=10000000)
      Display_Buf[2][4]=Char_Number[temp % 100000000 / 10000000 ];
    else
      Display_Buf[2][4]=' ';
    if(temp>=1000000)
      Display_Buf[2][5]=Char_Number[temp % 10000000 / 1000000 ];
    else
      Display_Buf[2][5]=' ';
    if(temp>=100000)
      Display_Buf[2][6]=Char_Number[temp % 1000000 / 100000 ];
    else
      Display_Buf[2][6]=' ';
    if(temp>=10000)
      Display_Buf[2][7]=Char_Number[temp % 100000 / 10000 ];
    else
      Display_Buf[2][7]=' ';
    
    Display_Buf[2][8]=Char_Number[temp % 10000 / 1000 ];    
    
    Display_Buf[2][9]='.';  
    
    Display_Buf[2][10]=Char_Number[temp % 1000 / 100 ];    
    Display_Buf[2][11]=Char_Number[temp % 100/10 ];
    Display_Buf[2][12]=Char_Number[temp % 10 ];
  Display_RefreshAll();
  while(1)
  {
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
    if(key.KEY_Falg)
			{		
					key_event_buys();//����æµ���
					/*��������ڰ�����Ч*/
					switch(key.KEY_Val)
					{
						case KEY_OFF_ON:break;
						case KEY_ENTER:
													/*���ò����˳�*/
													 OppositeColor(1,now,12);								
													 return;															
						case KEY_UP: 										
												 break;
						case KEY_DOWM:
												break;
						case KEY_BANK: OppositeColor(1,now,12);								
									return;
						case KEY_EDI:   break;
						default:break;
					}
					key_event_lesure();//�˳�æµ���
			}
  }
}


/*******************************************************************************
* �� �� ��: void Dis_HistoryEDI_Add(void)
* ��    ��: ��ʾ�豸EDI�ۼ�ȡˮ��
* ˵    ��: 
*******************************************************************************/
void Dis_HistoryEDI_Add(u8 now)
{
   u8 i;
   u32 temp;
   DisplayBuf_Clr();
   Clr_Pictrue();
   temp =(STMFLASH_ReadHalfWord(ALL_EDI_Warte_Addr)<<16)|STMFLASH_ReadHalfWord(ALL_EDI_Warte_Addr+2);
   key_event_lesure();//�˳�æµ���
  for(i=0;i<12;i++)
  {
    Display_Buf[0][i]   =    Char_History[2][i];
  }
  Display_Buf[0][12]    =    ':';
  
  Display_Buf[2][14]    =    'L';
    
  

    if(temp >= 1000000000)
      Display_Buf[2][2]=Char_Number[temp % 10000000000/1000000000 ]; 
    else
      Display_Buf[2][2]=' ';
    if(temp>=100000000)
      Display_Buf[2][3]=Char_Number[temp % 1000000000 / 100000000 ];
    else
      Display_Buf[2][3]=' ';
    if(temp>=10000000)
      Display_Buf[2][4]=Char_Number[temp % 100000000 / 10000000 ];
    else
      Display_Buf[2][4]=' ';
    if(temp>=1000000)
      Display_Buf[2][5]=Char_Number[temp % 10000000 / 1000000 ];
    else
      Display_Buf[2][5]=' ';
    if(temp>=100000)
      Display_Buf[2][6]=Char_Number[temp % 1000000 / 100000 ];
    else
      Display_Buf[2][6]=' ';
    if(temp>=10000)
      Display_Buf[2][7]=Char_Number[temp % 100000 / 10000 ];
    else
      Display_Buf[2][7]=' ';
    
    Display_Buf[2][8]=Char_Number[temp % 10000 / 1000 ];    
    
    Display_Buf[2][9]='.';  
    
    Display_Buf[2][10]=Char_Number[temp % 1000 / 100 ];    
    Display_Buf[2][11]=Char_Number[temp % 100/10 ];
    Display_Buf[2][12]=Char_Number[temp % 10 ];
  Display_RefreshAll();
  while(1)
  {
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
    if(key.KEY_Falg)
			{		
					key_event_buys();//����æµ���
					/*��������ڰ�����Ч*/
					switch(key.KEY_Val)
					{
						case KEY_OFF_ON:break;
						case KEY_ENTER:
													/*���ò����˳�*/
													 OppositeColor(1,now,12);								
													 return;															
						case KEY_UP: 										
												 break;
						case KEY_DOWM:
												break;
						case KEY_BANK: OppositeColor(1,now,12);								
									return;
						case KEY_EDI:   break;
						default:break;
					}
					key_event_lesure();//�˳�æµ���
			}
  }
}


/*******************************************************************************
* �� �� ��: void HistoryParameter_Dis(void)
* ��    ��: �����ʷ��¼
* ˵    ��: 
*******************************************************************************/
void ClearHistoryData_Dis(u8 now)
{
  u8   rl=0,rl1=0;
  u8  i;
  DisplayBuf_Clr();
	 Clr_Pictrue();
	key_event_lesure();//�˳�æµ���
  for(i=0;i<13;i++)
    Display_Buf[1][i]=Char_SureClearData[i];
  Display_Buf[2][4] = Char_YesNo[1][0];
  Display_Buf[2][5] = Char_YesNo[1][1];
  Display_Buf[2][10]= Char_YesNo[0][0];
  Display_Buf[2][11]= Char_YesNo[0][1];
  OppositeColor(rl*3+2,2,2);
  while(1)
  {  
#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 		
    if(rl1!=rl)
    {
      rl1=rl;
      OppositeColor(rl*3+2,2,2);
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
													/*���ò����˳�*/
													if(rl == 1)
														Clean_GetWater_Data();//�����¼													
													 OppositeColor(1,3,12);								
													 return;															
						case KEY_UP: 	
												if(rl==1)  rl=0;
												 else       rl=1;
												 break;
						case KEY_DOWM:
												 if(rl==1)  rl=0;
												 else       rl=1;
												break;
						case KEY_BANK: OppositeColor(1,3,12);								
									return;
						case KEY_EDI:   break;
						default:break;
					}
					key_event_lesure();//�˳�æµ���
			}
	}
   
}


extern void Timer_DisChar_EX( u8 *OUT,u16 *IN );

static void Dis_Number(u16 data)
{
	if(data >10000)
	Display_Buf[1][7]=Char_Number[data /10000];
	else Display_Buf[1][7] = 0x20;
	if(data > 1000)
	Display_Buf[1][8]=Char_Number[data/1000%10];
	else  Display_Buf[1][8] = 0x20;
	if(data > 100)
	Display_Buf[1][9]=Char_Number[data /100 %10 ]; 
	else  Display_Buf[1][9] = 0x20;
	if(data > 10)		
	Display_Buf[1][10]=Char_Number[data/10%10 ];
	else  Display_Buf[1][10] = 0x20;
	Display_Buf[1][11]=Char_Number[data % 10 ];
}
/*******************************************************************************
* �� �� ��: void Dis_History_Check(void);
* ��    ��: ȡˮ������¼��ѯ
* ˵    ��: 
*******************************************************************************/
void Dis_History_Check(u8 now)
{
	u16 temp_buff[12] ={0};
  u16 disindex=0,NowCount,NowCount1;
	u8  data2dispy[16]={0};
	u8 flag=0;
  u8 i;
  DisplayBuf_Clr();
  Clr_Pictrue();
	key_event_lesure();//�˳�æµ���
	/*�ж��ڴ�����Ƿ�������*/
	if((STMFLASH_ReadHalfWord(Data_Count_Addr) == 0)&&(STMFLASH_ReadHalfWord(Data_Count_flag_Addr) == 0))
	{
		flag = 0;
		for(i=0;i<14;i++)
		Display_Buf[1][i] = Char_Recordless[i];
	}else flag = 1;
  while(1)
  {
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
		if(Display_Delay>=DELAY_DISPLAY)
    {
			if(flag ==1)
			{
				//ȡ��flash������				
				NowCount1=STMFLASH_ReadHalfWord(Data_Count_Addr)-1;
				NowCount =NowCount1 - disindex;
				STMFLASH_Read(DataAddr+(NowCount*24),temp_buff,12);
				//��ʾʱ��
				Timer_DisChar_EX(data2dispy,&temp_buff[2]);
				for(i=0;i<16;i++)
				Display_Buf[0][i] = data2dispy[i];
				/*��ʾȡˮ��Ϣ*/
					if(temp_buff[0] == UP_Data) //��ʾ UPˮ��Ϣ
					{
						
						for(i=0;i<6;i++)					
						Display_Buf[1][i] = Char_GetWater[1][i];//��ʾȡˮ����
						Dis_Number(temp_buff[1]);//��ʾ ȡˮ��
						/*��ʾˮ�ʲ���*/
						for(i=0;i<8;i++)
						Display_Buf[2][i] = Char_ADD[1][i];  //��ʾ�������ʣ���
						if(temp_buff[10] != 0xffee)
						{
							for(i=0;i<6;i++)
							Display_Buf[3][i+9] = Char_Resistivity[i];//��ʾ��λ							
							DisplayFloat1(0,3,temp_buff[10]);//��ʾ����
						}else {for(i=0;i<12;i++)Display_Buf[3][i+4] = Char_SensorBreak_SC[i];}//��ʾΪѡ��ģ��
						Display_Buf[1][13]='M';Display_Buf[1][14]='L';
					}					
					else if(temp_buff[0] == RO_Data)//��ʾ ROˮ��Ϣ
					{
						for(i=0;i<6;i++)
						Display_Buf[1][i] = Char_GetWater[0][i];
						Dis_Number(temp_buff[1]);
						/*��ʾˮ�ʲ���*/
						for(i=0;i<8;i++)
						Display_Buf[2][i] = Char_ADD[0][i];  //��ʾ�������ʣ���
						if(temp_buff[10] != 0xffee)
						{
							for(i=0;i<5;i++)
							Display_Buf[3][i+10] = Char_Conductance[i];//��ʾ��λ
							DisplayFloat(3,3,temp_buff[10]);//��ʾ����
						}else {for(i=0;i<12;i++)Display_Buf[3][i+4] = Char_SensorBreak_SC[i];}//��ʾΪѡ��ģ��
						Display_Buf[1][13]='M';Display_Buf[1][14]='L';
					
					}
					else if(temp_buff[0] == EDI_Data)//��ʾ EDIˮ��Ϣ
					{
						for(i=0;i<6;i++)
						Display_Buf[1][i] = Char_GetWater[2][i];					
						Dis_Number(temp_buff[1]);
						/*��ʾˮ�ʲ���*/
						for(i=0;i<8;i++)
						Display_Buf[2][i] = Char_ADD[0][i];  //��ʾ�������ʣ���
						if(temp_buff[10] != 0xffee)
						{
							for(i=0;i<5;i++)
							Display_Buf[3][i+10] = Char_Conductance[i];//��ʾ��λ
							DisplayFloat(3,3,(float)temp_buff[10]);//��ʾ����
						}else {for(i=0;i<12;i++)Display_Buf[3][i+4] = Char_SensorBreak_SC[i];}//��ʾΪѡ��ģ��
						Display_Buf[1][13]='M';Display_Buf[1][14]='L';
					}
				
			}
      Display_Delay   =   0;
      Display_RefreshAll();
			if(flag ==1)
			DisplayBuf_Clr();
    }
				if(key.KEY_Falg)
			{		
					key_event_buys();//����æµ���
					/*��������ڰ�����Ч*/
					switch(key.KEY_Val)
					{
						case KEY_OFF_ON:break;
						case KEY_ENTER:
													/*���ò����˳�*/
													 OppositeColor(1,now,12);								
													 return;															
						case KEY_UP:
												if(flag == 1)
												{
													if(disindex)
													disindex--;														
												}
												break;
						case KEY_DOWM:
												if(flag == 1)
												{													
													if(disindex < NowCount1)
														disindex++;
												}
												break;
						case KEY_BANK: OppositeColor(1,now,12);								
									return;
						case KEY_EDI:   break;
						default:break;
					}
					key_event_lesure();//�˳�æµ���
			}
	}
}

/*******************************************************************************
* �� �� ��: void History_Query(void)
* ��    ��: ��ʷ��ѯ
* ˵    ��: 
*******************************************************************************/
void History_Query(u8 now)
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
  
  while(1)
  {  
#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 		
    for(i=0;i<12;i++)
    {
      Display_Buf[0][i+2]   =    Char_History[DisplayIndex][i];
      Display_Buf[1][i+2]   =    Char_History[DisplayIndex+1][i];
      Display_Buf[2][i+2]   =    Char_History[DisplayIndex+2][i];
      Display_Buf[3][i+2]   =    Char_History[DisplayIndex+3][i];
    }
    if(DisplayIndex!=0)  Display_Buf[0][15]=0x18;
    else Display_Buf[0][15]=' ';
    if(DisplayIndex!=1)  Display_Buf[3][15]=0x19;
    else Display_Buf[3][15]=' ';
    if(CursorIndex_per!=CursorIndex)
    {
      CursorIndex_per=CursorIndex;
      OppositeColor(1,CursorIndex,12);      
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
					case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
					case KEY_OFF_ON:break;
					case KEY_ENTER:
								 switch(MenuIndex)
								{
									case 0:Dis_HistoryRO_Add(CursorIndex);//�ۼ�ROȡˮ��
													DisplayBuf_Clr();break;
									case 1:Dis_HistoryUP_Add(CursorIndex);//�ۼ�UPȡˮ��
													DisplayBuf_Clr();break;
									case 2:Dis_HistoryEDI_Add(CursorIndex);//�ۼ�EDIȡˮ��										
													DisplayBuf_Clr();break;
									case 3:Dis_History_Check(CursorIndex);//ȡˮ��¼��ѯ
												  DisplayBuf_Clr();break;		
									case 4:ClearHistoryData_Dis(CursorIndex);//���ȡˮ��¼
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
													DisplayIndex=1;	
													MenuIndex		= 4;//ѡ���˵�4��Ŀ¼											
												}else DisplayIndex--; 
										}
										else 
										{
											CursorIndex--;	//���� ��Ӧ����0-3	
											MenuIndex--;
										}											
										
									break;
					case KEY_DOWM:
										CursorIndex++;	//���� ��Ӧ����0-3	
										MenuIndex++;
										if(CursorIndex>=4)
										{
											DisplayIndex++; 
											CursorIndex = 3;
										}										
										if(DisplayIndex>1) 
										{
											DisplayIndex=0;
											CursorIndex=0;
											MenuIndex=0;
										}	break;
					case KEY_BANK:	OppositeColor(2,now,8);								
													return;
					case KEY_EDI:break;
					default:break;
				}
				key_event_lesure();//�˳�æµ���
		}
   
  }
}


/*******************************************************************************
* �� �� ��: void HX_Function(u8 now)
* ��    ��: ѭ�����ܴ򿪻��߹ر�
* ˵    ��: 
*******************************************************************************/
void HX_Function(u8 now)
{
	u8 i;
  u16 force_stop;
  force_stop = AdminPra.XH_Flag;
	
	key_event_lesure();//�˳�æµ���
  Clr_Pictrue();
  DisplayBuf_Clr( );
  OppositeColor(2,2,8);
  for(i=0;i<8;i++)
  {
     Display_Buf[0][i+4]   = Char_UserMenu[8][i] ;
  }  
  Display_RefreshAll();
   while(1)
  { 
		#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
    if(force_stop)
    {
      for(i=0;i<4;i++)
      {
        Display_Buf[2][i+6]   =    Char_XUfunction[0][i]; 
      }
    }
    else
    {
      for(i=0;i<4;i++)
      {
        Display_Buf[2][i+6]   =    Char_XUfunction[1][i]; 
      }
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
							case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														/*���ò����˳�*/
															AdminPra.XH_Flag = force_stop;
															Seve_Admindata();
														 OppositeColor(2,now,8);								
														 return;															
							case KEY_UP: 										
													 break;
							case KEY_DOWM:
													if(force_stop) force_stop = 0;
													else           force_stop = 1;
													break;
							case KEY_BANK: OppositeColor(2,now,8);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//�˳�æµ���
				}
  }
}

/*******************************************************************************
* �� �� ��: void Menu_Userset(void)
* ��    ��: һ���˵�
* ˵    ��: 
*******************************************************************************/
void Menu_Userset(void)
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
  MenuIndex=0;     //��Ӧ�˵���0-7
  CursorIndex=0;   //��Ӧ����0-3
  DisplayIndex=0;  //��Ӧ��ʾ��0-4
	if(UserPra.PassWordFlag)
	{
		/*�����ж�*/
		if(UserCodeJudge())
		{
			OppositeColor((2*3),3,4);//ѡ������ѡ��
			return ;//�û������ж�
		}
		/*�����ж�*/
		DisplayBuf_Clr();//����ַ���ʾ
		key_event_lesure();//�˳�æµ���
	}
	
  while(1)
  { 
#if USESET_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 		
    for(i=0;i<8;i++)
    {
      Display_Buf[0][i+4]   =    Char_UserMenu[DisplayIndex][i];
      Display_Buf[1][i+4]   =    Char_UserMenu[DisplayIndex+1][i];
      Display_Buf[2][i+4]   =    Char_UserMenu[DisplayIndex+2][i];
      Display_Buf[3][i+4]   =    Char_UserMenu[DisplayIndex+3][i];
    }
    if(DisplayIndex!=0)  Display_Buf[0][15]=0x18;
    else Display_Buf[0][15]=' ';
    if(DisplayIndex!=4)  Display_Buf[3][15]=0x19;
    else Display_Buf[3][15]=' ';
    if(CursorIndex_per!=CursorIndex)
    {
      CursorIndex_per=CursorIndex;
      OppositeColor(2,CursorIndex,8); 
				
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
					
					case Sleep_TM:Clr_Pictrue(); return ;	//˯�߷��صȴ�����
					case KEY_OFF_ON:break;
					case KEY_ENTER:
								 switch(MenuIndex)
								{
									
									case 0:DeviceWash(CursorIndex);//�����ֶ���ϴ����
													DisplayBuf_Clr();break;
									case 1:Change_RO_UP(CursorIndex);//���ºĲ�
													DisplayBuf_Clr();break;
									case 2:History_Query(CursorIndex);//��ʷ��ѯ												
													DisplayBuf_Clr();break;
									case 3:DS1302_TimeSet(CursorIndex);//ʱ������
												  DisplayBuf_Clr();break;
									case 4:UserCodeChange(CursorIndex);//��������
													DisplayBuf_Clr();break;
									case 5:Communication_Set(CursorIndex);//�ӻ���ַ
													DisplayBuf_Clr();break;
									case 6: StartTime_Set(CursorIndex);//��ʱ����
													DisplayBuf_Clr();break;
									case 7:StopTime_Set(CursorIndex);  //��ʱ�ػ�
													DisplayBuf_Clr();break;
									
									case 8:HX_Function(CursorIndex);  //ѭ������
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
													DisplayIndex=5;	
													MenuIndex		= 8;//ѡ���˵�ʮ����Ŀ¼											
												}else DisplayIndex--; 
										}
										else 
										{
											CursorIndex--;	//���� ��Ӧ����0-3	
											MenuIndex--;
										}											
										
									break;
					case KEY_DOWM:
										CursorIndex++;	//���� ��Ӧ����0-3	
										MenuIndex++;
										if(CursorIndex>=4)
										{
											DisplayIndex++; 
											CursorIndex = 3;
										}										
										if(DisplayIndex>5) 
										{
											DisplayIndex=0;
											CursorIndex=0;
											MenuIndex=0;
										}	break;
					case KEY_BANK:
												DisplayBuf_Clr();
												Clr_Pictrue();
												OppositeColor((2*3),3,4);
											return;
										
					case KEY_EDI:break;
											 
					default:break;
				}
				key_event_lesure();//�˳�æµ���
		}
  }
}



