#include "admin.h"
#include "Dis12864.h"
#include "Timer.h"
#include "keyboard.h"
#include "flashmanege.h"
#include "IO.h"
#include "Voice.h"
#include "my_adc.h"
#include "APP.h"


extern const unsigned char Char_HighSet[20][10];
extern const unsigned char Char_DeviceM_0[17][12];
extern const unsigned char Char_DeviceM_1[17][12];
extern const unsigned char Char_Conductance[5];
extern const unsigned char Char_Resistivity[6];
extern const unsigned char Char_VoiceTest[5];
extern const unsigned char Char_Number[16];
extern const unsigned char Char_HighSet_RO_Flow[7];
extern const unsigned char Char_HighSet_UP_Flow[7];
extern const unsigned char Char_HighSet_ED_Flow[7];
extern const unsigned char Char_HighSet_Resistivity[13];
extern const unsigned char Char_HighSet_Conductance[13];
extern const unsigned char Char_HighSet_Conductance_SC[13];
extern const unsigned char Char_HighSet_Conductance_ED[13];
extern const unsigned char Char_SoftwareVersion[6];
extern const unsigned char Char_versionNum[3][10];
extern const unsigned char Char_Force_StopGetWater[2][8];
extern const unsigned char Char_RO_Conductance_Rang[3][6];
extern const unsigned char Char_InputCode[11];
extern const unsigned char Char_Wrong[4];
const unsigned char char_ON[10]="  未安装  ";
const unsigned char Char_ADD2[12]={"循环电阻率："};
const unsigned char Char_ADD3[12]={"循环UP时间："};
/*测试点检测界面 用做所有输入 输出变量的测试*/
void Highset_OutputTest(u8 now)
{
  u8 i;
  u8 MenuIndex;
  u8 CursorIndex;
  u8 DisplayIndex;
  u8 CursorIndex_per;
	u8 statebool[18]={0};
  DisplayBuf_Clr();
  CursorIndex_per=10;
  MenuIndex=0;     //对应菜单号0-14
  CursorIndex=0;   //对应光标号0-3
  DisplayIndex=0;  //对应显示号0-11
  key_event_lesure();//退出忙碌标记
	Close_ALL_IO ();//关闭所有的输出
  while(1)
  {	
		
    for(i=0;i<12;i++)
    {
      if(statebool[DisplayIndex]==0)
        Display_Buf[0][i]   =    Char_DeviceM_0[DisplayIndex][i];
      else
        Display_Buf[0][i]   =    Char_DeviceM_1[DisplayIndex][i];
      if(statebool[DisplayIndex+1]==0)
        Display_Buf[1][i]   =    Char_DeviceM_0[DisplayIndex+1][i];
      else
        Display_Buf[1][i]   =    Char_DeviceM_1[DisplayIndex+1][i];
      if(statebool[DisplayIndex+2]==0)
        Display_Buf[2][i]   =    Char_DeviceM_0[DisplayIndex+2][i];
      else
        Display_Buf[2][i]   =    Char_DeviceM_1[DisplayIndex+2][i];
      if(statebool[DisplayIndex+3]==0)
        Display_Buf[3][i]   =    Char_DeviceM_0[DisplayIndex+3][i];
      else
        Display_Buf[3][i]   =    Char_DeviceM_1[DisplayIndex+3][i];
    }
    if(DisplayIndex!=0)  Display_Buf[0][15]=0x18;
    else Display_Buf[0][15]=' ';
    if(DisplayIndex!=11)  Display_Buf[3][15]=0x19;
    else Display_Buf[3][15]=' ';
    if(CursorIndex_per!=CursorIndex)
    {
      CursorIndex_per=CursorIndex;
      OppositeColor(0,CursorIndex,12);
    }
    if(Display_Delay>=DELAY_DISPLAY)
    {
			/*更新输入*/
			statebool[13] = !L_F;statebool[14] = !H_F;
			statebool[15] = !L_Level;statebool[16] = !M_Level;statebool[17] = !H_Level;
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
							case 0:     //泵1.1
								if(statebool[0]==0)
								{
									statebool[0]=1;
									Pump_1_1_IO = ON;
								}
								else
								{
									statebool[0]=0;
									Pump_1_1_IO = OFF;
								}
								break;
							case 1:    //泵1.2
								if(statebool[1]==0)
								{
									statebool[1]=1;
									Pump_1_2_IO = ON;
								}
								else
								{
									statebool[1]=0;
									Pump_1_2_IO = OFF;
								}
								break;
							case 2:   //泵2
								if(statebool[2]==0)
								{
									statebool[2]=1;
									Pump_2_1_IO = ON;
								}
								else
								{
									statebool[2]=0;
									Pump_2_1_IO = OFF;
								}
								break;
							case 3:    //进水
								if(statebool[3]==0)
								{
									statebool[3]=1;
									JS_IO = ON;
								}
								else
								{
									statebool[3]=0;
									JS_IO = OFF;
								}
								break;
							case 4:     //超滤
								if(statebool[4]==0)
								{
									statebool[4]=1;
									Mllt_IO = ON;
								}
								else
								{
									statebool[4]=0;
									Mllt_IO = OFF;
								}
								break;
							case 5:     //冲洗       
								if(statebool[5]==0)
								{
									statebool[5]=1;
									CX_IO = ON;
								}
								else
								{
									statebool[5]=0;
									CX_IO = OFF;
								}
								break;
							case 6:     //RO取水       
								if(statebool[6]==0)
								{
									statebool[6]=1;
									RO_IO = ON;
								}
								else
								{
									statebool[6]=0;
									RO_IO = OFF;
								}
								break;
							case 7:    //UP取水        
								if(statebool[7]==0)
								{
									statebool[7]=1;
								  UP_IO = ON;
								}
								else
								{
									statebool[7]=0;
									UP_IO = OFF;
								}
								break;
							case 8:   //ED取水         
								if(statebool[8]==0)
								{
									statebool[8]=1;
									EDI_IO = ON;
								}
								else
								{
									statebool[8]=0;
									EDI_IO = OFF;
								}
								break;
							case 9:   //继电器         
								if(statebool[9]==0)
								{
									statebool[9]=1;
									JK_IO = ON;
								}
								else
								{
									statebool[9]=0;
									JK_IO = OFF;
								}
								break;
								case 10:   //指示灯         
								if(statebool[10]==0)
								{
									statebool[10]=1;
									LED_IO  =ON;
								}
								else
								{
									statebool[10]=0;
									LED_IO  =OFF;
								}
								break;
								case 11:    //循环阀        
								if(statebool[11]==0)
								{
									statebool[11]=1;
									Cri_IO = ON;
								}
								else
								{
									statebool[11]=0;
									Cri_IO = OFF;
								}
								break;
								case 12:   //循环泵         
								if(statebool[12]==0)
								{
									statebool[12]=1;
									Pump_2_2_IO = ON;
								}
								else
								{
									statebool[12]=0;
									Pump_2_2_IO = OFF;
								}
								break;
							default:
								break;
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
													DisplayIndex=14;	
													MenuIndex		= 17;//选择了第十三条目录											
												}else DisplayIndex--; 
										}
										else 
										{
											CursorIndex--;	//光标减 对应光标号0-3	
											MenuIndex--;
										}											
										
									break;
					case KEY_DOWM:
										CursorIndex++;	//光标加 对应光标号0-3	
										MenuIndex++;
										if(CursorIndex>=4)
										{
											DisplayIndex++; 
											CursorIndex = 3;
										}										
										if(DisplayIndex>14) 
										{
											DisplayIndex=0;
											CursorIndex=0;
											MenuIndex=0;
										}	break;
					case KEY_BANK:
								OppositeColor(0,now,10);//设置标号
								return ;
								
					case KEY_EDI:break;
					default:break;
				}
				key_event_lesure();//退出忙碌标记
		}
	}
		
	
	
	//    
