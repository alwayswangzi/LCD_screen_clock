#ifndef __LED_DISPLAY_H
#define __LED_DISPLAY_H
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

//标准08接口点阵屏管脚定义
//GND     A     ABCD是行扫描信号
//GND     B
//GND     C
//OE      D	  OE是屏的使能端  低有效
//R1      G1	     上半屏绿色
//R2      G2	     下半屏绿色
//GND	    STR	     锁存信号
//GND     SCK	     时钟信号D
//接口说明
//A B C D 是138行扫描信号
//SHCP(SCK)   是595的移位信号
//STCP(STR)  是595的锁存信号
//OE    是屏的使能端 可以控制亮度
//R     是红色信号线
//G     是绿色信号线

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

void rxd_data(unsigned char l);		// 发送移动数据
void load_data_num(unsigned int num);
void LED_Screen_Init(void);
void scan(unsigned char Value);	//不影响其他端口的扫描
void Display_Time(void);	//显示当前屏幕数据
void Display_Date(void);
void Display_Temp(void);
void Display_Item(void);
void Display_Light(void);
void Display_Alarm_Confirm(void);
void Display_Alarm(unsigned char step,unsigned char value);
void Display_Set_Time(unsigned char step,unsigned char value);
void delay(unsigned char x);
#endif
