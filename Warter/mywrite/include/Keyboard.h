
#ifndef __KEYBORAD__H__
#define __KEYBORAD__H__


#include "sys.h"
typedef  struct KEY
{
	u8 KEY_Falg;//按键事件标记有按键  按下为1无按键按下为0
	u8 KEY_Val;//按键值   
	/*
		bit0 -->key_Off_On 
		bit1 -->key_Enter 
		bit2 -->key_Up 
		bit3 -->key_Dowm 
		bit4 -->key_Edi 
	*/
	u8 KEY_Mode;//按键模式  0单按  1长按  2组合键
	u8 KEY_Shadow;
	u8 KEY_Count;
	u8 Event_Busy;
}KEY,*pKEY;
extern KEY key;


#define key_delay_Timer  50  //长按




#define key_Off_On PCin(4)	// Pc4
#define key_Enter  PCin(5)	// Pc5   
#define key_Up     PCin(6)	// Pc6   
#define key_Dowm   PCin(7)	// Pc7   
#define key_Bank   PCin(8)	// Pc8   
#define key_Edi    PCin(9)	// Pc8 


/*单独按的时候按键值*/
#define KEY_OFF_ON 0x01
#define KEY_BANK  0x02
#define KEY_UP     0x04
#define  KEY_DOWM  0x08
#define KEY_ENTER   0x10
#define KEY_EDI    0x20
#define Long_UP    0x40
#define Long_DOWM  0x80
#define Sleep_TM   0x03
extern u8 timer_event_Off;
void InitKeyboard(void);//按键初始化
void key_Event(void);   //定时器调用
void key_event_lesure(void);
void key_event_buys(void);


#endif   /* __KEYBORAD__H__ */