//    if(READ_LPV)
//      statebool[12]=0;
//    else
//      statebool[12]=1;
//    if(READ_HPV)
//      statebool[13]=0;
//    else
//      statebool[13]=1;
//    if(READ_LW)
//      statebool[14]=0;
//    else
//      statebool[14]=1;
//    if(READ_MW)
//      statebool[15]=0;
//    else
//      statebool[15]=1;
//    if(READ_HW)
//      statebool[16]=0;
//    else
//      statebool[16]=1;
//    
}

/*******************************************************************************
* 函 数 名: void Highset_WaterTest(void)
* 功    能: 二级菜单,水质采样
* 说    明: 
*******************************************************************************/
void Highset_WaterTest(u8 now)
{
  u8 i;
	float test_temp[4]={0} ;
	test_temp[0] = GET_ADVAL.GetSR_AD();
	test_temp[1] = GET_ADVAL.GetRO_AD();
	test_temp[2] = GET_ADVAL.GetEDI_AD();
	test_temp[3] = GET_ADVAL.GetUP_AD();
  Clr_Pictrue();
  DisplayBuf_Clr( );
	key_event_lesure();//退出忙碌标记
	
  Display_Buf[0][0]   =    'S';  
  Display_Buf[0][1]   =    'C'; 
  Display_Buf[0][2]   =    ':'; 
  Display_Buf[1][0]   =    'R';  
  Display_Buf[1][1]   =    'O'; 
  Display_Buf[1][2]   =    ':'; 
  Display_Buf[2][0]   =    'E';  
  Display_Buf[2][1]   =    'D'; 
  Display_Buf[2][2]   =    ':'; 
	Display_Buf[3][0]   =    'U';  
  Display_Buf[3][1]   =    'P'; 
  Display_Buf[3][2]   =    ':';
  for(i=0;i<2;i++)
  {
		Display_Buf[2][i+11]   =    Char_Conductance[i]; 
    Display_Buf[1][i+11]   =    Char_Conductance[i]; 
    Display_Buf[0][i+11]   =    Char_Conductance[i]; 
		
  }
  
  for(i=0;i<3;i++)
  {
    Display_Buf[3][i+11]   =    Char_Resistivity[i]; 
  }
  
  Display_RefreshAll();
	
   while(1)
  { 
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    if(Display_Delay>=125)
    {
      //ComputResult ( );
      Display_Delay=0;
      Display_RefreshAll();
			test_temp[0] = GET_ADVAL.GetSR_AD();
			test_temp[1] = GET_ADVAL.GetRO_AD();
			test_temp[2] = GET_ADVAL.GetEDI_AD();
			test_temp[3] = GET_ADVAL.GetUP_AD();
    }
		if(!GET_ADVAL.State3)
		for(i=0;i<10;i++)  Display_Buf[0][i+4] = char_ON[i];
		else  DisplayFloat( 3,0,test_temp[0] );
		
		if(!GET_ADVAL.State1)
		for(i=0;i<10;i++)  Display_Buf[1][i+4] = char_ON[i];
		else  DisplayFloat( 3,1,test_temp[1] );
		
		if(!GET_ADVAL.State2)
		for(i=0;i<10;i++)  Display_Buf[2][i+4] = char_ON[i];
		else DisplayFloat( 3,2,test_temp[2] );
		
		if(!GET_ADVAL.State)
		for(i=0;i<10;i++)  Display_Buf[3][i+4] = char_ON[i];
		else DisplayFloat( 3,3,test_temp[3] );

		if(key.KEY_Falg)
		{
				key_event_buys();//进入忙碌标记
				/*这段区域内按键无效*/
				switch(key.KEY_Val)
				{
					case KEY_OFF_ON:break;
					case KEY_ENTER:	break;
					case KEY_UP:    break;
					case KEY_DOWM:  break;
					case KEY_BANK: OppositeColor(0,now,10);
								
								return;
					case KEY_EDI:   break;
					default:break;
				}
				key_event_lesure();//退出忙碌标记
		}

  }
}










/*******************************************************************************
* 函 数 名: void Highset_VoiceTest(void)
* 功    能: 二级菜单,声音检测
* 说    明: 
*******************************************************************************/
void Highset_VoiceTest(u8 now)
{
  u8 i;
  u8 num=0;
  Clr_Pictrue();
  DisplayBuf_Clr( );
		key_event_lesure();//退出忙碌标记
  for(i=0;i<5;i++)
  {
    Display_Buf[1][i+4]   =    Char_VoiceTest[i]; 
  }  
  Display_RefreshAll();
	
   while(1)
  {  
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 		
    Display_Buf[1][10]  =   Char_Number[num/10];
    Display_Buf[1][11]  =   Char_Number[num%10];
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
					case KEY_ENTER:	VoiceOut(num);
													break;
					case KEY_UP:
											if(num==0)num = 23;												 
												else num--;												
													break;
					case KEY_DOWM:  
											if(num==23)num = 0;												 
												else num++;	
													break;
					case KEY_BANK: OppositeColor(0,now,10);								
								return;
					case KEY_EDI:   break;
					default:break;
				}
				key_event_lesure();//退出忙碌标记
		}
		
	}  
}


