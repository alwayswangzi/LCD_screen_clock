#ifndef __REMOTE_H
#define __REMOTE_H
#include "IO.h"

#define RDATA PBin(9)	 //红外数据输入脚


//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0
#define KEY_POWER 162
#define KEY_UP 98
#define KEY_PLAY 2
#define KEY_ALIENTEK 226
#define KEY_RIGHT 194
#define KEY_LEFT 34
#define KEY_VOL_DEC 224
#define KEY_DOWN 168
#define KEY_VOL_INC 144
#define KEY_1 104
#define KEY_2 152
#define KEY_3 176
#define KEY_4 48
#define KEY_5 24
#define KEY_6 122
#define KEY_7 16
#define KEY_8 56
#define KEY_9 90
#define KEY_0 66
#define KEY_DELETE 82

extern unsigned char RmtCnt;	//按键按下的次数

void Remote_Init(void);    //红外传感器接收头引脚初始化
unsigned char Remote_Scan(void);
#endif















