#ifndef __FLASHMANEGE_H
#define __FLASHMANEGE_H
#include "sys.h"



#define UserPraAddr   0x08030000   //�û����ò����洢λ��
#define AdminPraAddr  0x08030800   //����Ա���ò����洢λ��
#define DataAddr      0x08031000   //�洢ȡˮ��ʷ������ʼλ��
#define Data_Count_Addr      0x08035000//���� 
#define Data_Count_flag_Addr 0x08036000//��ǰȡˮ���ݱ�� 
#define ALL_UP_Warte_Addr    0x08037000//���UP��ȡˮ�� 2��16λ���� ���ģʽ
#define ALL_RO_Warte_Addr    0x08037100//���RO��ȡˮ��
#define ALL_EDI_Warte_Addr    0x08037200//���EDI��ȡˮ��
/*Ro ��2byte��  ȡˮ����2byte�� ȡˮʱ�䣨8byte��   �絼�ʣ�2byte�� �գ�2byte��  
* EDI��2byte��  ȡˮ����2byte�� ȡˮʱ�䣨8byte��   �絼�ʣ�2byte�� �գ�2byte��
* UP ��2byte��  ȡˮ����2byte�� ȡˮʱ�䣨8byte��   �����ʣ�2byte�� TOC��2byte��
* ÿ������14byte 
*/
#define MaxData 400
#define UP_Data 0
#define RO_Data 1
#define EDI_Data 2



/*�û������ṹ��*/
typedef struct Usepra
{
	u8 OnTimer[2];//����ʱ��OnTimer[0] ʱ  OnTimer[1] ��  
	u8 OffTimer[2];//�ػ�ʱ��OnTimer[0] ʱ  OnTimer[1] ��
	u16 RetionUP;//����ȡUPˮ
	u16 RetionRO;//����ȡROˮ
	u16 RetionEDI;//����ȡEDIˮ	
	u16 PassWord;//�û���������
	u8  PassWordFlag;//�Ƿ�����������  0�ر� 1��
	u8  OnOffFlag;//�Ƿ�򿪶�ʱ���ػ� ����  0�ر� 1��
}Usepra,*pUsepra;



/*����Ա�����ṹ��*/
typedef struct Adminpra
{
	 u16 Soft_Version;// ����汾��   0 A�汾  1 B�汾  2 C�汾
	 u16 FlowUPSet;  //UP��������    xxxML/1min   �����һ������Ҫ��õ�ʱ��ų���
	 u16 FlowROSet;  //RO��������    xxxML/1min   �����һ������Ҫ��õ�ʱ��ų���
	 u16 FlowEDISet;//EDI��������    xxxML/1min   �����һ������Ҫ��õ�ʱ��ų���
	 s16 UPpra;//UP����У׼����    ��ʾ����  = ʵ��ֵ +��UPpra/100��
	 s16 ROpra;//RO�絼У׼����    ��ʾ����  = ʵ��ֵ -��UPpra/100��
	 s16 EDIpra;//EDI�絼У׼����    ��ʾ����  = ʵ��ֵ -��UPpra/100��
	 u16 UPthres;//UP��������       ʵ��ֵ  = UPthres/100
	 u16 ROthres;//RO�絼����       ʵ��ֵ  = ROthres/100
	 u16 EDIthres;//EDI�絼����     ʵ��ֵ  = EDIthres/100
	 u16 ROrange;//RO�絼����       0 0-20  1  0-200   2 0-2000
	 u16 EDIrange;//EDI�絼����     0 0-20  1  0-200   2 0-2000
	 u16 Voltage;//��ѹ΢��
	 u16  Material_falg;//0ֻ��ʾ     1��ֹȡˮ
	 s16  SRpra;//SR�絼У׼����    ��ʾ����  = ʵ��ֵ -��UPpra/100��
	 u16 XHthres;//ѭ����������      ʵ��ֵ  = XHthres/100
	 u16 XH_Flag;//ѭ�������Ƿ��   0�ر�  1 ��
	 u16 XH_Timer;//ѭ��ʱ��
}Adminpra,*pAdminpra;



extern Usepra  UserPra;//�û�ȫ�ֱ�������
extern Adminpra AdminPra;//����ȫ�ֱ�������
void Seve_Userdata(void);//���������û�����
void Read_Userdata(void);//���������û�����
void Writer_GetWater_Data(u8 waterType,u16 water,u16 par1,u16 par2);//�洢ȡˮ����
void Read_Admindata(void);//����flash  �����еĲ�������
void Seve_Admindata(void);//�������й���Ա������flash  ��
void Clean_GetWater_Data(void);//���flash �е���ʷ��¼
#endif