/*******************************************************************************
* 函 数 名: void Highset_RO_Flow_Set(void)
* 功    能: 二级菜单,RO取水流量设置；
* 说    明: 
*******************************************************************************/
void Highset_RO_Flow_Set( u8 now)
{
  u8   i,rl=0,rl1=1;
  u8   Flow1=0,Flow2=0;
  u16  Flow=0;
	key_event_lesure();//退出忙碌标记
  Flow       =   AdminPra.FlowROSet;//当前设置流量
  Flow1  =   Flow/100;
  Flow2  =   Flow%100;
  DisplayBuf_Clr();
  OppositeColor(2,2,1);
  for(i=0;i<7;i++)
    Display_Buf[0][i]=Char_HighSet_RO_Flow[i];
  Display_Buf[2][9]   =  'm';
  Display_Buf[2][10]   =  'L';
  Display_Buf[2][11]   =  '/';
  Display_Buf[2][12]   =  'm';
  Display_Buf[2][13]   =  'i';
  Display_Buf[2][14]   =  'n';
  
  
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(Flow1/10)];
    Display_Buf[2][5]= Char_Number[(u8)(Flow1%10)];
    Display_Buf[2][6]= Char_Number[(u8)(Flow2/10)];
    Display_Buf[2][7]= Char_Number[(u8)(Flow2%10)];
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }    
    if(rl1!=rl)
    {
      rl1=rl;      
      OppositeColor(rl+2,2,2);
      Display_RefreshAll();
    }

		if(key.KEY_Falg||key.KEY_Mode)
				{		
						key_event_buys();//进入忙碌标记
						/*这段区域内按键无效*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:if(rl==0)
																{
																	rl=1;
																}
																else
																{
																	/*设置参数退出*/
																	 AdminPra.FlowROSet = Flow1*100+Flow2;
																	 Seve_Admindata();
																	 OppositeColor(0,now,10);								
																	 return;
																}	
															break;
							case KEY_UP: 
													 if(rl==1)
														{
															if(Flow2<99)
																Flow2++;
															else
																Flow2=0;                    
														}
														else
														{
															if(Flow1<99)
																Flow1++;
															else
																Flow1=0;
														}											
															break;
							case KEY_DOWM:  
													if(rl==1)
														{
															if(Flow2>0)
																Flow2--;
															else
																Flow2=99;                    
														}
														else
														{
															if(Flow1>0)
																Flow1--;
															else
																Flow1=99;
														}
															break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}

  }
}

/*******************************************************************************
* 函 数 名: void Highset_UP_Flow_Set(void)
* 功    能: 二级菜单,UP取水流量设置；
* 说    明: 
*******************************************************************************/
void Highset_UP_Flow_Set(u8 now)
{
	 u8   i,rl=0,rl1=1;
  u8   Flow1=0,Flow2=0;
  u16  Flow=0;
	key_event_lesure();//退出忙碌标记
  Flow       =   AdminPra.FlowUPSet;//当前设置流量
  Flow1  =   Flow/100;
  Flow2  =   Flow%100;
  DisplayBuf_Clr();
  OppositeColor(2,2,1);
  for(i=0;i<7;i++)
    Display_Buf[0][i]=Char_HighSet_UP_Flow[i];
  Display_Buf[2][9]   =  'm';
  Display_Buf[2][10]   =  'L';
  Display_Buf[2][11]   =  '/';
  Display_Buf[2][12]   =  'm';
  Display_Buf[2][13]   =  'i';
  Display_Buf[2][14]   =  'n';
  
  
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(Flow1/10)];
    Display_Buf[2][5]= Char_Number[(u8)(Flow1%10)];
    Display_Buf[2][6]= Char_Number[(u8)(Flow2/10)];
    Display_Buf[2][7]= Char_Number[(u8)(Flow2%10)];
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }    
    if(rl1!=rl)
    {
      rl1=rl;      
      OppositeColor(rl+2,2,2);
      Display_RefreshAll();
    }

		if(key.KEY_Falg||key.KEY_Mode)
				{		
						key_event_buys();//进入忙碌标记
						/*这段区域内按键无效*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:if(rl==0)
																{
																	rl=1;
																}
																else
																{
																	/*设置参数退出*/
																	AdminPra.FlowUPSet = (Flow1*100)+Flow2;
																	Seve_Admindata();
																	 OppositeColor(0,now,10);								
																	 return;
																}	
															break;
							case KEY_UP: 
													 if(rl==1)
														{
															if(Flow2<99)
																Flow2++;
															else
																Flow2=0;                    
														}
														else
														{
															if(Flow1<99)
																Flow1++;
															else
																Flow1=0;
														}											
															break;
							case KEY_DOWM:  
													if(rl==1)
														{
															if(Flow2>0)
																Flow2--;
															else
																Flow2=99;                    
														}
														else
														{
															if(Flow1>0)
																Flow1--;
															else
																Flow1=99;
														}
															break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}

  }
}

/*******************************************************************************
* 函 数 名: void Highset_ED_Flow_Set(void)
* 功    能: 二级菜单,ED取水流量设置；
* 说    明: 
*******************************************************************************/
void Highset_ED_Flow_Set(u8 now)
{
	u8   i,rl=0,rl1=1;
  u8   Flow1=0,Flow2=0;
  u16  Flow=0;
	key_event_lesure();//退出忙碌标记
  Flow       =   AdminPra.FlowEDISet;//当前设置流量
  Flow1  =   Flow/100;
  Flow2  =   Flow%100;
  DisplayBuf_Clr();
  OppositeColor(2,2,1);
  for(i=0;i<7;i++)
    Display_Buf[0][i]=Char_HighSet_ED_Flow[i];
  Display_Buf[2][9]   =  'm';
  Display_Buf[2][10]   =  'L';
  Display_Buf[2][11]   =  '/';
  Display_Buf[2][12]   =  'm';
  Display_Buf[2][13]   =  'i';
  Display_Buf[2][14]   =  'n';
  
  
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(Flow1/10)];
    Display_Buf[2][5]= Char_Number[(u8)(Flow1%10)];
    Display_Buf[2][6]= Char_Number[(u8)(Flow2/10)];
    Display_Buf[2][7]= Char_Number[(u8)(Flow2%10)];
    if(Display_Delay>=DELAY_DISPLAY)
    {
      Display_Delay   =   0;
      Display_RefreshAll();
    }    
    if(rl1!=rl)
    {
      rl1=rl;      
      OppositeColor(rl+2,2,2);
      Display_RefreshAll();
    }

		if(key.KEY_Falg)
				{		
						key_event_buys();//进入忙碌标记
						/*这段区域内按键无效*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:if(rl==0)
																{
																	rl=1;
																}
																else
																{
																	/*设置参数退出*/
																	 AdminPra.FlowEDISet= Flow1*100+Flow2;
																	 Seve_Admindata();
																	 OppositeColor(0,now,10);								
																	 return;
																}	
															break;
							case KEY_UP: 
													 if(rl==1)
														{
															if(Flow2<99)
																Flow2++;
															else
																Flow2=0;                    
														}
														else
														{
															if(Flow1<99)
																Flow1++;
															else
																Flow1=0;
														}											
															break;
							case KEY_DOWM:  
													if(rl==1)
														{
															if(Flow2>0)
																Flow2--;
															else
																Flow2=99;                    
														}
														else
														{
															if(Flow1>0)
																Flow1--;
															else
																Flow1=99;
														}
															break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}

  }
}


