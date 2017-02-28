#ifndef __RTC_TIMER_H
#define __RTC_TIMER_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "misc.h"

void RTC_Configuration(void);	//RTC���ú���
void NVIC_Configuration(void);	//�жϳ�ʼ������
void Time_Adjust(unsigned long sec);	//ʱ�����������ֻ����RTC����֮����ܵ���
void RTC_IRQHandler(void);	//�жϷ�������ÿһ�뷢��һ���жϣ��ͻص�ǰʱ������
void RTC_Timer_Init(void);
#endif
