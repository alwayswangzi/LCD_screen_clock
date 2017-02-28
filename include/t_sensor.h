#ifndef __T_SENSOR_H
#define __T_SENSOR_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#define ADC_CH_TEMP  	ADC_Channel_16 //�¶ȴ�����ͨ��

unsigned int  T_Get_Temp(void);  //ȡ���¶�ֵ
void AD_Init(void); //ADCͨ����ʼ��
unsigned int  T_Get_Adc(u8 ch); //���ĳ��ͨ��ֵ
unsigned int  T_Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ��10�β�����ƽ��ֵ
#endif