/*******************************************************************************
* 函 数 名: void Highset_Resistivity_ADJ_Set(void)
* 功    能: 二级菜单,电阻率系数设置；
* 说    明: 
*******************************************************************************/
void Highset_Resistivity_ADJ_Set(u8 now)
{
  u8 i;
  s16  Flow,Flow_buf;
	Flow_buf       =   AdminPra.UPpra;
  DisplayBuf_Clr();
	key_event_lesure();//退出忙碌标记
  OppositeColor(2,2,6);
  for(i=0;i<13;i++)
    Display_Buf[0][i]=Char_HighSet_Resistivity[i];  
 
  
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    if(Flow_buf>=0)
    {
      Flow=Flow_buf;
      Display_Buf[2][4]= ' ';      
    }
    else
    {
      Flow=-Flow_buf;
      Display_Buf[2][4]= '-';
    }
    Display_Buf[2][5]= Char_Number[(u8)(Flow/1000)];    
    Display_Buf[2][6]= Char_Number[(u8)(Flow%1000/100)];
    Display_Buf[2][7]='.';
    Display_Buf[2][8]= Char_Number[(u8)(Flow%100/10)];    
    Display_Buf[2][9]= Char_Number[(u8)(Flow%10)];
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
														/*设置参数退出*/
														AdminPra.UPpra =Flow_buf;
														Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 
													if(Flow_buf<9999)
														Flow_buf++;
														else
															Flow_buf=-9999;
													 break;
							case KEY_DOWM:
														if(Flow_buf>-9999)
															Flow_buf--;
															else
																Flow_buf=9999;  
													break;
							case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow_buf++;
															else
																Flow_buf=0; 
															if(Flow_buf>=0)
																{
																	Flow=Flow_buf;
																	Display_Buf[2][4]= ' ';      
																}
																else
																{
																	Flow=-Flow_buf;
																	Display_Buf[2][4]= '-';
																}
															Display_Buf[2][5]= Char_Number[(u8)(Flow_buf/1000)];    
															Display_Buf[2][6]= Char_Number[(u8)(Flow_buf%1000/100)];
															Display_Buf[2][7]='.';
															Display_Buf[2][8]= Char_Number[(u8)(Flow_buf%100/10)];
															Display_Buf[2][9]= Char_Number[(u8)(Flow_buf%10)];
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow_buf--;
															else
														  Flow_buf=9999;
															if(Flow_buf>=0)
																{
																	Flow=Flow_buf;
																	Display_Buf[2][4]= ' ';      
																}
																else
																{
																	Flow=-Flow_buf;
																	Display_Buf[2][4]= '-';
																}
															Display_Buf[2][5]= Char_Number[(u8)(Flow_buf/1000)];    
															Display_Buf[2][6]= Char_Number[(u8)(Flow_buf%1000/100)];
															Display_Buf[2][7]='.';
															Display_Buf[2][8]= Char_Number[(u8)(Flow_buf%100/10)];
															Display_Buf[2][9]= Char_Number[(u8)(Flow_buf%10)];
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
  }
}


/*******************************************************************************
* 函 数 名: void Highset_Conductance_ADJ_Set(void)
* 功    能: 二级菜单,RO电导率系数；
* 说    明: 
*******************************************************************************/
void Highset_Conductance_ADJ_Set(u8 now)
{
	  u8 i;
  s16  Flow,Flow_buf;
	Flow_buf       =   AdminPra.ROpra;
  DisplayBuf_Clr();
	key_event_lesure();//退出忙碌标记
  OppositeColor(2,2,6);
  for(i=0;i<13;i++)
    Display_Buf[0][i]=Char_HighSet_Conductance[i];  
 
  
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    if(Flow_buf>=0)
    {
      Flow=Flow_buf;
      Display_Buf[2][4]= ' ';      
    }
    else
    {
      Flow=-Flow_buf;
      Display_Buf[2][4]= '-';
    }
    Display_Buf[2][5]= Char_Number[(u8)(Flow/1000)];    
    Display_Buf[2][6]= Char_Number[(u8)(Flow%1000/100)];
    Display_Buf[2][7]='.';
    Display_Buf[2][8]= Char_Number[(u8)(Flow%100/10)];    
    Display_Buf[2][9]= Char_Number[(u8)(Flow%10)];
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
														/*设置参数退出*/
															AdminPra.ROpra =Flow_buf;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 
													if(Flow_buf<9999)
														Flow_buf++;
														else
															Flow_buf=-9999;
													 break;
							case KEY_DOWM:
														if(Flow_buf>-9999)
															Flow_buf--;
															else
																Flow_buf=9999;  
													break;
						  case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow_buf++;
															else
																Flow_buf=0; 
															if(Flow_buf>=0)
																{
																	Flow=Flow_buf;
																	Display_Buf[2][4]= ' ';      
																}
																else
																{
																	Flow=-Flow_buf;
																	Display_Buf[2][4]= '-';
																}
															Display_Buf[2][5]= Char_Number[(u8)(Flow_buf/1000)];    
															Display_Buf[2][6]= Char_Number[(u8)(Flow_buf%1000/100)];
															Display_Buf[2][7]='.';
															Display_Buf[2][8]= Char_Number[(u8)(Flow_buf%100/10)];
															Display_Buf[2][9]= Char_Number[(u8)(Flow_buf%10)];
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow_buf--;
															else
														  Flow_buf=9999;
															if(Flow_buf>=0)
																{
																	Flow=Flow_buf;
																	Display_Buf[2][4]= ' ';      
																}
																else
																{
																	Flow=-Flow_buf;
																	Display_Buf[2][4]= '-';
																}
															Display_Buf[2][5]= Char_Number[(u8)(Flow_buf/1000)];    
															Display_Buf[2][6]= Char_Number[(u8)(Flow_buf%1000/100)];
															Display_Buf[2][7]='.';
															Display_Buf[2][8]= Char_Number[(u8)(Flow_buf%100/10)];
															Display_Buf[2][9]= Char_Number[(u8)(Flow_buf%10)];
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
  }
}

