#ifndef __REMOTE_H
#define __REMOTE_H
#include "IO.h"

#define RDATA PBin(9)	 //�������������


//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
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

extern unsigned char RmtCnt;	//�������µĴ���

void Remote_Init(void);    //���⴫��������ͷ���ų�ʼ��
unsigned char Remote_Scan(void);
#endif















