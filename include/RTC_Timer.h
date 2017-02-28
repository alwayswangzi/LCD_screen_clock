#ifndef __RTC_TIMER_H
#define __RTC_TIMER_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "misc.h"

void RTC_Configuration(void);	//RTC配置函数
void NVIC_Configuration(void);	//中断初始化函数
void Time_Adjust(unsigned long sec);	//时间调整函数，只有在RTC配置之后才能调整
void RTC_IRQHandler(void);	//中断服务函数，每一秒发生一次中断，送回当前时间数据
void RTC_Timer_Init(void);
#endif
