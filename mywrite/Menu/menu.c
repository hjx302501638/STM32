#include "menu.h"
#include "Dis12864.h"
#include "Timer.h"
#include "keyboard.h"
#include "getwarte.h"
#include "query.h"
#include "useset.h"
#include "admin.h"
#include "delay.h"
#include "DS1302.h"
#include "flashmanege.h"
#include "APP.h"
#include "IO.h"


u8 CurrentMenuFlag  =  0;
u8 CurrentMenuFlag_per=10;
extern const unsigned char Char_QuickDisplay [4][16];
extern const unsigned char Char_WaitDisplay [4][16];
extern const unsigned char Char_CurrentMenu[16];
extern const unsigned char Char_CurrentState[7][10];
extern const unsigned char Char_WordState[5];
extern const unsigned char Char_SoftwareVersion[6] ;
extern const unsigned char Char_ChangeWarn[2][16];
extern const unsigned char Char_versionNum[3][10];
extern const unsigned char Char_QuickDisplay1 [4][16];
/*******************************************************************************
* 函 数 名: void Quick_GetWater(void)
* 功    能: 快捷取水
* 说    明: 
*******************************************************************************/
void Quick_GetWater(void)
{  
    u16 i;
	if(AdminPra.Soft_Version == 3)
	{
		for(i=0;i<16;i++)
		{
			Display_Buf[0][i]=Char_QuickDisplay[0][i];  
			Display_Buf[1][i]=Char_QuickDisplay[1][i]; 
			Display_Buf[2][i]=Char_QuickDisplay[2][i]; 
			Display_Buf[3][i]=Char_QuickDisplay[3][i]; 
		}
	}
	else 
	{
		for(i=0;i<16;i++)
		{
			Display_Buf[0][i]=Char_QuickDisplay1[0][i];  
			Display_Buf[1][i]=Char_QuickDisplay1[1][i]; 
			Display_Buf[2][i]=Char_QuickDisplay1[2][i]; 
			Display_Buf[3][i]=Char_QuickDisplay1[3][i]; 
		}
	}
}


/*******************************************************************************
* 函 数 名: void WaitDisplay(void)
* 功    能: 待机显示
* 说    明: 
*******************************************************************************/
void WaitDisplay(void)
{  
    u16 i;
  for(i=0;i<16;i++)
  {
    Display_Buf[0][i]=Char_WaitDisplay[0][i];  
    Display_Buf[1][i]=Char_WaitDisplay[1][i]; 
    Display_Buf[2][i]=Char_WaitDisplay[2][i]; 
    Display_Buf[3][i]=Char_WaitDisplay[3][i]; 
  }
 // Display_RefreshAll();
}
	extern u8 Ds1302DataBuf10[7];
	extern u8 Ds1302DataBufChar[16];
	extern u8 Ds1302DataBuf[8];

/*******************************************************************************
* 函 数 名: void ShowDataTime(void)
* 功    能: 显示时钟；第一行；
* 说    明: 
*******************************************************************************/
void ShowDataTime(void)
{
  u16 i;   

  BurstRead1302(Ds1302DataBuf);  
  DS1302_POWER();
  DS1302_Datato10();
  Timer_DisChar();
  for(i=0;i<16;i++)
    Display_Buf[0][i]=Ds1302DataBufChar[i];
  
  i    = (u16)Ds1302DataBuf10[3];
  i  <<=  8;
  i   |= (u16)Ds1302DataBuf10[4];  
  
//  if((User_Parameter.StopTime!=0)&&
//     (User_Parameter.StopTime==i)&&
//     (User_Parameter.StartTime!=User_Parameter.StopTime)&&
//      !gbCellFault)
//  {
//     Start_Stop=FALSE;        
//  }  
	
	/*判断是否需要睡眠*/
}


/*******************************************************************************
* 函 数 名: void ShowMeun(void)
* 功    能: 显示菜单选项
* 说    明: 
*******************************************************************************/
void ShowMeun(void)
{
  u8 i;   
  for(i=0;i<16;i++)
    Display_Buf[3][i]=Char_CurrentMenu[i];    
}


/*******************************************************************************
* 函 数 名: void ShowState(void)
* 功    能: 显示状态
* 说    明: 
*******************************************************************************/
void ShowState(void)
{
  u8 i;
	u8 temp=0;
	u8 State = 0;  
  for(i=0;i<6;i++)
    Display_Buf[2][i]=Char_WordState[i];
  if(Machine_State == QS_State)State = 0;
	else if(Machine_State == CX_State)State = 1;
	if(Machine_State == ZS_State)
	{
		State = 2;
		if(NO_Warter_Flag)State = 0;
	}
	if(AdminPra.Soft_Version == 1)//如果是版本B
	{
		if((Machine_State == ZS1_State)||(Machine_State == ZS2_State)||(Machine_State == ZS1and2_State))
		{
			if((GPIOC->ODR&(1<<10)) == (1<<10));
			else {State = 5;temp |= 1<<0;}
			if((GPIOA->ODR&(1<<8)) == (1<<8));
			else {State = 4;temp |= 1<<1;}
			if(temp == 3)
				State = 2;
		}
		if(NO_Warter_Flag) State = 0;
	}
	if(Machine_State == BY_State)
	{
			State = 3;
		 if(NO_Warter_Flag)State = 0;
	}
	if((AdminPra.Soft_Version == 0)||(AdminPra.Soft_Version == 3));//A D 版本不需要液位异常
	else 
	{
		if(L_Level)State = 6;
	}
  
  for(i=0;i<10;i++)
    Display_Buf[2][i+6]=Char_CurrentState[State][i];
}


