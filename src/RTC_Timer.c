#include "RTC_Timer.h"
#include "event.h"
extern unsigned char h,min;	//Сʱ������
extern unsigned char dot;	//��˸�ĵ�
extern unsigned char on_clock;
unsigned char tick;
void RTC_Timer_Init(void)	//RTC���ú���
{
    NVIC_Configuration();	//�ж����ú���

    ////*********************************************///
    ////ϵͳ��λ�󣬶Ժ󱸼Ĵ�����RTC�ķ��ʱ���ֹ������Ϊ�˷�ֹ�Ժ�����(BKP)������д������
    ////ִ�����²�����ʹ�ܶԺ󱸼Ĵ�����RTC�ķ���

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RCC_RTCCLKCmd(ENABLE);


    ////*********************************************///


    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)	//�����ǰ�󱸼Ĵ�����ֵ��һ��˵��Ҫ����ʱ����
    {
        RTC_Configuration();//RTC����
        Time_Adjust(0);		//ʱ�����
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	//ʹ�󱸼Ĵ�����ֵ�뵱ǰֵ����һ�£���ֹ�´θ���
    }
   else	//����ֱ�����У���������
    {
	   RTC_WaitForSynchro();
	   RTC_ITConfig(RTC_IT_SEC, ENABLE);
       RTC_WaitForLastTask();
    }


}
void RTC_IRQHandler(void)	//�жϷ�������ÿһ�뷢��һ���жϣ��ͻص�ǰʱ������
{
    unsigned long sec;
    sec=RTC_GetCounter();

    h=(sec%86400)/3600;
    if(h>12) h-=12;

    min=(sec%3600)/60;

    if(dot==0)
       dot=1;
    else
       dot=0;

//    if(min==0 && sec%60==0 &&)
//    	BEEP_ON;
//    else
//    	BEEP_OFF;

    if(on_clock)
    {
    	if(tick==1)
    	{
    		tick=0;
    		BEEP_ON;
    	}
    	else
    	{
    		tick=1;
    		BEEP_OFF;
    	}

    }
    RTC_ClearITPendingBit(RTC_IT_SEC);

}

void NVIC_Configuration(void)	//�жϳ�ʼ������
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void RTC_Configuration(void)	//RTC���ú���
{

	BKP_DeInit();//BKP�Ĵ���ȫ����Ϊȱʡֵ

    RCC_LSEConfig(RCC_LSE_ON);

    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    RCC_RTCCLKCmd(ENABLE);

    RTC_WaitForSynchro();

    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    RTC_WaitForLastTask();

    RTC_EnterConfigMode();/// ��������

    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    RTC_WaitForLastTask();

    RTC_ExitConfigMode(); //�˳�����ģʽ

}


void Time_Adjust(unsigned long sec)
{
    RTC_WaitForLastTask();
    RTC_EnterConfigMode();/// ��������
    RTC_SetCounter(sec);
    RTC_WaitForLastTask();
    RTC_ExitConfigMode(); //�˳�����ģʽ
}

