#ifndef __T_SENSOR_H
#define __T_SENSOR_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#define ADC_CH_TEMP  	ADC_Channel_16 //温度传感器通道

unsigned int  T_Get_Temp(void);  //取得温度值
void AD_Init(void); //ADC通道初始化
unsigned int  T_Get_Adc(u8 ch); //获得某个通道值
unsigned int  T_Get_Adc_Average(u8 ch,u8 times);//得到某个通道10次采样的平均值
#endif