/*******************************************************************************
* 函 数 名: void ShowVersion(void)
* 功    能: 显示版本
* 说    明: 
*******************************************************************************/
void ShowVersion(void)
{
  u8 i; 
	u8 VersionNumber=AdminPra.Soft_Version;
	
	/*判断耗材是否失效*/
	u8 RO_Change_Number = 1;
	u8 UP_Change_Number=1;
	
	
  DisplayBuf_ClrRow(1);
	
  if(RO_Change_Number==10)
  {
    for(i=0;i<16;i++)
      Display_Buf[1][i]=Char_ChangeWarn[0][i];
  }
  else if(UP_Change_Number==10)
  {
    for(i=0;i<16;i++)
      Display_Buf[1][i]=Char_ChangeWarn[1][i];
  }
  else
  {
    for(i=0;i<6;i++)
      Display_Buf[1][i]=Char_SoftwareVersion[i];  
    for(i=0;i<10;i++)
      Display_Buf[1][i+6]=Char_versionNum[VersionNumber][i];
  }
}
u8 wait_disp=0 ;

/*******************************************************************************
* 函 数 名: void CurrentDisplay(void)
* 功    能: 当前显示
* 说    明: 
*******************************************************************************/
void CurrentDisplay(void)
{  

	 
  //u8 CursorIndex_per;

  ShowDataTime( );
  ShowMeun( );
  ShowVersion( );
  ShowState( );
	OppositeColor((CurrentMenuFlag*3),3,4);
	while(1)
	{
		Sotf_Versions_Controller();//软件版本控制器
  if(Display_Delay>=DELAY_DISPLAY) 
  {
		ShowDataTime( );
    Display_RefreshAll(  );
		ShowState( );//显示状态
    Display_Delay=0;
		
		
		//key_event_lesure();//退出忙碌标记
  }   
  if(CurrentMenuFlag_per!=CurrentMenuFlag)
  {    
    CurrentMenuFlag_per=CurrentMenuFlag;
    OppositeColor((CurrentMenuFlag*3),3,4);
    //Display_RefreshAll(  );
  }
	
	if(key.KEY_Falg)
				{		
						key_event_buys();//进入忙碌标记
						/*这段区域内按键无效*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:														
														switch(CurrentMenuFlag)
														{
															case 0:
																		GetWater();
																		ShowDataTime( );
																		ShowMeun( );
																		ShowVersion( );
																		ShowState( );
																		break;
															case 1:
																	
																		Menu_UserCheck();
																		ShowDataTime( );
																		ShowMeun( );
																		ShowVersion( );
																		ShowState( );
																		break;
															case 2:
																		Menu_Userset();
																		ShowDataTime( );
																		ShowMeun( );
																		ShowVersion( );
																		ShowState( );
																		break;
														}
														/*设置参数退出*/
														break;
							case Sleep_TM:Clr_Pictrue(); return ;														
							case KEY_UP: 
													if(CurrentMenuFlag==0)
														CurrentMenuFlag = 2;
													 else CurrentMenuFlag--;
													 break;
							case KEY_DOWM:
													CurrentMenuFlag++;
													if(CurrentMenuFlag == 3)
														CurrentMenuFlag=0;
													break;
							case KEY_BANK: 
															wait_disp --;
															DisplayBuf_Clr();
															Clr_Pictrue();
															key_event_lesure();//退出忙碌标记
															//delay_ms(500);
																Display_Delay=0;
															return ;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
				
			}
}


//程序菜单
void  menu(void)
{
	switch(wait_disp)	
	{
		case 0:WaitDisplay();
			break;
		case 1:Quick_GetWater();
			break;
		case 2:CurrentDisplay();
			break;
		default :break;
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
							case KEY_OFF_ON:														
														break;
							case KEY_ENTER:
														wait_disp++;
														if(wait_disp==3)
															wait_disp =2;
														break;															
							case KEY_UP: 	//快捷取RO水键
														if(wait_disp==1)
														GetRO(0xff);
													  break;
							case KEY_DOWM:
														if(wait_disp==1)
														GetUP(0xff);
														break;
							case Long_UP:
														if(AdminPra.Soft_Version == 3)
														{
															if(wait_disp==1)
															GetEDI(0xff);
														}else 
														{
															if(wait_disp==1)
															GetRO(0xff);
														}
														break;
							case KEY_BANK: 
														if(wait_disp==0)
															wait_disp=0;
														else 
															wait_disp--;
														break;
							case KEY_EDI:  
														break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
			go_admin();	
}