/*******************************************************************************
* 函 数 名: void Highset_Conductance_source_ADJ_Set(void);d)
* 功    能: 二级菜单,原水电导率系数；
* 说    明: 
*******************************************************************************/
void Highset_Conductance_source_ADJ_Set(u8 now)
{
	 u8 i;
  s16  Flow,Flow_buf;
  Flow_buf       =   AdminPra.SRpra;
  DisplayBuf_Clr();
	key_event_lesure();//退出忙碌标记
  OppositeColor(2,2,6);
  for(i=0;i<13;i++)
    Display_Buf[0][i]=Char_HighSet_Conductance_SC[i];  
 
  
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    if(Flow_buf>=0)
    {
      Flow=Flow_buf;
      Display_Buf[2][4]= ' ';      
    }
    else
    {
      Flow=-Flow_buf;
      Display_Buf[2][4]= '-';
    }
    Display_Buf[2][5]= Char_Number[(u8)(Flow/1000)];    
    Display_Buf[2][6]= Char_Number[(u8)(Flow%1000/100)];
    Display_Buf[2][7]='.';
    Display_Buf[2][8]= Char_Number[(u8)(Flow%100/10)];    
    Display_Buf[2][9]= Char_Number[(u8)(Flow%10)];
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
														/*设置参数退出*/
							               AdminPra.SRpra=Flow_buf;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 
													if(Flow_buf<9999)
														Flow_buf++;
														else
															Flow_buf=-9999;
													 break;
							case KEY_DOWM:
														if(Flow_buf>-9999)
															Flow_buf--;
															else
																Flow_buf=9999;  
													break;
							case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow_buf++;
															else
																Flow_buf=0; 
															if(Flow_buf>=0)
																{
																	Flow=Flow_buf;
																	Display_Buf[2][4]= ' ';      
																}
																else
																{
																	Flow=-Flow_buf;
																	Display_Buf[2][4]= '-';
																}
															Display_Buf[2][5]= Char_Number[(u8)(Flow_buf/1000)];    
															Display_Buf[2][6]= Char_Number[(u8)(Flow_buf%1000/100)];
															Display_Buf[2][7]='.';
															Display_Buf[2][8]= Char_Number[(u8)(Flow_buf%100/10)];
															Display_Buf[2][9]= Char_Number[(u8)(Flow_buf%10)];
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow_buf--;
															else
														  Flow_buf=9999;
															if(Flow_buf>=0)
																{
																	Flow=Flow_buf;
																	Display_Buf[2][4]= ' ';      
																}
																else
																{
																	Flow=-Flow_buf;
																	Display_Buf[2][4]= '-';
																}
															Display_Buf[2][5]= Char_Number[(u8)(Flow_buf/1000)];    
															Display_Buf[2][6]= Char_Number[(u8)(Flow_buf%1000/100)];
															Display_Buf[2][7]='.';
															Display_Buf[2][8]= Char_Number[(u8)(Flow_buf%100/10)];
															Display_Buf[2][9]= Char_Number[(u8)(Flow_buf%10)];
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
  }
}



/*******************************************************************************
* 函 数 名: void Highset_Conductance_EDI_ADJ_Set(void);d)
* 功    能: 二级菜单,EDI电导率系数；
* 说    明: 
*******************************************************************************/
void Highset_Conductance_EDI_ADJ_Set(u8 now)
{
	 u8 i;
  s16  Flow,Flow_buf;
   Flow_buf       =   AdminPra.EDIpra;
  DisplayBuf_Clr();
	key_event_lesure();//退出忙碌标记
  OppositeColor(2,2,6);
  for(i=0;i<13;i++)
    Display_Buf[0][i]=Char_HighSet_Conductance_ED[i];  
 
  
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    if(Flow_buf>=0)
    {
      Flow=Flow_buf;
      Display_Buf[2][4]= ' ';      
    }
    else
    {
      Flow=-Flow_buf;
      Display_Buf[2][4]= '-';
    }
    Display_Buf[2][5]= Char_Number[(u8)(Flow/1000)];    
    Display_Buf[2][6]= Char_Number[(u8)(Flow%1000/100)];
    Display_Buf[2][7]='.';
    Display_Buf[2][8]= Char_Number[(u8)(Flow%100/10)];    
    Display_Buf[2][9]= Char_Number[(u8)(Flow%10)];
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
														/*设置参数退出*/
														 AdminPra.EDIpra = Flow_buf;
														 Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 
													if(Flow_buf<9999)
														Flow_buf++;
														else
															Flow_buf=-9999;
													 break;
							case KEY_DOWM:
														if(Flow_buf>-9999)
															Flow_buf--;
															else
																Flow_buf=9999;  
													break;
							case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow_buf++;
															else
																Flow_buf=0; 
															if(Flow_buf>=0)
																{
																	Flow=Flow_buf;
																	Display_Buf[2][4]= ' ';      
																}
																else
																{
																	Flow=-Flow_buf;
																	Display_Buf[2][4]= '-';
																}
															Display_Buf[2][5]= Char_Number[(u8)(Flow_buf/1000)];    
															Display_Buf[2][6]= Char_Number[(u8)(Flow_buf%1000/100)];
															Display_Buf[2][7]='.';
															Display_Buf[2][8]= Char_Number[(u8)(Flow_buf%100/10)];
															Display_Buf[2][9]= Char_Number[(u8)(Flow_buf%10)];
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow_buf--;
															else
														  Flow_buf=9999;
															if(Flow_buf>=0)
																{
																	Flow=Flow_buf;
																	Display_Buf[2][4]= ' ';      
																}
																else
																{
																	Flow=-Flow_buf;
																	Display_Buf[2][4]= '-';
																}
															Display_Buf[2][5]= Char_Number[(u8)(Flow_buf/1000)];    
															Display_Buf[2][6]= Char_Number[(u8)(Flow_buf%1000/100)];
															Display_Buf[2][7]='.';
															Display_Buf[2][8]= Char_Number[(u8)(Flow_buf%100/10)];
															Display_Buf[2][9]= Char_Number[(u8)(Flow_buf%10)];
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
  }
}

/*******************************************************************************
* 函 数 名: void Highset_Version_Set(void)
* 功    能: 软件版本选择
* 说    明: 
*******************************************************************************/
void Highset_Version_Set(u8 now)
{
  u16 version_number,i;
	version_number =AdminPra.Soft_Version;//当前软件版本等于管理员参数中的软件版本
	key_event_lesure();//退出忙碌标记
  Clr_Pictrue();
  DisplayBuf_Clr( );
  
  for(i=0;i<5;i++)
  {
    Display_Buf[0][i+4]   =    Char_SoftwareVersion[i]; 
  }  
  Display_RefreshAll();
   while(1)
  {  
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 	
    for(i=0;i<10;i++)
    {
      Display_Buf[2][i+2]   =    Char_versionNum[version_number][i]; 
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
														/*设置参数退出*/
							               AdminPra.Soft_Version=version_number;
														 Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 
													if(version_number<VERSION_NUMBER)
														version_number++;
													else
														version_number=0;
													 break;
							case KEY_DOWM:
													if(version_number>0)
														version_number--;
													else
														version_number=VERSION_NUMBER;	
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
 
  }
}



//if(key.KEY_Falg)
//				{		
//						key_event_buys();//进入忙碌标记
//						/*这段区域内按键无效*/
//						switch(key.KEY_Val)
//						{
//							case KEY_OFF_ON:break;
//							case KEY_ENTER:
//														/*设置参数退出*/
//														 OppositeColor(0,now,10);								
//														 return;															
//							case KEY_UP: 										
//													 break;
//							case KEY_DOWM:
//													break;
//							case KEY_BANK: OppositeColor(0,now,10);								
//										return;
//							case KEY_EDI:   break;
//							default:break;
//						}
//						key_event_lesure();//退出忙碌标记
//				}


/*******************************************************************************
* 函 数 名: void Highset_WaterQuality_UP(void)
* 功    能: 水质限制 UP,电阻率下限
* 说    明: 
*******************************************************************************/
void Highset_WaterQuality_UP(u8 now)
{
  u8 i;
  u16  Flow;
  Flow       =   AdminPra.UPthres;
	key_event_lesure();//退出忙碌标记
  DisplayBuf_Clr();
  OppositeColor(2,2,5);
  for(i=0;i<8;i++)
    Display_Buf[0][i]=Char_HighSet_Resistivity[i];  
 
  Display_Buf[0][8]=':';
  for(i=0;i<6;i++)
  {
    Display_Buf[2][i+9]   =    Char_Resistivity[i]; 
  }
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
    Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
    Display_Buf[2][6]='.';
    Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];
    Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
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
														/*设置参数退出*/
															AdminPra.UPthres=Flow;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: if(Flow<9999)
														Flow++;
													else
														Flow=0;  										
													 break;
							case KEY_DOWM:
													if(Flow>0)
													Flow--;
													else
														Flow=9999;
													break;
							case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow++;
															else
																Flow=0; 
															
															Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
															Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
															Display_Buf[2][6]='.';
															Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];
															Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow--;
															else
														  Flow=9999;
															
															Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
															Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
															Display_Buf[2][6]='.';
															Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];
															Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
    
  }
}


