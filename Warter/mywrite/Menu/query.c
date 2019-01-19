#include "query.h"
#include "Dis12864.h"
#include "Timer.h"
#include "keyboard.h"
#include "APP.h"
#include "flashmanege.h"
#include "my_adc.h"
#include "IO.h"
extern  const unsigned char Char_CheckMenu[5][8];
extern const unsigned char Char_SC_Conductance[11];
extern const unsigned char Char_Conductance[5] ;
extern const unsigned char Char_SensorBreak_SC[12];
extern const unsigned char Char_Check_Wash_per[2][10];
extern const unsigned char Char_Check_PressPump[2][12];
extern const unsigned char Char_Check_Wash[2][10];
extern const unsigned char Char_Check_PressPump2[2][12];
extern const unsigned char Char_Check_PressPump1[2][12];
extern const unsigned char Char_Check_InputWater[2][12];
extern const unsigned char Char_Check_PressCan[2][12];
extern const unsigned char Char_Check_WaterLevel[5][12];
extern const unsigned char Char_CompanyInfor[4][16];
extern const unsigned char Char_versionInfor[9];
extern const unsigned char Char_versionNum[3][10];
extern const unsigned char Char_Resistivity[6] ;


/*********************************�����˵�*************************************/
/*******************************************************************************
* �� �� ��: void Check_SC_Conductance(void)
* ��    ��: Դˮ�絼�ʲ�ѯ
* ˵    ��: 
*******************************************************************************/
void Check_SC_Conductance(u8  now)
{
  u8 i;
	float quantify;
	u8 cont=0;
  DisplayBuf_Clr();
  Clr_Pictrue();
	key_event_lesure();//�˳�æµ���
  for(i=0;i<11;i++)
    Display_Buf[0][i]  =  Char_SC_Conductance[i];
  while(1)
  {
		#if QUERY_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 
      
			if(GET_ADVAL.State3)//������ڸ�ģ��
			{
				DisplayBuf_ClrRow(2);		
				DisplayBuf_ClrRow(1);
				DisplayFloat( 1,1,quantify);	
				for(i=0;i<6;i++)
				{
					Display_Buf[1][i+9]   =    Char_Resistivity[i]; 
				}
			}
			else 
			{
				for(i=0;i<12;i++)
        Display_Buf[2][i+2]   = Char_SensorBreak_SC[i];
			}
    if(Display_Delay>=DELAY_DISPLAY)
    {
				if(GET_ADVAL.State3)//������ڸ�ģ��
				{
					cont++;
					if(cont == 50)
					{
						cont =0 ;
						quantify = GET_ADVAL.GetSR_AD();
					}
				}
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
* �� �� ��: void Check_Output_VA1_0(void)
* ��    ��: 1.0�汾A�������ѯ
* ˵    ��: 
*******************************************************************************/
void Check_Output_VA1_0(void)
{
  u8 i;
  if((GPIOC->ODR &(1<<15))==(1<<15))
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_PressPump[1][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_PressPump[0][i];
  }
  
  if((GPIOC->ODR &(1<<14))==(1<<14))
  {
    for(i=0;i<10;i++)
      Display_Buf[1][i]   =    Char_Check_Wash_per[1][i];
  }
  else
  {
    for(i=0;i<10;i++)
      Display_Buf[1][i]   =    Char_Check_Wash_per[0][i];
  }
  
  if((GPIOA->ODR &(1<<11))==(1<<11))
  {
    for(i=0;i<10;i++)
      Display_Buf[2][i]   =    Char_Check_Wash[1][i];
  }
  else
  {
    for(i=0;i<10;i++)
      Display_Buf[2][i]   =    Char_Check_Wash[0][i];
  } 
}


/*******************************************************************************
* �� �� ��: void Check_Output_VB1_0(void)
* ��    ��: 1.0�汾B�������ѯ
* ˵    ��: 
*******************************************************************************/
void Check_Output_VB1_0(void)
{
  u8 i;
  if((GPIOC->ODR &(1<<15))==(1<<15))
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_PressPump2[1][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_PressPump2[0][i];
  }
  
  if((GPIOC->ODR &(1<<13))==(1<<13))
  {
    for(i=0;i<12;i++)
      Display_Buf[1][i]   =    Char_Check_PressPump1[1][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[1][i]   =    Char_Check_PressPump1[0][i];
  }

  if((GPIOA->ODR &(1<<11))==(1<<11))
  {
    for(i=0;i<10;i++)
      Display_Buf[2][i]   =    Char_Check_Wash[1][i];
  }
  else
  {
    for(i=0;i<10;i++)
      Display_Buf[2][i]   =    Char_Check_Wash[0][i];
  } 
}

void Check_Output_VC1_0(void)
{
	u8 i;
  if((GPIOC->ODR &(1<<15))==(1<<15))
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_PressPump[1][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_PressPump[0][i];
  }
  
  if((GPIOC->ODR &(1<<14))==(1<<14))
  {
    for(i=0;i<10;i++)
      Display_Buf[1][i]   =    Char_Check_Wash_per[1][i];
  }
  else
  {
    for(i=0;i<10;i++)
      Display_Buf[1][i]   =    Char_Check_Wash_per[0][i];
  }
  
  if((GPIOA->ODR &(1<<11))==(1<<11))
  {
    for(i=0;i<10;i++)
      Display_Buf[2][i]   =    Char_Check_Wash[1][i];
  }
  else
  {
    for(i=0;i<10;i++)
      Display_Buf[2][i]   =    Char_Check_Wash[0][i];
  } 
}
/*******************************************************************************
* �� �� ��: void Check_Output(void)
* ��    ��: ���״̬��ѯ
* ˵    ��: 
*******************************************************************************/
void Check_Output(u8 now)
{
	u8  VersionNumber;
	VersionNumber = AdminPra.Soft_Version;
  DisplayBuf_Clr();
	key_event_lesure();//�˳�æµ���
  Clr_Pictrue();

  while(1)
  {
		#if QUERY_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
    switch(VersionNumber)
    {
    case 0:
      Check_Output_VA1_0( );
    break;
    case 1:
      Check_Output_VB1_0( );          
    break;
		case 2:
      Check_Output_VC1_0( );          
    break;
    default:
    break;
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
* �� �� ��: void Check_Input_VA1_0(void)
* ��    ��: 1.0�汾A�������ѯ
* ˵    ��: 
*******************************************************************************/
void Check_Input_VA1_0(void)
{
  u8 i;
  if(L_F)
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_InputWater[0][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_InputWater[1][i];
  }
  
  if(H_F)
  {
    for(i=0;i<12;i++)
      Display_Buf[1][i]   =    Char_Check_PressCan[0][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[1][i]   =    Char_Check_PressCan[1][i];
  }
}


/*******************************************************************************
* �� �� ��: void Check_Input_VB1_0(void)
* ��    ��: 1.0�汾B�������ѯ
* ˵    ��: 
*******************************************************************************/
void Check_Input_VB1_0(void)
{
  u8 i;
	u8 in_leve =0;
	if(H_Level)	in_leve =0;	
	else if(M_Level)in_leve =1;
	else if(L_Level)in_leve =2;
	else if(!L_Level) in_leve =4;
	else if(L_F)in_leve =3; 
		
  if(L_F)
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_InputWater[0][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_InputWater[1][i];
  }
  
  if(H_F)
  {
    for(i=0;i<12;i++)
      Display_Buf[1][i]   =    Char_Check_PressCan[0][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[1][i]   =    Char_Check_PressCan[1][i];
  }
  
  for(i=0;i<12;i++)
    Display_Buf[2][i]   =    Char_Check_WaterLevel[in_leve][i];
  
}
void Check_Input_VC1_0(void)
{
	u8 i;
	u8 in_leve =0;
	if(H_Level)	in_leve =0;	
	else if(M_Level)in_leve =1;
	else if(L_Level)in_leve =2;
	else if(!L_Level) in_leve =4;
	else if(L_F)in_leve =3; 
		
  if(L_F)
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_InputWater[0][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[0][i]   =    Char_Check_InputWater[1][i];
  }
  
  if(H_F)
  {
    for(i=0;i<12;i++)
      Display_Buf[1][i]   =    Char_Check_PressCan[0][i];
  }
  else
  {
    for(i=0;i<12;i++)
      Display_Buf[1][i]   =    Char_Check_PressCan[1][i];
  }
  
  for(i=0;i<12;i++)
    Display_Buf[2][i]   =    Char_Check_WaterLevel[in_leve][i];
  
}
/*******************************************************************************
* �� �� ��: void Check_Input(void)
* ��    ��: ����״̬��ѯ/������汾�йأ��ı�����ӹ�����Ҫ�޸Ĵ˺���
* ˵    ��: 
*******************************************************************************/
void Check_Input(u8 now)
{  
	u8 VersionNumber=0;
	VersionNumber = AdminPra.Soft_Version;
  DisplayBuf_Clr();
  Clr_Pictrue();
	key_event_lesure();//�˳�æµ���
  while(1)
  {
		#if QUERY_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
    switch(VersionNumber)
    {
    case 0:
      Check_Input_VA1_0( );
    break;
    case 1:
      Check_Input_VB1_0( );    
    break;
		case 2:
      Check_Input_VC1_0( );    
    break;
    default:
    break;
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


void CompanyInformation(u8 now)
{
  u8 i;
	key_event_lesure();//�˳�æµ���
  DisplayBuf_Clr();
  Clr_Pictrue();
  for(i=0;i<16;i++)
  {
    Display_Buf[0][i]   =    Char_CompanyInfor[0][i];
    Display_Buf[1][i]   =    Char_CompanyInfor[1][i];
    Display_Buf[2][i]   =    Char_CompanyInfor[2][i];
    Display_Buf[3][i]   =    Char_CompanyInfor[3][i];
  }
  Display_RefreshAll();
  while(1)
  {
    #if QUERY_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
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

void SoftwareInformation(u8 now)
{
  u8 i;
	u8 VersionNumber=0;
	VersionNumber = AdminPra.Soft_Version;
	key_event_lesure();//�˳�æµ���
  DisplayBuf_Clr();
  Clr_Pictrue();
  for(i=0;i<9;i++)
  {
    Display_Buf[1][i]   =    Char_versionInfor[i];
  }
  for(i=0;i<8;i++)
  {
    Display_Buf[2][i+4] =     Char_versionNum[VersionNumber][i];
  }
  Display_RefreshAll();
  while(1)
  {
    #if QUERY_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 	
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
* �� �� ��: void Menu_UserCheck(void)
* ��    ��: һ���˵�
* ˵    ��: 
*******************************************************************************/
void Menu_UserCheck(void)
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
#if QUERY_VERSION_CONTROLLER
		Sotf_Versions_Controller();//�汾��������Ч
		#endif 			
    for(i=0;i<8;i++)
    {
      Display_Buf[0][i+4]   =    Char_CheckMenu[DisplayIndex][i];
      Display_Buf[1][i+4]   =    Char_CheckMenu[DisplayIndex+1][i];
      Display_Buf[2][i+4]   =    Char_CheckMenu[DisplayIndex+2][i];
      Display_Buf[3][i+4]   =    Char_CheckMenu[DisplayIndex+3][i];
    }
    if(DisplayIndex!=0)  Display_Buf[0][15]=0x18;
    else Display_Buf[0][15]=' ';
    if(DisplayIndex!=1)  Display_Buf[3][15]=0x19;
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
									case 0:Check_Output(CursorIndex);//���״̬��ѯ
													DisplayBuf_Clr();break;
									case 1:Check_Input(CursorIndex); //����״̬��ѯ
													DisplayBuf_Clr();break;
									case 2:Check_SC_Conductance(CursorIndex);//��ˮˮԴ��ѯ										
													DisplayBuf_Clr();break;
									case 3:CompanyInformation(CursorIndex);//��˾��Ϣ��ѯ
												  DisplayBuf_Clr();break;
									case 4:SoftwareInformation(CursorIndex);//����汾��ѯ
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
					case KEY_BANK:
												DisplayBuf_Clr();
												Clr_Pictrue();
												OppositeColor((1*3),3,4);
											return ;
					case KEY_EDI:break;
					default:break;
				}
				key_event_lesure();//�˳�æµ���
		}
   
  }
}

