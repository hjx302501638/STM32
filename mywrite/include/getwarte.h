#ifndef __GETWARTE_H
#define __GETWARTE_H
#include "sys.h"

#define XU_TIMER    1000
#define GET_WT_VERSION_CONTROLLER  1
void GetWater(void);
void GetRO(u8 now);
void GetUP(u8 now);
void GetEDI(u8 now);
#endif 