/*******************************************************************************
* 函 数 名: void Highset_WaterQuality_XH(void)
* 功    能: 设置循环电阻值
* 说    明: 
*******************************************************************************/
void Highset_WaterQuality_XH(u8 now)
{
  u8 i;
  u16  Flow;
  Flow       =   AdminPra.XHthres;
	key_event_lesure();//退出忙碌标记
  DisplayBuf_Clr();
  OppositeColor(2,2,5);
  for(i=0;i<10;i++)
    Display_Buf[0][i]=Char_ADD2[i];  
 
  Display_Buf[0][11]=':';
  for(i=0;i<6;i++)
  {
    Display_Buf[2][i+9]   =    Char_Resistivity[i]; 
  }
  while(1)
  {
	  #if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
    Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
    Display_Buf[2][6]='.';
    Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];
    Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
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
														/*设置参数退出*/
															AdminPra.XHthres=Flow;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: if(Flow<9999)
														Flow++;
													else
														Flow=0;  										
													 break;
							case KEY_DOWM:
													if(Flow>0)
													Flow--;
													else
														Flow=9999;
													break;
							case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow++;
															else
																Flow=0; 
															
															Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
															Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
															Display_Buf[2][6]='.';
															Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];
															Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow--;
															else
														  Flow=9999;
															
															Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
															Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
															Display_Buf[2][6]='.';
															Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];
															Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
    
  }
}


/*******************************************************************************
* 函 数 名: void Highset_WaterQuality_XH(void)
* 功    能: 设置循环电阻值
* 说    明: 
*******************************************************************************/
void Highset_WaterQuality_XH_TM(u8 now)
{
  u8 i;
  u16  Flow;
  Flow       =   AdminPra.XH_Timer;
	key_event_lesure();//退出忙碌标记
  DisplayBuf_Clr();
  OppositeColor(2,2,5);
  for(i=0;i<10;i++)
    Display_Buf[0][i]=Char_ADD3[i];  
 
  Display_Buf[0][11]=':';
  
  while(1)
  {
	  #if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
    Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
    Display_Buf[2][6]=Char_Number[(u8)(Flow%100/10)];
    Display_Buf[2][7]=Char_Number[(u8)(Flow%10)];
   
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
														/*设置参数退出*/
															AdminPra.XH_Timer=Flow;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: if(Flow<9999)
														Flow++;
													else
														Flow=0;  										
													 break;
							case KEY_DOWM:
													if(Flow>0)
													Flow--;
													else
														Flow=9999;
													break;
							case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow++;
															else
																Flow=0; 
															
															Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
															Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
															Display_Buf[2][6]=Char_Number[(u8)(Flow%100/10)];
															Display_Buf[2][7]= Char_Number[(u8)(Flow%10)];															
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow--;
															else
														  Flow=9999;
															
															Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
															Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
															Display_Buf[2][6]=Char_Number[(u8)(Flow%100/10)];
															Display_Buf[2][7]= Char_Number[(u8)(Flow%10)];											
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
    
  }
}
//根据量程显示数据
void RO_Dis(u16 Flow)
{
	//根据量程判断小数点位数
		if(AdminPra.ROrange == 0)//量程为0-20
		{
			Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
			Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
			Display_Buf[2][6]='.';
			Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];
			Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
		}else if(AdminPra.ROrange == 1)//量程为0-200
		{
			Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
			Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
			Display_Buf[2][6]=Char_Number[(u8)(Flow%100/10)];
			Display_Buf[2][7]= '.';
			Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
		}
		else if(AdminPra.ROrange == 2)//量程为0-2000
		{
			Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
			Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
			Display_Buf[2][6]=Char_Number[(u8)(Flow%100/10)];
			Display_Buf[2][7]= Char_Number[(u8)(Flow%10)];
			
		}
}

/*******************************************************************************
* 函 数 名: void Highset_WaterQuality_UP(void)
* 功    能: 水质限制 RO,电导率上限
* 说    明: 
*******************************************************************************/
void Highset_WaterQuality_RO(u8 now)
{
  u8 i;
  u16  Flow;
  Flow       =   AdminPra.ROthres;
	key_event_lesure();//退出忙碌标记
	
  DisplayBuf_Clr();
  OppositeColor(2,2,5);
  for(i=0;i<8;i++)
    Display_Buf[0][i]=Char_HighSet_Conductance[i];  
 
  Display_Buf[0][8]=':';
  for(i=0;i<6;i++)
  {
    Display_Buf[2][i+9]   =    Char_Conductance[i]; 
  }
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
		RO_Dis(Flow);
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
														/*设置参数退出*/
															 AdminPra.ROthres = Flow;
															 Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: if(Flow<9999)
														Flow++;
													else
														Flow=0;  										
													 break;
							case KEY_DOWM:
													if(Flow>0)
													Flow--;
													else
														Flow=9999;
													break;
							case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow++;
															else
																Flow=0; 
															
															RO_Dis(Flow);
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow--;
															else
														  Flow=9999;
															
															RO_Dis(Flow);
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
    
  }
}


