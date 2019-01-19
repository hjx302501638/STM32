/* ********** Copyright (C) 2010 Chengdu Reliance Eltctric Co., Ltd. ***********
* FileName     : Display.h
* Author       : LGQ
* Version      : 1.0
* Date         : 2010.11.1
* Description  : Head file of Display.c
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY__H__
#define __DISPLAY__H__

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/



/* Exported constants --------------------------------------------------------*/
const unsigned char Char_Space[]=
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const unsigned char Char_Degree[]=
{0x00,0x00,0x00,0x00,0x10,0x00,0x28,0x00,0x13,0x80,0x04,0x40,0x08,0x00,0x08,0x00,
 0x08,0x00,0x08,0x00,0x04,0x40,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

const unsigned char Char_Number[16]=
{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const unsigned char Char_Week[7][6]       =   {"星期一",
                                               "星期二",
                                               "星期三",
                                               "星期四",
                                               "星期五",
                                               "星期六",
                                               "星期天",
                                               };
const unsigned char Char_CompanyInfor[4][16]  =   { "四川沃特尔水处理",
                                                    "  设备有限公司  ",
                                                    "  400-626-4939  ",
                                                    "www.sc-woter.com",                                                    
                                                  };

const unsigned char Char_versionNum[3][10]    =   {
                                                  "WTcnA_V1.5",
                                                  "WTcnB_V1.5",
																									"WTcnC_V1.5",
                                                  };
const unsigned char Char_ChangeWarn[2][16]   ={"请更换RO反渗透膜",
                                               "  请更换耗材    ",
                                              };
const unsigned char Char_versionInfor[9]  =   {"版本信息:"};

const unsigned char Char_InputCode[11]    =   {"请输入密码:"};
const unsigned char Char_InputNewCode[13] =   {"请输入新密码:"};
const unsigned char Char_StartPassWord[2][14] =   {"开机密码  开启",
                                                   "开机密码  关闭",};
const unsigned char Char_XUfunction[2][4]={
																					"开启",
																					"关闭",
																					};
const unsigned char Char_Wrong[4]         =   {"错误"};
const unsigned char Char_SureClearData[13]  = {"确认清除数据?"};
const unsigned char Char_SureChange[15]     = {"确认已更换耗材?"};
const unsigned char Char_YesNo[2][2]        = {"是","否"};
const unsigned char Char_SoftwareVersion[6] = {"版本: "};
const unsigned char Char_VoiceTest[5]       = {"语音:"};

const unsigned char Char_ProductionDate[9]= {"生产日期:"};
const unsigned char Char_ProductionDate_Num[10]= {"2011.09.10"};


const unsigned char Char_UserMenu[9][8]  = {"手动冲洗", 
                                            "耗材更新",
                                            "历史查询",
                                            "时钟设置",
                                            "密码修改",
                                            "子机地址",
                                            "定时开机",
                                            "定时关机",
																						"循环功能",
                                            };

const unsigned char Char_CheckMenu[5][8] = {"输出状态",
                                            "输入状态",
                                            "进水水源",
                                            "公司信息",
                                            "软件版本",
                                            };
const unsigned char Char_CurrentMenu[16]  =   {"取水  查询  设置"};

const unsigned char Char_WordState[6]     =   {"状态："};
const unsigned char Char_CurrentState[7][10]= {"纯水机缺水",
                                               "纯水机冲洗",
                                               "纯水机制水",
                                               "纯水机备水",
                                               "一级制水中",
                                               "二级制水中",
                                               "液位异常  ",
                                               };
const unsigned char Char_WaitDisplay [4][16]= {"沃特尔水处理公司",
                                               "  免费服务电话  ",
                                               "  400-626-4939  ",
                                               "按确认键进入取水",
                                               };
const unsigned char Char_QuickDisplay [4][16]={"    快捷取水    ",
                                               "上: RO    下: UP",
                                               "  长按上：EDI   ",
                                               "按确认键进入系统",
                                               };
const unsigned char Char_HighSet[20][10]   =   {"控制点检测",
                                               "水质采样  ",
                                               "语音检测  ",
                                               "RO流量设置",																							 
                                               "UP流量设置",
																							 "ED流量设置",
                                               "UP电阻校准",
                                               "RO电导校准",
                                               "SC电导校准",
																							 "ED电导校准",
                                               "软件版本  ", 
                                               "UP电阻下限",
                                               "RO电导上限",
																							 "ED电导上限",
                                               "电压偏移量",
                                               "耗材失效  ",
                                               "RO电导量程",
																							 "ED电导量程",
																							 "UP循环电阻",
																							 "UP循环时间",
                                               };
const unsigned char Char_Force_StopGetWater[2][8] ={
                                                  "禁止取水",
                                                  "仅提示  ",
                                                  };
const unsigned char Char_RO_Conductance_Rang[3][6] ={
                                                  "0-20  ",
                                                  "0-200 ",
                                                  "0-2000",
                                                  };

const unsigned char Char_HighSet_RO_Flow[7]  =   {"RO流量:"};
const unsigned char Char_HighSet_UP_Flow[7]  =   {"UP流量:"};
const unsigned char Char_HighSet_ED_Flow[7]  =   {"ED流量:"};
const unsigned char Char_SensorBreak[10]     =   {"传感器断线"};
const unsigned char Char_SC_Conductance[11]  =   {"源水电导率:"};
const unsigned char Char_SensorBreak_SC[12]  =   {"未选购该模块"};
const unsigned char Char_HighSet_Resistivity[13]     =   {"UP电阻率系数:"};
const unsigned char Char_HighSet_Conductance[13]     =   {"RO电导率系数:"};
const unsigned char Char_HighSet_Conductance_SC[13]  =   {"SC电导率系数:"};
const unsigned char Char_HighSet_Conductance_ED[13]  =   {"ED电导率系数:"};
const unsigned char Char_DeviceM_0[18][12]= {"泵1.1     关", 
                                             "泵1.2     关",
                                             "泵2       关",
                                             "进水阀JS  关",
                                             "超滤Mult  关",
                                             "冲洗CX    关",
                                             "RO取水    关", 
                                             "UP取水    关",
																						 "ED取水    关",
                                             "继电器    关", 
                                             "指示灯    关",
																						 "循环阀    关",
																						 "循环泵    关",
                                             "低压阀  断开", 
                                             "高压阀  断开",
                                             "低液位  断开", 
                                             "中液位  断开",
                                             "高液位  断开",
                                             };
const unsigned char Char_DeviceM_1[18][12]= {"泵1.1     开", 
                                             "泵1.2     开",
                                             "泵2       开",
                                             "进水阀JS  开",
                                             "超滤Mult  开",
                                             "冲洗CX    开",
                                             "RO取水    开", 
                                             "UP取水    开",
																						 "ED取水    开",
                                             "继电器    开", 
                                             "指示灯    开",
																						 "循环阀    开",
																						 "循环泵    开",
                                             "低压阀  接通", 
                                             "高压阀  接通",
                                             "低液位  接通", 
                                             "中液位  接通",
                                             "高液位  接通",
                                             };

const unsigned char Char_GetWater[6][10]  =   {"取RO水    ",
                                               "取UP水    ",
																							 "取ED水   ",
                                               "定量取RO水",
                                               "定量取UP水",
																							 "定量取ED水",
                                               };
const unsigned char Char_History[5][12]     = {"累计RO取水量",
                                               "累计UP取水量",
																							 "累计ED取水量",
                                               "取水记录查询",
                                               "清除历史记录",
                                              };

const unsigned char Char_Recordless[14]   =   {"  --无记录--  ",};


const unsigned char Char_DeviceWashTxt[14]          =   {"按确认关闭冲洗"};
const unsigned char Char_DeviceWashTxt1[14]         =   {"系统冲洗中...."}; 

const unsigned char Char_Check_PressPump[2][12]  =  {"压力泵  启动",
                                                     "压力泵  关闭",
                                                    };
const unsigned char Char_Check_Wash_per[2][10]   =  {"超滤阀  开",
                                                     "超滤阀  关",
                                                    };
const unsigned char Char_Check_Wash[2][10]       =  {"冲洗阀  开",
                                                     "冲洗阀  关",
                                                    };
const unsigned char Char_Check_PressPump1[2][12] =  {"一级泵  启动",
                                                     "一级泵  关闭",
                                                    };
const unsigned char Char_Check_PressPump2[2][12] =  {"二级泵  启动",
                                                     "二级泵  关闭",
                                                    };
const unsigned char Char_Check_InputWater[2][12] =  {"进  水: 正常",
                                                     "进  水: 缺水",
                                                    };
const unsigned char Char_Check_PressCan[2][12]   =  {"压力桶: 满  ",
                                                     "压力桶: 不满",
                                                    };
const unsigned char Char_Check_WaterLevel[5][12] =  {"液  位: 高  ",
                                                     "液  位: 中  ",
                                                     "液  位: 低  ",
                                                     "液  位: 缺水",
                                                     "液  位: 异常",
                                                    };
const unsigned char Char_GetWaterClose[14]          =   {"按确认关闭取水"};
const unsigned char Char_GetRO[14]                  =   {"正在取RO水...."};                                      
const unsigned char Char_GetUP[14]                  =   {"正在取UP水...."};
const unsigned char Char_GetEDI[14]                  =   {"正在取ED水...."}; 
const unsigned char Char_Conductance[5]             =   {"uS/cm"};
const unsigned char Char_Resistivity[6]             =   {"MΩ.cm"};
const unsigned char Char_GetWaterQuantify[7]        =   {"取水量:"};

const unsigned char Char_GetROStatus[2][12]         =   {"RO取水  关闭",
                                                         "RO取水  开启",};
const unsigned char Char_GetEDIStatus[2][12]         =   {"ED取水  关闭",
                                                         "ED取水  开启",};
const unsigned char Char_GetUPStatus[2][12]         =   {"UP取水  关闭",
                                                         "UP取水  开启",};                                                         

const unsigned char Char_Auto_Test[8]               =   {"自动检测"};
const unsigned char Char_Auto_Test_AD[8]            =   {"采样检测"};
const unsigned char Char_Auto_Test_CLK[8]           =   {"时钟校准"};
const unsigned char Char_Auto_Test_OUT[8]           =   {"输出测试"};
const unsigned char Char_Auto_Test_IN[8]            =   {"输入测试"};

const unsigned char WTLogo[] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0xF0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x01,0x98,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x01,0x08,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x08,0x00,0x00,0xC8,0x03,0xF4,
0x7F,0x83,0xFC,0x7F,0xFF,0xE0,0x00,0x0E,0x00,0x00,0x08,0x00,0x01,0xD8,0x02,0x94,
0x7F,0x83,0xFE,0x7F,0xFF,0xE0,0x00,0x0E,0x00,0x00,0x08,0x00,0x03,0xDC,0x02,0x8A,
0x7F,0xC3,0xFE,0x7F,0xFF,0xC0,0x00,0x0E,0x00,0x00,0x08,0x00,0x03,0xCC,0x04,0x8A,
0x3F,0xC3,0xFE,0x7F,0xFF,0xC0,0xE0,0x1E,0x00,0x00,0x08,0x01,0x03,0xCC,0x04,0xF2,
0x3F,0xC3,0xFE,0x7F,0xFF,0xC0,0x60,0x1E,0x00,0x00,0x0A,0x00,0x87,0xC4,0x04,0xF2,
0x3F,0xC1,0xFF,0x3F,0xFF,0x80,0x70,0x38,0x00,0x00,0x0F,0x00,0x86,0xC2,0x04,0x92,
0x1F,0xE1,0xFF,0x3F,0xFF,0x80,0x70,0x30,0x00,0x00,0x0F,0x00,0x86,0xCE,0x04,0x92,
0x1F,0xE1,0xFF,0x3F,0xFF,0x80,0x30,0x60,0x00,0x0C,0x3C,0x00,0xC6,0xFE,0x04,0x92,
0x1F,0xE1,0xFF,0xBF,0xFF,0x00,0x00,0xE0,0x00,0xCC,0x38,0x00,0xC4,0xF8,0x02,0x92,
0x1F,0xE1,0xFF,0x9F,0xFF,0x00,0x00,0xE0,0xC0,0xCC,0x78,0x00,0xC7,0xE0,0x02,0x94,
0x1F,0xF0,0xFF,0x9F,0xFE,0x00,0x01,0xF1,0xC0,0xCC,0xD9,0xE0,0x87,0xC0,0x01,0x04,
0x0F,0xF0,0xFF,0x9F,0xFE,0x01,0x83,0x37,0xC0,0xCC,0xDB,0xE0,0x07,0xC0,0x01,0x98,
0x0F,0xF0,0xFF,0x8F,0xFE,0x01,0xC0,0x3F,0x80,0xCF,0x1F,0xC0,0x00,0xF0,0x00,0xF0,
0x0F,0xF0,0x7F,0xCF,0xFE,0x01,0xC0,0x3F,0x00,0xCF,0x1F,0x80,0x00,0xF8,0x00,0x00,
0x0F,0xF8,0x7F,0xCF,0xFC,0x00,0xC4,0x7C,0x01,0xDE,0x3F,0x00,0x10,0xF8,0x00,0x00,
0x07,0xF8,0x7F,0xCF,0xFC,0x00,0xE4,0xF8,0x01,0xFE,0x76,0x04,0x10,0xFC,0x00,0x00,
0x07,0xF8,0x7F,0xCF,0xF8,0x00,0xEB,0xF8,0x01,0xFF,0xFE,0x06,0x35,0xCC,0x00,0x00,
0x07,0xF8,0x7F,0xC7,0xF8,0x00,0xCB,0xF8,0x03,0xEF,0xDE,0x07,0x37,0xCC,0x00,0x00,
0x03,0xFC,0x3F,0xE7,0xF0,0x00,0xC9,0xB0,0x03,0xEF,0x87,0x03,0x66,0xCC,0x00,0x00,
0x03,0xFC,0x3F,0xE7,0xF0,0x00,0x49,0x30,0x03,0xDF,0x27,0x03,0x66,0xEC,0x00,0x00,
0x03,0xFC,0x3F,0xE7,0xF0,0x00,0x18,0x30,0x03,0xDF,0x2F,0x03,0x64,0xEC,0x00,0x00,
0x03,0xFC,0x3F,0xF3,0xF0,0x00,0x18,0x3E,0x01,0xBC,0x7E,0x03,0x47,0xEE,0x00,0x00,
0x03,0xFC,0x3F,0xF3,0xE0,0x00,0x30,0x3F,0x01,0x38,0x66,0x03,0x47,0xE6,0x00,0x00,
0x01,0xFE,0x1F,0xF3,0xE0,0x01,0xF0,0x37,0xC0,0x68,0x46,0x03,0x46,0xD6,0x00,0x00,
0x01,0xFE,0x1F,0xF1,0xC0,0x03,0xF0,0x27,0xC0,0x68,0x06,0x03,0xCE,0x96,0x00,0x00,
0x01,0xFE,0x1F,0xF9,0xC0,0x07,0xF1,0xE3,0xE1,0xC8,0x3E,0x03,0xCD,0xF6,0x00,0x00,
0x01,0xFE,0x1F,0xF9,0xC0,0x07,0xF1,0xE3,0xE1,0xC8,0x3E,0x03,0x8F,0xE6,0x00,0x00,
0x00,0xFF,0x3F,0xF9,0x80,0x03,0xE1,0xE1,0xF1,0xC8,0x46,0x03,0x8F,0xC6,0x00,0x00,
0x00,0xFF,0xFF,0xF9,0x80,0x03,0xE3,0xC1,0xF1,0x88,0x46,0x03,0x8F,0x86,0x00,0x00,
0x00,0x7F,0xFF,0xF8,0x80,0x01,0xC3,0x81,0xF1,0x08,0x46,0x03,0x8C,0x86,0x00,0x00,
0x00,0x7F,0xFF,0xFC,0x00,0x01,0xC3,0x01,0xF1,0x08,0x7E,0x03,0x8C,0x8C,0x00,0x00,
0x00,0x7F,0xFF,0xFC,0x00,0x00,0x83,0x01,0xE1,0x08,0x7E,0x03,0x84,0xC4,0x00,0x00,
0x00,0x7F,0xFF,0xFC,0x00,0x00,0x00,0x00,0xE0,0x08,0x3E,0x03,0x00,0xBC,0x00,0x00,
0x00,0x7F,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x08,0x1E,0x01,0x00,0xBC,0x00,0x00,
0x00,0x3F,0xE3,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
0x00,0x3F,0xE3,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
0x00,0x3F,0xC1,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
0x00,0x3F,0xC1,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x1F,0x81,0xF8,0x00,0x01,0x18,0x81,0x00,0x00,0x00,0x02,0x68,0x00,0x00,0x00,
0x00,0x1F,0x81,0xF8,0x00,0x01,0x19,0x01,0x00,0x00,0x00,0x02,0x48,0x00,0x00,0x00,
0x00,0x1F,0x81,0xF8,0x00,0x01,0x19,0x19,0x08,0xA0,0x00,0x00,0x40,0x80,0x00,0x00,
0x00,0x0F,0x80,0xF0,0x00,0x00,0x99,0x39,0xDC,0xE0,0xE4,0xAA,0xE9,0xCA,0x00,0x00,
0x00,0x0F,0x00,0xF0,0x00,0x00,0xB9,0x2D,0x16,0x81,0xA4,0xBA,0x4B,0x6E,0x00,0x00,
0x00,0x0F,0x00,0xF0,0x00,0x00,0xA5,0x45,0x22,0x81,0x24,0xA2,0x4A,0x28,0x00,0x00,
0x00,0x0F,0x00,0x70,0x00,0x00,0xA5,0x05,0x22,0x81,0x14,0xA2,0x4A,0x28,0x00,0x00,
0x00,0x0E,0x00,0x60,0x00,0x00,0xA5,0x3D,0x3E,0x81,0x14,0xA2,0x4B,0xE8,0x00,0x00,
0x00,0x06,0x00,0x60,0x00,0x00,0xA7,0x65,0x20,0x81,0x14,0xA2,0x4A,0x08,0x00,0x00,
0x00,0x06,0x00,0x40,0x00,0x00,0xE6,0x45,0x22,0x81,0x24,0xA2,0x4A,0x28,0x00,0x00,
0x00,0x04,0x00,0x40,0x00,0x00,0x46,0x4D,0xB6,0x81,0x24,0xA2,0x4B,0x68,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x34,0xDC,0x81,0xE7,0xA2,0x49,0xC8,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*库贝科技
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x7F,0xFF,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x3F,0xFF,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x1F,0xFF,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x0F,0xFF,0xC0,0x00,0x1F,0xFF,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x0F,0xE0,0x00,0x0F,0xFF,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x03,0xF0,0x00,0x0F,0xFF,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x01,0xF0,0x00,0x0F,0xFF,0xC0,
0x00,0x00,0xFF,0xFC,0x00,0x00,0x07,0xC0,0x00,0x00,0x01,0xF0,0x00,0x0F,0xFF,0xC0,
0x00,0x0F,0xFF,0xFF,0xE0,0x00,0x07,0xC0,0x00,0x00,0x01,0xF8,0x00,0x0F,0xFF,0xC0,
0x00,0x7F,0xFF,0xFF,0xFC,0x00,0x07,0xC0,0x00,0x00,0x00,0xF8,0x00,0x0F,0xFF,0x80,
0x00,0xFF,0x00,0x01,0xFF,0x80,0x07,0xC0,0x00,0x00,0x01,0xF8,0x00,0x0F,0xFF,0x80,
0x03,0xF8,0x00,0x00,0x3F,0x80,0x07,0xC0,0x00,0x00,0x01,0xF0,0x00,0x0F,0xFF,0x80,
0x07,0xE0,0x00,0x00,0x0F,0x00,0x07,0xC0,0x00,0x00,0x01,0xF0,0x00,0x1F,0xFF,0x80,
0x0F,0xC0,0x00,0x00,0x06,0x00,0x07,0xC0,0x00,0x00,0x03,0xE0,0x00,0x1F,0xFF,0x00,
0x1F,0x00,0x00,0x00,0x04,0x00,0x07,0xC0,0x00,0x00,0x1F,0xC0,0x00,0x1F,0xFF,0x00,
0x3F,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x1F,0xFE,0x00,
0x3E,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x3F,0xFE,0x00,
0x3C,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x3F,0xFC,0x00,
0x7C,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x3F,0xFC,0x00,
0x7C,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x0F,0xE0,0x00,0x7F,0xFC,0x00,
0x7C,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x03,0xF0,0x00,0x7F,0xF8,0x00,
0x7C,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x01,0xF8,0x00,0x77,0xF8,0x00,
0x7C,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0xF8,0x00,0x67,0xF0,0x00,
0x7C,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0x7C,0x00,0xE7,0xF0,0x00,
0x7C,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0x7C,0x00,0xC7,0xE0,0x00,
0x3E,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0x7C,0x00,0x8F,0xE0,0x00,
0x3F,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0x7C,0x01,0x0F,0xC0,0x00,
0x1F,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0xFC,0x00,0x0F,0xC0,0x00,
0x0F,0xC0,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0xF8,0x00,0x1F,0x80,0x00,
0x07,0xE0,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x01,0xF8,0x00,0x1F,0x80,0x00,
0x03,0xF8,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x03,0xF0,0x00,0x1F,0x00,0x00,
0x01,0xFF,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x1F,0xE0,0x00,0x1E,0x00,0x00,
0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x3E,0x00,0x00,
0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x3C,0x00,0x00,
0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x7C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x68,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x0E,0x00,0x38,0x0F,0x00,0x00,
0x00,0x00,0x07,0x00,0x00,0x7F,0xFF,0xC0,0x3F,0xF8,0x8E,0x00,0x38,0x0F,0x00,0x00,
0x00,0x1F,0xFF,0xFF,0x80,0x7F,0xFF,0xC0,0x1F,0xFB,0xCE,0x00,0x38,0x0F,0x00,0x00,
0x00,0x1F,0xFF,0xFF,0x80,0x78,0x03,0xC0,0x03,0x81,0xEE,0x00,0x39,0xFF,0xFC,0x00,
0x00,0x1C,0x0F,0x00,0x00,0x79,0xE3,0xC0,0x03,0x80,0xEE,0x03,0xFF,0xFF,0xFC,0x00,
0x00,0x1D,0xFF,0xFF,0x00,0x79,0xE3,0xC0,0x3F,0xF8,0x0E,0x03,0xFF,0x0F,0x00,0x00,
0x00,0x1D,0xFF,0xFF,0x00,0x79,0xE3,0xC0,0x3F,0xF9,0x8E,0x00,0x38,0x0F,0x00,0x00,
0x00,0x1C,0x3C,0x00,0x00,0x79,0xE3,0xC0,0x03,0x83,0xCE,0x00,0x38,0xFF,0xF0,0x00,
0x00,0x1C,0x73,0xC0,0x00,0x79,0xE3,0xC0,0x07,0xB1,0xCE,0x00,0x3E,0xFF,0xF0,0x00,
0x00,0x1D,0xFF,0xFE,0x00,0x79,0xE3,0xC0,0x07,0xF8,0xEE,0x03,0xFE,0x70,0xF0,0x00,
0x00,0x1D,0xFF,0xFE,0x00,0x79,0xE3,0xC0,0x0F,0xBC,0x0F,0xC3,0xF8,0x79,0xE0,0x00,
0x00,0x1C,0x83,0xC0,0x00,0x79,0xE3,0xC0,0x1F,0x9B,0xFF,0xC1,0xB8,0x3D,0xC0,0x00,
0x00,0x3C,0x03,0xC0,0x00,0x03,0xF8,0x00,0x3B,0x8F,0xFE,0x00,0x38,0x1F,0x80,0x00,
0x00,0x3F,0xFF,0xFF,0x80,0x0F,0xFF,0x00,0x3B,0x80,0x0E,0x00,0x38,0x0F,0x80,0x00,
0x00,0x7B,0xFF,0xFF,0x80,0x3F,0x1F,0xE0,0x13,0x80,0x0E,0x00,0x38,0x3F,0xE0,0x00,
0x00,0x78,0x03,0xC0,0x03,0xFE,0x07,0xF0,0x03,0x80,0x0E,0x01,0xF9,0xFF,0xFC,0x00,
0x00,0x30,0x03,0xC0,0x01,0xF8,0x01,0xE0,0x03,0x80,0x0E,0x00,0xF9,0xF0,0x7C,0x00,
0x00,0x00,0x03,0xC0,0x00,0x80,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x80,0x08,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,*/
};
/* Exported macro ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */



#endif /**/
/* Copyright (C) 2011 Chengdu Reliance Eltctric Co., Ltd. **** END OF FILE ****/
