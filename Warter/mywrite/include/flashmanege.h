#ifndef __FLASHMANEGE_H
#define __FLASHMANEGE_H
#include "sys.h"



#define UserPraAddr   0x08030000   //用户设置参数存储位置
#define AdminPraAddr  0x08030800   //管理员设置参数存储位置
#define DataAddr      0x08031000   //存储取水历史数据起始位置
#define Data_Count_Addr      0x08035000//超出 
#define Data_Count_flag_Addr 0x08036000//当前取水数据标号 
#define ALL_UP_Warte_Addr    0x08037000//存放UP总取水量 2个16位数据 大端模式
#define ALL_RO_Warte_Addr    0x08037100//存放RO总取水量
#define ALL_EDI_Warte_Addr    0x08037200//存放EDI总取水量
/*Ro （2byte）  取水量（2byte） 取水时间（8byte）   电导率（2byte） 空（2byte）  
* EDI（2byte）  取水量（2byte） 取水时间（8byte）   电导率（2byte） 空（2byte）
* UP （2byte）  取水量（2byte） 取水时间（8byte）   电阻率（2byte） TOC（2byte）
* 每条数据14byte 
*/
#define MaxData 400
#define UP_Data 0
#define RO_Data 1
#define EDI_Data 2



/*用户参数结构体*/
typedef struct Usepra
{
	u8 OnTimer[2];//开机时间OnTimer[0] 时  OnTimer[1] 分  
	u8 OffTimer[2];//关机时间OnTimer[0] 时  OnTimer[1] 分
	u16 RetionUP;//定量取UP水
	u16 RetionRO;//定量取RO水
	u16 RetionEDI;//定量取EDI水	
	u16 PassWord;//用户设置密码
	u8  PassWordFlag;//是否开启设置密码  0关闭 1打开
	u8  OnOffFlag;//是否打开定时开关机 功能  0关闭 1打开
}Usepra,*pUsepra;



/*管理员参数结构体*/
typedef struct Adminpra
{
	 u16 Soft_Version;// 软件版本号   0 A版本  1 B版本  2 C版本
	 u16 FlowUPSet;  //UP流量设置    xxxML/1min   计算出一毫升需要多久的时间放出来
	 u16 FlowROSet;  //RO流量设置    xxxML/1min   计算出一毫升需要多久的时间放出来
	 u16 FlowEDISet;//EDI流量设置    xxxML/1min   计算出一毫升需要多久的时间放出来
	 s16 UPpra;//UP电阻校准参数    显示参数  = 实际值 +（UPpra/100）
	 s16 ROpra;//RO电导校准参数    显示参数  = 实际值 -（UPpra/100）
	 s16 EDIpra;//EDI电导校准参数    显示参数  = 实际值 -（UPpra/100）
	 u16 UPthres;//UP电阻上限       实际值  = UPthres/100
	 u16 ROthres;//RO电导下限       实际值  = ROthres/100
	 u16 EDIthres;//EDI电导下限     实际值  = EDIthres/100
	 u16 ROrange;//RO电导量程       0 0-20  1  0-200   2 0-2000
	 u16 EDIrange;//EDI电导量程     0 0-20  1  0-200   2 0-2000
	 u16 Voltage;//电压微调
	 u16  Material_falg;//0只提示     1禁止取水
	 s16  SRpra;//SR电导校准参数    显示参数  = 实际值 -（UPpra/100）
	 u16 XHthres;//循环电阻上限      实际值  = XHthres/100
	 u16 XH_Flag;//循环功能是否打开   0关闭  1 打开
	 u16 XH_Timer;//循环时间
}Adminpra,*pAdminpra;



extern Usepra  UserPra;//用户全局变量声明
extern Adminpra AdminPra;//管理全局变量声明
void Seve_Userdata(void);//保存所有用户参数
void Read_Userdata(void);//读出所有用户参数
void Writer_GetWater_Data(u8 waterType,u16 water,u16 par1,u16 par2);//存储取水数据
void Read_Admindata(void);//读出flash  中所有的参数设置
void Seve_Admindata(void);//保存所有管理员参数到flash  中
void Clean_GetWater_Data(void);//清除flash 中的历史记录
#endif