//根据量程显示数据
void EDI_Dis(u16 Flow)
{
 //根据量程判断小数点位数
	if(AdminPra.EDIrange == 0)//量程为0-20
	{
		Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
		Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
		Display_Buf[2][6]='.';
		Display_Buf[2][7]= Char_Number[(u8)(Flow%100/10)];
		Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
	}else if(AdminPra.EDIrange == 1)//量程为0-200
	{
		Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
		Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
		Display_Buf[2][6]=Char_Number[(u8)(Flow%100/10)];
		Display_Buf[2][7]= '.';
		Display_Buf[2][8]= Char_Number[(u8)(Flow%10)];
	}
	else if(AdminPra.EDIrange == 2)//量程为0-2000
	{
		Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)];    
		Display_Buf[2][5]= Char_Number[(u8)(Flow%1000/100)];
		Display_Buf[2][6]=Char_Number[(u8)(Flow%100/10)];
		Display_Buf[2][7]= Char_Number[(u8)(Flow%10)];
		
	}
}
/*******************************************************************************
* 函 数 名: void Highset_WaterQuality_ED(void)
* 功    能: 水质限制 ED,电导率上限
* 说    明: 
*******************************************************************************/
void Highset_WaterQuality_ED(u8 now)
{
  u8 i;
  u16  Flow;
  Flow       =   AdminPra.EDIthres;
	key_event_lesure();//退出忙碌标记
	
  DisplayBuf_Clr();
  OppositeColor(2,2,5);
  for(i=0;i<8;i++)
    Display_Buf[0][i]=Char_HighSet_Conductance_ED[i];  
 
  Display_Buf[0][8]=':';
  for(i=0;i<6;i++)
  {
    Display_Buf[2][i+9]   =    Char_Conductance[i]; 
  }
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    EDI_Dis(Flow);//显示设置数据
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
														/*设置参数退出*/
														AdminPra.EDIthres = Flow;
														Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: if(Flow<9999)
														Flow++;
													else
														Flow=0;  										
													 break;
							case KEY_DOWM:
													if(Flow>0)
													Flow--;
													else
														Flow=9999;
													break;
							case Long_UP:while(!key_Up)
													 {
														 if(Flow<9999)
																Flow++;
															else
																Flow=0; 
															
															EDI_Dis(Flow);//显示设置数据
															Display_RefreshAll();
													 }
													 break;
							case Long_DOWM:
													while(!key_Dowm)
													 {
														 if(Flow>0)
															Flow--;
															else
														  Flow=9999;
															
															EDI_Dis(Flow);//显示设置数据
															Display_RefreshAll();
													 }
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
    
  }
}


/*******************************************************************************
* 函 数 名: void Highset_Voltage_offset(void );
* 功    能: 电压偏移量系数
* 说    明: 
*******************************************************************************/
void Highset_Voltage_offset(u8 now)
{
  u8 i;
  u16  Flow;
  Flow       =   AdminPra.Voltage;

	key_event_lesure();//退出忙碌标记
  DisplayBuf_Clr();
  OppositeColor(2,2,5);
  for(i=0;i<10;i++)
    Display_Buf[0][i]=Char_HighSet[11][i];    
  Display_Buf[0][10]=':';
 
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    Display_Buf[2][4]= Char_Number[(u8)(Flow/1000)]; 
    Display_Buf[2][5]='.';
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
						key_event_buys();//进入忙碌标记
						/*这段区域内按键无效*/
						switch(key.KEY_Val)
						{
							case KEY_OFF_ON:break;
							case KEY_ENTER:
														/*设置参数退出*/
															AdminPra.Voltage= Flow;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 
												 if(Flow<9999)
													Flow++;	
												 else
													Flow=0; 								
													 break;
							case KEY_DOWM:
												 if(Flow>0)
													Flow--;
												else
													Flow=9999;
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}

  }
}


/*******************************************************************************
* 函 数 名: void Highset_Force_Stop_GetWater(void );
* 功    能: 耗材失效强制停止取水
* 说    明: 
*******************************************************************************/
void Highset_Force_Stop_GetWater(u8 now )
{
  u8 i;
  u16 force_stop;
  force_stop = AdminPra.Material_falg;
	
		key_event_lesure();//退出忙碌标记
  Clr_Pictrue();
  DisplayBuf_Clr( );
  OppositeColor(2,2,8);
  for(i=0;i<8;i++)
  {
    Display_Buf[0][i]   =    Char_HighSet[12][i]; 
  }  
  Display_Buf[0][8]   =  ':';
  Display_RefreshAll();
   while(1)
  { 
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
    if(force_stop)
    {
      for(i=0;i<8;i++)
      {
        Display_Buf[2][i+4]   =    Char_Force_StopGetWater[0][i]; 
      }
    }
    else
    {
      for(i=0;i<8;i++)
      {
        Display_Buf[2][i+4]   =    Char_Force_StopGetWater[1][i]; 
      }
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
														/*设置参数退出*/
															AdminPra.Material_falg = force_stop;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 										
													 break;
							case KEY_DOWM:
													if(force_stop) force_stop = 0;
													else           force_stop = 1;
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
  }
}

/*******************************************************************************
* 函 数 名: void Highset_RO_Conductance_Rang(void );
* 功    能: RO电导率量程
* 说    明: 
*******************************************************************************/
void Highset_RO_Conductance_Rang(u8 now)
{
  u8 i;
  u8 Rang;
  Rang = AdminPra.ROrange;
	key_event_lesure();//退出忙碌标记
  Clr_Pictrue();
  DisplayBuf_Clr( );
  OppositeColor(2,2,6);
  for(i=0;i<10;i++)
  {
    Display_Buf[0][i]   =    Char_HighSet[16][i]; 
  }  
  Display_Buf[0][10]   =  ':';
  Display_RefreshAll();
   while(1)
  {    
			#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 
      for(i=0;i<6;i++)
      {
        Display_Buf[2][i+4]   =    Char_RO_Conductance_Rang[Rang][i]; 
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
														/*设置参数退出*/
															AdminPra.ROrange = Rang;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 	
												 if(Rang<2)
													Rang++;
												else
													Rang=0;
													 break;
							case KEY_DOWM:
												 if(Rang>0)
													Rang--;
												else
													Rang=2;
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
  }
}



/*******************************************************************************
* 函 数 名: void Highset_EDI_Conductance_Rang(void );
* 功    能: RO电导率量程
* 说    明: 
*******************************************************************************/
void Highset_EDI_Conductance_Rang(u8 now)
{
  u8 i;
  u8 Rang;
  Rang = AdminPra.EDIrange;
	key_event_lesure();//退出忙碌标记
  Clr_Pictrue();
  DisplayBuf_Clr( );
  OppositeColor(2,2,6);
  for(i=0;i<10;i++)
  {
    Display_Buf[0][i]   =    Char_HighSet[17][i]; 
  }  
  Display_Buf[0][10]   =  ':';
  Display_RefreshAll();
   while(1)
  {    
#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 		
      for(i=0;i<6;i++)
      {
        Display_Buf[2][i+4]   =    Char_RO_Conductance_Rang[Rang][i]; 
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
														/*设置参数退出*/
															AdminPra.EDIrange = Rang;
															Seve_Admindata();
														 OppositeColor(0,now,10);								
														 return;															
							case KEY_UP: 	
												 if(Rang<2)
													Rang++;
												else
													Rang=0;
													 break;
							case KEY_DOWM:
												 if(Rang>0)
													Rang--;
												else
													Rang=2;
													break;
							case KEY_BANK: OppositeColor(0,now,10);								
										return;
							case KEY_EDI:   break;
							default:break;
						}
						key_event_lesure();//退出忙碌标记
				}
  }
}

/*******************************************************************************
* 函 数 名: void UserCodeJudge(void);
* 功    能: 管理员密码判断
* 说    明: 
*******************************************************************************/
u8  AdminCodeJudge(void)
{
  u8   i,rl=0,rl1=1;
  u8   password1=0,password2=0;
  u16  password;
  key_event_lesure();//退出忙碌标记		
  DisplayBuf_Clr();
  OppositeColor(3,2,1);
  for(i=0;i<11;i++)
    Display_Buf[1][i]=Char_InputCode[i];
	
  
  while(1)
  {
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
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
			    key_event_buys();//进入忙碌标记
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
								if(password == 100)
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
						case KEY_BANK: timer_event_Off=0; return 1;
						case Sleep_TM:Clr_Pictrue(); return 1;					
							default:
							break;
					}
						key_event_lesure();//退出忙碌标记		
				}
  }
}


