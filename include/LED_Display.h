#ifndef __LED_DISPLAY_H
#define __LED_DISPLAY_H
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

//��׼08�ӿڵ������ܽŶ���
//GND     A     ABCD����ɨ���ź�
//GND     B
//GND     C
//OE      D	  OE������ʹ�ܶ�  ����Ч
//R1      G1	     �ϰ�����ɫ
//R2      G2	     �°�����ɫ
//GND	    STR	     �����ź�
//GND     SCK	     ʱ���ź�D
//�ӿ�˵��
//A B C D ��138��ɨ���ź�
//SHCP(SCK)   ��595����λ�ź�
//STCP(STR)  ��595�������ź�
//OE    ������ʹ�ܶ� ���Կ�������
//R     �Ǻ�ɫ�ź���
//G     ����ɫ�ź���

#define SHCP_0	GPIOC->BRR=GPIO_Pin_13
#define SHCP_1	GPIOC->BSRR=GPIO_Pin_13

#define STCP_0	GPIOC->BRR=GPIO_Pin_12
#define STCP_1	GPIOC->BSRR=GPIO_Pin_12

#define R_0		GPIOC->BRR=GPIO_Pin_11
#define R_1		GPIOC->BSRR=GPIO_Pin_11

#define G_0		GPIOC->BRR=GPIO_Pin_10
#define G_1		GPIOC->BSRR=GPIO_Pin_10

#define IA_0	GPIOC->BRR=GPIO_Pin_9
#define IA_1	GPIOC->BSRR=GPIO_Pin_9

#define IB_0	GPIOC->BRR=GPIO_Pin_8
#define IB_1	GPIOC->BSRR=GPIO_Pin_8

#define IC_0	GPIOC->BRR=GPIO_Pin_7
#define IC_1	GPIOC->BSRR=GPIO_Pin_7

#define ID_0	GPIOC->BRR=GPIO_Pin_6
#define ID_1	GPIOC->BSRR=GPIO_Pin_6

#define OE_0	GPIOD->BRR=GPIO_Pin_2
#define OE_1	GPIOD->BSRR=GPIO_Pin_2

#define  scan0    {IA_0;IB_0;IC_0;ID_0;}
#define  scan1    {IA_1;IB_0;IC_0;ID_0;}
#define  scan2    {IA_0;IB_1;IC_0;ID_0;}
#define  scan3    {IA_1;IB_1;IC_0;ID_0;}
#define  scan4    {IA_0;IB_0;IC_1;ID_0;}
#define  scan5    {IA_1;IB_0;IC_1;ID_0;}
#define  scan6    {IA_0;IB_1;IC_1;ID_0;}
#define  scan7    {IA_1;IB_1;IC_1;ID_0;}
#define  scan8    {IA_0;IB_0;IC_0;ID_1;}
#define  scan9    {IA_1;IB_0;IC_0;ID_1;}
#define scan10    {IA_0;IB_1;IC_0;ID_1;}
#define scan11    {IA_1;IB_1;IC_0;ID_1;}
#define scan12    {IA_0;IB_0;IC_1;ID_1;}
#define scan13    {IA_1;IB_0;IC_1;ID_1;}
#define scan14    {IA_0;IB_1;IC_1;ID_1;}
#define scan15    {IA_1;IB_1;IC_1;ID_1;}

void rxd_data(unsigned char l);		// �����ƶ�����
void load_data_num(unsigned int num);
void LED_Screen_Init(void);
void scan(unsigned char Value);	//��Ӱ�������˿ڵ�ɨ��
void Display_Time(void);	//��ʾ��ǰ��Ļ����
void Display_Date(void);
void Display_Temp(void);
void Display_Item(void);
void Display_Light(void);
void Display_Alarm_Confirm(void);
void Display_Alarm(unsigned char step,unsigned char value);
void Display_Set_Time(unsigned char step,unsigned char value);
void delay(unsigned char x);
#endif
