#include "LED_Display.h"	//LED��������ʾ
#include "RTC_Timer.h"		//RTCʱ��
#include "delay.h"			//��ʱ����
#include "Remote.h"			//����ң��
#include "event.h"			//��������
#include "timer.h"			//��ʱ��
unsigned char line;  		// ��
unsigned long Data_R[16];	//��ǰ��Ļ����-��ɫ
unsigned long Data_G[16];	//��ǰ��Ļ����-��ɫ
unsigned char h,min;		//Сʱ������
unsigned char dot;			//ʱ����˸�ĵ�
unsigned char item;			//�˵�����	0����ʾʱ��	1����ʾ�˵�
unsigned char light;		//���ȼ���
unsigned char color;		//������ɫ
unsigned char item_step;	//�˵����
unsigned char item_page;	//�˵�ҳ��
unsigned char bright_step;	//���Ȳ���
unsigned char single_value; //��������ֵ
unsigned char press_count;	//��������
unsigned char set_time_n1;	//����ʱ��	Сʱ ʮλ
unsigned char set_time_n2;	//����ʱ��	Сʱ ��λ
unsigned char set_time_n3;	//����ʱ��	���� ʮλ
unsigned char set_time_n4;	//����ʱ��	���� ��λ
unsigned char on_clock;
int main(void)
{
//	RTC_Timer_Init();		//RTCʱ�ӳ�ʼ��
	delay_init();			//��ʱ������ʼ��
	LED_Screen_Init();		//LED��������ʾ��ʼ��
	Tim3_Init();			//��ʱ��3��ʼ��
	Remote_Init();			//����ң�س�ʼ��
	light=BKP_ReadBackupRegister(BKP_DR2); //�Ӻ󱸼Ĵ���2�ж�ȡ֮ǰ�洢������ֵ
	color=BKP_ReadBackupRegister(BKP_DR3);
    while(1)
    {

    	Scanning();				//����ɨ��	�ж��Ƿ�������ʾ���ǲ˵���ʾ
    	while(item)
    	{
    		while(item)			//��һ���˵�
    			Item_Display();
    		delay_ms(100);
    		item=1;
    		while(item)		//�ڶ����˵�
    			Function_Display();
    	}

    	Display_Time();			//������ʾ

    }
}