extern u8  Sleep_Flag;
extern u8 OPEN_FLAG;
extern u8 admin_sleep_flag;
//管理员主界面
void Admin_Interface(void)
{

  u8 i;
  u8 MenuIndex;
  u8 CursorIndex;
  u8 DisplayIndex;
  u8 CursorIndex_per;
	key_event_lesure();//退出忙碌标记
	/*关闭所有输出*/
  DisplayBuf_Clr();
	Clr_Pictrue();
		//Display_RefreshAll();
	/*密码判断*/
	if(AdminCodeJudge())
	{
			return ;
	}
	DisplayBuf_Clr();
	Clr_Pictrue();
	
	/*密码判断*/
	admin_sleep_flag =0;//进入管理员界面 不休眠
	key_event_lesure();//退出忙碌标记
  CursorIndex_per=10;
  MenuIndex=0;     //对应菜单号0-16
  CursorIndex=0;   //对应光标号0-3
  DisplayIndex=0;  //对应显示号0-13
	
	/*按键清零*/
  while(1)
  {  
		#if VERSION_CONTROLLER
		Sotf_Versions_Controller();//版本控制器生效
		#endif 		
		//LCD_LightOn;
    for(i=0;i<10;i++)
    {
      Display_Buf[0][i]   =    Char_HighSet[DisplayIndex][i];
      Display_Buf[1][i]   =    Char_HighSet[DisplayIndex+1][i];
      Display_Buf[2][i]   =    Char_HighSet[DisplayIndex+2][i];
      Display_Buf[3][i]   =    Char_HighSet[DisplayIndex+3][i];
    }
    if(DisplayIndex!=0)  Display_Buf[0][15]=0x18;
    else Display_Buf[0][15]=' ';
    if(DisplayIndex!=10)  Display_Buf[3][15]=0x19;
    else Display_Buf[3][15]=' ';
    if(CursorIndex_per!=CursorIndex)
    {
      CursorIndex_per=CursorIndex;
      OppositeColor(0,CursorIndex,10);      
    }
    if(Display_Delay>=DELAY_DISPLAY)
    {
			/*使用定时器更新显示  10*DELAY_DISPLAY ms 更新一次*/
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
									case 0:Highset_OutputTest(CursorIndex);//进入控制点检测界面
													DisplayBuf_Clr();break;
									case 1:Highset_WaterTest(CursorIndex);//水质采样界面
													DisplayBuf_Clr();break;
									case 2:Highset_VoiceTest(CursorIndex);//语言检测													
													DisplayBuf_Clr();break;
									case 3:Highset_RO_Flow_Set(CursorIndex);//RO流量设置
												  DisplayBuf_Clr();break;
									case 4:Highset_UP_Flow_Set(CursorIndex);//UP流量设置
													DisplayBuf_Clr();break;
									case 5:Highset_ED_Flow_Set(CursorIndex);//ED流量设置
													DisplayBuf_Clr();break;
									case 6:Highset_Resistivity_ADJ_Set(CursorIndex); //UP电阻校准
													DisplayBuf_Clr();break;
									case 7:Highset_Conductance_ADJ_Set(CursorIndex);//Ro电导校准
													DisplayBuf_Clr();break;
									case 8:Highset_Conductance_source_ADJ_Set(CursorIndex);//SC电导校准
													DisplayBuf_Clr();break;
									case 9:Highset_Conductance_EDI_ADJ_Set(CursorIndex);//EDI电导校准
													DisplayBuf_Clr();break;
									case 10:Highset_Version_Set(CursorIndex);           //软件版本选择
													DisplayBuf_Clr();break; 
									case 11:Highset_WaterQuality_UP(CursorIndex);       //UP水质电阻下限
													DisplayBuf_Clr();break;
									case 12:Highset_WaterQuality_RO(CursorIndex);		     //RO电导上限		
													DisplayBuf_Clr();break;
									case 13:Highset_WaterQuality_ED(CursorIndex);		     //ED电导上限		
													DisplayBuf_Clr();break;
									case 14:Highset_Voltage_offset(CursorIndex);					//电压微调
													DisplayBuf_Clr();break;
									case 15:Highset_Force_Stop_GetWater(CursorIndex);			//耗材失效
													DisplayBuf_Clr();break;
									case 16:Highset_RO_Conductance_Rang(CursorIndex);			//RO电导量程
													DisplayBuf_Clr();break;
									case 17:Highset_EDI_Conductance_Rang(CursorIndex);			//RO电导量程
									  			DisplayBuf_Clr();break;
									case 18:Highset_WaterQuality_XH(CursorIndex);			       //循环上限电阻
									  			DisplayBuf_Clr();break;
									case 19:Highset_WaterQuality_XH_TM(CursorIndex);			       //循环上限电阻
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
													DisplayIndex=16;	
													MenuIndex		= 19;//选择了第十三条目录											
												}else DisplayIndex--; 
										}
										else 
										{
											CursorIndex--;	//光标减 对应光标号0-3	
											MenuIndex--;
										}											
										
									break;
					case KEY_DOWM:
										CursorIndex++;	//光标加 对应光标号0-3	
										MenuIndex++;
										if(CursorIndex>=4)
										{
											DisplayIndex++; 
											CursorIndex = 3;
										}										
										if(DisplayIndex>16) 
										{
											DisplayIndex=0;
											CursorIndex=0;
											MenuIndex=0;
										}	break;
					case KEY_BANK:   OPEN_FLAG = 0;
														__set_FAULTMASK(1); // 关闭所有中断
														NVIC_SystemReset(); // 复位
														break;
					case KEY_EDI:break;
					default:break;
				}
				key_event_lesure();//退出忙碌标记
		}

  }
  
}



void go_admin(void)
{
	if(timer_event_Off)
	{
		Admin_Interface();
	}
}

