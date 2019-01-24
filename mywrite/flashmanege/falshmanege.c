#include "flashmanege.h"
#include "stmflash.h"
#include "DS1302.h"
Usepra  UserPra;
Adminpra AdminPra;



/*******************************************************************************
* �� �� ��: void Seve_Userdata(void)
* ��    ��: �����û����в�����falsh ��
* ˵    ��: 
*******************************************************************************/
void Seve_Userdata(void)
{
		u8 i=0;
		u16 Indata[10]={0};
		Indata[i++] = UserPra.OnTimer[0];
		Indata[i++] = UserPra.OnTimer[1];
		Indata[i++] = UserPra.OffTimer[0];
		Indata[i++] = UserPra.OffTimer[1];
		Indata[i++] = UserPra.RetionUP;
		Indata[i++] = UserPra.RetionRO;
	  Indata[i++] = UserPra.RetionEDI;
		Indata[i++] = UserPra.PassWord;
		Indata[i++] = UserPra.PassWordFlag;
	  Indata[i++] = UserPra.OnOffFlag;
		STMFLASH_Write(UserPraAddr,Indata,10);
}
/*******************************************************************************
* �� �� ��: void Read_Userdata(void)
* ��    ��: ��ȡ�û��������ṹ����
* ˵    ��: 
*******************************************************************************/
void Read_Userdata(void)
{
		u8 i=0;
		u16 Outdata[10]={0};
		STMFLASH_Read(UserPraAddr,Outdata,10);
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.OnTimer[0]= (u8)Outdata[i++];
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.OnTimer[1]= (u8)Outdata[i++];
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.OffTimer[0] = (u8)Outdata[i++];		
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.OffTimer[1] = (u8)Outdata[i++];		
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.RetionUP    = Outdata[i++];		
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.RetionRO		= Outdata[i++];		
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.RetionEDI		= Outdata[i++];		
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.PassWord		= Outdata[i++];		
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.PassWordFlag= (u8)Outdata[i++];		
		if(Outdata[i] == 0xffff)
		{
			UserPra.OnTimer[0] =0;
			i++;
		}
		else UserPra.OnOffFlag		= (u8)Outdata[i++];		
		if(UserPra.OnOffFlag)
		{
			if((UserPra.OffTimer[0] == UserPra.OnTimer[0])&&(UserPra.OffTimer[1] == UserPra.OnTimer[1]))
			{
				UserPra.OnOffFlag = 0;
				Seve_Userdata();
			}
		}
	if(UserPra.PassWordFlag == 0)
	{
		UserPra.PassWord = 0;
		Seve_Userdata();
	}
		i++;
}	
extern u8 Ds1302DataBuf[8];
/*******************************************************************************
* �� �� ��: void Writer_GetWater_Data(u8 waterType,u16 water,u8 *Timer,float par1,float par2)
* ��    ��: ��flashд��ȡ����ʷ��¼
* ˵    ��: par1   par2 Ϊ���ADֵ��ȡ������ʱ��Ҫ��ԭ 
*******************************************************************************/
void Writer_GetWater_Data(u8 waterType,u16 water,u16 par1,u16 par2)
{
		u16 temp[12]={0};
		u16 temp1[12]={0};
		u16 NowCount=0;
		u8 i=0,j;
		BurstRead1302(Ds1302DataBuf);//��ȡʱ��
		
		temp[i++] = (u16)waterType;//ȡˮ����
		temp[i++] = (u16)water;    //ȡˮ��
		for(j=0;j<8;j++)
		temp[i++] =(u16) Ds1302DataBuf[j];//ȡˮʱ��
		temp[i++] = par1;    //�絼�ʻ��ߵ�����
		temp[i++] = par2;    //TOC   ֻ��UP���������
		if(STMFLASH_ReadHalfWord(Data_Count_Addr) == 0xffff)
			Test_Write(Data_Count_Addr,0);
		if(STMFLASH_ReadHalfWord(Data_Count_flag_Addr) == 0xffff)
			Test_Write(Data_Count_flag_Addr,0);
		
		NowCount = STMFLASH_ReadHalfWord(Data_Count_Addr);
		if(NowCount==MaxData)
		{
			Test_Write(Data_Count_flag_Addr,1); //��¼����400������
			Test_Write(Data_Count_Addr,0);			//������0��ʼд��
		}
		else Test_Write(Data_Count_Addr,NowCount+1);//ָ����һ�����ݵ�ַ
		STMFLASH_Write(DataAddr+(NowCount*24),temp,12);
		STMFLASH_Read(DataAddr+(NowCount*24),temp1,12);
}
/*******************************************************************************
* �� �� ��: void Clean_GetWater_Data(void)
* ��    ��: ���flash�е���ʷ��¼
* ˵    ��: 
*******************************************************************************/
void Clean_GetWater_Data(void)
{
		u16 temp[2]={0};
		Test_Write(Data_Count_Addr,0);
		Test_Write(Data_Count_flag_Addr,0);
		STMFLASH_Write(ALL_UP_Warte_Addr,temp,2);
		STMFLASH_Write(ALL_RO_Warte_Addr,temp,2);
		STMFLASH_Write(ALL_EDI_Warte_Addr,temp,2);
}



/*******************************************************************************
* �� �� ��: void Seve_Admindata(void)
* ��    ��: �������Ա���в�����falsh ��
* ˵    ��: 
*******************************************************************************/
void Seve_Admindata(void)
{
		u16 temp=0;
	  temp = sizeof(AdminPra)/2;
		STMFLASH_Write(AdminPraAddr,&AdminPra.Soft_Version,temp);//���ṹ��������д��flash ��
}
/*******************************************************************************
* �� �� ��: void Read_Admindata(void)
* ��    ��: ��flash�ж������еĹ���Ա����
* ˵    ��: 
*******************************************************************************/
void Read_Admindata(void)
{
		u16 temp=0,i;
		u16 *head;
		u32 addr= AdminPraAddr;
	  temp = sizeof(AdminPra)/2;
	  head = &AdminPra.Soft_Version;
		/*�ж�flash�������Ƿ���Ч*/
		for(i=0;i<temp;i++)
		{
				if(STMFLASH_ReadHalfWord(addr) == 0xffff)
					Test_Write(addr,0);
				addr+=2;
		}
		/*�������в������ṹ����*/
		addr= AdminPraAddr;
		for(i=0;i<temp;i++)
		{
			*head++ = STMFLASH_ReadHalfWord(addr);
			addr+=2;	
		}
}

