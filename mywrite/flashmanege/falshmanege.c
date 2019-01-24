#include "flashmanege.h"
#include "stmflash.h"
#include "DS1302.h"
Usepra  UserPra;
Adminpra AdminPra;



/*******************************************************************************
* 函 数 名: void Seve_Userdata(void)
* 功    能: 保存用户所有参数到falsh 中
* 说    明: 
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
* 函 数 名: void Read_Userdata(void)
* 功    能: 读取用户参数到结构体中
* 说    明: 
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
* 函 数 名: void Writer_GetWater_Data(u8 waterType,u16 water,u8 *Timer,float par1,float par2)
* 功    能: 向flash写入取数历史记录
* 说    明: par1   par2 为测得AD值，取出数据时需要还原 
*******************************************************************************/
void Writer_GetWater_Data(u8 waterType,u16 water,u16 par1,u16 par2)
{
		u16 temp[12]={0};
		u16 temp1[12]={0};
		u16 NowCount=0;
		u8 i=0,j;
		BurstRead1302(Ds1302DataBuf);//获取时间
		
		temp[i++] = (u16)waterType;//取水类型
		temp[i++] = (u16)water;    //取水量
		for(j=0;j<8;j++)
		temp[i++] =(u16) Ds1302DataBuf[j];//取水时间
		temp[i++] = par1;    //电导率或者电阻率
		temp[i++] = par2;    //TOC   只有UP有这个参数
		if(STMFLASH_ReadHalfWord(Data_Count_Addr) == 0xffff)
			Test_Write(Data_Count_Addr,0);
		if(STMFLASH_ReadHalfWord(Data_Count_flag_Addr) == 0xffff)
			Test_Write(Data_Count_flag_Addr,0);
		
		NowCount = STMFLASH_ReadHalfWord(Data_Count_Addr);
		if(NowCount==MaxData)
		{
			Test_Write(Data_Count_flag_Addr,1); //记录满了400条数据
			Test_Write(Data_Count_Addr,0);			//继续从0开始写入
		}
		else Test_Write(Data_Count_Addr,NowCount+1);//指向下一条数据地址
		STMFLASH_Write(DataAddr+(NowCount*24),temp,12);
		STMFLASH_Read(DataAddr+(NowCount*24),temp1,12);
}
/*******************************************************************************
* 函 数 名: void Clean_GetWater_Data(void)
* 功    能: 清除flash中的历史记录
* 说    明: 
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
* 函 数 名: void Seve_Admindata(void)
* 功    能: 保存管理员所有参数到falsh 中
* 说    明: 
*******************************************************************************/
void Seve_Admindata(void)
{
		u16 temp=0;
	  temp = sizeof(AdminPra)/2;
		STMFLASH_Write(AdminPraAddr,&AdminPra.Soft_Version,temp);//将结构体中数据写入flash 中
}
/*******************************************************************************
* 函 数 名: void Read_Admindata(void)
* 功    能: 从flash中读出所有的管理员参数
* 说    明: 
*******************************************************************************/
void Read_Admindata(void)
{
		u16 temp=0,i;
		u16 *head;
		u32 addr= AdminPraAddr;
	  temp = sizeof(AdminPra)/2;
	  head = &AdminPra.Soft_Version;
		/*判断flash中数据是否有效*/
		for(i=0;i<temp;i++)
		{
				if(STMFLASH_ReadHalfWord(addr) == 0xffff)
					Test_Write(addr,0);
				addr+=2;
		}
		/*读出所有参数到结构体中*/
		addr= AdminPraAddr;
		for(i=0;i<temp;i++)
		{
			*head++ = STMFLASH_ReadHalfWord(addr);
			addr+=2;	
		}
}

